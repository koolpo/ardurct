/*
 * ButtonMonitor - Monitor buttons with No Interrups 
 *
 * Copyright (c) 2011 Laurent Wibaux <lm.wibaux@gmail.com>
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

/* 
 * Versions: see ButtonMonitor.h
 */

#include "ButtonMonitor.h"

NIButtonMonitor::NIButtonMonitor() {
	_numberOfButtons = 0;
	_nextCheck = 0;
}

bool NIButtonMonitor::add(uint8_t buttonId, uint8_t pin, bool pullUp) {
	// prepare for first active run
	if (_numberOfButtons == 0) _nextCheck = millis() + BUTTON_MONITOR_DEBOUNCE_DELAY_MS;
	// return false if no more ServoManager slot available
	if (_numberOfButtons >= BUTTON_MONITOR_NB_BUTTONS) return false;
	_button[_numberOfButtons].id = buttonId;
	_button[_numberOfButtons].pin = pin;
	_button[_numberOfButtons].state = BUTTON_MONITOR_NOT_PRESSED;
	// set the pin as input
	pinMode(pin, OUTPUT);
	// if required set a pull-up
	if (pullUp) digitalWrite(pin, HIGH);
	// store the number of monitored buttones
	_numberOfButtons ++;
	return true;
}


void NIButtonMonitor::remove(uint8_t buttonId) {
	uint8_t i = getButtonIndex(buttonId);
	if (i == 0xFF) return;
	for (;i<_numberOfButtons-1; i++) {
		_button[i].id = _button[i+1].id;
		_button[i].pin = _button[i+1].pin;
		_button[i].state = _button[i+1].state;
	}
	_numberOfButtons --;
}


/**
 * Returns true if the button has been depressed
 * once this function has been called, it will return false
 */
bool NIButtonMonitor::isDepressed(uint8_t buttonId) {
	uint8_t i = getButtonIndex(buttonId);
	if (i == 0xFF) return false;
	if (_button[i].state <= BUTTON_MONITOR_STATE_NOT_READ) return false;
	_button[i].state &= ~BUTTON_MONITOR_STATE_NOT_READ;
	return true;
}

/**
 * Returns true as long as the button is pressed
 */
bool NIButtonMonitor::isPressed(uint8_t buttonId) {
	uint8_t i = getButtonIndex(buttonId);
	if (i == 0xFF) return false;
	return (_button[i].state & ~BUTTON_MONITOR_STATE_NOT_READ > 2);
}

/**
 * Returns true if the button has been released
 * once this function has been called, it will return false
 */
bool NIButtonMonitor::isReleased(uint8_t buttonId) {
	uint8_t i = getButtonIndex(buttonId);
	if (i == 0xFF) return false;
	if (_button[i].state != BUTTON_MONITOR_STATE_NOT_READ) return false;
	_button[i].state = BUTTON_MONITOR_NOT_PRESSED;
	return true;
}

/**
 * Returns true if the button has been pressed more than BUTTON_MONITOR_REPEATING_TRIGGER
 */
bool NIButtonMonitor::isRepeating(uint8_t buttonId) {
	uint8_t i = getButtonIndex(buttonId);
	if (i == 0xFF) return false;
	return (_button[i].state & ~BUTTON_MONITOR_STATE_NOT_READ > BUTTON_MONITOR_REPEATING_TRIGGER/BUTTON_MONITOR_DEBOUNCE_DELAY_MS);
}

/**
 * Returns true if the button has been pressed more than BUTTON_MONITOR_FAST_REPEATING_TRIGGER
 */
bool NIButtonMonitor::isFastRepeating(uint8_t buttonId) {
	uint8_t i = getButtonIndex(buttonId);
	if (i == 0xFF) return false;
	return (_button[i].state & ~BUTTON_MONITOR_STATE_NOT_READ > BUTTON_MONITOR_FAST_REPEATING_TRIGGER/BUTTON_MONITOR_DEBOUNCE_DELAY_MS);
}


/**
 * Returns the state of the button
 *	BUTTON_MONITOR_NOT_PRESSED 0
 *  BUTTON_MONITOR_DEPRESSED 1
 *  BUTTON_MONITOR_PRESSED 2
 *  BUTTON_MONITOR_REPEATING 3
 *  BUTTON_MONITOR_FAST_REPEATING 4
 *  BUTTON_MONITOR_RELEASED 5
 */
uint8_t NIButtonMonitor::getState(uint8_t buttonId) {
	if (isDepressed(buttonId)) return BUTTON_MONITOR_DEPRESSED;
	if (isReleased(buttonId)) return BUTTON_MONITOR_RELEASED;
	if (isFastRepeating(buttonId)) return BUTTON_MONITOR_FAST_REPEATING;
	if (isRepeating(buttonId)) return BUTTON_MONITOR_REPEATING;
	if (isPressed(buttonId)) return BUTTON_MONITOR_PRESSED;
	return BUTTON_MONITOR_NOT_PRESSED;
}


/** 
 *	This function has to be called at least every BUTTON_MONITOR_DEBOUNCE_DELAY_MS (50ms) 
 **/
void NIButtonMonitor::update() {
	// if no button monitored, do nothing
	if (_numberOfButtons == 0) return;
	
	// wait to reach the debounce time
	if ((int32_t)(millis() - _nextCheck) < 0) return;

	// Prepare for next pulse
	_nextCheck = millis() + BUTTON_MONITOR_DEBOUNCE_DELAY_MS;
	
	// update the states of the buttons
	for (int i=0; i<_numberOfButtons; i++) {
		if (digitalRead(_button[i].pin) == HIGH) {
			if (_button[i].state != BUTTON_MONITOR_NOT_PRESSED) _button[i].state = BUTTON_MONITOR_STATE_NOT_READ;
			else _button[i].state = BUTTON_MONITOR_NOT_PRESSED;
		} else {
			if (_button[i].state & ~BUTTON_MONITOR_STATE_NOT_READ < BUTTON_MONITOR_STATE_NOT_READ-1) _button[i].state ++;
			if (_button[i].state == 2) _button[i].state += BUTTON_MONITOR_STATE_NOT_READ;
		}
	}
}


/* ---------------------- Private -------------------------------- */


uint8_t NIButtonMonitor::getButtonIndex(uint8_t buttonId) {
	for (uint8_t buttonIndex=0; buttonIndex<_numberOfButtons; buttonIndex++) {
		if (_button[buttonIndex].id == buttonId) return buttonIndex;
	}
	return 0xFF;
}


// Preinstantiate Objects //////////////////////////////////////////////////////
NIButtonMonitor ButtonMonitor = NIButtonMonitor();