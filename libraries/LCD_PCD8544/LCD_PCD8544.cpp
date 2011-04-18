/*
 * LCD_PCD8544 - Interface with Nokia 5110 LCDs (or compatible) 
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


#include "LCD_PCD8544.h"

#include <WProgram.h>
#include <avr/pgmspace.h>

// If this was a ".h", it would get added to sketches when using
// the "Sketch -> Import Library..." menu on the Arduino IDE
#include "fonts.cpp"


LCD_PCD8544::LCD_PCD8544(uint8_t sce, uint8_t sclk, uint8_t sdin, uint8_t dc, uint8_t reset) {
	setup(sclk, sdin, dc, reset, sce);
}


void LCD_PCD8544::begin(uint8_t bias) {
	init(bias);
}


void LCD_PCD8544::stop() {
    clear();
    setPower(false);
}


void LCD_PCD8544::clear() {
    setXYAddress(0, 0);
    for (uint8_t l = 0; l < LCD_PCD8544_LINES; l++) {
	    for (uint8_t c = 0; c < LCD_PCD8544_WIDTH; c++) {
			send(LCD_PCD8544_DATA, 0x00);
		}
    }
    setXYAddress(0, 0);
}


void LCD_PCD8544::clearLine() {
    setXYAddress(0, _line);
    for (uint8_t c = 0; c < LCD_PCD8544_WIDTH; c++) {
        send(LCD_PCD8544_DATA, 0x00);
    }
    setXYAddress(0, _line);
}


void LCD_PCD8544::setPower(bool on) {
    send(LCD_PCD8544_CMD, on ? 0x20 : 0x24);
}


void LCD_PCD8544::display() {
    setPower(true);
}


void LCD_PCD8544::noDisplay() {
    setPower(false);
}


void LCD_PCD8544::setInverse(bool inverse) {
    send(LCD_PCD8544_CMD, inverse ? 0x0d : 0x0c);
}


void LCD_PCD8544::home() {
    setCursor(0, _line);
}


void LCD_PCD8544::setCursor(uint8_t column, uint8_t line) {
	setXYAddress(column * LCD_PCD8544_CHAR_WIDTH, line);
}


bool LCD_PCD8544::defineGlyph(uint8_t chr, const uint8_t *data) {
	if (chr >= LCD_PCD8544_MAX_USER_GLYPHS) return false;
	_userGlyph[chr] = data;
	return true;
}


void LCD_PCD8544::write(uint8_t chr) {
	uint8_t buffer[LCD_PCD8544_CHAR_WIDTH];

	if (!bufferChar(buffer, chr)) return;
	
    // Output one column at a time...
    for (uint8_t i = 0; i < LCD_PCD8544_CHAR_WIDTH; i++) {
        send(LCD_PCD8544_DATA, buffer[i]);
    }

    // Update the XY RAM position...
    _column = (_column + LCD_PCD8544_CHAR_WIDTH) % LCD_PCD8544_WIDTH;
    if (_column == 0) _line = (_line + 1) % LCD_PCD8544_LINES;
}


void LCD_PCD8544::drawBitmap(const uint8_t *data, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	// draw in the frame
	if ((x > LCD_PCD8544_WIDTH) || (y > LCD_PCD8544_HEIGHT)) return;
	
	// clip the bitmap to the screen
	uint8_t line = y / 8;
	uint8_t stopLine = (y + height > LCD_PCD8544_HEIGHT ? LCD_PCD8544_HEIGHT-y : height) / 8;
	if (stopLine == 0) stopLine = 1;
	uint8_t stopColumn = (x + width > LCD_PCD8544_WIDTH ? LCD_PCD8544_WIDTH-x : width);
	
	for (uint8_t l = 0; l < stopLine; l++) {
		setXYAddress(x, line + l);
		for (uint8_t c = 0; c < stopColumn; c++) {
			send(LCD_PCD8544_DATA, data[l*width+c]);
		}
	}
	setCursor(_column, _line);
}


void LCD_PCD8544::drawProgressBar(uint8_t column, uint8_t line, uint8_t size, uint8_t percentage, bool filled) {
	// sanity checks
	if ((size < 2) || (size > LCD_PCD8544_COLUMNS) || (column + size > LCD_PCD8544_COLUMNS) || (percentage > 100) || (line >= LCD_PCD8544_LINES)) return;

	uint8_t range = 4 + 6 * (size - 2);
	uint16_t filling = percentage * range / 100;
	
	setXYAddress(column * LCD_PCD8544_CHAR_WIDTH, line);
	for (uint8_t i=0; i<3; i++) send(LCD_PCD8544_DATA, 0x00);
	send(LCD_PCD8544_DATA, 0x7F);	
	if (filled) {
		for (uint8_t i=0; i<filling; i++) send(LCD_PCD8544_DATA, 0x7F);
		for (uint8_t i=filling; i<range; i++) send(LCD_PCD8544_DATA, 0x41);			
	} else {
		// draw an empty bar
		for (uint8_t i=0; i<range; i++) send(LCD_PCD8544_DATA, 0x41);
		// draw the double bar
		if (filling < 2) setXYAddress(column * LCD_PCD8544_CHAR_WIDTH + 4, line);
		else setXYAddress(column * LCD_PCD8544_CHAR_WIDTH + 4 + filling - 2, line);
		send(LCD_PCD8544_DATA, 0x7F);
		send(LCD_PCD8544_DATA, 0x7F);
		setXYAddress(column * LCD_PCD8544_CHAR_WIDTH + 4 + range, line);
	}
	send(LCD_PCD8544_DATA, 0x7F);
	for (uint8_t i=0; i<3; i++) send(LCD_PCD8544_DATA, 0x00);
}


// --------------- Protected ----------------------

void LCD_PCD8544::setup(uint8_t sclk, uint8_t sdin, uint8_t dc, uint8_t reset, uint8_t sce) {
	// store the hardware connections
	_sclk = sclk;
	_sdin = sdin;
	_dc = dc;
	_reset = reset;
	_sce = sce;

    // All pins are outputs 
    pinMode(_sclk, OUTPUT);
    pinMode(_sdin, OUTPUT);
    pinMode(_dc, OUTPUT);
    pinMode(_reset, OUTPUT);
    pinMode(_sce, OUTPUT);

    // Reset the controller
    digitalWrite(_reset, HIGH);
    digitalWrite(_sce, HIGH);
    digitalWrite(_reset, LOW);
}


void LCD_PCD8544::init(uint8_t bias) {
	
    delay(100);  
    digitalWrite(_reset, HIGH);

    // Sanitize the user glyphs...
    memset(_userGlyph, 0, sizeof(_userGlyph));

    _column = 0;
    _line = 0;

    // Set the LCD parameters...
    send(LCD_PCD8544_CMD, 0x21);  // extended instruction set control (H=1)
    send(LCD_PCD8544_CMD, 0x10 | bias);  // bias system 
    send(LCD_PCD8544_CMD, 0xC2);  // set Vop (3.06 + 66 * 0.06 = 7V)	-> 	control contrast
    send(LCD_PCD8544_CMD, 0x20);  // normal instruction set control (H=0)
	
    send(LCD_PCD8544_CMD, 0x09);  // all display segments on
    // Clear RAM contents...
    clear();

    // Activate LCD...
    send(LCD_PCD8544_CMD, 0x08);  // display blank
    send(LCD_PCD8544_CMD, 0x0c);  // normal mode (0x0d = inverse mode)
    delay(100);

    // Place the cursor at the origin...
    send(LCD_PCD8544_CMD, 0x80);
    send(LCD_PCD8544_CMD, 0x40);
}


void LCD_PCD8544::setXYAddress(uint8_t x, uint8_t line) {
    _column = (x % LCD_PCD8544_WIDTH);
    _line = (line % LCD_PCD8544_LINES);
    send(LCD_PCD8544_CMD, 0x80 | _column);
    send(LCD_PCD8544_CMD, 0x40 | _line); 
}


void LCD_PCD8544::send(uint8_t type, uint8_t data) {
    digitalWrite(_dc, type);
    digitalWrite(_sce, LOW);
    shiftOut(_sdin, _sclk, MSBFIRST, data);
    digitalWrite(_sce, HIGH);
}


bool LCD_PCD8544::bufferChar(uint8_t *buffer, uint8_t chr) {
	// deal with line feed
	if (chr == '\n') {
		_line = (_line + 1) % LCD_PCD8544_LINES;
		clearLine();
		return false;
	}
	
	// if unknown return false
	if ((chr < ' ') || (chr >= 0x80+LCD_PCD8544_MAX_USER_GLYPHS)) return false;
	
	if (chr >= 0x80) {
		if (_userGlyph[chr-0x80] == NULL) return false;
		for (uint8_t i=0; i<LCD_PCD8544_CHAR_WIDTH; i++) buffer[i] = _userGlyph[chr-0x80][i];
	} else {
		// Regular ASCII characters are kept in flash to save RAM...
		memcpy_P(buffer, &font_5x7[chr - ' '], LCD_PCD8544_CHAR_WIDTH-1);
		// One column between characters
		buffer[LCD_PCD8544_CHAR_WIDTH-1] = 0x00;
	}
	return true;
}

