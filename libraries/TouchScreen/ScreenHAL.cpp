#include "ScreenHAL.hpp"

#include "fontBitmaps.hpp"

ScreenHAL::ScreenHAL(void) {
}


void ScreenHAL::defineScreenPins(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset) {	
	_outPort = portOutputRegister(port);
	_inPort = portInputRegister(port);
	_portMode = portModeRegister(port);
	_rdPort = portOutputRegister(digitalPinToPort(rd));;
	_rdHigh = digitalPinToBitMask(rd);
	_rdLow = ~_rdHigh;
	_wrPort = portOutputRegister(digitalPinToPort(wr));;
	_wrHigh = digitalPinToBitMask(wr);
	_wrLow = ~_wrHigh;
	_cdPort = portOutputRegister(digitalPinToPort(cd));;
	_cdBitMask = digitalPinToBitMask(cd);
	_cs = cs;
	
	pinMode(rd, OUTPUT);
	pinMode(wr, OUTPUT);
	pinMode(cd, OUTPUT);
	if (cs != 0xFF) pinMode(cs, OUTPUT);
	if (reset != 0xFF) pinMode(reset, OUTPUT);
	
	// if we have no reset pin, the RD and WR have been OR'ed to create the reset
	// they have to be on the same port for this to work
	if (reset == 0xFF) {
		*_rdPort &= ~(_rdHigh | _wrHigh);
		delay(100);
		*_rdPort |= (_rdHigh | _wrHigh);
	} else {
		digitalWrite(reset, LOW);
		delay(100);
		digitalWrite(reset, HIGH);
	}
	
	_width = 240;
	_height = 320;
	_spiUsed = 0;
	_busTaken = 0;
	_rotation = 0;
	setFontSize(1);
}


void ScreenHAL::setRotation(uint8_t rotation) {
	_rotation = rotation;
	if (_rotation > SCREEN_ROTATION_270) _rotation = SCREEN_ROTATION_0;
	if ((_rotation == SCREEN_ROTATION_0) || (_rotation == SCREEN_ROTATION_180)) {
		_width = 240;
		_height = 320;
	} else {
		_width = 320;
		_height = 240;
	}
	rotate();
}


uint8_t ScreenHAL::getRotation() {
	return _rotation;
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
	drawCharNoCheck(chr, x, y, color, validFontSize, fontDef, fontScale, overlay, grabAndReleaseBus);
}


void ScreenHAL::drawString(char *s, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, bool overlay, bool grabAndReleaseBus) {
	uint8_t validFontSize = (fontSize > FONT_LAST*2 ? FONT_LAST*2 : (fontSize < 1 ? 1 : fontSize));
	uint8_t fontScale = (validFontSize <= FONT_LAST ? 1 : 2);
	fontDefinition_t *fontDef = &fontDefinition_small;
	if ((x + fontDef->width * fontScale >= _width) || (y + fontDef->height * fontScale >= _height)) return;
	if ((validFontSize == 2) || (validFontSize == 4)) fontDef = &fontDefinition_medium;
	
	if (grabAndReleaseBus) grabBus();
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
		drawCharNoCheck(validChr, x, y, _foregroundColor, validFontSize, fontDef, fontScale, overlay);
		x += (fontDef->width + fontDef->charSpacing) * fontScale;
		if (x > _width) {
			x = 0;
			y += (fontDef->height + fontDef->lineSpacing) * fontScale;
			if (y > _height) return;
		}
		s++;
	}
	if (grabAndReleaseBus) releaseBus();
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
	} else if (chr == '\r') {
		// skip the character
		return 1;
	}
	uint8_t validChr = chr;
	if ((validChr < _fontDef->firstChar) || (validChr > _fontDef->lastChar)) validChr = '?';
	drawCharNoCheck(validChr, _x, _y, _foregroundColor, _fontSize, _fontDef, _fontScale, false, true);
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


// virtual 
void ScreenHAL::drawPixel(uint16_t x, uint16_t y, uint16_t color, bool grabAndReleaseBus) {
	if ((x >= _width) || (y >= _height)) return;
	if (grabAndReleaseBus) grabBus();
	drawPixelNoCheck(x, y, color);
	if (grabAndReleaseBus) releaseBus();
}


void ScreenHAL::drawHorizontalLine(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	uint16_t lx = (x1 <= x2 ? x1 : x2);
	if ((lx >= _width) || (y >= _height)) return;
	uint16_t hx = (x1 <= x2 ? x2 : x1);
	if (hx > _width) hx = _width-1;
	int16_t ly = y - (thickness-1) / 2;
	if (ly < 0) ly = 0;
	uint16_t hy = y + thickness/2;
	if (hy >= _height) hy = _height-1;
	if (grabAndReleaseBus) grabBus();
	fillRectangleNoCheck(lx, ly, hx, hy, color);
	if (grabAndReleaseBus) releaseBus();
}


void ScreenHAL::drawVerticalLine(uint16_t x, uint16_t y1, uint16_t y2, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	uint16_t ly = (y1 <= y2 ? y1 : y2);
	if ((x >= _width) || (ly >= _height)) return;
	uint16_t hy = (y1 <= y2 ? y2 : y1);
	int16_t lx = x - (thickness-1) / 2;
	if (lx < 0) lx = 0;
	uint16_t hx = x + thickness / 2;
	if (hx >= _width) hx = _width-1;
	if (grabAndReleaseBus) grabBus();
	fillRectangleNoCheck(lx, ly, hx, hy, color);
	if (grabAndReleaseBus) releaseBus();
}


void ScreenHAL::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness, bool grabAndReleaseBus) {
	if (grabAndReleaseBus) grabBus();
	drawHorizontalLine(x1, x2, y1, color, thickness, false);
	drawHorizontalLine(x1, x2, y2, color, thickness, false);
	drawHorizontalLine(x1, y1, y2, color, thickness, false);
	drawHorizontalLine(x2, y1, y2, color, thickness, false);
	if (grabAndReleaseBus) releaseBus();
}


void ScreenHAL::fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, bool grabAndReleaseBus) {
	uint16_t lx = (x1 <= x2 ? x1 : x2);
	uint16_t ly = (y1 <= y2 ? y1 : y2);
	if ((lx >= _width) || (ly >= _height)) return;
	uint16_t hx = (x1 <= x2 ? x2 : x1);
	uint16_t hy = (y1 <= y2 ? y2 : y1);
	if (grabAndReleaseBus) grabBus();
	fillRectangleNoCheck(lx, ly, hx, hy, color);
	if (grabAndReleaseBus) releaseBus();
}

/* ---------------- Protected functions ------------------------ */

// virtual 
void ScreenHAL::initScreen() {
	// needs to be implemented by the class inheriting from this class
}

// virtual 
void ScreenHAL::rotate() {
	// needs to be implemented by the class inheriting from this class
}

// virtual 
void ScreenHAL::drawCharNoCheck(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, fontDefinition_t *fontDef, uint8_t fontScale, bool overlay, bool grabAndRelease) {
	// needs to be implemented by the class inheriting from this class
}

// virtual
void ScreenHAL::drawPixelNoCheck(uint16_t x, uint16_t y, uint16_t color) {
	// needs to be implemented by the class inheriting from this class
}


// Fill the rectangle lx,ly -> hx, hy
// this functions assumes that lx <= hx and ly <= hy
// virtual
void ScreenHAL::fillRectangleNoCheck(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
	// needs to be implemented by the class inheriting from this class
}


void ScreenHAL::grabBus() {
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

void ScreenHAL::releaseBus() {
	// restore the SPI if it was active
	if (_spiUsed) SPCR |= _BV(SPE);
	// unselect the screen
	if (_cs != 0xFF) digitalWrite(_cs, HIGH);
	_busTaken = 0;
}


#define pulseWR() { *_wrPort &= _wrLow; *_wrPort |= _wrHigh; *_wrPort |= _wrHigh; }

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
	*_outPort = (uint8_t) data >> 8;
	pulseWR();
	*_outPort = (uint8_t) (data & 0x0FF);
	pulseWR();
}

void ScreenHAL::write16bDataBuffer(uint16_t *data, uint32_t len) {
	for (uint32_t i=0; i<len; i++) { 
		*_outPort = (uint8_t) data[i] >> 8;
		pulseWR();
		*_outPort = (uint8_t) (data[i] & 0x0FF);
		pulseWR();
	}
}

void ScreenHAL::read16bDataBuffer(uint16_t *data, uint32_t len) {
	*_portMode = 0x00;	
	for (uint32_t i=0; i<len; i++) { 
		*_rdPort &= _rdLow;
		*_rdPort &= _rdLow;
		data[i] = *_inPort;
		*_rdPort |= _rdHigh; 
		data[i] = data[i] << 8;
		*_rdPort &= _rdLow;
		*_rdPort &= _rdLow;
		data[i] |= *_inPort;
		*_rdPort |= _rdHigh; 
	}
	*_portMode = 0xFF;
}
