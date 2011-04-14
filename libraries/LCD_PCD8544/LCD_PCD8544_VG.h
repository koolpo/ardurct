/*
 * LCD_PCD8544_VG - Interface with Nokia 5110 LCDs (or compatible) 
 *	[Philips PCD8544 controller].
 *	With VectorGraphics
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 *	Some portions:
 * 		Copyright (c) 2010 Carlos Rodrigues <cefrodrigues@gmail.com>
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


#ifndef LCD_PCD8544_VG_H
#define LCD_PCD8544_VG_H

#include "LCD_PCD8544.h"

class LCD_PCD8544_VG: public LCD_PCD8544 {
    
	public:
		LCD_PCD8544_VG(uint8_t sce = LCD_PCD8544_SCE, uint8_t sclk = LCD_PCD8544_SCLK, uint8_t sdin = LCD_PCD8544_SDIN, uint8_t dc = LCD_PCD8544_DC, uint8_t reset = LCD_PCD8544_RESET);
	
        // Write an ASCII character at the current cursor position
		// to implement the Print superclass
        virtual void write(uint8_t chr);

        // Erase everything on the display...
        virtual void clear();
        virtual void clearLine();  // ...or just the current line
        
        // LCD initialization and finalization
        virtual void begin(uint8_t bias = 2);

		// Set the cursor on a precise column
		void setXCursor(uint8_t x, uint8_t line);
		
        // Draw a bitmap of width x height at x y
        virtual void drawBitmap(const uint8_t *data, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
		
#ifdef LCD_PCD8544_FONT_4X6
		// Draw a integer value of maxLength size, Right Aligned, padding the value with space at x, y position, using font_4x6
		uint8_t drawMediumFontValueRA(uint16_t val, uint8_t maxLength, uint8_t space, uint8_t x, uint8_t y, bool isTenTimes = false);
		
		// Draw the time at x, y, using font_4x6
		void drawMediumFontTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t x, uint8_t y);
#endif

#ifdef LCD_PCD8544_FONT_3X5
		// Draw a string of size length at x, y
		void drawSmallFontText(uint8_t *text, uint8_t length, int x, int y);
#endif
		
		// Draw a pixel
		void drawPixel(uint8_t x, uint8_t y, bool on);

		// Draw a line
		bool drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on, uint8_t thickness = 1);
  
		// Draw a rectangle
		void drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on);
		
		// Draw a circle
		void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, bool on);
		
		// Draw an ellipse
		void drawEllipse(uint8_t x0, uint8_t y0, uint8_t a, uint8_t b, bool on);
		
		// Draw a progress bar
		virtual void drawProgressBar(uint8_t column, uint8_t line, uint8_t size, uint8_t percentage, bool filled = true);

		// Fill a rectangle
		void fillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on);
		
		// Fill a circle
		void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, bool on);

		// Fill an ellipse
		void fillEllipse(uint8_t x0, uint8_t y0, uint8_t a, uint8_t b, bool on);
		
		// Draw the buffer 
		void updateDisplay();

    private:		
		// Buffer to hold the pixels and draw the graphics
        uint8_t _buffer[LCD_PCD8544_LINES][LCD_PCD8544_WIDTH];
		
#ifdef LCD_PCD8544_FONT_4X6
		// Draw a single digit for 0 to 9 or space using font_4x6
		uint8_t drawMediumFontDigit(uint8_t val, uint8_t x, uint8_t y);
#endif		

		// Set a pixel in the buffer
		void bufferPixel(int8_t x, int8_t y, bool on);
		
		// Set a full horizontal line in the buffer
		bool bufferHLine(int8_t x1, int8_t x2, int8_t y, bool on);

		// Set a full vertical line in the buffer
		void bufferVLine(uint8_t x, uint8_t y1, uint8_t y2, bool on);
		
		// Set 4 quadrants around a center in the buffer, either filled or not
		void bufferQuadrants(uint8_t x0, uint8_t y0, int8_t x, int8_t y, bool on, bool filled);
		
		// Set a circle in the buffer
		void bufferCircle(uint8_t x0, uint8_t y0, uint8_t r, bool on, bool filled);
		
		// Set an ellipse in the buffer
		void bufferEllipse(uint8_t x0, uint8_t y0, uint8_t a, uint8_t b, bool on, bool filled);
		
};


#endif  /* LCD_PCD8544_VG_H */

