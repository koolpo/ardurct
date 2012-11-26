/*
 * TouScruinoV1 - Screen and 5 ways switch management
 *
 * Copyright (c) 2012 Laurent Wibaux <lm.wibaux@gmail.com>
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
 *	v0.1	Initial release
 */
 
#ifndef TOUSCRUINO_V1_H
#define TOUSCRUINO_V1_H

#include "Graphics.hpp"
#include "colors.hpp"

//#include "../Printf/Printf.h"

class ArduRCT_TouScruinoV1: public ArduRCT_ST7735 {

	public:
		ArduRCT_TouScruinoV1(uint8_t cd, uint8_t cs, uint8_t reset, uint8_t backlightPin = 0xFF);
		
		void begin(uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK, uint8_t fontSize = FONT_SMALL, bool fontBold = false, bool fontOverlay = false);

		void setupBacklight(uint8_t backlightPin = CONFIGURATION_BACKLIGHT);
		
		void setBacklight(uint8_t value);
		
		uint8_t getBacklight();
		
	private:
		uint8_t _backlightPin;
		uint8_t _backlight;
}