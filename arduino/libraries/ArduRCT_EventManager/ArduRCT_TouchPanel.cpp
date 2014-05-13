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

//#define TOUCHPANEL_MANUAL_CALIBRATION 1

#include "ArduRCT_TouchPanel.hpp"
#include "ArduRCT_EventManager_Configuration.hpp"
#include "ArduRCT_Events.hpp"

#include <avr/eeprom.h>

//#include "eepromUtils.hpp"

#ifdef TOUCHPANEL_AR1021
#define AR1021_ADDRESS  0x4D
#define AR1021_X_MIN    300
#define AR1021_X_MAX    3800
#define AR1021_Y_MIN    200
#define AR1021_Y_MAX    3900
#endif

#define TOUCHPANEL_X_MIN 880
#define TOUCHPANEL_X_MAX 100
#define TOUCHPANEL_Y_MIN 930
#define TOUCHPANEL_Y_MAX 110
#define TOUCHPANEL_TOUCH_TRIGGER 35
#define TOUCHPANEL_DRAG_SPEED_ANOMALY 15

#ifdef TOUCHPANEL_MATRIX_CALIBRATION
ArduRCT_TouchPanel::ArduRCT_TouchPanel(uint8_t interruptPin, uint8_t dragTrigger, uint16_t width, uint16_t height, uint16_t calibrationMatrixEepromAddress) {
    _calibrationMatrixEepromAddress = calibrationMatrixEepromAddress;
#else
ArduRCT_TouchPanel::ArduRCT_TouchPanel(uint8_t interruptPin, uint8_t dragTrigger, uint16_t width, uint16_t height) {
#endif
    _interruptPin = interruptPin;
    _xp = 0xFF;
    _width = width;
    _height = height;
    _dragTrigger = dragTrigger;
    _state = EVENT_STATE_IDLE;
    _calibrationStatus = TOUCHPANEL_NOT_INITIALIZED;
    _touchX = TOUCHPANEL_NO_TOUCH;
    _touchY = TOUCHPANEL_NO_TOUCH;
    _touchZ = TOUCHPANEL_NO_TOUCH;    
    if (interruptPin == 0xFF) return; 
    
    pinMode(interruptPin, INPUT);
	digitalWrite(interruptPin, HIGH);
}


#ifdef TOUCHPANEL_MATRIX_CALIBRATION
ArduRCT_TouchPanel::ArduRCT_TouchPanel::ArduRCT_TouchPanel(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint8_t dragTrigger, 
                uint16_t width = TOUCHPANEL_WIDTH, uint16_t height = TOUCHPANEL_HEIGHT, uint16_t calibrationMatrixEepromAddress) {
#else
ArduRCT_TouchPanel::ArduRCT_TouchPanel(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint8_t dragTrigger, uint16_t width, uint16_t height) {
#endif
    _interruptPin = 0xFF;
    _xm = xm;
    _xp = xp;
    _ym = ym;
    _yp = yp;
    _width = width;
    _height = height;
    _dragTrigger = dragTrigger;
    _state = EVENT_STATE_IDLE;
    _calibrationStatus = TOUCHPANEL_NOT_INITIALIZED;
    _touchX = TOUCHPANEL_NO_TOUCH;
    _touchY = TOUCHPANEL_NO_TOUCH;
    _touchZ = TOUCHPANEL_NO_TOUCH;    
    if (_xp == 0xFF) return; 

    pinMode(_xp, OUTPUT);
    digitalWrite(_xp, LOW);
    pinMode(_yp, OUTPUT);
    digitalWrite(_yp, LOW);
    pinMode(_xm, INPUT);
    pinMode(_ym, INPUT);
}


int16_t ArduRCT_TouchPanel::getPenXYZ(int16_t *x, int16_t *y, int16_t *z) {
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
    else if ((abs(_penX-_touchX) > TOUCHPANEL_DRAG_SPEED_ANOMALY) || (abs(_penY-_touchY) > TOUCHPANEL_DRAG_SPEED_ANOMALY)) _state = EVENT_STATE_PRESSED_MOTIONLESS;
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

int16_t ArduRCT_TouchPanel::getPenZ() {
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

int16_t ArduRCT_TouchPanel::_getTouchZ() {
    // check if we have a driver chip
    if (_xp == 0xFF) {
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
            if (!_adjustTouchWithCalibration()) return TOUCHPANEL_NO_TOUCH;
        }
        return (packet[0] & 0x01 ? 100 : TOUCHPANEL_NO_TOUCH);
#endif
    } else {
#if defined(__arm__) && defined(CORE_TEENSY)
        analogReference(EXTERNAL);
        analogReadRes(10);
#endif
        // read Z:
        // if the 2 ITO layers don't touch, the value will be 0 since XP is tied to ground
        // else it will be a different value
        pinMode(_xp, OUTPUT);
        pinMode(_yp, OUTPUT);
        digitalWrite(_yp, HIGH);
        digitalWrite(_xp, LOW);
        delay(1);
        _touchZ = analogRead(_xm);
        if (_touchZ < TOUCHPANEL_TOUCH_TRIGGER) return TOUCHPANEL_NO_TOUCH;
        // read X
        pinMode(_yp, INPUT);
        pinMode(_xm, OUTPUT);
        digitalWrite(_xp, HIGH);
        digitalWrite(_xm, LOW);
        delay(1);
        _touchX = analogRead(_ym);
        // read Y
        pinMode(_xp, INPUT);
        pinMode(_xm, INPUT);
        digitalWrite(_xp, LOW);
        pinMode(_yp, OUTPUT);
        pinMode(_ym, OUTPUT);
        digitalWrite(_yp, HIGH);
        digitalWrite(_ym, LOW);
        delay(1);
        _touchY = analogRead(_xm);
        // reset the reader: empty the capacitors on xm and ym
        pinMode(_xp, OUTPUT);
        digitalWrite(_yp, LOW);
        digitalWrite(_xp, LOW);
        pinMode(_xm, INPUT);
        pinMode(_ym, INPUT);
#ifdef TOUCHPANEL_MANUAL_CALIBRATION
        Serial.print(_touchZ); Serial.print(" "); Serial.print(_touchX); Serial.print(" "); Serial.print(_touchY);
#endif
        if (!_adjustTouchWithCalibration()) return TOUCHPANEL_NO_TOUCH;
#ifdef TOUCHPANEL_MANUAL_CALIBRATION
        Serial.print(" -> "); Serial.print(_touchX); Serial.print(" "); Serial.println(_touchY);
#endif
        return _touchZ;
    }
    return TOUCHPANEL_NO_TOUCH;
}

boolean ArduRCT_TouchPanel::_adjustTouchWithCalibration() {
    if (_calibrationStatus == TOUCHPANEL_NOT_CALIBRATED) {
        if (_width > 0) {
            if (_xp == 0xFF) {
#ifdef TOUCHPANEL_AR1021
                // experimental non calibrated, not very exact, but working
                _touchX = map(_touchX, AR1021_X_MIN, AR1021_X_MAX, 0, _width-1);
                _touchY = map(_touchY, AR1021_Y_MIN, AR1021_Y_MAX, 0, _height-1);
#endif
            } else {
                _touchX = map(_touchX, TOUCHPANEL_X_MIN, TOUCHPANEL_X_MAX, 0, _width-1);
                _touchY = map(_touchY, TOUCHPANEL_Y_MIN, TOUCHPANEL_Y_MAX, 0, _height-1);                
            }
            if ((_touchX < 0) || (_touchX >= _width) || (_touchY < 0) || (_touchY >= _height)) return false;
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
    } else return true;
    // rotate the co-ordinates
    if (_rotation == TOUCHPANEL_ROTATION_0) return true;
    if (_rotation == TOUCHPANEL_ROTATION_90) {
        int16_t t = _width - 1 - _touchX;
        _touchX = _touchY;
        _touchY = t;
    } else if (_rotation == TOUCHPANEL_ROTATION_180) {
        _touchX = _width - 1 - _touchX;
        _touchY = _height - 1 - _touchY;
    } else if (_rotation == TOUCHPANEL_ROTATION_270) {
        int16_t t = _touchX;
        _touchX = _height - 1 - _touchY;
        _touchX = t;
    }
    return true;
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
