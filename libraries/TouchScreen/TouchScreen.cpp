#include "TouchScreen.h"
#include "eepromUtils.hpp"

	
TouchScreen::TouchScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset, uint8_t backlightPin) {
	setupScreen(port, cd, wr, rd, cs, reset);
	_xm = 0xFF;
	_backlightPin = backlightPin;
}


void TouchScreen::begin(uint16_t foregroundColor, uint16_t backgroundColor, uint8_t fontSize, bool fontBold) {
	initScreen();
	if (_backlightPin != 0xFF) setupBacklight(_backlightPin);
	setFont(fontSize, fontBold);
	setBackgroundColor(backgroundColor);
	setForegroundColor(foregroundColor);
	if (_backlightPin != 0xFF) setBacklight(0);
	if (backgroundColor != BLACK) fillRectangle(0, 0, getWidth(), getHeight(), backgroundColor);
	if (_backlightPin != 0xFF) setBacklight(128);
}


void TouchScreen::setupBacklight(uint8_t backlightPin) {
	_backlightPin = backlightPin;
	if (_backlightPin == 0xFF) return;
	pinMode(_backlightPin, OUTPUT);
	_backlight = 128;
	analogWrite(_backlightPin, 128);
}


void TouchScreen::setBacklight(uint8_t value) {
	if (_backlightPin == 0xFF) return;
	_backlight = value;
	analogWrite(_backlightPin, _backlight);
}


uint8_t TouchScreen::getBacklight() {
	return _backlight;
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
	analogReference(DEFAULT);
	// calibrate the touch panel if required
	_calibrationMatrix.Dx = 0;
	if (!calibrateTouchPanel()) return;
	// read the matrix from eeprom if not already set
	if (_calibrationMatrix.Dx == 0) {
		_calibrationMatrix.Ax = eeprom_read_int32_t(TOUCHSCREEN_EEPROM_AX);
		_calibrationMatrix.Bx = eeprom_read_int32_t(TOUCHSCREEN_EEPROM_AX+4);
		_calibrationMatrix.Dx = eeprom_read_int32_t(TOUCHSCREEN_EEPROM_AX+8);
		_calibrationMatrix.Ay = eeprom_read_int32_t(TOUCHSCREEN_EEPROM_AX+12);
		_calibrationMatrix.By = eeprom_read_int32_t(TOUCHSCREEN_EEPROM_AX+16);
		_calibrationMatrix.Dy = eeprom_read_int32_t(TOUCHSCREEN_EEPROM_AX+20);
	}
}


uint16_t TouchScreen::getTouchX() {
	int16_t x, y, z;
	if (getTouchXYZ(&x, &y, &z) != TOUCHSCREEN_NO_TOUCH) return x;
	return TOUCHSCREEN_NO_TOUCH;
}


uint16_t TouchScreen::getTouchY() {
	int16_t x, y, z;
	if (getTouchXYZ(&x, &y, &z) != TOUCHSCREEN_NO_TOUCH) return y;
	return TOUCHSCREEN_NO_TOUCH;
}


uint16_t TouchScreen::getTouchZ() {
	int16_t x, y, z;
	if (getTouchXYZ(&x, &y, &z) != TOUCHSCREEN_NO_TOUCH) return z;
	return TOUCHSCREEN_NO_TOUCH;
}


uint16_t TouchScreen::getTouchXYZ(int16_t *x, int16_t *y, int16_t *z) {
	if (_xm == 0xFF) return 0;
	
	uint16_t val1, val2;
	int16_t X, Y;
	
	// void the values
	*x = TOUCHSCREEN_NO_TOUCH;
	*y = TOUCHSCREEN_NO_TOUCH;
	*z = TOUCHSCREEN_NO_TOUCH;
	
	// restore the analog reference: just in case...
	analogReference(DEFAULT);
	
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
	if (abs(val1-val2) > TOUCHSCREEN_EQUAL) return TOUCHSCREEN_NO_TOUCH;
	X = (val1 + val2)/2;
	
	// measure Y
	pinMode(_xp, INPUT);
	pinMode(_xm, INPUT);
	*_xpPort &= ~_xpBitMask;
	pinMode(_yp, OUTPUT);
	pinMode(_ym, OUTPUT);
	*_ypPort |= _ypBitMask;
	val1 = analogRead(_xm);
	val2 = analogRead(_xm);
	if (abs(val1-val2) > TOUCHSCREEN_EQUAL) return TOUCHSCREEN_NO_TOUCH;
	Y = (val1 + val2)/2;
	
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
	*z = Z; 
	
	if (*z >= _pressureThreshold) {
		_getDisplayXY(&X, &Y);
		*x = X;
		*y = Y;
		return *z;
	}
	return TOUCHSCREEN_NO_TOUCH;
}

bool TouchScreen::calibrateTouchPanel(bool force) {
	// check if we are calibrated or forced to calibrate
	if (!force && (eeprom_read_byte(TOUCHSCREEN_EEPROM_CALIBRATED) != 0xFF)) return true;
	
	// blank the screen
	if (_backlightPin != 0xFF) setBacklight(0);
	setRotation(SCREEN_ROTATION_0);
	fillRectangle(0, 0, getWidth(), getHeight(), WHITE);
	if (_backlightPin != 0xFF) setBacklight(128);
	
	// draw calibration points and get resulting touch screen points
	int32_t tsX0, tsY0, tsX1, tsY1, tsX2, tsY2;
	int32_t dX0 = 15 * getWidth() / 100;
	int32_t dY0 = 15 * getHeight() / 100;
	if (!_calibrateTouchPanelPoint(dX0, dY0, &tsX0, &tsY0)) return false;
	uint16_t dX1 = 50 * getWidth() / 100;
	uint16_t dY1 = 85 * getHeight() / 100;
	if (!_calibrateTouchPanelPoint(dX1, dY1, &tsX1, &tsY1)) return false;
	uint16_t dX2 = 85 * getWidth() / 100;
	uint16_t dY2 = 50 * getHeight() / 100;
	if (!_calibrateTouchPanelPoint(dX2, dY2, &tsX2, &tsY2)) return false;

	/*
		Assuming the alignment of the touchscreen is correct, we have 
			dX0 = ax * tsX0 + bx
			dX2 = ax * tsX2 + bx
		so
			ax = (dX2 - dX0) / (tsX2 - tsX0)
			bx = dX0 - tsX0 * (dX2 - dX0) / (tsX2 - tsX0)
		we can solve
			dXn = (Ax * tsXn + Bx) / Dx
		with 
			Ax = dX2 - dX0
			Bx = dX0 * (tsX2 - tsX0) - tsX0 * (dX2 - dX0)
			Dx = tsX2 - tsX0
	*/
	_calibrationMatrix.Ax = dX2 - dX0;
	_calibrationMatrix.Bx = dX0 * (tsX2 - tsX0) - tsX0 * (dX2 - dX0);
	_calibrationMatrix.Dx = tsX2 - tsX0;
	_calibrationMatrix.Ay = dY1 - dY0;
	_calibrationMatrix.By = dY0 * (tsY1 - tsY0) - tsY0 * (dY1 - dY0);
	_calibrationMatrix.Dy = tsY1 - tsY0;
	
    // persist data to EEPROM
    eeprom_write_int32_t(TOUCHSCREEN_EEPROM_AX, _calibrationMatrix.Ax);
    eeprom_write_int32_t(TOUCHSCREEN_EEPROM_AX+4, _calibrationMatrix.Bx);
    eeprom_write_int32_t(TOUCHSCREEN_EEPROM_AX+8, _calibrationMatrix.Dx);
    eeprom_write_int32_t(TOUCHSCREEN_EEPROM_AX+12, _calibrationMatrix.Ay);
    eeprom_write_int32_t(TOUCHSCREEN_EEPROM_AX+16, _calibrationMatrix.By);
    eeprom_write_int32_t(TOUCHSCREEN_EEPROM_AX+20, _calibrationMatrix.Dy);
    eeprom_write_byte(TOUCHSCREEN_EEPROM_CALIBRATED, 1);
	
	return true;
}


/* ------------------------------ Private functions ------------------------------------ */

bool TouchScreen::_calibrateTouchPanelPoint(int32_t dX, int32_t dY, int32_t *tsX, int32_t *tsY) {
	int16_t x, y, z;
	uint8_t wait = 0;
	
	// draw a cross
	drawHorizontalLine(dX-10, dX+11, dY, BLACK, 2);
	drawVerticalLine(dX, dY-10, dY+11, BLACK, 2);
	while (wait < 200) {
		if (getTouchXYZ(&x, &y, &z) != TOUCHSCREEN_NO_TOUCH) {
			*tsX = x;
			*tsY = y;
			// debounce the touchscreen
			while (getTouchZ() != TOUCHSCREEN_NO_TOUCH) delay(100);
			delay(300);
			while (getTouchZ() != TOUCHSCREEN_NO_TOUCH) delay(100);
			// erase the cross
			drawHorizontalLine(dX-10, dX+11, dY, WHITE, 2);
			drawVerticalLine(dX, dY-10, dY+11, WHITE, 2);
			delay(200);
			return true;
		}
		delay(100);
		wait ++;
	}
	return false;
}

void TouchScreen::_getDisplayXY(int16_t *x, int16_t *y) {
	if (_calibrationMatrix.Dx != 0) {
		int32_t X = *x;
		int32_t Y = *y;		
		X = (_calibrationMatrix.Ax * X + _calibrationMatrix.Bx) / _calibrationMatrix.Dx;
		Y = (_calibrationMatrix.Ay * Y + _calibrationMatrix.By) / _calibrationMatrix.Dy;
		*x = X;
		*y = Y;
	}
	getRotatedXY(x, y, _rotation);
}

