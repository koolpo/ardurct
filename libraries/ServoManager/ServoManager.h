/*
 * ServoManager - Running as many servos as there are pins with No Interrups 
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
 *	v1.2	Added mapSet method
 *	v1.1	Updated the update rate to 50Hz
 *	v1.0	Initial release
 */

#ifndef SERVO_MANAGER_H
#define SERVO_MANAGER_H

#include <WProgram.h>
#include <inttypes.h>

// On pulse every 20ms, this is the standard 50Hz update rate for servos
#define SERVO_MANAGER_INTER_PULSE_MS 20

// Number of servos managed by the library,can be as high as the number of pins
#define SERVO_MANAGER_NB_SERVOS 12

// Default min max and medium pulse for the servos
#define SERVO_MANAGER_MIN_PULSE 544
#define SERVO_MANAGER_MAX_PULSE 2400
#define SERVO_MANAGER_DEFAULT_PULSE 1500

typedef struct {
	uint8_t id;
	uint8_t pin;
	uint16_t value;
	uint16_t min;
	uint16_t max;
} ServoManager_Servo_t;


class NIServoManager {
    
	public:
		NIServoManager();

		// Attach a servo to a pin
		bool attach(uint8_t servo, uint8_t pin, uint16_t min = SERVO_MANAGER_MIN_PULSE, uint16_t max = SERVO_MANAGER_MAX_PULSE);
		
		// Detach a servo
		void detach(uint8_t servo);
		
		// Set the pulse to a value mapped between min and max
		void mapSet(uint8_t servo, uint16_t value, int16_t min, int16_t max);

		// Set the pulse to value (if <= 180 setAngle, otherwise setMicroseconds)
		void set(uint8_t servo, uint16_t value);
		
		// Set the pulse to pulse microsecond for servo
		void setMicroseconds(uint8_t servo, uint16_t value);

		// Set the pulse to angle degrees for servo
		void setAngle(uint8_t servo, uint16_t angle);

		// Get the pulse for servo
		uint16_t getMicroseconds(uint8_t servo);

		// Get the angle degrees pulse for servo
		uint16_t getAngle(uint8_t servo);
		
		// Check if a servo is attached to the manager or not
		bool isAttached(uint8_t servo);
		
		// Run the ServoManager, need to be called in the main loop
		void run();
		
	private:
		uint8_t _lastServo;
		uint32_t _nextPulse;
		ServoManager_Servo_t _servos[SERVO_MANAGER_NB_SERVOS];
		uint8_t _pulseOrder[SERVO_MANAGER_NB_SERVOS];
		
		// Get the index of the servo in the servo array
		uint8_t getServoIndex(uint8_t servo);
		
		// Reorder the pulse in ascending order
		void reOrderPulse(uint8_t index, uint16_t oldValue);
};

extern NIServoManager ServoManager;

#endif  /* LCD_PCD8544_H */

