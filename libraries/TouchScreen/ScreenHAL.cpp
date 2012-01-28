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
		delay(20);
		*_rdPort |= (_rdHigh | _wrHigh);
	} else {
		pinMode(_reset, OUTPUT);
		digitalWrite(_reset, LOW);
		delay(20);
		digitalWrite(_reset, HIGH);
	}
	
	_width = getWidthImpl();
	_height = getHeightImpl();
	_spiUsed = 0;
	_busTaken = 0;
	_rotation = 0;
	_margin = 0;
	setFont(1, FONT_PLAIN, NO_OVERLAY);
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
	setRotationImpl(_rotation);
	_releaseBus();
}


uint8_t ScreenHAL::getRotation() {
	return _rotation;
}


void ScreenHAL::getRotatedXY(int16_t *x, int16_t *y, uint8_t rotation) {
	if (rotation == SCREEN_ROTATION_0) return;

	int16_t X = *x;
	int16_t Y = *y;
	
	if (rotation == SCREEN_ROTATION_180) {
		*x = getWidthImpl() - X;
		*y = getHeightImpl() - Y;
	} else if (rotation == SCREEN_ROTATION_90) {
		*x = getHeightImpl() - Y;
		*y = X;
	} else if (rotation == SCREEN_ROTATION_270) {
		*x = Y;
		*y = getWidthImpl() - X;
	} 
}


bool ScreenHAL::isVertical() {
	return ((_rotation == SCREEN_ROTATION_0) || (_rotation == SCREEN_ROTATION_180));
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


uint16_t ScreenHAL::getMargin() {
	return _margin;
}

void ScreenHAL::setMargin(uint16_t margin) {
	_margin = margin;
}


uint8_t ScreenHAL::getFontSize() {
	return _fontSize;
}

bool ScreenHAL::isFontBold() {
	return _isFontBold;
}

bool ScreenHAL::isFontOverlay() {
	return _isFontOverlay;
}

void ScreenHAL::setFont(uint8_t size, bool bold, bool overlay) {
	_fontSize = size;
	_isFontBold = bold;
	_isFontOverlay = overlay;
	if (_fontSize > FONT_LAST_DEF*2) _fontSize = FONT_LAST_DEF*2;
	if (_fontSize == 0) _fontSize = 1;
	_fontScale = (_fontSize <= FONT_LAST_DEF ? 1 : 2);
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


void ScreenHAL::drawChar(uint8_t chr, int16_t x, int16_t y, uint16_t color, uint8_t fontSize, bool isBold, bool overlay, bool grabAndReleaseBus) {
	if ((x < 0) || (y < 0)) return;
	uint8_t validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
	uint8_t fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
	fontDefinition_t *fontDef = &fontDefinition_small;
	if ((validFontSize == 2) || (validFontSize == 4)) fontDef = &fontDefinition_medium;
	if ((x + fontDef->width * fontScale >= _width) || (y + fontDef->height * fontScale >= _height)) return;
	if ((chr < fontDef->firstChar) || (chr > fontDef->lastChar)) return;
	if (grabAndReleaseBus) _grabBus();
	_drawValidChar(chr, x, y, color, validFontSize, fontDef, fontScale, isBold, overlay);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawString(char *s, int16_t x, int16_t y, uint16_t color, uint8_t fontSize, bool isBold, bool overlay, bool grabAndReleaseBus) {
	if ((x < 0) || (y < 0)) return;
	uint8_t validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
	uint8_t fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
	fontDefinition_t *fontDef = &fontDefinition_small;
	if ((validFontSize == 2) || (validFontSize == 4)) fontDef = &fontDefinition_medium;
	if ((x + fontDef->width * fontScale >= _width) || (y + fontDef->height * fontScale >= _height)) return;
	
	if (grabAndReleaseBus) _grabBus();
	while (s[0] != 0) {
		if (s[0] == '\n') {
			y += (fontDef->height + fontDef->lineSpacing + (isBold ? 1 : 0)) * fontScale;
			if (y >= _height) break;
			x = 0;
		} else if (s[0] == '\r') {
			// skip the character
		} else {
			uint8_t validChr = s[0];
			if ((validChr < fontDef->firstChar) || (validChr > fontDef->lastChar)) validChr = '?';
			_drawValidChar(validChr, x, y, color, validFontSize, fontDef, fontScale, isBold, overlay);
			x += (fontDef->width + fontDef->charSpacing) * fontScale;
			if (x > _width) {
				x = 0;
				y += (fontDef->height + fontDef->lineSpacing + (isBold ? 1 : 0)) * fontScale;
				if (y >= _height) break;
			}
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
	_grabBus();
	if (chr == '\n') {
		_y += (_fontDef->height + _fontDef->lineSpacing + (_isFontBold ? 1 : 0)) * _fontScale;
		if (_y > _height - 2*_margin) _y = 0;
		_x = 0;
	} else if (chr == '\r') {
		// skip the character
	} else {
		uint8_t validChr = chr;
		if ((validChr < _fontDef->firstChar) || (validChr > _fontDef->lastChar)) validChr = '?';
		_drawValidChar(validChr, _x + _margin, _y + _margin, _foregroundColor, _fontSize, _fontDef, _fontScale, _isFontBold, _isFontOverlay);
		_x += (_fontDef->width + _fontDef->charSpacing) * _fontScale;
		if (_x > _width - 2*_margin) {
			_x = 0;
			_y += (_fontDef->height + _fontDef->lineSpacing + (_isFontBold ? 1 : 0)) * _fontScale;
			if (_y > _height-2*_margin) _y = 0;
		}
	}
	_releaseBus();
#if ARDUINO >= 100
	return 1;
#endif
}


void ScreenHAL::drawPixel(int16_t x, int16_t y, uint16_t color, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	_drawBoundedPixel(x, y, color);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawHorizontalLine(int16_t x1, int16_t x2, int16_t y, uint16_t color, int8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	if (thickness <= 1) _fillBoundedArea(x1, y, x2, y, color);
	else _fillBoundedArea(x1, y-(thickness-1)/2, x2, y+thickness/2, color);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawVerticalLine(int16_t x, int16_t y1, int16_t y2, uint16_t color, int8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	if (thickness <= 1) _fillBoundedArea(x, y1, x, y2, color);
	else _fillBoundedArea(x-(thickness-1)/2, y1, x+thickness/2, y2, color);
	if (grabAndReleaseBus) _releaseBus();
}


/*
 *	From Wikipedia - Bresenham's line algorithm
 */
void ScreenHAL::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int8_t thickness, bool grabAndReleaseBus) {
	int16_t dx = abs(x2-x1);
	if (dx == 0) {
		drawVerticalLine(x1, y1, y2, color, thickness, grabAndReleaseBus);
		return;
	}
	int16_t dy = abs(y2-y1);
	if (dy == 0) {
		drawHorizontalLine(x1, x2, y1, color, thickness, grabAndReleaseBus);
		return;
	}
	
	int8_t sx = -1;
	if (x1 < x2) sx = 1;
	int8_t sy = -1;
	if (y1 < y2) sy = 1;
	int16_t err = dx - dy;
	int8_t d1 = (thickness-1) / 2;
	int8_t d2 = thickness / 2;

	if (grabAndReleaseBus) _grabBus();	
	while (1) {
		if (thickness == 1) drawPixelImpl(x1, y1, color);
		else _fillBoundedArea(x1-d1, y1-d1, x1+d2, y1+d2, color);
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


void ScreenHAL::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int8_t thickness, bool grabAndReleaseBus) {
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
		for (; sy<=y1; sy++, sx1+=dx2, sx2+=dx1) _fillBoundedArea(sx1/1000, sy, sx2/1000, sy, color);
		sx2 = x1 * 1000;
		sy = y1;
		for (; sy<=y2; sy++, sx1+=dx2, sx2+=dx3) _fillBoundedArea(sx1/1000, sy, sx2/1000, sy, color);
	} else {
		for (; sy<=y1; sy++, sx1+=dx1, sx2+=dx2) _fillBoundedArea(sx1/1000, sy, sx2/1000, sy, color);
		sx1 = x1*1000;
		sy = y1;
		for (; sy<=y2; sy++, sx1+=dx3, sx2+=dx2) _fillBoundedArea(sx1/1000, sy, sx2/1000, sy, color);
	}
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color, int8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	drawHorizontalLine(x, x+width-1, y, color, thickness, false);
	drawHorizontalLine(x, x+width-1, y+height-1, color, thickness, false);
	drawVerticalLine(x, y, y+height-1, color, thickness, false);
	drawVerticalLine(x+width-1, y, y+height-1, color, thickness, false);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::fillRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();
	_fillBoundedArea(x, y, x+width-1, y+height-1, color);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawArc(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, int8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillOctant(x0, y0, r, octant, color, thickness, false);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::fillArc(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillOctant(x0, y0, r, octant, color, 0, true);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawCircle(int16_t x0, int16_t y0, uint16_t r, uint16_t color, int8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillOctant(x0, y0, r, SCREEN_ARC_ALL, color, thickness, false);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::fillCircle(int16_t x0, int16_t y0, uint16_t r, uint16_t color, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillOctant(x0, y0, r, SCREEN_ARC_ALL, color, 0, true);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t r, uint16_t color, int8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillOctant(x+r, y+r, r, SCREEN_ARC_NW, color, thickness, false);
	drawHorizontalLine(x+r, x+width-1-r, y, color, thickness, false);
	_drawOrFillOctant(x+width-1-r, y+r, r, SCREEN_ARC_NE, color, thickness, false);
	drawVerticalLine(x+width-1, y+r, y+height-1-r, color, thickness, false);
	_drawOrFillOctant(x+r, y+height-1-r, r, SCREEN_ARC_SW, color, thickness, false);
	drawHorizontalLine(x+r, x+width-1-r, y+height-1, color, thickness, false);
	_drawOrFillOctant(x+width-1-r, y+height-1-r, r, SCREEN_ARC_SE, color, thickness, false);
	drawVerticalLine(x, y+r, y+height-1-r, color, thickness, false);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::fillRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t r, uint16_t color, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) _grabBus();	
	_drawOrFillOctant(x+r, y+r, r, SCREEN_ARC_NW, color, 0, true);
	_drawOrFillOctant(x+width-1-r, y+r, r, SCREEN_ARC_NE, color, 0, true);
	_drawOrFillOctant(x+r, y+height-1-r, r, SCREEN_ARC_SW, color, 0, true);
	_drawOrFillOctant(x+width-1-r, y+height-1-r, r, SCREEN_ARC_SE, color, 0, true);
	_fillBoundedArea(x, y+r, x+width-1, y+height-1-r, color);
	_fillBoundedArea(x+r, y, x+width-1-r, y+r, color);
	_fillBoundedArea(x+r, y+height-1-r, x+width-1-r, y+height-1, color);
	if (grabAndReleaseBus) _releaseBus();
}


uint16_t *ScreenHAL::retrieveBitmap(uint16_t *buffer, int16_t x, int16_t y, uint16_t width, uint16_t height, bool grabAndReleaseBus) {
	if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height) || (width*height > SCREENHAL_MAX_BITMAP_SPACE)) return 0;
	if (grabAndReleaseBus) _grabBus();
	retrieveBitmapImpl(buffer, x, y, width, height);
	if (grabAndReleaseBus) _releaseBus();
	return buffer;
}


void ScreenHAL::pasteBitmap(uint16_t *bitmap, int16_t x, int16_t y, uint16_t width, uint16_t height, bool hasTransparency, uint16_t transparentColor, bool grabAndReleaseBus) {
	uint16_t buffer[SCREENHAL_MAX_BITMAP_SPACE];

	if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
	// check for overflow
	if (hasTransparency && (width * height > SCREENHAL_MAX_BITMAP_SPACE)) return;
	if (grabAndReleaseBus) _grabBus();
	if (hasTransparency) {
		// get the background image
		retrieveBitmapImpl(buffer, x, y, width, height);
		// set all non transparent pixels of the bitmap in the buffer
		for (uint32_t i=0; i<((uint32_t)width)*height; i++) {
			if (bitmap[i] != transparentColor) buffer[i] = bitmap[i];
		}
		pasteBitmapImpl(buffer, x, y, width, height);
	} else pasteBitmapImpl(bitmap, x, y, width, height);
	if (grabAndReleaseBus) _releaseBus();
}


void ScreenHAL::drawPattern(uint8_t *pattern, uint8_t orientation, int16_t x, int16_t y, uint8_t width, uint8_t height, 
		uint16_t color, uint16_t backColor, uint8_t scale, bool overlay, bool grabAndReleaseBus) {
	uint16_t buffer[SCREENHAL_MAX_BITMAP_SPACE];
	uint8_t unpacked[SCREENHAL_MAX_BITMAP_SPACE];

	if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
	if (width * height * scale * scale > SCREENHAL_MAX_BITMAP_SPACE) return;
	// unpack the pattern
	_unpackPattern(pattern, unpacked, width, height, 0, 0, orientation);
	if (grabAndReleaseBus) _grabBus();
	// if overlay, get the bitmap from the screen
	if (overlay) retrieveBitmapImpl(buffer, x, y, width*scale, height*scale);
	// scale and colorize it, setting a background color if no need to overlay
	_scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, backColor, width, height, scale, 0, 0, overlay != true);
	// draw it
	pasteBitmapImpl(buffer, x, y, width*scale, height*scale);
	if (grabAndReleaseBus) _releaseBus();
}


/* ---------------- Protected functions ------------------------ */

/* ---------------- Virtual functions -------------------------- */
/* ---------------- NEED TO BE IMPLEMENTED BY CHILD CLASS ------ */

void ScreenHAL::initScreenImpl() {
	// needs to be implemented by the class inheriting from this class
}

void ScreenHAL::setRotationImpl(uint8_t rotation) {
	// needs to be implemented by the class inheriting from this class
}

void ScreenHAL::drawPixelImpl(uint16_t x, uint16_t y, uint16_t color) {
	// needs to be implemented by the class inheriting from this class
}

// Fills the area lx,ly -> hx, hy
// this function assumes that lx <= hx and ly <= hy
void ScreenHAL::fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
	// needs to be implemented by the class inheriting from this class
}

// Draws the bitmap
void ScreenHAL::pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
}

// retrieves a bitmap from the screen
void ScreenHAL::retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
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

#define _pulseWR() { *_wrPort &= _wrLow; *_wrPort |= _wrHigh; }

void ScreenHAL::writeCommand(uint8_t command) {
	// set command mode
	*_cdPort &= ~_cdBitMask;
	// write the command
	*_outPort = command;
	_pulseWR();
	// set data mode
	*_cdPort |= _cdBitMask;
}

void ScreenHAL::writeData(uint8_t data) {
	*_outPort = data;
	_pulseWR();
}

void ScreenHAL::write16bData(uint16_t data) {
	*_outPort = data >> 8;
	_pulseWR();
	*_outPort = data;
	_pulseWR();
}

void ScreenHAL::write16bDataBuffer(uint16_t *data, uint32_t len) {
	for (uint32_t i=0; i<len; i++) { 
		*_outPort = (uint8_t) (data[i] >> 8);
		_pulseWR();
		*_outPort = (uint8_t) (data[i] & 0x0FF);
		_pulseWR();
	}
}

uint8_t ScreenHAL::readData() {
	uint8_t data;
	*_portMode = 0x00;
	*_rdPort &= _rdLow;
	*_rdPort &= _rdLow;
	data = *_inPort;
	*_rdPort |= _rdHigh; 
	*_portMode = 0xFF;
	return data;
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
	/*
	else {
		// we had to keep WR down not to influence the bus
		// so we have to complete a NOP operation
		*_outPort = 0;
		*_wrPort |= _wrHigh;
	}
	*/
	// put the screen in data mode
	*_cdPort |= _cdBitMask;
	_busTaken = 1;
}

void ScreenHAL::_releaseBus() {
	// unselect the screen
	if (_cs != 0xFF) digitalWrite(_cs, HIGH);
	/*
	else {
		// we have to keep WR down not to influence the bus
		*_cdPort &= ~_cdBitMask;
		*_wrPort |= _wrHigh;
		*_wrPort &= _wrLow;
	}
	*/
	// restore the SPI if it was active
	if (_spiUsed) {
		// we have to set SS as an output to enable SPI
		pinMode(SS, OUTPUT);
		// we always restore the master mode
		SPCR |= _BV(MSTR);
		SPCR |= _BV(SPE);
	}
	_busTaken = 0;
}


void ScreenHAL::_drawValidChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, fontDefinition_t *fontDef, uint8_t fontScale, bool isBold, bool overlay) {
	uint8_t pattern[FONT_MAX_PATTERN];
	uint8_t unpacked[FONT_MAX_SPACE];
	uint16_t buffer[FONT_MAX_SPACE*2*2];

	if ((fontSize % 2) == 1) memcpy_P(pattern, &font_small[chr - fontDef->firstChar], fontDef->orientation == PATTERN_ORIENTATION_HORIZONTAL ? fontDef->height : fontDef->width);
	else if ((fontSize % 2) == 0) memcpy_P(pattern, &font_medium[chr - fontDef->firstChar], fontDef->orientation == PATTERN_ORIENTATION_HORIZONTAL ? fontDef->height : fontDef->width);
	// unpack the pattern
	_unpackPattern(pattern, unpacked, fontDef->width, fontDef->height, fontDef->charSpacing, fontDef->lineSpacing, fontDef->orientation);	
	// if overlay, get the bitmap from the screen
	if (overlay) retrieveBitmapImpl(buffer, x, y, (fontDef->width+fontDef->charSpacing)*fontScale, (fontDef->height+fontDef->lineSpacing)*fontScale);
	// scale and colorize it
	_scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef->width+fontDef->charSpacing, fontDef->height+fontDef->lineSpacing, fontScale, 0, 0, overlay != true);
	if (isBold) {
		_scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef->width+fontDef->charSpacing, fontDef->height+fontDef->lineSpacing, fontScale, 1, 0, false);
		_scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef->width+fontDef->charSpacing, fontDef->height+fontDef->lineSpacing, fontScale, 0, 1, false);
	}
	// draw it
	pasteBitmapImpl(buffer, x, y, (fontDef->width+fontDef->charSpacing)*fontScale, (fontDef->height+fontDef->lineSpacing)*fontScale);
}

// Unpacks the bits contained in pattern into unpacked: each byte of unpacked is a bit of pattern
// the bits of the pattern can be stored horizontaly of verticaly
// we can add a border on the right and on the bottom
void ScreenHAL::_unpackPattern(uint8_t *pattern, uint8_t *unpacked, uint8_t width, uint8_t height, uint8_t borderRight, uint8_t borderBottom, uint8_t orientation) {
	for (uint16_t i=0; i<(width+borderRight)*(height+borderBottom); i++) unpacked[i] = 0;
	for (uint8_t l=0; l<height; l++) {
		for (uint8_t c=0; c<width; c++) {
			uint8_t patternByte;
			uint8_t mask;
			if (orientation == PATTERN_ORIENTATION_HORIZONTAL) {
				patternByte = pattern[l*(1+width/8)+c/8];
				mask = 0x01 << (c & 0x07);
				unpacked[l*(width+borderRight)+c] = patternByte & mask;
			} else {
				patternByte = pattern[c*(1+height/8)+l/8];
				mask = 0x01 << (l & 0x07);
				unpacked[l*(width+borderRight)+c] = patternByte & mask;
			}
		}
	}
}

// draws an unpacked pattern into the buffer, replacing all the result pixels by onColor if the corresponding byte is not null in unpacked
// can scale and shift the unpacked pattern before setting the pixels in the result
// can also fill the result with offColor before replacing
void ScreenHAL::_scaleShiftAndColorizeUnpackedPattern(uint8_t *unpacked, uint16_t *result, uint16_t onColor, uint16_t offColor, uint8_t width, uint8_t height, uint8_t scale,
	uint8_t xShift, uint8_t yShift, bool blankFirst) {
	if (blankFirst) {
		for (uint16_t i=0; i<(width*height)*scale*scale; i++) result[i] = offColor;
	}
	for (uint8_t l=0; l<height; l++) {
		for (uint8_t c=0; c<width; c++) {
			if (scale == 1) {
				if (unpacked[l*width+c]) result[(l+yShift)*width+(c+xShift)] = onColor;
			} else {
				for (uint8_t pl=0; pl<scale; pl++) {
					for (uint8_t pc=0; pc<scale; pc++) {
						if (unpacked[l*width+c]) result[(l*scale+pl+yShift)*width*scale+c*scale+pc+xShift] = onColor;
					}
				}
			}
		}
	}
}


void ScreenHAL::_fillBoundedArea(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
	if (x2 < x1) swap(x1, x2);
	if (x1 >= _width) return;
	if (y2 < y1) swap(y1, y2);
	if (y1 >= _height) return;
	if (x1 < 0) x1 = 0;
	if (x2 >= _width) x2 = _width-1;
	if (y1 < 0) y1 = 0;
	if (y2 >= _height) y2 = _height-1;
	fillAreaImpl(x1, y1, x2, y2, color);
}


void ScreenHAL::_drawBoundedPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x < 0) || (x >= _width)) return;
	if ((y < 0) || (y >= _height)) return;
	drawPixelImpl(x, y, color);
}

/*
 * From Wikipedia: Andres arc drawing algorithm, extended to 
 * 	- draw thick lines and 
 * 	- fill with optimal use of horizontal lines
 */	
void ScreenHAL::_drawOrFillOctant(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, int8_t thickness, bool fill) {
	int16_t x = 0;
	int16_t y = r;
	int16_t d = r - 1;
	int8_t t1 = (thickness-1)/2;
	int8_t t2 = thickness/2;
	
	while (y >= x) {
		if (fill) {
			if (octant & (SCREEN_ARC_SSW | SCREEN_ARC_SSE)) _fillBoundedArea(x0-x, y0+y, x0+x, y0+y, color);
			else if (octant & SCREEN_ARC_SSW) _fillBoundedArea(x0, y0+y, x0-x, y0+y, color);
			else if (octant & SCREEN_ARC_SSE) _fillBoundedArea(x0, y0+y, x0+x, y0+y, color);
			if (octant & (SCREEN_ARC_SWW | SCREEN_ARC_SEE)) _fillBoundedArea(x0-y, y0+x, x0+y, y0+x, color);
			else if (octant & SCREEN_ARC_SWW) _fillBoundedArea(x0, y0+x, x0-y, y0+x, color);
			else if (octant & SCREEN_ARC_SEE) _fillBoundedArea(x0, y0+x, x0+y, y0+x, color);
			if (octant & (SCREEN_ARC_NNW | SCREEN_ARC_NNE)) _fillBoundedArea(x0-x, y0-y, x0+x, y0-y, color);
			else if (octant & SCREEN_ARC_NNW) _fillBoundedArea(x0, y0-y, x0-x, y0-y, color);
			else if (octant & SCREEN_ARC_NNE) _fillBoundedArea(x0, y0-y, x0+x, y0-y, color);
			if (octant & (SCREEN_ARC_NWW | SCREEN_ARC_NEE)) _fillBoundedArea(x0-y, y0-x, x0+y, y0-x, color);		
			else if (octant & SCREEN_ARC_NWW) _fillBoundedArea(x0, y0-x, x0-y, y0-x, color);		
			else if (octant & SCREEN_ARC_NEE) _fillBoundedArea(x0, y0-x, x0+y, y0-x, color);
		} else if (thickness != 1) {
			if (octant & SCREEN_ARC_SSE) _fillBoundedArea(x0+x-t1, y0+y-t1, x0+x+t2, y0+y+t2, color);
			if (octant & SCREEN_ARC_SEE) _fillBoundedArea(x0+y-t1, y0+x-t1, x0+y+t2, y0+x+t2, color);
			if (octant & SCREEN_ARC_SSW) _fillBoundedArea(x0-x-t1, y0+y-t1, x0-x+t2, y0+y+t2, color);
			if (octant & SCREEN_ARC_SWW) _fillBoundedArea(x0-y-t1, y0+x-t1, x0-y+t2, y0+x+t2, color);
			if (octant & SCREEN_ARC_NNE) _fillBoundedArea(x0+x-t1, y0-y-t1, x0+x+t2, y0-y+t2, color);
			if (octant & SCREEN_ARC_NEE) _fillBoundedArea(x0+y-t1, y0-x-t1, x0+y+t2, y0-x+t2, color);
			if (octant & SCREEN_ARC_NNW) _fillBoundedArea(x0-x-t1, y0-y-t1, x0-x+t2, y0-y+t2, color);
			if (octant & SCREEN_ARC_NWW) _fillBoundedArea(x0-y-t1, y0-x-t1, x0-y+t2, y0-x+t2, color);		
		} else {
			if (octant & SCREEN_ARC_SSE) _drawBoundedPixel(x0+x, y0+y, color);
			if (octant & SCREEN_ARC_SEE) _drawBoundedPixel(x0+y, y0+x, color);
			if (octant & SCREEN_ARC_SSW) _drawBoundedPixel(x0-x, y0+y, color);
			if (octant & SCREEN_ARC_SWW) _drawBoundedPixel(x0-y, y0+x, color);
			if (octant & SCREEN_ARC_NNE) _drawBoundedPixel(x0+x, y0-y, color);
			if (octant & SCREEN_ARC_NEE) _drawBoundedPixel(x0+y, y0-x, color);
			if (octant & SCREEN_ARC_NNW) _drawBoundedPixel(x0-x, y0-y, color);
			if (octant & SCREEN_ARC_NWW) _drawBoundedPixel(x0-y, y0-x, color);
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

