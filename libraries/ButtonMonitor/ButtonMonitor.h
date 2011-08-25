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
 * Versions
 *	v1.0	Initial release
 */

#ifndef BUTTON_MONITOR_H
#define BUTTON_MONITOR_H

#include <WProgram.h>
#include <inttypes.h>

// Check every 50ms as default
#define BUTTON_MONITOR_DEBOUNCE_DELAY_MS 50
// Consider that the button is repeating after ? ms
#define BUTTON_MONITOR_REPEATING_TRIGGER 1000
// Consider that the button is fast repeating after ? ms
#define BUTTON_MONITOR_FAST_REPEATING_TRIGGER 3000

// Number of servos managed by the library,can be as high as the number of pins
#define BUTTON_MONITOR_NB_BUTTONS 4

// States that can be occupied by the button
#define BUTTON_MONITOR_NOT_DEPRESSED 0
#define BUTTON_MONITOR_DEPRESSED 1
#define BUTTON_MONITOR_PRESSED 2
#define BUTTON_MONITOR_REPEATING 3
#define BUTTON_MONITOR_FAST_REPEATING 4
#define BUTTON_MONITOR_RELEASED 5


// Flag to indicate that the state has not been read
#define BUTTON_MONITOR_STATE_NOT_READ 0x80


typedef struct {
	uint8_t id;
	uint8_t pin;
	uint8_t state;
} ButtonManager_Button_t;


class NIButtonMonitor {
    
	public:
		NIButtonMonitor();

		// Add a button to monitor
		bool add(uint8_t buttonId, uint8_t pin, bool pullUp = true);
		
		// Remove a button to monitor
		void remove(uint8_t buttonId);
		
		// Returns true if the button has been depressed
		// once this function has been called, it will return false
		bool isDepressed(uint8_t buttonId);
		
		// Returns true as long as the button is pressed
		bool isPressed(uint8_t buttonId);
				
		// Returns true if the button has been pressed more than BUTTON_MONITOR_REPEATING_TRIGGER
		bool isRepeating(uint8_t buttonId);
		
		// Returns true if the button has been pressed more than BUTTON_MONITOR_FAST_REPEATING_TRIGGER
		bool isFastRepeating(uint8_t buttonId);
		
		// Returns true if the button has been released
		// once this function has been called, it will return false
		bool isReleased(uint8_t buttonId);

		// Returns the state of the button
		uint8_t getState(uint8_t buttonId);
		
		// Periodically run the ButtonMonitor, need to be called in the main loop
		void update();
		
	private:
		uint8_t _numberOfButtons;
		uint32_t _nextCheck;
		ButtonManager_Button_t _button[BUTTON_MONITOR_NB_BUTTONS];
		
		// Get the index of the button in the button array
		uint8_t getButtonIndex(uint8_t buttonId);
		
};

extern NIButtonMonitor ButtonMonitor;

#endif  /* BUTTON_MONITOR_H */

