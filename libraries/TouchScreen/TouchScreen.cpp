/*
 * Touchscreen - Screen and Touchpanel management
 *
 * Copyright (c) 2012 Laurent Wibaux <lm.wibaux@gmail.com>
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

#include "TouchScreen.h"
#include "eepromUtils.hpp"

//#include "../Printf/Printf.cpp"

TouchScreen::TouchScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset, uint8_t backlightPin) {
	setupScreen(port, cd, wr, rd, cs, reset);
	_xm = 0xFF;
	_backlightPin = backlightPin;
}


void TouchScreen::begin(uint16_t foregroundColor, uint16_t backgroundColor, uint8_t fontSize, bool fontBold, bool fontOverlay) {
	initScreen();
#if defined(CONFIGURATION_HAS_MACROS)	
	_initializeMacros();
#endif
	if (_backlightPin != 0xFF) setupBacklight(_backlightPin);
	setFont(fontSize, fontBold, fontOverlay);
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


void TouchScreen::setupTouchPanel(uint8_t xm, uint8_t xp, uint8_t ym, uint8_t yp) {
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
	// calibrate the touch panel if required
	_xCalibrationEquation.divider = 0;
	if (!calibrateTouchPanel()) return;
	// read the matrix from eeprom if not already set by calibrateTouchPanel
	if (_xCalibrationEquation.divider == 0) {
		_xCalibrationEquation.a = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_A);
		_xCalibrationEquation.b = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_B);
		_xCalibrationEquation.divider = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_DIVIDER);
		_yCalibrationEquation.a = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_A);
		_yCalibrationEquation.b = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_B);
		_yCalibrationEquation.divider = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_DIVIDER);
	}
}


int16_t TouchScreen::getTouchX() {
	int16_t x, y, z;
	if (getTouchXYZ(&x, &y, &z) > 0) return x;
	return TOUCHSCREEN_NO_TOUCH;
}


int16_t TouchScreen::getTouchY() {
	int16_t x, y, z;
	if (getTouchXYZ(&x, &y, &z)) return y;
	return TOUCHSCREEN_NO_TOUCH;
}


int16_t TouchScreen::getTouchZ() {
	int16_t x, y, z;
	if (getTouchXYZ(&x, &y, &z) > 0) return z;
	return TOUCHSCREEN_NO_TOUCH;
}


bool TouchScreen::isTouched() {
	int16_t x, y, z;
	return (getTouchXYZ(&x, &y, &z) != 0);
}


int16_t TouchScreen::getTouchXYZ(int16_t *x, int16_t *y, int16_t *z) {
	if (_xm == 0xFF) return TOUCHSCREEN_NO_TOUCH;
	
#if defined(CONFIGURATION_BUS_IS_SHARED_WITH_TOUCHPANEL)	
	unselectScreen();
#endif

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
	int16_t X = _analogAverage(_yp);
	if (X == TOUCHSCREEN_NO_TOUCH) return TOUCHSCREEN_NO_TOUCH;
	
	// measure Y
	pinMode(_xp, INPUT);
	pinMode(_xm, INPUT);
	*_xpPort &= ~_xpBitMask;
	pinMode(_yp, OUTPUT);
	pinMode(_ym, OUTPUT);
	*_ypPort |= _ypBitMask;
	int16_t Y = _analogAverage(_xm);
	if (Y == TOUCHSCREEN_NO_TOUCH) return TOUCHSCREEN_NO_TOUCH;
	
	// we return with XP and YM as outputs
	// and XM and YP as inputs, no pull-up
	pinMode(_xp, OUTPUT);
	*_ymPort |= _ymBitMask;
	pinMode(_yp, INPUT);
	*_ypPort &= ~_ypBitMask;

	// Measure Z
	int16_t val1 = analogRead(_xm);
	int16_t val2 = analogRead(_yp);	
	float Z = val2;
	Z /= val1;
	Z -= 1;
	Z *= (X * CONFIGURATION_TOUCHPANEL_X_PLANE_RESISTANCE) / 1024;
	*z = abs(Z); 
		
	if ((*z >= CONFIGURATION_TOUCHPANEL_PRESSURE_THRESHOLD) && (*z < CONFIGURATION_TOUCHPANEL_PRESSURE_MAX)) {
		// convert the measures according to the calibration
		_getDisplayXY(&X, &Y);
		*x = X;
		*y = Y;
		return *z;
	}
	return TOUCHSCREEN_NO_TOUCH;
}


bool TouchScreen::calibrateTouchPanel(bool force) {
	// check if we are calibrated or forced to calibrate
	if (!force && (eeprom_read_uint8_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_DONE) != 0xFF)) return true;
	
	// reset the X calibration divider to prohibit calculation of display points
	_xCalibrationEquation.divider = 0;
	// set the rotation to 0
	setRotation(SCREEN_ROTATION_0);

	// blank the screen
	if (_backlightPin != 0xFF) setBacklight(0);
	fillRectangle(0, 0, getWidth(), getHeight(), WHITE);
	if (_backlightPin != 0xFF) setBacklight(128);
	
	// draw calibration points and get resulting touch panel points
	int32_t tpX0, tpY0, tpX1, tpY1, tpX2, tpY2;
	int32_t dX0 = 15 * getWidth() / 100;
	int32_t dY0 = 15 * getHeight() / 100;
	if (!_calibrateTouchPanelPoint(dX0, dY0, &tpX0, &tpY0)) return false;
	uint16_t dX1 = 50 * getWidth() / 100;
	uint16_t dY1 = 85 * getHeight() / 100;
	if (!_calibrateTouchPanelPoint(dX1, dY1, &tpX1, &tpY1)) return false;
	uint16_t dX2 = 85 * getWidth() / 100;
	uint16_t dY2 = 50 * getHeight() / 100;
	if (!_calibrateTouchPanelPoint(dX2, dY2, &tpX2, &tpY2)) return false;

	/* --------------------------------------------------------------------- *
		Assuming that the touch panel is verticaly aligned with the screen,
		and that the touch panel resistance is almost linear;
		D being a Display coordinate and TP being a Touch Panel coordinate,
		m and n being 2 points, 
		
		we have:
			Dm = A * TPm + B
			Dn = A * TPn + B
		so
			A = (Dm - Dn) / (TPm - TPn)
			B = Dn - TPn * (Dm - Dn) / (TPm - TPn)
			
		to use only integers, we can solve D through
			D = (a * TP + b) / divider
		with 
				  a = Dm - Dn
			      b = Dn * (TPm - TPn) - TPn * (Dm - Dn)
			divider = TPm - TPn
	* ---------------------------------------------------------------------- */
	// use the points with the biggest X difference: 0 and 2
	_xCalibrationEquation.a = dX2 - dX0;
	_xCalibrationEquation.b = dX0 * (tpX2 - tpX0) - tpX0 * (dX2 - dX0);
	_xCalibrationEquation.divider = tpX2 - tpX0;
	// use the points with the biggest Y difference: 0 and 1
	_yCalibrationEquation.a = dY1 - dY0;
	_yCalibrationEquation.b = dY0 * (tpY1 - tpY0) - tpY0 * (dY1 - dY0);
	_yCalibrationEquation.divider = tpY1 - tpY0;
	
    // persist the calibration factors to EEPROM
    eeprom_write_uint8_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_DONE, 1);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_A, _xCalibrationEquation.a);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_B, _xCalibrationEquation.b);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_DIVIDER, _xCalibrationEquation.divider);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_A, _yCalibrationEquation.a);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_B, _yCalibrationEquation.b);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_DIVIDER, _yCalibrationEquation.divider);
	
	return true;
}


/* ------------------------------ Private functions ------------------------------------ */

bool TouchScreen::_calibrateTouchPanelPoint(int32_t dX, int32_t dY, int32_t *tpX, int32_t *tpY) {
	int16_t x, y, z;
	uint8_t wait = 0;
	
	// draw a cross
	drawHorizontalLine(dX-10, dX+11, dY, BLACK, 2);
	drawVerticalLine(dX, dY-10, dY+11, BLACK, 2);
	while (wait < 100) {
		if (getTouchXYZ(&x, &y, &z)) {
			*tpX = x;
			*tpY = y;
			// debounce the touchscreen
			while (isTouched()) delay(100);
			delay(300);
			while (isTouched()) delay(100);
			break;
		}
		// wait 10 seconds maximum for a touch panel contact
		delay(100);
		wait ++;
	}
	// erase the cross
	drawHorizontalLine(dX-10, dX+11, dY, WHITE, 2);
	drawVerticalLine(dX, dY-10, dY+11, WHITE, 2);
	delay(200);
	return (wait < 200);
}

void TouchScreen::_getDisplayXY(int16_t *x, int16_t *y) {
	if (_xCalibrationEquation.divider != 0) {
		int32_t X = *x;
		int32_t Y = *y;		
		X = (_xCalibrationEquation.a * X + _xCalibrationEquation.b) / _xCalibrationEquation.divider;
		Y = (_yCalibrationEquation.a * Y + _yCalibrationEquation.b) / _yCalibrationEquation.divider;
		*x = X;
		*y = Y;
	}
	getRotatedXY(x, y, getRotation());
}


int16_t TouchScreen::_analogAverage(uint8_t pin) {
#if (CONFIGURATION_TOUCHPANEL_NB_MEASURES <= 1)
	return analogRead(pin);
#else
	int16_t val1 = analogRead(pin);
//	Serial.print(val1, DEC);Serial.print(" "); 
	int32_t average = val1;
	for (uint8_t i=1; i<CONFIGURATION_TOUCHPANEL_NB_MEASURES; i++) {
		int16_t val2 = analogRead(pin);
// Serial.print(val2, DEC);Serial.print(" ");
		if (abs(val1-val2) > CONFIGURATION_TOUCHPANEL_DISTANCE_EQUAL) return TOUCHSCREEN_NO_TOUCH;
		average += val2;
	}
	return average / CONFIGURATION_TOUCHPANEL_NB_MEASURES;
#endif
}

uint16_t TouchScreen::_measureTouchXYZ(int16_t *x, int16_t *y, int16_t *z) {

}
