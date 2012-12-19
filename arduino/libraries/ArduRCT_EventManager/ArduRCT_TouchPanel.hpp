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
 
#ifndef ARDURCT_TOUCH_PANEL
#define ARDURCT_TOUCH_PANEL 1

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <inttype.h>

class ArduRCT_TouchPanel {
	
	public:
	
		ArduRCT_TouchPanel();
		
		uint8_t getPenXYZ(uint16_t *x, uint16_t *y, uint8_t *z);
		
		uint8_t update();
		
		bool isPenPressed();
		
		bool isPenPressedMotionless();
		
		bool isPenReleased();
		
		bool isPenDragged();
		
		uint16_t getPenX();
		
		uint16_t getPenY();
		
		uint8_t getPenZ();
		
	private: 
		uint8_t _state;
		uint16_t _penX;
		uint16_t _penY;
		uint8_t _penZ;
		
		uint16_t _getTouchX();
		
		uint16_t _getTouchY();
		
		uint16_t _getTouchZ();

};

#endif
