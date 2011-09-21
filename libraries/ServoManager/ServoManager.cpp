/*
 * ServoManager - Running as many servos as there are pins with No Interrups 
 *		This breaks pwm functions associated with timer 1: pin 9 and pin 10
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
#include <pins_arduino.h>

#if F_CPU == 8000000
// with prescaler at 8, one increase per 1탎, so reload after 20000 count
#define SERVO_MANAGER_TIMER_RELOAD	(SERVO_MANAGER_PERIOD_MS*1000)
// start to pulse the servos 500탎 after the start of framePin
#define SERVO_MANAGER_PULSE_OFFSET SERVO_MANAGER_PULSE_SERVO_US_AFTER_FRAME_PULSE
// how many microseconds can separate 2 servos to consider them the same pulse
#define SERVO_MANAGER_SAME_TIME_US 32
// adjust according to the CPU frequency 
#define SERVO_MANAGER_TIME_MULTIPLIER 1
#define SERVO_MANAGER_TIME_DIVIDER 1
// compensate for to the time it takes to raise the pins in a for loop
#define SERVO_MANAGER_ADJUST_FRACTION_TOP 8
#define SERVO_MANAGER_ADJUST_FRACTION_BOTTOM 3
#elif F_CPU == 16000000
// with prescaler at 8, one increase per 0.5탎, so reload after 40000 count
#define SERVO_MANAGER_TIMER_RELOAD	(SERVO_MANAGER_PERIOD_MS*2)
#define SERVO_MANAGER_PULSE_OFFSET (SERVO_MANAGER_PULSE_SERVO_US_AFTER_FRAME_PULSE*2)
#define SERVO_MANAGER_SAME_TIME_US 16
#define SERVO_MANAGER_TIME_MULTIPLIER 2
#define SERVO_MANAGER_TIME_DIVIDER 1
#define SERVO_MANAGER_ADJUST_FRACTION_TOP 4
#define SERVO_MANAGER_ADJUST_FRACTION_BOTTOM 3
#elif F_CPU == 20000000
// with prescaler at 8, one increase per 0.4탎, so reload after 50000 count
#define SERVO_MANAGER_TIMER_RELOAD	(SERVO_MANAGER_PERIOD_MS*5/2)
#define SERVO_MANAGER_PULSE_OFFSET (SERVO_MANAGER_PULSE_SERVO_US_AFTER_FRAME_PULSE*5/2)
#define SERVO_MANAGER_SAME_TIME_US 12
#define SERVO_MANAGER_TIME_MULTIPLIER 5
#define SERVO_MANAGER_TIME_DIVIDER 2
#define SERVO_MANAGER_ADJUST_FRACTION_TOP 1
#define SERVO_MANAGER_ADJUST_FRACTION_BOTTOM 1
#else
#error This version of ServoManager supports only 20, 16 and 8MHz processors
#endif

static ServoManager_ServoPulse_t ServoManager_servoPulse[SERVO_MANAGER_NB_SERVOS];
static uint8_t ServoManager_numberOfServoPulses = 0;
static volatile uint8_t ServoManager_servoPulseIndex = 0;
static uint8_t ServoManager_framePinBitMask;
static volatile uint8_t *ServoManager_framePinPort;
static volatile bool ServoManager_inFrame = false;

#define SERVO_MANAGER_DEBUG 1
#define SERVO_MANAGER_DEBUG_PIN1 13
#define SERVO_MANAGER_DEBUG_PIN2 12
static inline void ServoManager_debugPulse(uint8_t pin, uint8_t count);

T1ServoManager::T1ServoManager() {
	_numberOfServos = 0;
#ifdef SERVO_MANAGER_DEBUG
	pinMode(SERVO_MANAGER_DEBUG_PIN1, OUTPUT);
	digitalWrite(SERVO_MANAGER_DEBUG_PIN1, LOW);
	pinMode(SERVO_MANAGER_DEBUG_PIN2, OUTPUT);
	digitalWrite(SERVO_MANAGER_DEBUG_PIN2, LOW);
#endif	
}


void T1ServoManager::setup(uint8_t framePin) {
	setFramePin(framePin);
	
	// reset the control registers for timer1
	TCCR1A = 0;
	TCCR1B = 0;
	// set timer 1 prescale factor to 8
#if defined(TCCR1B) && defined(CS11)
	TCCR1B |= _BV(CS11);
#elif defined(TCCR1) && defined(CS11)
	TCCR1B |= _BV(CS11);
#endif
	// remove all interrupts on timer1
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
    TIMSK = 0; 
#else
	TIMSK1 = 0;
#endif    
	// reset the timer registers
	TCNT1 = 0; 
	OCR1A = 0;
}

bool T1ServoManager::attach(uint8_t servo, uint8_t pin, bool waitToSynchronize) {
	attach(servo, pin, SERVO_MANAGER_MIN_PULSE, SERVO_MANAGER_MAX_PULSE, waitToSynchronize);
}


bool T1ServoManager::attach(uint8_t servo, uint8_t pin, uint16_t min, uint16_t max, bool waitToSynchronize) {
	// return false if no more ServoManager slot available
	if (_numberOfServos >= SERVO_MANAGER_NB_SERVOS) return false;
	_servo[_numberOfServos].id = servo;
	_servo[_numberOfServos].pin = pin;
	_servo[_numberOfServos].min = min;
	_servo[_numberOfServos].max = max;
	// set the servo to center position
	_servo[_numberOfServos].value = SERVO_MANAGER_DEFAULT_PULSE;
	// set the pin as output
	pinMode(pin, OUTPUT);
	// lower the pin, prepare for pulse, turn-off timers ...
	// called once so we save some time in the loop by accessing directly the ports
	digitalWrite(pin, LOW);
	// insert the servo pulse at the right place in _pulseOrder
	uint8_t i;
	for (i=0; i<_numberOfServos; i++) {
		if (_servo[_numberOfServos].value < _servo[_pulseOrder[i]].value) break;
	}
	for (uint8_t j=_numberOfServos; j>i; j--) _pulseOrder[j] = _pulseOrder[j-1];
	_pulseOrder[i] = servo;
	// store the number of active servos
	_numberOfServos ++;
	// prepare to pulse
	if (!waitToSynchronize) setServoPulses(_numberOfServos);
	return true;
}


void T1ServoManager::detach(uint8_t servo, bool waitToSynchronize) {
	uint8_t i = getServoIndex(servo);
	if (i == 0xFF) return;
	for (;i<_numberOfServos-1; i++) {
		_servo[i].id = _servo[i+1].id;
		_servo[i].pin = _servo[i+1].pin;
		_servo[i].min = _servo[i+1].min;
		_servo[i].max = _servo[i+1].max;
		_servo[i].value =  _servo[i+1].value;
		_pulseOrder[i] = _pulseOrder[i+1];
	}
	_numberOfServos --;
	// prepare to pulse
	if (!waitToSynchronize) setServoPulses(_numberOfServos);
}


void T1ServoManager::set(uint8_t servo, uint16_t value, bool waitToSynchronize) {
	if (value <= 180) mapSet(servo, value, 0, 180, waitToSynchronize);
	else setMicroseconds(servo, value, waitToSynchronize);
}


void T1ServoManager::setMicroseconds(uint8_t servo, uint16_t value, bool waitToSynchronize) {
	uint8_t servoIndex = getServoIndex(servo);
	if (servoIndex == 0xFF) return;
	uint16_t oldValue = _servo[servoIndex].value;
	_servo[servoIndex].value = constrain(value, _servo[servoIndex].min, _servo[servoIndex].max);
	if (oldValue != _servo[servoIndex].value) reOrderPulses(servoIndex, waitToSynchronize);
}


void T1ServoManager::mapSet(uint8_t servo, int16_t value, int16_t mapMin, int16_t mapMax, bool waitToSynchronize) {
	uint8_t servoIndex = getServoIndex(servo);
	if (servoIndex == 0xFF) return;
	uint16_t oldValue = _servo[servoIndex].value;
	_servo[servoIndex].value = constrain(map(value, mapMin, mapMax, _servo[servoIndex].min, _servo[servoIndex].max), _servo[servoIndex].min, _servo[servoIndex].max);
	if (oldValue != _servo[servoIndex].value) reOrderPulses(servoIndex, waitToSynchronize);
}


void T1ServoManager::setAngle(uint8_t servo, uint8_t angle, bool waitToSynchronize) {
	mapSet(servo, angle, 0, 180, waitToSynchronize);
}


uint16_t T1ServoManager::getMicroseconds(uint8_t servo) {
	uint8_t servoIndex = getServoIndex(servo);
	if (servoIndex == 0xFF) return 0xFFFF;
	return _servo[servoIndex].value;
}


uint16_t T1ServoManager::getAngle(uint8_t servo) {
	uint8_t servoIndex = getServoIndex(servo);
	if (servoIndex == 0xFF) return 0xFFFF;
	return map(_servo[servoIndex].value, _servo[servoIndex].min, _servo[servoIndex].max, 0, 180);
}


bool T1ServoManager::isDefined(uint8_t servo) {
	uint8_t servoIndex = getServoIndex(servo);
	if (servoIndex == 0xFF) return false;
	return true;
}


bool T1ServoManager::isAttached(uint8_t pin) {
	for (uint8_t i=0; i<_numberOfServos; i++) {
		if (_servo[i].pin == pin) return false;
	}
	return true;
}


bool T1ServoManager::isPulsing() {
	return ServoManager_inFrame;
}


void T1ServoManager::synchronizeChangeRequests() {
	setServoPulses(_numberOfServos);
}


void T1ServoManager::start() {
	setFramePin(_framePin);
	setServoPulses(_numberOfServos);
}


void T1ServoManager::stop() {
	setServoPulses(0);
}


void T1ServoManager::debug() {
	Serial.print(millis()/1000);
	Serial.print("s - "); 
	for (uint8_t i=0; i<_numberOfServos; i++) {
		Serial.print(_pulseOrder[i], DEC);
		Serial.print(':');
		Serial.print(_servo[_pulseOrder[i]].value, DEC);
		Serial.print(':');
		Serial.print(((uint32_t)ServoManager_servoPulse[i].compareTo+SERVO_MANAGER_TIMER_RELOAD-SERVO_MANAGER_PULSE_OFFSET-0xFFFF)*SERVO_MANAGER_TIME_DIVIDER/SERVO_MANAGER_TIME_MULTIPLIER
			- i * SERVO_MANAGER_ADJUST_FRACTION_BOTTOM / SERVO_MANAGER_ADJUST_FRACTION_TOP, DEC);
		Serial.print(':');
		Serial.print(ServoManager_servoPulse[i].nextIsSame);
		Serial.print(", ");
	}
	Serial.println();
}



/* ---------------------- static interrupts functions ----------------------------- */

SIGNAL(TIMER1_OVF_vect) {
	ServoManager_debugPulse(SERVO_MANAGER_DEBUG_PIN1, 1);
	
	// load the timer for a 20ms interval pulse
	TCNT1 = 0xFFFF - SERVO_MANAGER_TIMER_RELOAD;
	// show we are pulsing
	ServoManager_inFrame = true;
	// start the pulse on the pulsePin
	if (ServoManager_framePinBitMask != SERVO_MANAGER_NO_FRAME_PIN) *ServoManager_framePinPort |= ServoManager_framePinBitMask;
	// prepare to pulse after offset
	ServoManager_servoPulseIndex = 0xFF;
	OCR1A = 0xFFFF - SERVO_MANAGER_TIMER_RELOAD + SERVO_MANAGER_PULSE_OFFSET;
	
	ServoManager_debugPulse(SERVO_MANAGER_DEBUG_PIN1, 1);
}


SIGNAL (TIMER1_COMPA_vect) { 
	ServoManager_debugPulse(SERVO_MANAGER_DEBUG_PIN2, 1);

	// if entering the pulse
	if (ServoManager_servoPulseIndex == 0xFF) {
		// start the pulse on all the pins
		for (uint8_t i=0; i<ServoManager_numberOfServoPulses; i++) *ServoManager_servoPulse[i].pinPort |= ServoManager_servoPulse[i].pinBitMask;
		// prepare the match to the shortest pulse
		ServoManager_servoPulseIndex = 0;
	} else {
		// stop the pulse on relevant pin
		do {
			*ServoManager_servoPulse[ServoManager_servoPulseIndex].pinPort &= ~ServoManager_servoPulse[ServoManager_servoPulseIndex].pinBitMask;
			ServoManager_servoPulseIndex ++;
		} while (ServoManager_servoPulse[ServoManager_servoPulseIndex-1].nextIsSame);
		// if last pulse, stop the pulse on pinPulse
		if (ServoManager_servoPulseIndex == ServoManager_numberOfServoPulses) {
			// stop the pulse on pulsePin
			if (ServoManager_framePinBitMask != SERVO_MANAGER_NO_FRAME_PIN) *ServoManager_framePinPort &= ~ServoManager_framePinBitMask;
			ServoManager_servoPulseIndex = 0;
			ServoManager_inFrame = false;
		}
	}
	// prepare for next pulse
	OCR1A = ServoManager_servoPulse[ServoManager_servoPulseIndex].compareTo;

	ServoManager_debugPulse(SERVO_MANAGER_DEBUG_PIN2, 1);
}
	

static inline void ServoManager_debugPulse(uint8_t pin, uint8_t count) {
#ifdef SERVO_MANAGER_DEBUG
    volatile uint8_t *pport = portOutputRegister(digitalPinToPort(pin));
    uint8_t val = *pport;

    while (count--) {
        *pport = val | digitalPinToBitMask(pin);
        *pport = val;
    }
#endif
}

	
/* ---------------------- Private -------------------------------- */


uint8_t T1ServoManager::getServoIndex(uint8_t servo) {
	for (uint8_t servoIndex=0; servoIndex<_numberOfServos; servoIndex++) {
		if (_servo[servoIndex].id == servo) return servoIndex;
	}
	return 0xFF;
}


void T1ServoManager::reOrderPulses(uint8_t servoIndex, bool waitToSynchronize) {
	// find the pulse
	uint8_t i;
	for (i=0; i<_numberOfServos; i++) {
		if (_pulseOrder[i] == servoIndex) break;
	}
	// remove the pulse
	for (uint8_t j=i; j<_numberOfServos-1; j++) _pulseOrder[j] = _pulseOrder[j+1];
	// insert the pulse
	for (i=0; i<_numberOfServos-1; i++) {
		if (_servo[servoIndex].value < _servo[_pulseOrder[i]].value) break;
	}
	for (uint8_t j=_numberOfServos-1; j>i; j--) _pulseOrder[j] = _pulseOrder[j-1];
	_pulseOrder[i] = servoIndex;
	if (!waitToSynchronize) setServoPulses(_numberOfServos);
}


void T1ServoManager::setServoPulses(uint8_t nbServos) {
	// wait for the current pulse to finish
	while (ServoManager_inFrame) delayMicroseconds(20);
	// set up the pulses
	ServoManager_numberOfServoPulses = nbServos;
	// if no servo attached
	if (nbServos == 0) {
		// clear any pending interrupts
		// disable the timer1 overflow and the output compare 1A interrupts  
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
		TIFR &= ~(_BV(TOV1) | _BV(OCF1A));      
		TIMSK &= ~(_BV(OCIE1A) | _BV(TOIE1));  
#else
		TIFR1 &= ~(_BV(TOV1) | _BV(OCF1A));
		TIMSK1 &= ~(_BV(OCIE1A) | _BV(TOIE1));
#endif    	
		OCR1A = 0;
	} else {
		for (uint8_t i=0; i<ServoManager_numberOfServoPulses; i++) {
			// get direct access to ports to speed things up
			// from wiring_digital.c
			ServoManager_servoPulse[i].pinBitMask = digitalPinToBitMask(_servo[_pulseOrder[i]].pin);
			ServoManager_servoPulse[i].pinPort = portOutputRegister(digitalPinToPort(_servo[_pulseOrder[i]].pin));
			// set the compare value
			ServoManager_servoPulse[i].compareTo = 0xFFFF - SERVO_MANAGER_TIMER_RELOAD + SERVO_MANAGER_PULSE_OFFSET + 
				_servo[_pulseOrder[i]].value * SERVO_MANAGER_TIME_MULTIPLIER / SERVO_MANAGER_TIME_DIVIDER;
			if (i == ServoManager_numberOfServoPulses-1) ServoManager_servoPulse[i].nextIsSame = false;
			if (i > 0) {
				if (ServoManager_servoPulse[i].compareTo - ServoManager_servoPulse[i-1].compareTo < SERVO_MANAGER_SAME_TIME_US) ServoManager_servoPulse[i-1].nextIsSame = true;
				else ServoManager_servoPulse[i-1].nextIsSame = false;
				// avoid cascadind effects
				if (i > 1) {
					if (ServoManager_servoPulse[i-2].nextIsSame && (ServoManager_servoPulse[i].compareTo - ServoManager_servoPulse[i-2].compareTo > SERVO_MANAGER_SAME_TIME_US))
						ServoManager_servoPulse[i-1].nextIsSame = false;
				}
			}
		}	
		// adjust the compareTo to compensate for the start of pulse
		for (uint8_t i=1; i<ServoManager_numberOfServoPulses; i++) {
			ServoManager_servoPulse[i].compareTo += i * SERVO_MANAGER_ADJUST_FRACTION_TOP / SERVO_MANAGER_ADJUST_FRACTION_BOTTOM;
		}

		if (OCR1A == 0) {
			// load the timer for a 20ms interval pulse
			TCNT1 = 0xFFFF - SERVO_MANAGER_TIMER_RELOAD;
			// enable timer1 overflow and OC1A interrupts
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
			TIMSK |=  _BV(TOIE1) | _BV(OCIE1A);
#else
			TIMSK1 |= _BV(TOIE1) | _BV(OCIE1A);
#endif    
		}
		// load the first compareTo
		ServoManager_servoPulseIndex = 0xFF;
		OCR1A = 0xFFFF - SERVO_MANAGER_TIMER_RELOAD + SERVO_MANAGER_PULSE_OFFSET;

	}
}

void T1ServoManager::setFramePin(uint8_t framePin) {
	_framePin = framePin;
	ServoManager_framePinBitMask = framePin;
	if (framePin != SERVO_MANAGER_NO_FRAME_PIN) {
		ServoManager_framePinBitMask = digitalPinToBitMask(framePin);
		ServoManager_framePinPort = portOutputRegister(digitalPinToPort(framePin));
		// set the pin as output
		pinMode(framePin, OUTPUT);
		// lower the pin, prepare for pulse, turn-off timers ...
		// called once so we save some time in the loop by accessing directly the ports
		digitalWrite(framePin, LOW);
	}
}


// Preinstantiate Objects //////////////////////////////////////////////////////
T1ServoManager ServoManager = T1ServoManager();