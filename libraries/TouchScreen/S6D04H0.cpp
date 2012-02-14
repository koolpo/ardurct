/*
 * S6D04H0 - Implementation of the ScreenHAL abstract functions for the S6D04H0
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

#include "S6D04H0.hpp"

#define S6D04H0_DELAY	120

#define S6D04H0_CASET	0x2a
#define S6D04H0_PASET	0x2b
#define S6D04H0_RAMWR	0x2c
#define S6D04H0_RAMRD	0x2e
#define S6D04H0_MADCTL	0x36

#define S6D04H0_R0		0x48
#define S6D04H0_R90		0x28
#define S6D04H0_R180	0x88
#define S6D04H0_R270	0xE8

// the following functions are defined as macros to speed up the execution
#define _clearRD() { *_rdPort &= _rdLow; *_rdPort &= _rdLow; }

#define _prepareWR() { _wrPortLowWR = (*_wrPort) & _wrLow; _wrPortHighWR = (*_wrPort) | _wrHigh; }

#define _pulseWR() { *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; }

#define _writeCommand(cmd) { *_cdPort &= ~_cdBitMask; *_outPort = cmd; *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; *_cdPort |= _cdBitMask; }

#define _writeData(data8b) { *_outPort = data8b; *_wrPort =	_wrPortLowWR;  *_wrPort = _wrPortHighWR; }

#define _write16bData(data16b) { *_outPort = data16b >> 8; *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; *_outPort = data16b; *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; }

const unsigned char PROGMEM S6D04H0_initialization_code[] = {
	0xFE,				/* delay(S6D04H0_DELAY) */
	0, 	0x11,			/* SLPOFF: SLeeP OFF */
	0xFE,				/* delay(S6D04H0_DELAY) */
	0, 	0x13,			/* NORON: NORmal mode ON */
	0xFE,				/* delay(S6D04H0_DELAY) */
	5, 	0xf3, 0x01, 0xff, 0x1f, 0x00, 0x03,
	17, 0xf2, 0x28, 0x60, 0x7f, 0x08, 0x08, 0x00, 0x00, 0x15, 0x48, 0x00, 0x07, 0x01, 0x00, 0x00, 0x94, 0x08, 0x08,
	20, 0xf4, 0x09, 0x00, 0x00, 0x00, 0x21, 0x47, 0x01, 0x02, 0x2A, 0x5D, 0x07, 0x2A, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	12, 0xf5, 0x00, 0x3c, 0x30, 0x00, 0x00, 0x12, 0x00, 0x00, 0x0d, 0x01, 0x00, 0x00,
	9, 	0xf6, 0x01, 0x01, 0x07, 0x00, 0x02, 0x0c, 0x02, 0x08, 0x03,
	4,	0xf7, 0x01, 0x00, 0x10, 0x00,
	3,	0xf8, 0x99, 0x00, 0x00,
	1,	0xf9, 0x01,
	1, 	0x26, 0x02,		/* GASET: Gamma Set */
	1, 	0x35, 0x00,		/* TEON: Tearing on */
	1,	0x36, 0x48,		/* MADCTL: Memory ADdress ConTroL, B7,B6,B5=0 => no rotation, B3=1 => BGR filter */
	1, 	0x3a, 0x55, 	/* COLMOD: COLor MODe: 16 bits/pixel */
	4,	0x2a, 0x00, 0x00, 0x00, 0xef,	/* CASET: Column Address SET */
	4,	0x2b, 0x00, 0x00, 0x01, 0x3f, 	/* PASET:Page Address SET */
	0,	0x29, 			/* DISPON:	DISplay ON */
	0,	0x2c,			/* RAMWR: RAM WRite */
	0xFE,				/* delay(S6D04H0_DELAY) */
	0xFF				/* End initialization */
};


S6D04H0::S6D04H0() {	
}
 
/* ---------------- Protected functions ------------------------ */

void S6D04H0::initScreenImpl(void) {
	uint8_t buffer[32];
	uint16_t i = 0;
	
	// init the screen
	_prepareWR();
	while (1) {
		memcpy_P(buffer, &(S6D04H0_initialization_code[i]), 32);
		if (buffer[0] == 0xFF) break;
		else if (buffer[0] == 0xFE) delay(S6D04H0_DELAY);
		else {
			_writeCommand(buffer[1]);
			for (uint8_t j=0; j<buffer[0]; j++) _writeData(buffer[2+j]);
			i += buffer[0]+1;
		}
		i++;
	}
}

// draw a single pixel
// we inline the function to go as fast as possible
void S6D04H0::drawPixelImpl(uint16_t x, uint16_t y, uint16_t color) {
	uint8_t xh = x >> 8;
	uint8_t xl = x;
	uint8_t yh = y >> 8;
	uint8_t yl = y;
	_prepareWR();
	_writeCommand(S6D04H0_CASET);
	_writeData(xh);
	_writeData(xl);
	_writeData(xh);
	_writeData(xl);
	_writeCommand(S6D04H0_PASET);
	_writeData(yh);
	_writeData(yl);
	_writeData(yh);
	_writeData(yl);
	_writeCommand(S6D04H0_RAMWR);
	_write16bData(color);
}


// Fill the area lx,ly -> hx, hy
// this functions assumes that lx <= hx and ly <= hy
void S6D04H0::fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
	uint32_t nbPixels = hx - lx + 1;
	nbPixels *= (hy - ly + 1);
	uint8_t colH = color >> 8;
	uint8_t colL = color;
	_prepareWR();
	_setClippingRectangle(lx, ly, hx, hy);
	_writeCommand(S6D04H0_RAMWR);
	if (colH == colL) {
		*_outPort = colH;
		nbPixels = nbPixels << 1;
		while (nbPixels-- > 0) _pulseWR();
	} else {
		while (nbPixels-- > 0) {
			_writeData(colH);
			_writeData(colL);
		}
	}
}


void S6D04H0::retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	uint32_t nbPixels = width;
	nbPixels *= height;
	
	_prepareWR();
	_setClippingRectangle(x, y, x+width-1, y+height-1); 
	_writeCommand(S6D04H0_RAMRD);
	*_portMode = 0x00;	
	// one dummy read
	_clearRD();
	bitmap[0] = *_inPort;
	*_rdPort |= _rdHigh; 
	// the data is written in R5G6B5, changed to B6G6R6 by MADCTL
	// it therefore reads back in B6G6R6 format, each color byte is [cccccc00]
	for (uint32_t i=0; i<nbPixels; i++) {
		// read B, keep 5
		_clearRD();
		bitmap[i] = (*_inPort) >> 3;
		*_rdPort |= _rdHigh; 
		// read G, keep 6
		_clearRD();
		bitmap[i] |= ((*_inPort) >> 2) << 5;
		*_rdPort |= _rdHigh; 
		// read R, keep 5
		_clearRD();
		bitmap[i] |= ((*_inPort) >> 3) << 11;
		*_rdPort |= _rdHigh; 
	}
	*_portMode = 0xFF;
}	


void S6D04H0::pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	uint32_t nbPixels = width;
	nbPixels *= height;
	_prepareWR();
	_setClippingRectangle(x, y, x+width-1, y+height-1); 
	_writeCommand(S6D04H0_RAMWR);
	for (uint32_t i=0; i<nbPixels; i++) _write16bData(bitmap[i]);
}


void S6D04H0::setRotationImpl(uint8_t rotation) {
	_prepareWR();
	if (rotation == SCREEN_ROTATION_0) {
		_writeCommand(S6D04H0_MADCTL);
		_writeData(S6D04H0_R0);
	} else if (rotation == SCREEN_ROTATION_90) {
		_writeCommand(S6D04H0_MADCTL);
		_writeData(S6D04H0_R90);
	} else if (rotation == SCREEN_ROTATION_180) {
		_writeCommand(S6D04H0_MADCTL);
		_writeData(S6D04H0_R180);		
	} else if (rotation == SCREEN_ROTATION_270) {
		_writeCommand(S6D04H0_MADCTL);
		_writeData(S6D04H0_R270);
	}
}

/* ---------------- Private functions -------------------------- */

// define the zone we are going to write to
void S6D04H0::_setClippingRectangle(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy) {
	_writeCommand(S6D04H0_CASET);
	_write16bData(lx);
	_write16bData(hx);
	_writeCommand(S6D04H0_PASET);
	_write16bData(ly);
	_write16bData(hy);
}

