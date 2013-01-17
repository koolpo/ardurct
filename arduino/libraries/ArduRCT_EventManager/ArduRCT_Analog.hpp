/*
  * ArduRCT_Analog - Handles analog channel changes
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

#ifndef ARDURCT_ANALOG
#define ARDURCT_ANALOG 1

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <inttype.h>

#include "ArduRCT_EventManager.h"

class ArduRCT_Analog {

	public:
		ArduRCT_Analog(uint8_t pin, uint8_t averaging = ANALOG_AVERAGING, uint8_t resolution = ANALOG_HARDWARE_RESOLUTION);
        
        uint8_t getPin();
        
        ArduRCT_Analog *getNext();
        
        void setNext(ArduRCT_Analog *next);
        
        uint16_t updateValue();
        
        int16_t getChange();
        
        uint16_t getValue();
		
	private:
		ArduRCT_Analog *_next;
		uint8_t _pin;
        uint8_t _averaging;
        uint16_t _resolution;
		uint16_t _value;
		uint16_t _previousValue;
};

#endif