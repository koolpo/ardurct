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
 

#include "ArduRCT_TouchPanel.hpp"
#include "ArduRCT_EventManager.h"

ArduRCT_TouchPanel::ArduRCT_TouchPanel() {
	_state = EVENT_STATE_IDLE;
}

uint8_t ArduRCT_TouchPanel::getPenXYZ(uint16_t *x, uint16_t *y, uint8_t *z) {
	*x = _penX;
	*y = _penY;
	*z = _penZ;
	return *z;
}

uint8_t ArduRCT_TouchPanel::update() {
	if (_getTouchZ() == TOUCHPANEL_NO_TOUCH) {
		if (_state >  EVENT_STATE_IDLE) _state = EVENT_STATE_RELEASED;
		else _state = EVENT_STATE_IDLE;
		return _state;
	} 
	if ((_state == EVENT_STATE_RELEASED) || (_state == EVENT_STATE_IDLE)) _state = EVENT_STATE_PRESSED;
	else if ((abs(_penX-_getTouchX()) > TOUCHPANEL_TOLERANCE) || 
			(abs(_penY-_getTouchY()) > TOUCHPANEL_TOLERANCE)) _state = EVENT_STATE_DRAGGED;
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

uint16_t ArduRCT_TouchPanel::getPenX() {
	return _penX;
}

uint16_t ArduRCT_TouchPanel::getPenY() {
	return _penY;
}

uint8_t ArduRCT_TouchPanel::getPenZ() {
	return _penZ;
}

uint16_t ArduRCT_TouchPanel::_getTouchX() {
	return 0;
}

uint16_t ArduRCT_TouchPanel::_getTouchY() {
	return 0;
}

uint16_t ArduRCT_TouchPanel::_getTouchZ() {
	return 0;
}
