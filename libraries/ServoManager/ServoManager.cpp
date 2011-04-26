/*
 * ServoManager - Running as many servos as there are pins with No Interrups 
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
 * Versions: see ServoManager.h
 */

#include "ServoManager.h"

NIServoManager::NIServoManager() {
	_lastServo = 0;
	_nextPulse = 0;
}


bool NIServoManager::attach(uint8_t servo, uint8_t pin, uint16_t min, uint16_t max) {
	// prepare for first active run
	if (_lastServo == 0) _nextPulse = millis() + SERVO_MANAGER_INTER_PULSE_MS;
	// return false if no more ServoManager slot available
	if (_lastServo >= SERVO_MANAGER_NB_SERVOS) return false;
	_servos[_lastServo].id = servo;
	_servos[_lastServo].pin = pin;
	_servos[_lastServo].min = min;
	_servos[_lastServo].max = max;
	// set the servo to center position
	_servos[_lastServo].value = SERVO_MANAGER_DEFAULT_PULSE;
	// set the pin as output
	pinMode(pin, OUTPUT);
	// lower the pin, prepare for pulse
	digitalWrite(_servos[_lastServo].pin, LOW);
	// insert the servo pulse in the _pulseOrder
	uint8_t i;
	for (i=0; i<_lastServo; i++) {
		if (_servos[_lastServo].value < _servos[_pulseOrder[i]].value) break;
	}
	for (uint8_t j=i; j<_lastServo; j++) _pulseOrder[j+1] = _pulseOrder[j];
	_pulseOrder[i] = _lastServo;
	// store the number f active servos
	_lastServo ++;
	return true;
}


void NIServoManager::detach(uint8_t servo) {
	uint8_t i = getServoIndex(servo);
	if (i == 0xFF) return;
	for (;i<_lastServo-1; i++) {
		_servos[i].id = _servos[i+1].id;
		_servos[i].pin = _servos[i+1].pin;
		_servos[i].min = _servos[i+1].min;
		_servos[i].max = _servos[i+1].max;
		_servos[i].value =  _servos[i+1].value;
		_pulseOrder[i] = _pulseOrder[i+1];
	}
	_lastServo --;
}


void NIServoManager::set(uint8_t servo, uint16_t value) {
	if (value <= 180) setAngle(servo, value);
	else setMicroseconds(servo, value);
}


void NIServoManager::setMicroseconds(uint8_t servo, uint16_t value) {
	uint8_t i = getServoIndex(servo);
	if (i == 0xFF) return;
	uint16_t oldValue = _servos[i].value;
	_servos[i].value = constrain(value, _servos[i].min, _servos[i].max);
	if (oldValue != _servos[i].value) reOrderPulse(i, oldValue);
}


void NIServoManager::setAngle(uint8_t servo, uint16_t angle) {
	uint8_t i = getServoIndex(servo);
	if (i == 0xFF) return;
	uint16_t oldValue = _servos[i].value;
	_servos[i].value = map(angle, 0, 180, _servos[i].min,  _servos[i].max);
	if (oldValue != _servos[i].value) reOrderPulse(i, oldValue);
}


uint16_t NIServoManager::getMicroseconds(uint8_t servo) {
	uint8_t i = getServoIndex(servo);
	if (i == 0xFF) return 0xFFFF;
	return _servos[i].value;
}


uint16_t NIServoManager::getAngle(uint8_t servo) {
	uint8_t i = getServoIndex(servo);
	if (i == 0xFF) return 0xFFFF;
	return map(_servos[i].value, _servos[i].min, _servos[i].max, 0, 180);
}


bool NIServoManager::isAttached(uint8_t servo) {
	uint8_t i = getServoIndex(servo);
	if (i == 0xFF) return false;
	return true;
}

/** 
 *	This function has to be called at least every SERVO_MANAGER_INTER_PULSE_MS (50ms) 
 *	It will take between 0 and 2.5ms to run, whatever the number of servo to run
 **/
void NIServoManager::run() {
	// if no servo attached, do nothing
	if (_lastServo == 0) return;
	
	// wait to reach the interpulse
	if (millis() - _nextPulse < 0) return;
	
	uint16_t pulse = 0;
	uint8_t i = 0;

	// start the pulse on all the servos
	for (uint8_t i=0; i<_lastServo; i++) digitalWrite(_servos[i].pin, HIGH);
	
	while (i < _lastServo) {
		// wait time between last pulse width and current pulse
		delayMicroseconds(_servos[_pulseOrder[i]].value - pulse);
		// remember last pulse
		pulse = _servos[_pulseOrder[i]].value;
		// finish the pulse on the servo
		while (_servos[_pulseOrder[i]].value == pulse) {
			digitalWrite(_servos[_pulseOrder[i]].pin, LOW);
			i++;
		}
	}
	// Prepare for next pulse
	_nextPulse = millis() + SERVO_MANAGER_INTER_PULSE_MS;
}


/* ---------------------- Private -------------------------------- */


uint8_t NIServoManager::getServoIndex(uint8_t servo) {
	for (uint8_t i=0; i<_lastServo; i++) {
		if (_servos[i].id == servo) return i;
	}
	return 0xFF;
}


#define swap(a, b) { uint8_t t = a; a = b; b = t; }

void NIServoManager::reOrderPulse(uint8_t index, uint16_t oldValue) {
	uint8_t i;
	for (i=0; i<_lastServo; i++) {
		if (_pulseOrder[i] == index) break;
	}
	if (_servos[index].value > oldValue) {
		for (uint8_t j=i; j<_lastServo-1; j++) {
			if (_servos[_pulseOrder[j]].value > _servos[_pulseOrder[j+1]].value) swap(_pulseOrder[j], _pulseOrder[j+1]);
		}
	} else {
		for (uint8_t j=i; j>0; j--) {
			if (_servos[_pulseOrder[j]].value < _servos[_pulseOrder[j-1]].value) swap(_pulseOrder[j], _pulseOrder[j-1]);
		}
	}
}

// Preinstantiate Objects //////////////////////////////////////////////////////
NIServoManager ServoManager = NIServoManager();