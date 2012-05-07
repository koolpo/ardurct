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
	// set the touch panel
	setupTouchPanel();
}


void TouchScreen::setupTouchPanel(uint8_t xm, uint8_t xp, uint8_t ym, uint8_t yp) {
	if (xm == 0xFF) return;
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
	_calibrationStatus = TOUCHSCREEN_CALIBRATION_UNDEFINED;
	// read the matrix from eeprom if it is defined
	uint8_t eepromCalibrationStatus = eeprom_read_uint8_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_DONE);
	if (eepromCalibrationStatus == TOUCHSCREEN_CALIBRATION_DONE) {
		_calibrationStatus = TOUCHSCREEN_CALIBRATION_DONE;
		_xCalibrationEquation.a = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_A);
		_xCalibrationEquation.b = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_B);
		_xCalibrationEquation.divider = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_DIVIDER);
		_yCalibrationEquation.a = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_A);
		_yCalibrationEquation.b = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_B);
		_yCalibrationEquation.divider = eeprom_read_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_DIVIDER);
	}
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

	// the very first call to get the touchscreen coordinates will call the calibration routine if required
	if (_calibrationStatus == TOUCHSCREEN_CALIBRATION_ERROR) return TOUCHSCREEN_NO_TOUCH;
	if (_calibrationStatus == TOUCHSCREEN_CALIBRATION_UNDEFINED) {
		_calibrationStatus = TOUCHSCREEN_CALIBRATION_RUNNING;
		if (!_calibrateTouchPanel()) _calibrationStatus = TOUCHSCREEN_CALIBRATION_ERROR;
		return TOUCHSCREEN_NO_TOUCH;
	}
		
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


void TouchScreen::resetTouchPanelCalibration() {
	eeprom_write_uint8_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_DONE, 0xFF);
}



/* ------------------------------ Private functions ------------------------------------ */

bool TouchScreen::_calibrateTouchPanel() {
	// prohibit calculation of display points, will return raw values
	//_xCalibrationEquation.divider = 0;
	// check if we are calibrated or forced to calibrate
	//if (!force && (eeprom_read_uint8_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_DONE) != 0xFF)) return true;
	
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
    eeprom_write_uint8_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_DONE, TOUCHSCREEN_CALIBRATION_DONE);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_A, _xCalibrationEquation.a);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_B, _xCalibrationEquation.b);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_DIVIDER, _xCalibrationEquation.divider);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_A, _yCalibrationEquation.a);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_B, _yCalibrationEquation.b);
    eeprom_write_int32_t(CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_DIVIDER, _yCalibrationEquation.divider);

	_calibrationStatus = TOUCHSCREEN_CALIBRATION_DONE;
	return true;
}

bool TouchScreen::_calibrateTouchPanelPoint(int32_t dX, int32_t dY, int32_t *tpX, int32_t *tpY) {
	int16_t x, y, z;
	uint8_t wait = 0;
	uint8_t fh = getFontHeight(FONT_MEDIUM);
	uint8_t fcw = getFontCharWidth(FONT_MEDIUM);
	
	// draw a cross and a message
	drawString("Touch the cross", (getWidth()-sizeof("Touch the cross")*fcw)/2, (getHeight()-fh)/2, BLACK, FONT_MEDIUM, FONT_BOLD);
	drawHorizontalLine(dX-15, dX+15, dY, FAST_BLUE, 3);
	drawVerticalLine(dX, dY-15, dY+15, FAST_BLUE, 3);
	while (wait < 50) {
		if (getTouchXYZ(&x, &y, &z)) {
			*tpX = x;
			*tpY = y;
			// erase the cross and tell the use we have a successfull touch
			drawHorizontalLine(dX-15, dX+15, dY, WHITE, 3);
			drawVerticalLine(dX, dY-15, dY+15, WHITE, 3);
			fillRectangle((getWidth()-sizeof("Touch the cross")*fcw)/2-2, (getHeight()-fh)/2-2, sizeof("Touch the cross")*fcw+4, fh+4, WHITE);
			drawString("OK", (getWidth()-sizeof("OK")*fcw)/2, (getHeight()-fh)/2, BLACK, FONT_MEDIUM, FONT_BOLD);
			// debounce the touchscreen
			delay(300);
			while (isTouched()) delay(50);
			delay(300);
			while (isTouched()) delay(50);
			break;
		}
		// wait 10 seconds maximum for a touch panel contact
		delay(200);
		wait ++;
	}
	// erase the message
	delay(500);
	if (wait >= 50) {
		drawString("Calibration failure", (getWidth()-sizeof("Calibration failure")*fcw)/2, (getHeight()-fh)/2, BLACK, FONT_MEDIUM, FONT_BOLD);
		delay(5000);
	} else fillRectangle((getWidth()-sizeof("OK")*fcw)/2-2, (getHeight()-fh)/2-2, sizeof("OK")*fcw+4, fh+4, WHITE);
	return (wait < 50);
}

void TouchScreen::_getDisplayXY(int16_t *x, int16_t *y) {
	if (_calibrationStatus == TOUCHSCREEN_CALIBRATION_DONE) {
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
#elif (CONFIGURATION_TOUCHPANEL_NB_MEASURES == 2)
	int16_t val1 = analogRead(pin);
	int16_t val2 = analogRead(pin);
	if (abs(val1-val2) > CONFIGURATION_TOUCHPANEL_DISTANCE_EQUAL) return TOUCHSCREEN_NO_TOUCH;
	return (val1+val2)/2;
#else
	int16_t val1 = analogRead(pin);
	int32_t average = val1;
	for (uint8_t i=1; i<CONFIGURATION_TOUCHPANEL_NB_MEASURES; i++) {
		int16_t val2 = analogRead(pin);
		if (abs(val1-val2) > CONFIGURATION_TOUCHPANEL_DISTANCE_EQUAL) return TOUCHSCREEN_NO_TOUCH;
		average += val2;
	}
	return average / CONFIGURATION_TOUCHPANEL_NB_MEASURES;
#endif
}
