#include "TouchScreen.h"

TouchScreen::TouchScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset) {
	setupScreen(port, cd, wr, rd, cs, reset);
	_xm = 0xFF;
}


void TouchScreen::begin(uint8_t fontSize, uint16_t foregroundColor, uint16_t backgroundColor) {
	setFontSize(fontSize);
	setBackgroundColor(backgroundColor);
	setForegroundColor(foregroundColor);
	initScreen();
	if (backgroundColor != BLACK) fillRectangle(0, 0, getWidth()-1, getHeight()-1, backgroundColor);
}


void TouchScreen::setupTouchPanel(uint8_t xm, uint8_t xp, uint8_t ym, uint8_t yp, uint16_t xPlaneResistance, uint16_t pressureThreshold) {
	_xm = xm;
	_ym = ym;
	_xp = xp;
	_yp = yp;
	_xmPort = portOutputRegister(digitalPinToPort(xm));
	_xpPort = portOutputRegister(digitalPinToPort(xp));
	_ymPort = portOutputRegister(digitalPinToPort(ym));
	_ypPort = portOutputRegister(digitalPinToPort(yp));
	_xmBitMask = digitalPinToBitMask(xm);
	_xpBitMask = digitalPinToBitMask(xp);
	_ymBitMask = digitalPinToBitMask(ym);
	_ypBitMask = digitalPinToBitMask(yp);
	_xPlaneResistance = xPlaneResistance;
	_pressureThreshold = pressureThreshold;
	analogReference(INTERNAL);
}


uint16_t TouchScreen::getTouchPanelX() {
	uint16_t x;
	if (getTouchPanelXYZ(&x, 0)) return x;
	return 0xFFFF;
}


uint16_t TouchScreen::getTouchPanelY() {
	uint16_t y;
	if (getTouchPanelXYZ(0, &y)) return y;
	return 0xFFFF;
}


uint16_t TouchScreen::getTouchPanelZ() {
	return getTouchPanelXYZ(0, 0);
}


uint16_t TouchScreen::getTouchPanelXYZ(uint16_t *x, uint16_t *y, uint16_t *z) {
	if (_xm == 0xFF) return 0;
	
	uint16_t val1, val2;
	uint16_t X;
	
	// measure X
	pinMode(_yp, INPUT);
	pinMode(_ym, INPUT);  
	*_ypPort &= ~_ypBitMask;
	*_ymPort &= ~_ymBitMask;
	pinMode(_xp, OUTPUT);
	pinMode(_xm, OUTPUT);
	*_xpPort |= _xpBitMask;
	*_xmPort &= ~_xmBitMask;
	val1 = analogRead(_yp);
	val2 = analogRead(_yp);
	if (val1 != val2) return 0;
	X = (1023 - (val1 + val2)/2);
	if (x != 0) *x = X;
	
	// measure Y
	pinMode(_xp, INPUT);
	pinMode(_xm, INPUT);
	*_xpPort &= ~_xpBitMask;
	pinMode(_yp, OUTPUT);
	pinMode(_ym, OUTPUT);
	*_ypPort |= _ypBitMask;
	if (y != 0) {
		val1 = analogRead(_xm);
		val2 = analogRead(_xm);
		if (val1 != val2) return 0;
		*y = (1023 - (val1 + val2)/2);
	}
	
	// Measure Z
	pinMode(_xp, OUTPUT);
	*_ymPort |= _ymBitMask;
	pinMode(_yp, INPUT);
	*_ypPort &= ~_ypBitMask;
	val1 = analogRead(_xm);
	val2 = analogRead(_yp);	
	float Z = val2;
	Z /= val1;
	Z -= 1;
	Z *= X *_xPlaneResistance / 1024;
	if (z != 0) *z = Z; 
	return Z;
}


bool TouchScreen::isTouchPanelZValid(uint16_t z) {
	return (z > _pressureThreshold);
}
