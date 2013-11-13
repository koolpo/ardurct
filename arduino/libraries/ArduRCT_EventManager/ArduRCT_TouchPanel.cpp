/*
 * TouchPanel - Manages a touch panel 
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
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
 
#include <Wire.h>

#include "ArduRCT_TouchPanel.hpp"
#include "ArduRCT_EventManager_Configuration.hpp"
#include "ArduRCT_Events.hpp"

#include <avr/eeprom.h>

//#include "eepromUtils.hpp"

#define TOUCHPANEL_NOT_INITIALIZED                  0
#define TOUCHPANEL_CALIBRATION_REQUEST_CROSS_1      1
#define TOUCHPANEL_CALIBRATION_WAIT_FOR_PENDOWN_1   11
#define TOUCHPANEL_CALIBRATION_REQUEST_CROSS_2      2
#define TOUCHPANEL_CALIBRATION_WAIT_FOR_PENDOWN_2   12
#define TOUCHPANEL_CALIBRATION_REQUEST_CROSS_3      3
#define TOUCHPANEL_CALIBRATION_WAIT_FOR_PENDOWN_3   13
#define TOUCHPANEL_CALIBRATED                       0x55
#define TOUCHPANEL_NOT_CALIBRATED                   0xFF

#ifdef TOUCHPANEL_AR1021
#define AR1021_ADDRESS  0x4D
#define AR1021_X_MIN    300
#define AR1021_X_MAX    3800
#define AR1021_Y_MIN    200
#define AR1021_Y_MAX    3900
#endif

#ifdef TOUCHPANEL_MATRIX_CALIBRATION
ArduRCT_TouchPanel::ArduRCT_TouchPanel(uint8_t interruptPin, uint8_t dragTrigger, uint16_t width, uint16_t height, uint16_t calibrationMatrixEepromAddress) {
    _calibrationMatrixEepromAddress = calibrationMatrixEepromAddress;
#else
ArduRCT_TouchPanel::ArduRCT_TouchPanel(uint8_t interruptPin, uint8_t dragTrigger, uint16_t width, uint16_t height) {
#endif
    _interruptPin = interruptPin;
    _width = width;
    _height = height;
    _dragTrigger = dragTrigger;
    _state = EVENT_STATE_IDLE;
    _calibrationStatus = TOUCHPANEL_NOT_INITIALIZED;
    _touchX = TOUCHPANEL_NO_TOUCH;
    _touchY = TOUCHPANEL_NO_TOUCH;
    _touchZ = TOUCHPANEL_NO_TOUCH;    
    _graphicsRotationHandler = 0;
    if (interruptPin == 0xFF) return; 
    
    pinMode(interruptPin, INPUT);
	digitalWrite(interruptPin, HIGH);
}


int8_t ArduRCT_TouchPanel::getPenXYZ(uint16_t *x, uint16_t *y, int8_t *z) {
	if (x != 0) *x = _penX;
	if (y != 0) *y = _penY;
	if (z != 0) *z = _penZ;
	return _penZ;
}


uint8_t ArduRCT_TouchPanel::update() {
    if (_calibrationStatus < TOUCHPANEL_CALIBRATED) return _calibrate();
	if (_getTouchZ() == TOUCHPANEL_NO_TOUCH) {
		if (_state >  EVENT_STATE_IDLE) _state = EVENT_STATE_RELEASED;
		else _state = EVENT_STATE_IDLE;
		return _state;
	} 
	if ((_state == EVENT_STATE_RELEASED) || (_state == EVENT_STATE_IDLE)) _state = EVENT_STATE_PRESSED;
	else if ((abs(_penX-_touchX) > _dragTrigger) || (abs(_penY-_touchY) > _dragTrigger)) _state = EVENT_STATE_DRAGGED;
    else _state = EVENT_STATE_PRESSED_MOTIONLESS;
	if (_state != EVENT_STATE_PRESSED_MOTIONLESS) {
		_penX = _getTouchX();
		_penY = _getTouchY();
		_penZ = _getTouchZ();
	}
	return _state;
}

bool ArduRCT_TouchPanel::isPenPressed() {
	return (_state >= EVENT_STATE_PRESSED);
}

bool ArduRCT_TouchPanel::isPenPressedMotionless() {
	return (_state == EVENT_STATE_PRESSED_MOTIONLESS);
}

bool ArduRCT_TouchPanel::isPenReleased() {
	return (_state == EVENT_STATE_RELEASED);
}

bool ArduRCT_TouchPanel::isPenDragged() {
	return (_state == EVENT_STATE_DRAGGED);
}

int16_t ArduRCT_TouchPanel::getPenX() {
	return _penX;
}

int16_t ArduRCT_TouchPanel::getPenY() {
	return _penY;
}

int8_t ArduRCT_TouchPanel::getPenZ() {
	return _penZ;
}

void ArduRCT_TouchPanel::setRotation(uint8_t rotation) {
    _rotation = rotation;
}

#ifdef TOUCHPANEL_MATRIX_CALIBRATION
uint8_t ArduRCT_TouchPanel::getCalibrationCrossNumber() {
    if ((_calibrationStatus < 1) || (_calibrationStatus > 3)) return 0;
    return _calibrationStatus;
}

bool ArduRCT_TouchPanel::isCalibrating() {
    return (_calibrationStatus < TOUCHPANEL_CALIBRATED);
}

void ArduRCT_TouchPanel::getCalibrationCrossXY(uint8_t number, uint16_t *x, uint16_t *y) {
    if (number == 1) {
        *x = 15 * _width / 100;
        *y = 15 * _height / 100;
    } else if (number == 2) {
        *x = 50 * _width / 100;
        *y = 85 * _height / 100;
    } else { // 3
        *x = 85 * _width / 100;
        *y = 50 * _height / 100;
    }
}

void ArduRCT_TouchPanel::deleteCalibration() {
    eeprom_write_byte((uint8_t *)_calibrationMatrixEepromAddress, 0xFF);
    _calibrationStatus = TOUCHPANEL_NOT_INITIALIZED;
}
#endif

/* ------------------- private functions --------------------------------------- */

int16_t ArduRCT_TouchPanel::_getTouchX() {
	return _touchX;
}

int16_t ArduRCT_TouchPanel::_getTouchY() {
	return _touchY;
}

int8_t ArduRCT_TouchPanel::_getTouchZ() {
#ifdef TOUCHPANEL_AR1021
    uint8_t packet[5];
    if (digitalRead(_interruptPin) == 0) return TOUCHPANEL_NO_TOUCH;
    Wire.requestFrom(AR1021_ADDRESS, 5);
    for (uint8_t i=0; i<5; i++) packet[i] = Wire.read();
    if (packet[0] & 0x01) {
        // get the coordinates
        _touchX = packet[2];
        _touchX = _touchX << 7;
        _touchX += packet[1];
        _touchY = packet[4];
        _touchY = _touchY << 7;
        _touchY += packet[3];
        _adjustTouchWithCalibration();
    }
    return (packet[0] & 0x01 ? 100 : TOUCHPANEL_NO_TOUCH);
#else
    return TOUCHPANEL_NO_TOUCH;
#endif
}

void ArduRCT_TouchPanel::_adjustTouchWithCalibration() {
    if (_calibrationStatus == TOUCHPANEL_NOT_CALIBRATED) {
        if (_width > 0) {
#ifdef TOUCHPANEL_AR1021
            // experimental non calibrated, not very exact, but working
            _touchX = map(_touchX, AR1021_X_MIN, AR1021_X_MAX, 0, _width-1);
            _touchY = map(_touchY, AR1021_Y_MIN, AR1021_Y_MAX, 0, _height-1);
#endif
        }
#ifdef TOUCHPANEL_MATRIX_CALIBRATION
    } else if (_calibrationStatus == TOUCHPANEL_CALIBRATED) {
        int32_t X = _touchX;
        int32_t Y = _touchY;		
        X = (_xCalibrationEquation.a * X + _xCalibrationEquation.b) / _xCalibrationEquation.divider;
        Y = (_yCalibrationEquation.a * Y + _yCalibrationEquation.b) / _yCalibrationEquation.divider;
        _touchX = X;
        _touchY = Y;
#endif        
    } else return;
    // rotate the co-ordinates
    if (_rotation == TOUCHPANEL_ROTATION_0) return;
    if (_rotation == TOUCHPANEL_ROTATION_90) {
        int16_t t = _width - 1 - _touchY;
        _touchY = _touchX;
        _touchX = t;
    } else if (_rotation == TOUCHPANEL_ROTATION_180) {
        _touchX = _width - 1 - _touchX;
        _touchY = _height - 1 - _touchY;
    } else if (_rotation == TOUCHPANEL_ROTATION_270) {
        int16_t t = _touchY;
        _touchY = _height - 1 - _touchX;
        _touchX = t;
    }
}

uint8_t ArduRCT_TouchPanel::_calibrate() {
    if (_calibrationStatus == TOUCHPANEL_NOT_INITIALIZED) {
#ifdef TOUCHPANEL_AR1021
        Wire.begin();
#endif
#ifdef TOUCHPANEL_MATRIX_CALIBRATION
        if (_calibrationMatrixEepromAddress == 0xFFFF) {
            _calibrationStatus = TOUCHPANEL_NOT_CALIBRATED;
            return EVENT_STATE_IDLE;
        }
        // check if we are calibrated
        uint8_t isCalibrated = eeprom_read_byte((const uint8_t *)_calibrationMatrixEepromAddress);
        if (isCalibrated == TOUCHPANEL_CALIBRATED) {
            // read the calibration matrix in 
            _calibrationStatus = TOUCHPANEL_CALIBRATED;
            _xCalibrationEquation.a = eeprom_read_dword((const uint32_t *)(_calibrationMatrixEepromAddress-4));
            _xCalibrationEquation.b = eeprom_read_dword((const uint32_t *)(_calibrationMatrixEepromAddress-8));
            _xCalibrationEquation.divider = eeprom_read_dword((const uint32_t *)(_calibrationMatrixEepromAddress-12));
            _yCalibrationEquation.a = eeprom_read_dword((const uint32_t *)(_calibrationMatrixEepromAddress-16));
            _yCalibrationEquation.b = eeprom_read_dword((const uint32_t *)(_calibrationMatrixEepromAddress-20));
            _yCalibrationEquation.divider = eeprom_read_dword((const uint32_t *)(_calibrationMatrixEepromAddress-24));
        } else {
            _rotation = TOUCHPANEL_ROTATION_0;
            _calibrationStatus = TOUCHPANEL_CALIBRATION_REQUEST_CROSS_1;
        }
    } else if ((_calibrationStatus >= 1) && (_calibrationStatus <= 3)) {
        // move to the next step
        _calibrationStatus += 10;
    } 
    if ((_calibrationStatus >= 11) && (_calibrationStatus <= 13)) {
        // wait for a pen down
        if (_getTouchZ() != TOUCHPANEL_NO_TOUCH) {
            _calibrationX[_calibrationStatus-11] = _touchX;
            _calibrationY[_calibrationStatus-11] = _touchY;
            // request the next cross drawing
            _calibrationStatus += 1 - 10;
        }
    }
    if (_calibrationStatus == 4) {
        // we got the 3 points, calculate the matrix
        int32_t dX0, dY0;
        uint16_t dX1, dY1, dX2, dY2;
        
        getCalibrationCrossXY(1, &dX1, &dY1);
        dX0 = dX1; dY0 = dY1;
        getCalibrationCrossXY(2, &dX1, &dY1);
        getCalibrationCrossXY(3, &dX2, &dY2);
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
        _xCalibrationEquation.b = dX0 * (_calibrationX[2] - _calibrationX[0]) - _calibrationX[0] * (dX2 - dX0);
        _xCalibrationEquation.divider = _calibrationX[2] - _calibrationX[0];
        // use the points with the biggest Y difference: 0 and 1
        _yCalibrationEquation.a = dY1 - dY0;
        _yCalibrationEquation.b = dY0 * (_calibrationY[1] - _calibrationY[0]) - _calibrationY[0] * (dY1 - dY0);
        _yCalibrationEquation.divider = _calibrationY[1] - _calibrationY[0];
        
        // persist the matrix
        eeprom_write_byte((uint8_t *)_calibrationMatrixEepromAddress, TOUCHPANEL_CALIBRATED);
        eeprom_write_dword((uint32_t *)(_calibrationMatrixEepromAddress-4), _xCalibrationEquation.a);
        eeprom_write_dword((uint32_t *)(_calibrationMatrixEepromAddress-8), _xCalibrationEquation.b);
        eeprom_write_dword((uint32_t *)(_calibrationMatrixEepromAddress-12), _xCalibrationEquation.divider);
        eeprom_write_dword((uint32_t *)(_calibrationMatrixEepromAddress-16), _yCalibrationEquation.a);
        eeprom_write_dword((uint32_t *)(_calibrationMatrixEepromAddress-20), _yCalibrationEquation.b);
        eeprom_write_dword((uint32_t *)(_calibrationMatrixEepromAddress-24), _yCalibrationEquation.divider);

        _calibrationStatus = TOUCHPANEL_CALIBRATED;
#else
        _calibrationStatus = TOUCHPANEL_NOT_CALIBRATED;
#endif
    }
    return EVENT_STATE_IDLE;
}
