/*
 * ScreenHAL - Screen Hardware Abstraction Layer
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

#include "ScreenPL.hpp"

ScreenPL::ScreenPL(void) {
}


void ScreenPL::setupScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset) {	
	_outPort = portOutputRegister(port);
	_inPort = portInputRegister(port);
	_portMode = portModeRegister(port);
	_rdPort = portOutputRegister(digitalPinToPort(rd));
	_rdHigh = digitalPinToBitMask(rd);
	_rdLow = ~_rdHigh;
	_wrPort = portOutputRegister(digitalPinToPort(wr));
	_wrHigh = digitalPinToBitMask(wr);
	_wrLow = ~_wrHigh;
	_cdPort = portOutputRegister(digitalPinToPort(cd));
	_cdBitMask = digitalPinToBitMask(cd);
	_rd = rd;
	_wr = wr;
	_cd = cd;
	_cs = cs;
	_reset = reset;
#if defined(CONFIGURATION_BUS_IS_SHARED_WITH_SPI)	
	_spiUsed = 0;
#endif
	_screenSelected = 0;
}


#if ARDUINO >= 100
size_t ScreenPL::write(uint8_t chr) {
#else
void ScreenPL::write(uint8_t chr) {
#endif
#if ARDUINO >= 100
	return 1;
#endif
}

void ScreenPL::selectScreen() {
	if (_screenSelected) return;
#if defined(CONFIGURATION_BUS_IS_SHARED_WITH_SPI)	
	// if SPI is active, disable it but remember that we disabled it
	if (SPCR & _BV(SPE)) {
		SPCR &= ~_BV(SPE);
		_spiUsed = 1;
	}
#endif
	// set the direction to output
	*_portMode = 0xFF;
	// select the screen
	if (_cs != 0xFF) digitalWrite(_cs, LOW);
	// put the screen in data mode
	*_cdPort |= _cdBitMask;
	_screenSelected = true;
}

void ScreenPL::unselectScreen() {
	// unselect the screen
	if (_cs != 0xFF) digitalWrite(_cs, HIGH);
#if defined(CONFIGURATION_BUS_IS_SHARED_WITH_SPI)		
	// restore the SPI if it was active
	if (_spiUsed) {
		// we have to set SS as an output to enable SPI
		pinMode(SS, OUTPUT);
		// we always restore the master mode
		SPCR |= _BV(MSTR);
		SPCR |= _BV(SPE);
	}
#endif
	_screenSelected = false;
}
