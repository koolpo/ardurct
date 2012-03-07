/*
 * ServoManager - Running as many servos as there are pins with Timer1 Interrups 
 *		This breaks pwm functions associated with timer 1: pin 9 and pin 10
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

/* 
 * Versions
 *	v1.0	Initial release
 */

#ifndef SERVO_MANAGER_H
#define SERVO_MANAGER_H

#include <WProgram.h>
#include <inttypes.h>

// One pulse every 20ms, this is the standard 50Hz update rate for servos
#define SERVO_MANAGER_PERIOD_MS 20

// Set framePin and start to pulse the servos after this us time has elapsed 
#define SERVO_MANAGER_PULSE_SERVO_US_AFTER_FRAME_PULSE 500

// Number of servos managed by the library, can be as high as the number of digital pins
#define SERVO_MANAGER_NB_SERVOS 12

// Default min max and medium pulse for the servos
#define SERVO_MANAGER_MIN_PULSE 600
#define SERVO_MANAGER_MAX_PULSE 2400
#define SERVO_MANAGER_DEFAULT_PULSE 1500

// Defines the lack of framePin
#define SERVO_MANAGER_NO_FRAME_PIN	0xFF

// Synchronization: to apply all the changes at the same time rather than servo by servo
#define SERVO_MANAGER_SYNCHRONIZE true
#define SERVO_MANAGER_DON_T_SYNCHRONIZE false

typedef struct {
	uint8_t id;
	uint8_t pin;
	uint16_t value;
	uint16_t min;
	uint16_t max;
} ServoManager_Servo_t;

typedef struct {
	uint8_t pinBitMask;
	volatile uint8_t *pinPort;
	uint16_t compareTo;
	bool nextIsSame;
} ServoManager_ServoPulse_t;


class T1ServoManager {
    
	public:
		T1ServoManager();
		
		// Setup and define the pin we need to pulse to reflect the widest pulse on the servo manager
		// this framePin is used to prevent long interrupts from external sources (such as NSS)
		void setup(uint8_t framePin = SERVO_MANAGER_NO_FRAME_PIN);

		// Attach a servo to a pin
		bool attach(uint8_t servo, uint8_t pin, bool waitToSynchronize = SERVO_MANAGER_DON_T_SYNCHRONIZE);

		// Attach a servo to a pin, with a min and a max
		bool attach(uint8_t servo, uint8_t pin, uint16_t min, uint16_t max, bool waitToSynchronize = SERVO_MANAGER_DON_T_SYNCHRONIZE);
		
		// Detach a servo
		void detach(uint8_t servo, bool waitToSynchronize = SERVO_MANAGER_DON_T_SYNCHRONIZE);
		
		// Set the pulse to a value mapped between min and max
		void mapSet(uint8_t servo, int16_t value, int16_t min, int16_t max, bool waitToSynchronize = SERVO_MANAGER_DON_T_SYNCHRONIZE);

		// Set the pulse to value (if <= 180 setAngle, otherwise setMicroseconds)
		void set(uint8_t servo, uint16_t value, bool waitToSynchronize = SERVO_MANAGER_DON_T_SYNCHRONIZE);
		
		// Set the pulse to pulse microsecond for servo
		void setMicroseconds(uint8_t servo, uint16_t value, bool waitToSynchronize = SERVO_MANAGER_DON_T_SYNCHRONIZE);

		// Set the pulse to angle degrees for servo
		void setAngle(uint8_t servo, uint8_t angle, bool waitToSynchronize = SERVO_MANAGER_DON_T_SYNCHRONIZE);

		// Get the pulse for servo
		uint16_t getMicroseconds(uint8_t servo);

		// Get the angle degrees pulse for servo
		uint16_t getAngle(uint8_t servo);
		
		// Check if a servo is defined in the manager or not
		bool isDefined(uint8_t servo);
		
		// Check if a servoPin is attached to the manager or not
		bool isAttached(uint8_t servoPin);
				
		// Prints on the serial port the values of the servos
		void debug();
		
		// See if we are pulsing
		bool isPulsing();
		
		// Apply the waitToSynchronized change requests
		void synchronizeChangeRequests();
		
		// Start the servo pulses for this ServoManager
		void start();
		
		// Stop pulsing the servos
		void stop();
		
		
	private:
		uint8_t _numberOfServos;
		ServoManager_Servo_t _servo[SERVO_MANAGER_NB_SERVOS];
		uint8_t _pulseOrder[SERVO_MANAGER_NB_SERVOS];
		uint8_t _framePin;
		
		// Get the index of the servo in the servo array
		uint8_t getServoIndex(uint8_t servo);
		
		// Reorder the pulses in ascending order
		void reOrderPulses(uint8_t servoIndex, bool waitToSynchronize);
		
		// Set the servo pulses used during the interrupt
		void setServoPulses(uint8_t nbServos);
		
		// Set the pulse pin
		void setFramePin(uint8_t framePin);
};

extern T1ServoManager ServoManager;

#endif  /* LCD_PCD8544_H */

