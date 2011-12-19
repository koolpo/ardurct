#ifndef SCREEN_HAL_H
#define SCREEN_HAL_H

#include <Arduino.h>
#include <inttypes.h>

#include "fonts.hpp"

#define SCREEN_ROTATE_0		0
#define SCREEN_ROTATE_90	1
#define SCREEN_ROTATE_180	2
#define SCREEN_ROTATE_270	3

class ScreenHAL: public Print {
    
	public:
		ScreenHAL();
	
		// required by the Print superclass
#if ARDUINO >= 100
		size_t write(uint8_t chr);
#else
		void write(uint8_t chr) {
#endif
	
		// needs to be implemented by the class inheriting from this class
		virtual void ScreenHAL::initScreen();

		// needs to be implemented by the class inheriting from this class
		virtual void drawChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, fontDefinition_t *fontDef, uint8_t fontScale, boolean overlay, boolean grabAndRelease);

		// needs to be implemented by the class inheriting from this class
		virtual void drawPixel(uint16_t x, uint16_t y, uint16_t color);

		// needs to be implemented by the class inheriting from this class
		virtual void fillRectangleNoCheck(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);	

		void setupScreen(uint8_t port = PB, uint8_t cd = 21, uint8_t wr = 22, uint8_t rd = 23, uint8_t cs = 0xFF, uint8_t reset = 0xFF);
		
		uint16_t getForegroundColor();

		uint16_t getBackgroundColor();

		uint16_t getFontSize();

		uint16_t getWidth();

		uint16_t getHeight();

		void setForegroundColor(uint16_t color);
		
		void setBackgroundColor(uint16_t color);

		void setFontSize(uint8_t size);

		void setCursor(uint8_t x, uint8_t y);

		void drawChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color = WHITE, uint8_t fontSize = 1, boolean overlay = false, boolean grabAndRelease = true);

		void drawString(char *s, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize = 1, boolean overlay = false);

		void drawHorizontalLine(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color, uint8_t thickness = 1, boolean grabAndRelease = true);

		void drawVerticalLine(uint16_t x, uint16_t y1, uint16_t y2, uint16_t color, uint8_t thickness = 1, boolean grabAndRelease = true);

		void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness = 1);
		
		void fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
		
		void grabBus();
		
		void releaseBus();

		void writeCommand(uint8_t command);

		void writeData(uint8_t data);

		void write16bData(uint16_t data);

		void write16bDataBuffer(uint16_t *data, uint32_t len);

		void read16bDataBuffer(uint16_t *data, uint32_t len);

	protected
		volatile uint8_t *_outPort;
		volatile uint8_t *_inPort;
		volatile uint8_t *_portMode;
		volatile uint8_t *_rdPort;
		volatile uint8_t *_wrPort;
		volatile uint8_t *_cdPort;
		uint8_t _rdHigh;
		uint8_t _rdLow;
		uint8_t _wrHigh;
		uint8_t _wrLow;
		uint8_t _cdBitMask;
		uint8_t _spiUsed;
		uint8_t _busTaken;
		uint16_t _width;
		uint16_t _height;
		uint16_t _x;
		uint16_t _y;
		uint8_t _fontSize;
		uint16_t _backgroundColor;
		uint16_t _foregroundColor;
		fontDefinition_t *_fontDef;
		uint8_t _fontScale;
		uint8_t _rotation;
		
		// needs to be implemented by the class inheriting from this class
		virtual void rotate();
}

#endif