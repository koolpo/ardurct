#include "TouchScreen.h"

TouchScreen::TouchScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset) {
	defineScreenPins(port, cd, wr, rd, cs, reset);
	_xm = 0xFF;
	_ym = 0xFF;
	_xp = 0xFF;
	_yp = 0xFF;
}


void TouchScreen::defineTouchPanelPins(uint8_t xm, uint8_t xp, uint8_t ym, uint8_t yp) {
	_xm = xm;
	_ym = ym;
	_xp = xp;
	_yp = yp;
}


void TouchScreen::begin(uint8_t fontSize, uint16_t foregroundColor, uint16_t backgroundColor) {
	setFontSize(fontSize);
	setBackgroundColor(backgroundColor);
	setForegroundColor(foregroundColor);
	initScreen();
	if (backgroundColor != BLACK) fillRectangle(0, 0, getWidth()-1, getHeight()-1, backgroundColor);
}


uint16_t TouchScreen::create565Color(uint8_t r, uint8_t g, uint8_t b) {
	uint16_t color;
	color = r >> 3;
	color <<= 6;
	color |= g >> 2;
	color <<= 5;
	color |= b >> 3;
	return color;
}


bool TouchScreen::getPressedPoint(uint16_t *x, uint16_t *y) {
	if (_xp == 0xFF) return false;
	return true;
}

