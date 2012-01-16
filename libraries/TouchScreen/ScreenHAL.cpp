#include "ScreenHAL.hpp"
#include "fontBitmaps.hpp"

ScreenHAL::ScreenHAL(void) {
}


void ScreenHAL::setupScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset) {	
	_outPort = portOutputRegister(port);
	_inPort = portInputRegister(port);
	_portMode = portModeRegister(port);
	_rdPort = portOutputRegister(digitalPinToPort(rd));
	_rdHigh = digitalPinToBitMask(rd);
	_rdLow = ~_rdHigh;
	_wrPort = portOutputRegister(digitalPinToPort(wr));
	_wrHigh = digitalPinToBitMask(wr);
	_wrLow = ~_wrHigh;
	_cdPort = portOutputRegister(digitalPinToPort(cd));
	_cdBitMask = digitalPinToBitMask(cd);
	_rd = rd;
	_wr = wr;
	_cd = cd;
	_cs = cs;
	_reset = reset;
}


void ScreenHAL::initScreen() {
	pinMode(_rd, OUTPUT);
	pinMode(_wr, OUTPUT);
	pinMode(_cd, OUTPUT);
	if (_cs != 0xFF) pinMode(_cs, OUTPUT);
	
	// if we have no reset pin, the RD and WR have been OR'ed to create the reset
	// they have to be on the same port for this to work
	if (_reset == 0xFF) {
		*_rdPort &= ~(_rdHigh | _wrHigh);
		delay(200);
		*_rdPort |= (_rdHigh | _wrHigh);
	} else {
		pinMode(_reset, OUTPUT);
		digitalWrite(_reset, LOW);
		delay(200);
		digitalWrite(_reset, HIGH);
	}
	
	_width = getWidthImpl();
	_height = getHeightImpl();
	_spiUsed = 0;
	_busTaken = 0;
	_rotation = 0;
	setFontSize(1);
	_grabBus();
	initScreenImpl();
	_releaseBus();
}


void ScreenHAL::setRotation(uint8_t rotation) {
	_rotation = rotation;
	if (_rotation > SCREEN_ROTATION_270) _rotation = SCREEN_ROTATION_0;
	if ((_rotation == SCREEN_ROTATION_0) || (_rotation == SCREEN_ROTATION_180)) {
		_width = getWidthImpl();
		_height = getHeightImpl();
	} else {
		_width = getHeightImpl();
		_height = getWidthImpl();
	}
	_grabBus();
	setRotationImpl();
	_releaseBus();
}


uint8_t ScreenHAL::getRotation() {
	return _rotation;
}


void ScreenHAL::getTransformedXY(uint16_t *x, uint16_t *y, uint16_t maxX, uint16_t maxY, uint8_t rotation) {
	uint32_t X = *x;
	uint32_t Y = *y;
	
	if (rotation == SCREEN_ROTATION_180) {
		*x = (maxX - X) * getWidthImpl() / maxX;
		*y = (maxY - Y) * getHeightImpl() / maxY;
	} else if (rotation == SCREEN_ROTATION_90) {
		*x = (maxY - Y) * getHeightImpl() / maxY;
		*y = X * getWidthImpl() / maxX;
	} else if (rotation == SCREEN_ROTATION_270) {
		*x = Y * getWidthImpl() / maxY;
		*y = (maxX - X) * getHeightImpl() / maxX;
	} else {
		*x = X * getWidthImpl() / maxX;
		*y = Y * getHeightImpl() / maxY;
	} 
}


bool ScreenHAL::isVertical() {
	return ((_rotation == SCREEN_ROTATION_0) || (_rotation == SCREEN_ROTATION_180));
}


uint16_t ScreenHAL::create565Color(uint8_t r, uint8_t g, uint8_t b) {
	uint16_t color;
	color = r >> 3;
	color <<= 6;
	color |= g >> 2;
	color <<= 5;
	color |= b >> 3;
	return color;
}

uint16_t ScreenHAL::getForegroundColor() {
	return _foregroundColor;
}

void ScreenHAL::setForegroundColor(uint16_t color) {
	_foregroundColor = color;
}


uint16_t ScreenHAL::getBackgroundColor() {
	return _backgroundColor;
}

void ScreenHAL::setBackgroundColor(uint16_t color) {
	_backgroundColor = color;
}


uint8_t ScreenHAL::getFontSize() {
	return _fontSize;
}

void ScreenHAL::setFontSize(uint8_t size) {
	_fontSize = size;
	if (_fontSize > 4) _fontSize = 4;
	if (_fontSize == 0) _fontSize = 1;
	_fontScale = (_fontSize <= FONT_LAST ? 1 : 2);
	_fontDef = &fontDefinition_small;
	if ((_fontSize == 2) || (_fontSize == 4)) _fontDef = &fontDefinition_medium;
}


void ScreenHAL::setCursor(uint8_t x, uint8_t y) {
	_x = x;
	_y = y;
}


uint16_t ScreenHAL::getWidth() {
	return _width;
}

uint16_t ScreenHAL::getHeight() {
	return _height;
}


void ScreenHAL::drawChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, bool overlay, bool grabAndReleaseBus) {
	uint8_t validFontSize = (fontSize > FONT_LAST*2 ? FONT_LAST*2 : (fontSize < 1 ? 1 : fontSize));
	uint8_t fontScale = (validFontSize <= FONT_LAST ? 1 : 2);
	fontDefinition_t *fontDef = &fontDefinition_small;
	if ((x + fontDef->width * fontScale >= _width) || (y + fontDef->height * fontScale >= _height)) return;
	if ((validFontSize == 2) || (validFontSize == 4)) fontDef = &fontDefinition_medium;
	if ((chr < fontDef->firstChar) || (chr > fontDef->lastChar)) return;
	if (grabAndReleaseBus) _grabBus();
	_drawValidChar(chr, x, y, color, validFontSize, fontDef, fontScale, overlay);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawString(char *s, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, bool overlay, bool grabAndReleaseBus) {
	uint8_t validFontSize = (fontSize > FONT_LAST*2 ? FONT_LAST*2 : (fontSize < 1 ? 1 : fontSize));
	uint8_t fontScale = (validFontSize <= FONT_LAST ? 1 : 2);
	fontDefinition_t *fontDef = &fontDefinition_small;
	if ((validFontSize == 2) || (validFontSize == 4)) fontDef = &fontDefinition_medium;
	if ((x + fontDef->width * fontScale >= _width) || (y + fontDef->height * fontScale >= _height)) return;
	
	if (grabAndReleaseBus) _grabBus();
	while (s[0] != 0) {
		if (s[0] == '\n') {
			y += (fontDef->height + fontDef->lineSpacing) * fontScale;
			if (y >= _height) return;
			x = 0;
		} else if (s[0] == '\r') {
			// skip the character
			continue;
		}
		uint8_t validChr = s[0];
		if ((validChr < fontDef->firstChar) || (validChr > fontDef->lastChar)) validChr = '?';
		_drawValidChar(validChr, x, y, _foregroundColor, validFontSize, fontDef, fontScale, overlay);
		x += (fontDef->width + fontDef->charSpacing) * fontScale;
		if (x > _width) {
			x = 0;
			y += (fontDef->height + fontDef->lineSpacing) * fontScale;
			if (y > _height) return;
		}
		s++;
	}
	if (grabAndReleaseBus) _releaseBus();
}


#if ARDUINO >= 100
size_t ScreenHAL::write(uint8_t chr) {
#else
void ScreenHAL::write(uint8_t chr) {
#endif
	if (chr == '\n') {
		_y += (_fontDef->height + _fontDef->lineSpacing) * _fontScale;
		if (_y > _height) _y = 0;
		_x = 0;
#if ARDUINO >= 100
		return 1;
#else
		return;
#endif
	}
	if (chr == '\r') {
		// skip the character
#if ARDUINO >= 100
		return 1;
#else
		return;
#endif
	}
	uint8_t validChr = chr;
	if ((validChr < _fontDef->firstChar) || (validChr > _fontDef->lastChar)) validChr = '?';
	_drawValidChar(validChr, _x, _y, _foregroundColor, _fontSize, _fontDef, _fontScale, false);
	_x += (_fontDef->width + _fontDef->charSpacing) * _fontScale;
	if (_x > _width) {
		_x = 0;
		_y += (_fontDef->height + _fontDef->lineSpacing) * _fontScale;
		if (_y > _height) _y = 0;
	}
#if ARDUINO >= 100
	return 1;
#endif
}


void ScreenHAL::drawPixel(uint16_t x, uint16_t y, uint16_t color, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	_drawBoundedPixel(x, y, color);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawBitmap(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool hasTransparency, uint16_t transparentColor, bool grabAndReleaseBus) {
	if ((x >= _width) || (y >= _height)) return;
	if (grabAndReleaseBus) _grabBus();
	drawBitmapImpl(bitmap, x, y, width, height, hasTransparency, transparentColor);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawPattern(uint8_t *pattern, uint8_t orientation, uint16_t x, uint16_t y, uint8_t width, uint8_t height, 
		uint16_t color, uint16_t backColor, uint8_t scale, bool overlay, bool grabAndReleaseBus) {
	uint16_t buffer[SCREENHAL_MAX_BITMAP_SIZE];
	uint8_t unpacked[SCREENHAL_MAX_BITMAP_SIZE];

	if ((x >= _width) || (y >= _height)) return;
	if (width * height * scale > SCREENHAL_MAX_BITMAP_SIZE) return;
	// unpack the pattern
	_unpackPattern(pattern, unpacked, width, height, 0, 0, orientation);
	// scale and colorize it
	_scaleAndColorizeUnpackedPattern(unpacked, buffer, color, backColor, width, height, scale);
	// draw it
	if (grabAndReleaseBus) _grabBus();
	drawBitmapImpl(buffer, x, y, width*scale, height*scale, overlay, backColor);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawHorizontalLine(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	_fillBoundedRectangle(x1, y-(thickness-1)/2, x2, y+thickness/2, color);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawVerticalLine(uint16_t x, uint16_t y1, uint16_t y2, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	_fillBoundedRectangle(x-(thickness-1)/2, y1, x+thickness/2, y2, color);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	drawHorizontalLine(x1, x2, y1, color, thickness, false);
	drawHorizontalLine(x1, x2, y2, color, thickness, false);
	drawHorizontalLine(x1, y1, y2, color, thickness, false);
	drawHorizontalLine(x2, y1, y2, color, thickness, false);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	_fillBoundedRectangle(x1, y1, x2, y2, color);
	if (grabAndReleaseBus) _releaseBus();
}


/*
 *	From Wikipedia - Bresenham's line algorithm
 */
void ScreenHAL::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	uint8_t dx = abs(x2-x1);
	if (dx == 0) {
		drawVerticalLine(x1, y1, y2, color, thickness, grabAndReleaseBus);
		return;
	}
	uint8_t dy = abs(y2-y1);
	if (dy == 0) {
		drawHorizontalLine(x1, x2, y1, color, thickness, grabAndReleaseBus);
		return;
	}
	
	int8_t sx = -1;
	if (x1 < x2) sx = 1;
	int8_t sy = -1;
	if (y1 < y2) sy = 1;
	int8_t err = dx - dy;
	uint8_t d1 = (thickness-1) / 2;
	uint8_t d2 = thickness / 2;

	if (grabAndReleaseBus) _grabBus();	
	while (1) {
		if (thickness == 1) drawPixelImpl(x1, y1, color);
		else _fillBoundedRectangle(x1-d1, y1-d1, x1+d1, y1+d2, color);
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
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	drawLine(x0, y0, x1, y1, color, thickness, false);
	drawLine(x1, y1, x2, y2, color, thickness, false);
	drawLine(x2, y2, x0, y0, color, thickness, false);
	if (grabAndReleaseBus) _releaseBus();	
}

#define swap(a, b) { int16_t t = a; a = b; b = t; }

void ScreenHAL::fillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color, bool grabAndReleaseBus) {
	if (y0 > y1) {
		swap(y0, y1); 
		swap(x0, x1);
	}
	if (y1 > y2) {
		swap(y2, y1); 
		swap(x2, x1);
	}
	if (y0 > y1) {
		swap(y0, y1); 
		swap(x0, x1);
	}

	int32_t dx1, dx2, dx3;
	int32_t sx1, sx2, sy;

	sx2 = (int32_t)x0 * (int32_t)1000;
	sx1 = sx2;
	sy = y0;

	// Calculate interpolation deltas
	if (y1-y0 > 0) dx1 = ((x1-x0)*1000)/(y1-y0);
	else dx1 = 0;
	if (y2-y0 > 0) dx2 = ((x2-x0)*1000)/(y2-y0);
	else dx2 = 0;
	if (y2-y1 > 0) dx3 = ((x2-x1)*1000)/(y2-y1);
	else dx3 = 0;

	// Render scanlines 
	if (grabAndReleaseBus) _grabBus();	
	if (dx1 > dx2) {
		for (; sy<=y1; sy++, sx1+=dx2, sx2+=dx1) _fillBoundedRectangle(sx1/1000, sy, sx2/1000, sy, color);
		sx2 = x1 * 1000;
		sy = y1;
		for (; sy<=y2; sy++, sx1+=dx2, sx2+=dx3) _fillBoundedRectangle(sx1/1000, sy, sx2/1000, sy, color);
	} else {
		for (; sy<=y1; sy++, sx1+=dx1, sx2+=dx2) _fillBoundedRectangle(sx1/1000, sy, sx2/1000, sy, color);
		sx1 = x1*1000;
		sy = y1;
		for (; sy<=y2; sy++, sx1+=dx3, sx2+=dx2) _fillBoundedRectangle(sx1/1000, sy, sx2/1000, sy, color);
	}
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t arc, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillArc(x0, y0, r, color, arc, thickness, false);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::fillArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t arc, uint16_t color, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillArc(x0, y0, r, color, arc, 0, true);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillArc(x0, y0, r, color, SCREEN_ARC_ALL, thickness, false);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillArc(x0, y0, r, color, SCREEN_ARC_ALL, 0, true);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawRoundedRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	if (x2 > x1) swap(x1, x2);
	if (y2 > y1) swap(y1, y2);
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillArc(x1+r, y1+r, r, color, SCREEN_ARC_NW, thickness, false);
	drawHorizontalLine(x1+r, x2-r, y1, color, thickness, false);
	_drawOrFillArc(x2-r, y1+r, r, color, SCREEN_ARC_NE, thickness, false);
	drawVerticalLine(x2, y1+r, y2-r, color, thickness, false);
	_drawOrFillArc(x1+r, y2-r, r, color, SCREEN_ARC_SW, thickness, false);
	drawHorizontalLine(x1+r, x2-r, y2, color, thickness, false);
	_drawOrFillArc(x2-r, y2-r, r, color, SCREEN_ARC_SE, thickness, false);
	drawVerticalLine(x1, y1+r, y2-r, color, thickness, false);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::fillRoundedRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color, bool grabAndReleaseBus) {
	if (x2 > x1) swap(x1, x2);
	if (y2 > y1) swap(y1, y2);
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillArc(x1+r, y1+r, r, color, SCREEN_ARC_NW, 0, true);
	_drawOrFillArc(x2-r, y1+r, r, color, SCREEN_ARC_NE, 0, true);
	_drawOrFillArc(x1+r, y2-r, r, color, SCREEN_ARC_SW, 0, true);
	_drawOrFillArc(x2-r, y2-r, r, color, SCREEN_ARC_SE, 0, true);
	_fillBoundedRectangle(x1, y1+r, x2, y2-r, color);
	_fillBoundedRectangle(x1+r, y1, x2-r, y1+r, color);
	_fillBoundedRectangle(x1+r, y2-r, x2-r, y2, color);
	if (grabAndReleaseBus) _releaseBus();
}

/* ---------------- Protected functions ------------------------ */

/* ---------------- Virtual functions -------------------------- */
/* ---------------- NEED TO BE IMPLEMENTED BY CHILD CLASS ------ */

void ScreenHAL::initScreenImpl() {
	// needs to be implemented by the class inheriting from this class
}

void ScreenHAL::setRotationImpl() {
	// needs to be implemented by the class inheriting from this class
}

void ScreenHAL::drawPixelImpl(uint16_t x, uint16_t y, uint16_t color) {
	// needs to be implemented by the class inheriting from this class
}

// Fills the rectangle lx,ly -> hx, hy
// this function assumes that lx <= hx and ly <= hy
void ScreenHAL::fillRectangleImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
	// needs to be implemented by the class inheriting from this class
}

// Draws the bitmap
// if hasTransparency, replaces the transparentColor in the bitmap by the existing color on the screen at the pixel place
void ScreenHAL::drawBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool hasTransparency, uint16_t transparentColor) {
}

// returns the width of the non rotated screen
uint16_t ScreenHAL::getWidthImpl() {
	return 240;
}

// returns the height of the non rotated screen
uint16_t ScreenHAL::getHeightImpl() {
	return 160;
}

/* ---------------- End of virtual functions --------------------- */

#define pulseWR() { *_wrPort &= _wrLow; *_wrPort |= _wrHigh; }

void ScreenHAL::writeCommand(uint8_t command) {
	// set command mode
	*_cdPort &= ~_cdBitMask;
	// write the command
	*_outPort = command;
	pulseWR();
	// set data mode
	*_cdPort |= _cdBitMask;
}

void ScreenHAL::writeData(uint8_t data) {
	*_outPort = data;
	pulseWR();
}

void ScreenHAL::write16bData(uint16_t data) {
	*_outPort = data >> 8;
	pulseWR();
	*_outPort = data;
	pulseWR();
}

void ScreenHAL::write16bDataBuffer(uint16_t *data, uint32_t len) {
	for (uint32_t i=0; i<len; i++) { 
		*_outPort = (uint8_t) (data[i] >> 8);
		pulseWR();
		*_outPort = (uint8_t) (data[i] & 0x0FF);
		pulseWR();
	}
}

void ScreenHAL::read16bDataBuffer(uint16_t *data, uint32_t len) {
	*_portMode = 0x00;	
	// one dummy read
	*_rdPort &= _rdLow;
	*_rdPort &= _rdLow;
	*_rdPort |= _rdHigh; 
	data[0] = *_inPort;
	// then read the data
	for (uint32_t i=0; i<len; i++) { 
		*_rdPort &= _rdLow;
		*_rdPort &= _rdLow;
		*_rdPort |= _rdHigh; 
		data[i] = *_inPort;
		data[i] = data[i] << 8;
		*_rdPort &= _rdLow;
		*_rdPort &= _rdLow;
		*_rdPort |= _rdHigh; 
		data[i] |= *_inPort;
	}
	*_portMode = 0xFF;
}


/* ---------------- Private functions ------------------------ */

void ScreenHAL::_grabBus() {
	if (_busTaken) return;
	// if SPI is active, disable it but remember that we disabled it
	if (SPCR & _BV(SPE)) {
		SPCR &= ~_BV(SPE);
		_spiUsed = 1;
	}
	// set the direction to output
	*_portMode = 0xFF;
	// select the screen
	if (_cs != 0xFF) digitalWrite(_cs, LOW);
	// put the screen in data mode
	*_cdPort |= _cdBitMask;
	_busTaken = 1;
}

void ScreenHAL::_releaseBus() {
	// restore the SPI if it was active
	if (_spiUsed) SPCR |= _BV(SPE);
	// unselect the screen
	if (_cs != 0xFF) digitalWrite(_cs, HIGH);
	_busTaken = 0;
}


void ScreenHAL::_drawValidChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, fontDefinition_t *fontDef, uint8_t fontScale, boolean overlay) {
	uint8_t pattern[FONT_MAX_PATTERN];
	uint8_t unpacked[FONT_MAX_SPACE];
	uint16_t buffer[FONT_MAX_SPACE*2*2];

	if (fontSize == 1) memcpy_P(pattern, &font_small[chr - fontDef->firstChar], fontDef->orientation == FONT_ORIENTATION_HORIZONTAL ? fontDef->height : fontDef->width);
	else if (fontSize == 2) memcpy_P(pattern, &font_medium[chr - fontDef->firstChar], fontDef->orientation == FONT_ORIENTATION_HORIZONTAL ? fontDef->height : fontDef->width);
	// unpack the pattern
	_unpackPattern(pattern, unpacked, fontDef->width, fontDef->height, fontDef->charSpacing, fontDef->lineSpacing, fontDef->orientation);
	// scale and colorize it
	_scaleAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef->width+fontDef->charSpacing, fontDef->height+fontDef->lineSpacing, fontScale);
	// draw it
	drawBitmapImpl(buffer, x, y, (fontDef->width+fontDef->charSpacing)*fontScale, (fontDef->height+fontDef->lineSpacing)*fontScale, overlay, _backgroundColor);
}


void ScreenHAL::_unpackPattern(uint8_t *pattern, uint8_t *unpacked, uint8_t width, uint8_t height, uint8_t borderRight, uint8_t borderBottom, uint8_t orientation) {
	for (uint16_t i=0; i<(width+borderRight)*(height+borderBottom); i++) unpacked[i] = 0;
	for (uint8_t l=0; l<height; l++) {
		for (uint8_t c=0; c<width; c++) {
			if (orientation == FONT_ORIENTATION_HORIZONTAL) {
				uint8_t patternByte = pattern[l*(1+width/8)+c/8];
				unpacked[l*(width+borderRight)+c] = patternByte & (0x01 << (c & 0x07));
			} else {
				uint8_t patternByte = pattern[c*(1+height/8)+l/8];
				unpacked[l*(width+borderRight)+c] = patternByte & (0x01 << (l & 0x07));
			}
		}
	}
}


void ScreenHAL::_scaleAndColorizeUnpackedPattern(uint8_t *unpacked, uint16_t *scaled, uint16_t onColor, uint16_t offColor, uint8_t width, uint8_t height, uint8_t scale) {
	for (uint16_t i=0; i<width*height; i++) scaled[i] = offColor;
	for (uint8_t l=0; l<height; l++) {
		for (uint8_t c=0; c<width; c++) {
			if (scale== 1) scaled[l*width+c] = onColor;
			else {
				for (uint8_t pl=0; pl<scale; pl++) {
					for (uint8_t pc=0; pc<scale; pc++) {
						if (unpacked[l*width+c]) scaled[(l*scale+pl)*width+c*scale+pc] = onColor;
					}
				}
			}
		}
	}
}


void ScreenHAL::_fillBoundedRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
	if (x2 < x1) swap(x1, x2);
	if (y2 < y1) swap(y1, y2);
	if (x1 < 0) x1 = 0;
	if (x1 >= _width) return;
	if (x2 >= _width) x2 = _width-1;
	if (y1 < 0) y1 = 0;
	if (y1 >= _height) return;
	if (y2 >= _height) y2 = _height-1;
	fillRectangleImpl(x1, y1, x2, y2, color);
}


void ScreenHAL::_drawBoundedPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x < 0) || (x >= _width)) return;
	if ((y < 0) || (y >= _height)) return;
	drawPixelImpl(x, y, color);
}

/*
 * From Wikipedia: Andres arc drawing algorithm
 */	
void ScreenHAL::_drawOrFillArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t arc, uint16_t color, uint8_t thickness, bool fill) {
	if ((x0 >= _width) || (y0 >= _height)) return;

	int16_t x = 0;
	int16_t y = r;
	int16_t d = r - 1;
	uint8_t t1 = (thickness-1)/2;
	uint8_t t2 = thickness/2;
	
	while (y >= x) {
		if (fill) {
			if (arc & SCREEN_ARC_NNE) _fillBoundedRectangle(x0, y0+y, x0+x, y0+y, color);
			if (arc & SCREEN_ARC_NEE) _fillBoundedRectangle(x0, y0+x, x0+y, y0+x, color);
			if (arc & SCREEN_ARC_NNW) _fillBoundedRectangle(x0, y0+y, x0-x, y0+y, color);
			if (arc & SCREEN_ARC_NWW) _fillBoundedRectangle(x0, y0+x, x0-y, y0+x, color);
			if (arc & SCREEN_ARC_SSE) _fillBoundedRectangle(x0, y0-y, x0+x, y0-y, color);
			if (arc & SCREEN_ARC_SEE) _fillBoundedRectangle(x0, y0-x, x0+y, y0-x, color);
			if (arc & SCREEN_ARC_SSW) _fillBoundedRectangle(x0, y0-y, x0-x, y0-y, color);
			if (arc & SCREEN_ARC_SWW) _fillBoundedRectangle(x0, y0-x, x0-y, y0-x, color);		
		} else if (thickness != 1) {
			if (arc & SCREEN_ARC_NNE) _fillBoundedRectangle(x0+x-t1, y0+y-t1, x0+x+t2, y0+y+t2, color);
			if (arc & SCREEN_ARC_NEE) _fillBoundedRectangle(x0+y-t1, y0+x-t1, x0+y+t2, y0+x+t2, color);
			if (arc & SCREEN_ARC_NNW) _fillBoundedRectangle(x0-x-t1, y0+y-t1, x0-x+t2, y0+y+t2, color);
			if (arc & SCREEN_ARC_NWW) _fillBoundedRectangle(x0-y-t1, y0+x-t1, x0-y+t2, y0+x+t2, color);
			if (arc & SCREEN_ARC_SSE) _fillBoundedRectangle(x0+x-t1, y0-y-t1, x0+x+t2, y0-y+t2, color);
			if (arc & SCREEN_ARC_SEE) _fillBoundedRectangle(x0+y-t1, y0-x-t1, x0+y+t2, y0-x+t2, color);
			if (arc & SCREEN_ARC_SSW) _fillBoundedRectangle(x0-x-t1, y0-y-t1, x0-x+t2, y0-y+t2, color);
			if (arc & SCREEN_ARC_SWW) _fillBoundedRectangle(x0-y-t1, y0-x-t1, x0-y+t2, y0-x+t2, color);		
		} else {
			if (arc & SCREEN_ARC_NNE) _drawBoundedPixel(x0+x, y0+y, color);
			if (arc & SCREEN_ARC_NEE) _drawBoundedPixel(x0+y, y0+x, color);
			if (arc & SCREEN_ARC_NNW) _drawBoundedPixel(x0-x, y0+y, color);
			if (arc & SCREEN_ARC_NWW) _drawBoundedPixel(x0-y, y0+x, color);
			if (arc & SCREEN_ARC_SSE) _drawBoundedPixel(x0+x, y0-y, color);
			if (arc & SCREEN_ARC_SEE) _drawBoundedPixel(x0+y, y0-x, color);
			if (arc & SCREEN_ARC_SSW) _drawBoundedPixel(x0-x, y0-y, color);
			if (arc & SCREEN_ARC_SWW) _drawBoundedPixel(x0-y, y0-x, color);
		}
		if (d >= 2*x) {
			d = d - 2*x - 1;
			x = x + 1;
		} else if (d <= 2*(r-y)) {
			d = d + 2*y -1;
			y = y - 1;
		} else {
			d = d + 2*(y-x-1);
			y = y - 1;
			x = x + 1;
		}
	}
}

