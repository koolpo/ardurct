/*
 * ScreenPL - Screen Physical Layer
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
 
#ifndef SCREEN_PL_HPP
#define SCREEN_PL_HPP 1

#include <Arduino.h>
#include <inttypes.h>
#include "configuration.hpp"

#define SCREEN_ROTATION_0	0
#define SCREEN_ROTATION_90	1
#define SCREEN_ROTATION_180	2
#define SCREEN_ROTATION_270	3

class ScreenPL: public Print {
    
	public:
		ScreenPL();
			
		// required by the Print superclass
#if ARDUINO >= 100
		virtual size_t write(uint8_t chr);
#else
		virtual void write(uint8_t chr);
#endif
		
		void setupScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset);

	protected:
		volatile uint8_t *_outPort;
		volatile uint8_t *_inPort;
		volatile uint8_t *_portMode;
		volatile uint8_t *_rdPort;
		volatile uint8_t *_wrPort;
		volatile uint8_t *_cdPort;
		uint8_t _rd;
		uint8_t _wr;
		uint8_t _cd;		
		uint8_t _rdHigh;
		uint8_t _rdLow;
		uint8_t _wrHigh;
		uint8_t _wrLow;
		uint8_t _cdBitMask;
		uint8_t _cs;
		uint8_t _reset;

		virtual void selectScreen();
		
		virtual void unselectScreen();		
			
	private:
#if defined(CONFIGURATION_BUS_IS_SHARED_WITH_SPI)
		bool _spiUsed;
#endif
		bool _screenSelected;

};


#endif
