/*
 * ArduRCT_ST7735 - Screen driver layer
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
 *  ========================================================================
 * 		Initialization data written by Limor Fried/Ladyada 
 *		for Adafruit Industries.
 *
 * 		This library works with the Adafruit 1.8" TFT Breakout w/SD card
 * 		----> http://www.adafruit.com/products/358
 * 		as well as Adafruit raw 1.8" TFT display
 * 		----> http://www.adafruit.com/products/618
 *  ========================================================================
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
 
#include "ArduRCT_ST7735.hpp"
 
#define ST7735_WIDTH	128
#define ST7735_HEIGHT	160
 
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13
#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E
#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1
#define ST7735_PWCTR6  0xFC

#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x08
#define ST7735_MADCTL_MH  0x04

#define ST7735_COLMOD_12B 0x03
#define ST7735_COLMOD_16B 0x05

#define ST7735_DELAY 0x80

#define _spiWrite(d) { SPDR = d; while (!(SPSR & _BV(SPIF))); }

const unsigned char PROGMEM ArduRCT_ST7735_initialization_code[] = {
	0, ST7735_SWRESET,		// Software reset
	ST7735_DELAY+15,		// Delay 150ms
	0, ST7735_SLPOUT,		// SLeeP OUT
	ST7735_DELAY+50,		// Delay 500 ms
	3, ST7735_FRMCTR1, 0x01, 0x2C, 0x2D,  // Frame rate ctrl - normal mode
	3, ST7735_FRMCTR2, 0x01, 0x2C, 0x2D,  // Frame rate control - idle mode
	6, ST7735_FRMCTR3, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D,  //  5: Frame rate ctrl - partial mode, Dot inversion mode, Line inversion mode
    1, ST7735_INVCTR, 0x07,	//  Display inversion ctrl, No inversion
	3, ST7735_PWCTR1, 0xA2, 0x02, 0x84,  // Power control, -4.6V, AUTO:
    1, ST7735_PWCTR2, 0xC5,  // Power control: VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    2, ST7735_PWCTR3, 0x0A, 0x00,  // Power control, 2 args, no delay: Opamp current small, Boost frequency
    2, ST7735_PWCTR4, 0x8A, 0x2A,  // Power control: BCLK/2, Opamp current small & Medium low
    2, ST7735_PWCTR5, 0x8A, 0xEE,  // Power control
    1, ST7735_VMCTR1, 0x0E, // Power control
    0, ST7735_INVOFF,  		// Don't invert display
    1, ST7735_MADCTL, 0xC8,  // Memory access control (directions): row addr/col addr, bottom to top refresh
    1, ST7735_COLMOD, ST7735_COLMOD_16B,  // Color mode: 16bits/color
    4, ST7735_CASET, 0x00, 0x00, 0x00, 0x7F,  // Column addr set: 0 - 127
    4, ST7735_RASET, 0x00, 0x00, 0x00, 0x9F,  // Row addr set: 0 - 159
    16, ST7735_GMCTRP1, 0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10,	// Gamma control
    16, ST7735_GMCTRN1, 0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10,	// Gamma control
    0, ST7735_NORON, 		// NOrmal display ON
	ST7735_DELAY+1,			// Delay 10ms
    0, ST7735_DISPON, 		// DISPlay ON
	ST7735_DELAY+10,		// Delay 100ms
	0xFF
};

ArduRCT_ST7735::ArduRCT_ST7735(void) {
	_widthImpl = ST7735_WIDTH;
	_heightImpl = ST7735_HEIGHT;
}

void ArduRCT_ST7735::initScreenImpl(void) {
	uint8_t buffer[32];
	uint16_t i = 0;
	
	// init SPI
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2); // Full speed
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	// init the screen	
	while (1) {
		memcpy_P(buffer, &(ArduRCT_ST7735_initialization_code[i]), 32);
		if (buffer[0] == 0xFF) break;
		else if (buffer[0] & ST7735_DELAY) {
			uint16_t ms = (buffer[0] & ~ST7735_DELAY) * 10;
			delay(ms);
		} else {
			_writeCommand(buffer[1]);
			*_csPort &= ~_csBitMask;
			for (uint8_t j=0; j<buffer[0]; j++) _spiWrite(buffer[2+j]);
			i += buffer[0]+1;
			*_csPort |= _csBitMask;
		}
		i++;
	}
}

// draw a single pixel
// we inline the function to go as fast as possible
void ArduRCT_ST7735::drawPixelImpl(uint16_t x, uint16_t y, uint16_t color) {
	_setClippingRectangle(x, y, x, y);
	*_csPort &= ~_csBitMask;
	_spiWrite(color >> 8);
	_spiWrite(color);
	*_csPort |= _csBitMask;
}


// Fill the area lx,ly -> hx, hy
// this functions assumes that lx <= hx and ly <= hy
void ArduRCT_ST7735::fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
	uint16_t nbPixels = hx - lx + 1;
	nbPixels *= (hy - ly + 1);

#if defined(CONFIGURATION_12B_COLORS_ALLOWED)
	if (nbPixels > 64) {
		// switch to 12b mode
		_writeCommand(ST7735_COLMOD);
		*_csPort &= ~_csBitMask;
		_spiWrite(ST7735_COLMOD_12B);
		*_csPort |= _csBitMask;	
	}
#endif
	_setClippingRectangle(lx, ly, hx, hy);
	*_csPort &= ~_csBitMask;
#if defined(CONFIGURATION_12B_COLORS_ALLOWED)
	if (nbPixels > 64) {
		// ensure the last pixel is drawn if odd
		nbPixels = (nbPixels >> 1) + (nbPixels & 0x01);
		// keep 4 bits per color
		uint8_t colR = color >> 12;
		uint8_t colG = (color >> 6) & 0x0F;
		uint8_t colB = (color >> 1) & 0x0F;
		// build 2 times 12b in 3 bytes
		uint8_t colH = (colR << 4) + colG;
		uint8_t colM = (colB << 4) + colR;	
		uint8_t colL = (colG << 4) + colB;	
		while (nbPixels-- > 0) {
			// write 2 pixels
			_spiWrite(colH);
			_spiWrite(colM);
			_spiWrite(colL);
		}
		*_csPort |= _csBitMask;
		// switch back to 16b mode
		_writeCommand(ST7735_COLMOD);
		*_csPort &= ~_csBitMask;
		_spiWrite(ST7735_COLMOD_16B);
	} else {
#endif
		uint8_t colH = color >> 8;
		uint8_t colL = color;	
		while (nbPixels-- > 0) {
			// write 1 pixel
			_spiWrite(colH);
			_spiWrite(colL);
		}
#if defined(CONFIGURATION_12B_COLORS_ALLOWED)
	}
#endif
	*_csPort |= _csBitMask;
}


uint16_t *ArduRCT_ST7735::retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	return 0;
}	


void ArduRCT_ST7735::pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	uint16_t nbPixels = width;
	nbPixels *= height;
	_setClippingRectangle(x, y, x+width-1, y+height-1); 
	*_csPort &= ~_csBitMask;	
	for (uint16_t i=0; i<nbPixels; i++) {
		_spiWrite(bitmap[i] >> 8);
		_spiWrite(bitmap[i]);
	}
	*_csPort |= _csBitMask;
}

void ArduRCT_ST7735::setRotationImpl(uint8_t rotation) {
	_writeCommand(ST7735_MADCTL);
	*_csPort &= ~_csBitMask;	
	if (rotation == GRAPHICS_ROTATION_0) {
		_spiWrite(ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB);
	} else if (rotation == GRAPHICS_ROTATION_90) {
		_spiWrite(ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB);
	} else if (rotation == GRAPHICS_ROTATION_180) {
		_spiWrite(ST7735_MADCTL_RGB);
	} else if (rotation == GRAPHICS_ROTATION_270) {
		_spiWrite(ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB);
	}
	*_csPort |= _csBitMask;
}


void ArduRCT_ST7735::selectScreenImpl() {
#ifdef CONFIGURATION_SPI_IS_SHARED
	_spiCR = SPCR;
	_spiSR = SPSR;
	SPI.setClockDivider(SPI_CLOCK_DIV2); // Full speed
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
#endif
}


void ArduRCT_ST7735::unselectScreenImpl() {
#ifdef CONFIGURATION_SPI_IS_SHARED
	SPCR = _spiCR;
	SPSR = _spiSR;
#endif
}


void ArduRCT_ST7735::invertDisplay(boolean invert) {
	_writeCommand(invert ? ST7735_INVON : ST7735_INVOFF);
}

/* ---------------- Private functions -------------------------- */


void ArduRCT_ST7735::_writeCommand(uint8_t cmd) { 
	// change the C/D bit while unselected
	// switch to command mode
	*_cdPort &= ~_cdBitMask;  
	// select the tft
	*_csPort &= ~_csBitMask;
	// write the command
	_spiWrite(cmd);
	// unselect the screen
	*_csPort |= _csBitMask;
	// swtich to data mode
	*_cdPort |= _cdBitMask;  
}

// define the zone we are going to write to
void ArduRCT_ST7735::_setClippingRectangle(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy) {
	_writeCommand(ST7735_CASET);
	*_csPort &= ~_csBitMask;
	_spiWrite(0);
	_spiWrite(lx);
	_spiWrite(0);
	_spiWrite(hx);
	*_csPort |= _csBitMask;
	_writeCommand(ST7735_RASET);
	*_csPort &= ~_csBitMask;
	_spiWrite(0);
	_spiWrite(ly);
	_spiWrite(0);
	_spiWrite(hy);
	*_csPort |= _csBitMask;
	_writeCommand(ST7735_RAMWR);
}
