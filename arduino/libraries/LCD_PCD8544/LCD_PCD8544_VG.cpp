/*
 * LCD_PCD8544_VG - Interface with Nokia 5110 LCDs (or compatible) 
 *	[Philips PCD8544 controller].
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 *	Some portions:
 * 		Copyright (c) 2010 Carlos Rodrigues <cefrodrigues@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "LCD_PCD8544_VG.h"

#include <avr/pgmspace.h>

// If this was a ".h", it would get added to sketches when using
// the "Sketch -> Import Library..." menu on the Arduino IDE
#include "fonts.cpp"

#define swap(a, b) { uint8_t t; t = a; a = b; b = t; }

LCD_PCD8544_VG::LCD_PCD8544_VG(uint8_t sce, uint8_t sclk, uint8_t sdin, uint8_t dc, uint8_t reset) {
	setup(sclk, sdin, dc, reset, sce);
}


void LCD_PCD8544_VG::begin(uint8_t bias) {
	memset(_buffer, 0, sizeof(_buffer));
	init(bias);
}


void LCD_PCD8544_VG::clear() {
    setXYAddress(0, 0);
    for (uint8_t l = 0; l < LCD_PCD8544_LINES; l++) {
	    for (uint8_t c = 0; c < LCD_PCD8544_WIDTH; c++) {
			send(LCD_PCD8544_DATA, 0x00);
			_buffer[l][c] = 0x00;
		}
    }
    setXYAddress(0, 0);
}


void LCD_PCD8544_VG::clearLine() {
    setXYAddress(0, _line);
    for (uint8_t c = 0; c < LCD_PCD8544_WIDTH; c++) {
        send(LCD_PCD8544_DATA, 0x00);
		_buffer[_line][c] = 0x00;
    }
    setXYAddress(0, _line);
}


void LCD_PCD8544_VG::setXCursor(uint8_t x, uint8_t line) {
	setXYAddress(x, line);
}


void LCD_PCD8544_VG::write(uint8_t chr) {
	if (!bufferChar(&(_buffer[_line][_column]), chr)) return;
		
    // Output one column at a time...
    for (uint8_t i = 0; i < LCD_PCD8544_CHAR_WIDTH; i++) {
        send(LCD_PCD8544_DATA, _buffer[_line][_column + i]);
    }

    // Update the XY RAM position...
    _column = (_column + LCD_PCD8544_CHAR_WIDTH) % LCD_PCD8544_WIDTH;
    if (_column == 0) _line = (_line + 1) % LCD_PCD8544_LINES;
}


void LCD_PCD8544_VG::drawBitmap(const uint8_t *data, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {

	// draw in the frame
	if ((x > LCD_PCD8544_WIDTH) || (y > LCD_PCD8544_HEIGHT)) return;
	
	uint8_t line = y / 8;
	uint8_t shift = y % 8;
	
	// calculate the masks for the bitmap
	uint16_t mask = 0;
	for (uint8_t i=0; (i<height) && (i<8); i++) mask = mask*2+1;
	uint8_t lsmask = (uint8_t)~(mask << shift);
	uint8_t rsmask = (uint8_t)~(mask >> (8-shift));

	// clip the bitmap to the screen
	uint8_t stopLine = (y + height > LCD_PCD8544_HEIGHT ? LCD_PCD8544_HEIGHT-y : height) / 8;
	if (stopLine == 0) stopLine = 1;
	uint8_t stopColumn = (x + width > LCD_PCD8544_WIDTH ? LCD_PCD8544_WIDTH-x : width);
	
	uint8_t pix8;
	
	for (uint8_t l = 0; l < stopLine; l++) {
		for (uint8_t c = 0; c < stopColumn; c++) {
			pix8 = data[l*width+c] << shift;
			_buffer[line+l][x+c] &= lsmask;
			_buffer[line+l][x+c] |= pix8;
			if ((shift != 0) && (line+l+1 < LCD_PCD8544_LINES)) {
				pix8 = data[l*width+c] >> (8-shift);
				_buffer[line+l+1][x+c] &= rsmask;
				_buffer[line+l+1][x+c] |= pix8;
			}
		}
	}
}


#ifdef LCD_PCD8544_FONT_4X6

uint8_t LCD_PCD8544_VG::drawMediumFontDigit(uint8_t val, uint8_t x, uint8_t y) {
	uint8_t buffer[4];
	
	memcpy_P(buffer, &font_4x6[val == 0x20 ? 10 : val], 4);
	drawBitmap(buffer, x, y, 4, 6);
	bufferVLine(x+4, y, y+6, OFF);
	return x+5;
}


uint8_t LCD_PCD8544_VG::drawMediumFontValueRA(uint16_t value, uint8_t maxL, uint8_t space, uint8_t x, uint8_t y, bool isTenTimes) {
	uint8_t lx = x;
	uint16_t val = value;
	uint8_t maxLength = maxL;
	if (isTenTimes) {
		val /= 10;
		if (val < 100) {
			drawMediumFontDigit(' ', lx, y);
			lx += 3;
		}
		else maxLength += 2;
	}
	if (val >= 10000) return x;
	if (maxLength >= 4) {
		if (val >= 1000) lx = drawMediumFontDigit(val/1000, lx, y);
		else lx = drawMediumFontDigit(space, lx, y);
	}
	if (maxLength >= 3) {
		if (val >= 100) lx = drawMediumFontDigit(val/100-(10*(val/1000)), lx, y);
		else lx = drawMediumFontDigit(space, lx, y);
	}
	if (maxLength >= 2) {
		if (val >= 10)  lx = drawMediumFontDigit(val/10-(10*(val/100)), lx, y);
		else  lx = drawMediumFontDigit(space, lx, y);
	}
	lx = drawMediumFontDigit(val % 10, lx, y);
	if ((isTenTimes) && (val < 100)) {
		drawMediumFontDigit(' ', lx, y);
		bufferPixel(lx, y+5, true);
		drawMediumFontDigit(value%10, lx+2, y);			
	}
	return lx;
}

void LCD_PCD8544_VG::drawMediumFontTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t x, uint8_t y) {
	uint8_t lx = x+10;
	if (hours > 0) {
		lx = drawMediumFontValueRA(hours, 2, ' ', x, y, false);
		drawMediumFontDigit(' ', lx, y);
		bufferPixel(lx, y+2, ON);
		bufferPixel(lx, y+4, ON);
	}
	lx = drawMediumFontValueRA(minutes, 2, hours > 0 ? 0 : ' ', lx+2, y, false);
	drawMediumFontDigit(' ', lx, y);
	bufferPixel(lx, y+2, ON);
	bufferPixel(lx, y+4, ON);
	drawMediumFontValueRA(seconds, 2, 0, lx+2, y, false);
}

#endif


#ifdef LCD_PCD8544_FONT_3X5

void LCD_PCD8544_VG::drawSmallFontText(const char *text, uint8_t length, int x, int y) {
	uint8_t buffer[3];
	
	for (uint8_t i=0; i<length; i++) {
		uint8_t c = text[i];
		if ((c >= 0x20) && (c <= 0x7B)) {					// only chars from space to z are defined
			if ((c > 0x60) && (c < 0x7B)) c -= 'a'-'A';		// only uppercase chars are defined, replace lowercase by uppercase
			memcpy_P(buffer, &font_3x5[c - 0x20], 3);
			drawBitmap(buffer, x+i*4, y, 3, 5);
			bufferVLine(x+3+i*4, y, y+5, OFF);
		}
	}
}

#endif

void LCD_PCD8544_VG::drawPixel(uint8_t x, uint8_t y, bool on) {
	bufferPixel(x, y, on);
}


void LCD_PCD8544_VG::drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on) {	
	for (uint8_t x = x1; x <= x2; x++) {
		bufferPixel(x, y1, on);
		bufferPixel(x, y2, on);
	}
	for (uint8_t y = y1; y <= y2; y++) {
		bufferPixel(x1, y, on);
		bufferPixel(x2, y, on);
	}
}


/*
 *	From Wikipedia - Bresenham's line algorithm
 */
bool LCD_PCD8544_VG::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on, uint8_t thickness) {
	uint8_t d1 = (thickness-1) / 2;
	uint8_t d2 = thickness / 2;
	uint8_t dx = abs(x2-x1);
	if (dx == 0) {
		if ((d1 > 0) || (d2 > 0)) fillRectangle(x1-d1, y1, x1+d2, y2, on);
		else bufferVLine(x1, y1, y2, on); 
		return false;
	}
	uint8_t dy = abs(y2-y1);
	if (dy == 0) {
		if ((d1 > 0) || (d2 > 0)) fillRectangle(x1, y1-d1, x2, y2+d2, on);
		else bufferHLine(x1, x2, y1, on); 
		return false;
	}
	
	int8_t sx = -1;
	if (x1 < x2) sx = 1;
	int8_t sy = -1;
	if (y1 < y2) sy = 1;
	int8_t err = dx - dy;
	
	while (1) {
		if ((d1 > 0) || (d2 > 0)) fillRectangle(x1-d1, y1-d1, x1+d2, y1+d2, on);
		else bufferPixel(x1, y1, on);
		if ((x1 == x2) && (y1 == y2)) break;
		int16_t e2 = 2 * err;
		if (e2 > -dy) {
			err = err - dy;
			x1 = x1 + sx;
		}
		if (e2 < dx) {
			err = err + dx;
			y1 = y1 + sy;
		}
	}
	return true;
}


void LCD_PCD8544_VG::drawCircle(uint8_t x0, uint8_t y0, uint8_t r, bool on) {
	bufferCircle(x0, y0, r, on, false);
}


void LCD_PCD8544_VG::drawEllipse(uint8_t x0, uint8_t y0, uint8_t a, uint8_t b, bool on) {
	bufferEllipse(x0, y0, a, b, on, false);
}


void LCD_PCD8544_VG::drawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t percentage, bool filled) {
	// sanity checks
	if ((width < 8) || (width > LCD_PCD8544_WIDTH) || (x + width > LCD_PCD8544_WIDTH) || (percentage > 100) || (y >= LCD_PCD8544_HEIGHT)) return;

	uint8_t x1 = 3 + x;
	uint8_t x2 = x + width - 3;
	uint8_t y1 = y;
	uint8_t y2 = y1 + 6;
	fillRectangle(x1-3, y1, x2+3, y2, OFF);
	drawRectangle(x1, y1, x2, y2, ON);
	
	x2 = 3 + percentage * (x2 - x1) / 100;
	if (filled) fillRectangle(x1, y1, x2, y2, ON);
	else {
		bufferVLine(x2, y1, y2, ON);
		bufferVLine(x2-1, y1, y2, ON);
	}
}


void LCD_PCD8544_VG::drawToggleSwitch(uint8_t x, uint8_t y, uint8_t width, uint8_t nbPositions, uint8_t position) {
	// sanity checks
	if ((width < 8) || (width > LCD_PCD8544_WIDTH) || (x + width > LCD_PCD8544_WIDTH) || (position >= nbPositions) || (y >= LCD_PCD8544_HEIGHT)) return;

	uint8_t x1 = 3 + x;
	uint8_t x2 = x + width - 3;
	uint8_t y1 = y;
	uint8_t y2 = y1 + 6;
	fillRectangle(x1-3, y1, x2+3, y2, OFF);
	drawRectangle(x1, y1, x2, y2, ON);
	
	x2 = (width - 6) / nbPositions;
	while (x2 * nbPositions < width - 6) x2++;
	x1 = position * (width - 6) / nbPositions;
	x2 += x1;
	fillRectangle(x1, y1, x2, y2, ON);
}


void LCD_PCD8544_VG::fillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on) {
	if (y2 < y1) swap(y1, y2);
	for (uint8_t y = y1; y <= y2; y++) bufferHLine(x1, x2, y, on);
}


void LCD_PCD8544_VG::fillCircle(uint8_t x0, uint8_t y0, uint8_t r, bool on) {
	bufferCircle(x0, y0, r, on, true);
}


void LCD_PCD8544_VG::fillEllipse(uint8_t x0, uint8_t y0, uint8_t a, uint8_t b, bool on) {
	bufferEllipse(x0, y0, a, b, on, true);
}


// to go as fast as possible, we use hardware functions
void LCD_PCD8544_VG::updateDisplay() {
	// select the screen
    digitalWrite(_sce, LOW);
	// set address to 0, 0
    digitalWrite(_dc, LCD_PCD8544_CMD);
    shiftOut(_sdin, _sclk, MSBFIRST, 0x80);
    shiftOut(_sdin, _sclk, MSBFIRST, 0x40);
	// write all the data
    digitalWrite(_dc, LCD_PCD8544_DATA);
	for (uint8_t l=0; l<LCD_PCD8544_LINES; l++) {
		for (uint8_t c=0; c<LCD_PCD8544_WIDTH; c++) {
			shiftOut(_sdin, _sclk, MSBFIRST, _buffer[l][c]);
		}
	}
	// restore the previous location on display
    digitalWrite(_dc, LCD_PCD8544_CMD);
    shiftOut(_sdin, _sclk, MSBFIRST, 0x80 | _column);
    shiftOut(_sdin, _sclk, MSBFIRST, 0x40 | _line);
	// unselect the screen
    digitalWrite(_sce, HIGH);
}


// --------------- Private ----------------------

void LCD_PCD8544_VG::bufferPixel(int8_t x, int8_t y, bool on) {
	if ((x < 0) || (y < 0) || (x >= LCD_PCD8544_WIDTH) || (y >= LCD_PCD8544_HEIGHT)) return;
	
	uint8_t mask = 0x01 << (y & 0x07);
	uint8_t line = y >> 3;
	
	if (on) _buffer[line][x] |= mask;
	else _buffer[line][x] &= ~mask;
}


bool LCD_PCD8544_VG::bufferHLine(int8_t x1, int8_t x2, int8_t y, bool on) {
	if ((y < 0) || (y >= LCD_PCD8544_HEIGHT)) return false;
	if (x1 >= LCD_PCD8544_WIDTH) x1 = LCD_PCD8544_WIDTH-1;
	if (x1 < 0) x1 = 0;
	if (x2 >= LCD_PCD8544_WIDTH) x2 = LCD_PCD8544_WIDTH-1;
	if (x2 < 0) x2 = 0;
	
	uint8_t mask = 0x01 << (y & 0x07);
	uint8_t line = y >> 3;
	
	if (x2 < x1) swap(x1, x2);
	for (uint8_t x = x1; x <= x2; x++) {
		if (on) _buffer[line][x] |= mask;
		else _buffer[line][x] &= ~mask;
	}
	return true;
}


void LCD_PCD8544_VG::bufferVLine(uint8_t x, uint8_t y1, uint8_t y2, bool on) {
	if (y2 < y1) swap(y1, y2);
	for (uint8_t y = y1; y <= y2; y++) bufferPixel(x, y, on);
}


void LCD_PCD8544_VG::bufferQuadrants(uint8_t x0, uint8_t y0, int8_t x, int8_t y, bool on, bool filled) {
	if (filled) {
		bufferHLine(x0-x, x0+x, y0+y, on);
		bufferHLine(x0-x, x0+x, y0-y, on);
	} else {
		bufferPixel(x0+x, y0+y, on);
		bufferPixel(x0+x, y0-y, on);
		bufferPixel(x0-x, y0-y, on);
		bufferPixel(x0-x, y0+y, on);
	}
}


/*
 *	From Wikipedia - Bresenham's line algorithm
 */
void LCD_PCD8544_VG::bufferCircle(uint8_t x0, uint8_t y0, uint8_t r, bool on, bool filled) {
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;

	bufferPixel(x0, y0+r, on);
	bufferPixel(x0, y0-r, on);
	bufferPixel(x0+r, y0, on);
	bufferPixel(x0-r, y0, on);
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		bufferQuadrants(x0, y0, x, y, on, filled);
		bufferQuadrants(x0, y0, y, x, on, filled);
	}
}

void LCD_PCD8544_VG::bufferEllipse(uint8_t x0, uint8_t y0, uint8_t a, uint8_t b, bool on, bool filled) {
	uint16_t aa = a*a;
	uint16_t bb = b*b;
	uint16_t aa2 = aa*2;
	uint16_t bb2 = bb*2;
	int x = 0;
	int y = b;
	int32_t fx = 0;
	int32_t fy = aa2*b;
	int32_t p = (int)(bb-(aa*b)+(0.25*aa)+0.5);
	
	bufferPixel(x0, y0+b, on);
	bufferPixel(x0, y0-b, on);
	bufferPixel(x0+a, y0, on);
	bufferPixel(x0-a, y0, on);
	while (fx < fy) {
		x++;
		fx += bb2;
		if (p < 0) p += fx + bb;
		else {
			y--;
			fy -= aa2;
			p += fx + bb - fy;
		}
		bufferQuadrants(x0, y0, x, y, on, filled);
	}
	p = (int)((bb*(x+0.5)*(x+0.5))+(aa*(y-1)*(y-1))-(aa*bb)+0.5);
	while (y > 0) {
		y--;
		fy -= aa2;
		if (p >= 0) p += aa - fy;
		else {
			x++;
			fx += bb2;
			p += fx + aa - fy;
		}
		bufferQuadrants(x0, y0, x, y, on, filled);
	}
}
