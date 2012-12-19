/*
 * ArduRCT_Switch - Handles normally open switches
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
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


#include "ArduRCT_Switch.hpp"
#include "ArduRCT_EventManager.h"

ArduRCT_Switch::ArduRCT_Switch(uint8_t pin) {
	_pin = pin;
	_state = EVENT_STATE_IDLE;
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);
}

uint8_t ArduRCT_Switch::getPin() {
	return _pin;
}

ArduRCT_Switch *ArduRCT_Switch::getNext() {
	return _next;
}

void ArduRCT_Switch::setNext(ArduRCT_Switch *next) {
	_next = next;
}

/*
 * from idle
 * 	if pin=0 (down)
 * 		set to idle+1=debouncingDown
 * 		if still down, reach deboucingDown+3=pressed
 * 			if still down, reach pressed+20=repeating
 * 			if still down, reach repeating+80=fastRepeating
 * 			if still down and reach FF, reset to fastRepeating
 * 	if pin=1 (up)
 * 		if idle, do nothing
 * 		if was down ( >= pressed) set to debounceUp
 * 			if still down, reach debounceUp+3=released
 * 			if still down, reach released+1=idle
 * 		if was debouncingDown set to idle
 */
uint8_t ArduRCT_Switch::updateState() {
	if (digitalRead(_pin) == 1) {
		// if already idle, return
		if (_state == EVENT_STATE_IDLE) return _state;
		// if was pressed, start to debounce up
		if (_state >= EVENT_STATE_PRESSED) _state = EVENT_STATE_DEBOUNCING_UP;
		// else if was starting to be pressed, return to idle
		else if (_state >= EVENT_STATE_DEBOUNCING_DOWN) _state = EVENT_STATE_IDLE-1;
	} 
	if (_state == 0xFF) _state = EVENT_STATE_FAST_REPEATING - 1;
	_state ++;
	return _state;
}

bool ArduRCT_Switch::isPressed() {
	return (_state == EVENT_STATE_PRESSED);
}

bool ArduRCT_Switch::isReleased() {
	return (_state == EVENT_STATE_RELEASED);
}

bool ArduRCT_Switch::hasBeenPressed() {
	return (_state >= EVENT_STATE_PRESSED);
}

bool ArduRCT_Switch::isRepeating() {
	if (_state < EVENT_STATE_REPEATING) return false;
	// after SWITCH_STATE_FAST_REPEATING * EVENT_MANAGER_CYCLE = 4 seconds
	if (_state >= EVENT_STATE_FAST_REPEATING) {
		// indicate a repeat every 5 * EVENT_MANAGER_CYCLE = 250ms
		if ((_state % 5) == 0) return true;
	// after SWITCH_STATE_REPEATING * EVENT_MANAGER_CYCLE = 1 seconds
	// indicate a repeat every 10 * EVENT_MANAGER_CYCLE = 500ms
	} else if ((_state % 10) == 0) return true;
	return false;
}
