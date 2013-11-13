/*
 * ArduRCT_Encoder - Handles rotary encoders
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

#ifndef ARDURCT_ENCODER
#define ARDURCT_ENCODER 1

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <inttypes.h>

#include "ArduRCT_EventManager_Configuration.hpp"

class ArduRCT_Encoder {

    public:
        ArduRCT_Encoder(uint8_t pinA, uint8_t pinB, int16_t value = 0, int16_t min = 0, int16_t max = ENCODER_STEPS-1);
        
        ArduRCT_Encoder *getNext();
        
        void setNext(ArduRCT_Encoder *next);
        
        uint8_t getPinA();
        
        int16_t updateValue();
        
        void setValue(int16_t value);
        
        void setMinMax(int16_t min, int16_t max);
        
        int16_t getValue();
        
        int16_t getChange();
		
	private:
        ArduRCT_Encoder *_next;
        uint8_t _pinA;
        uint8_t _pinB;
        int16_t _value;
        int16_t _previousValue;
        int16_t _min;
        int16_t _max;
        uint8_t _previousPosition;
};

#endif