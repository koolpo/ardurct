/*
 * ArduRCT_ST7735 - Screen driver layer
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
 * Some parts written by Limor Fried/Ladyada for Adafruit Industries.
 * 
 * This is a library for the Adafruit 1.8" SPI display.
 * This library works with the Adafruit 1.8" TFT Breakout w/SD card
 * ----> http://www.adafruit.com/products/358
 * as well as Adafruit raw 1.8" TFT display
 * ----> http://www.adafruit.com/products/618
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
 
#ifndef ARDURCT_ST7735_HPP
#define ARDURCT_ST7735_HPP

#include "ArduRCT_Graphics.hpp"
 
class ArduRCT_ST7735: public ArduRCT_Graphics {
   
	public:
		ArduRCT_ST7735();
		
		void invertDisplay(boolean invert);	
		
	protected:
		// see ArduRCT_Graphics
		void initScreenImpl();
		void fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);
		virtual uint16_t *retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
		void pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
		void setRotationImpl(uint8_t rotation);
		void drawPixelImpl(uint16_t x, uint16_t y, uint16_t color);
		void selectScreenImpl();
		void unselectScreenImpl();		
	
	private:		
		void _setClippingRectangle(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy);
		void _writeCommand(uint8_t cmd);
		
};

#endif