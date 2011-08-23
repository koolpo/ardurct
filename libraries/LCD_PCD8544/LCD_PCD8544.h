/*
 * LCD_PCD8544_VG - Interface with Nokia 5110 LCDs (or compatible) 
 *	[Philips PCD8544 controller].
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

/* 
 * Versions
 *	v1.0	Initial release
 */

#ifndef LCD_PCD8544_H
#define LCD_PCD8544_H

#include <WProgram.h>
#include <inttypes.h>

#include "LCD_PCD8544_Config.cpp"

// the char width used
#define LCD_PCD8544_CHAR_WIDTH 6

#define LCD_PCD8544_LINES (LCD_PCD8544_HEIGHT / 8)
#define LCD_PCD8544_COLUMNS (LCD_PCD8544_WIDTH / LCD_PCD8544_CHAR_WIDTH)

#define ON true
#define OFF false

#define LCD_PCD8544_CMD  LOW
#define LCD_PCD8544_DATA HIGH

#define LCD_PCD8544_PROGRESS_BAR_STYLE_LINE false
#define LCD_PCD8544_PROGRESS_BAR_STYLE_FILLED true


class LCD_PCD8544: public Print {
    
	public:
		LCD_PCD8544(uint8_t sce = LCD_PCD8544_SCE, uint8_t sclk = LCD_PCD8544_SCLK, uint8_t sdin = LCD_PCD8544_SDIN, uint8_t dc = LCD_PCD8544_DC, uint8_t reset = LCD_PCD8544_RESET);

        // Write an ASCII character at the current cursor position
		// to implement the Print superclass
        virtual void write(uint8_t chr);

        // Draw a bitmap of width x height at x y
        virtual void drawBitmap(const uint8_t *data, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
		
        // Erase everything on the display
        virtual void clear();
        virtual void clearLine();  // or just the current line

        // LCD start and stop
        virtual void begin(uint8_t bias = 2);
        void stop();
  
        // Control the display's power state
        void setPower(bool on);

        // For compatibility with the LiquidCrystal library
        void display();
        void noDisplay();

        // Activate white-on-black mode (whole display)
        void setInverse(bool inverse);

        // Place the cursor at the start of the current line
        void home();

        // Place the cursor at position (column, line)
        void setCursor(uint8_t column, uint8_t line);

		// define a glyph
		bool defineGlyph(uint8_t chr, const uint8_t *data);

		// Draw a progress bar
		virtual void drawProgressBar(uint8_t column, uint8_t line, uint8_t size, uint8_t percentage, bool filled = true);
		
		// Draw a toggle switch
		virtual void drawToggleSwitch(uint8_t column, uint8_t line, uint8_t size, uint8_t nbPositions, uint8_t position);
		
    protected:
		// display interface
		uint8_t _sce;
		uint8_t _sclk;
		uint8_t _sdin;
		uint8_t _dc;
		uint8_t _reset;
		
        // Current cursor position...
        uint8_t _column;
        uint8_t _line;

		// User defined characters
		const uint8_t *_userGlyph[LCD_PCD8544_MAX_USER_GLYPHS];

		// Setup the pins
		void setup(uint8_t sclk, uint8_t sdin, uint8_t dc, uint8_t reset, uint8_t sce);

		// Init the display
		void init(uint8_t bias);

        // Send a command or data to the display...
        void send(uint8_t type, uint8_t data);
		
		// Prepare next write to be on column x, line line
		void setXYAddress(uint8_t x, uint8_t line);
		
		// Retrieve into a buffer the chr, returns true if we have to write it or false if nothing to do
		bool bufferChar(uint8_t *buffer, uint8_t chr);
		
};


#endif  /* LCD_PCD8544_H */

