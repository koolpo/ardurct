/*

 * SPFD5408 - Implementation of the ScreenHAL abstract functions for the SPFD5408
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

#ifndef SPFD5408_HPP
#define SPFD5408_HPP

// only define the connections if not defined before
#ifndef SPFD5408_DATA_OUT_PORT
#define SPFD5408_DATA_OUT_PORT PORTB
#define SPFD5408_DATA_IN_PORT PINB
#define SPFD5408_DATA_DDR_PORT DDRB
#define SPFD5408_WR_PORT PORTC
#define SPFD5408_WR_PIN 23
#define SPFD5408_RD_PIN 21
#define SPFD5408_CD_PIN 22
#define SPFD5408_CS_PIN 31
#define SPFD5408_RESET_PIN 0xFF
#define SPFD5408_SPI_ON_BUS true
#endif
// same for backlight
#ifndef SPFD5408_BACKLIGHT_PIN
#define SPFD5408_BACKLIGHT_PIN 5
#endif

#include "ArduRCT_Graphics.h"

class ArduRCT_SPFD5408: public ArduRCT_Graphics {
    
	public:
		ArduRCT_SPFD5408();
		
        uint16_t getChipId();
        
	protected:
		// see ArduRCT_Graphics
		void initScreenImpl();
		void fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);
		uint16_t *retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
		void pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
		void setRotationImpl(uint8_t rotation);
		void drawPixelImpl(uint16_t x, uint16_t y, uint16_t color);
		void selectScreenImpl();
		void unselectScreenImpl();		

	private:
		uint8_t _wrPortLowWR;
		uint8_t _wrPortHighWR;
		
        void _writeRegister(uint8_t reg, uint16_t data);
		void _setClippingRectangle(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy);
};

#endif