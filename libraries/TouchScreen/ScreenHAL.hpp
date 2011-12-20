#ifndef SCREEN_HAL_H
#define SCREEN_HAL_H

#include <Arduino.h>
#include <inttypes.h>

#include "fonts.hpp"

#define TOUSCRUINO_PORT 	PB
#define TOUSCRUINO_CD 		21
#define TOUSCRUINO_WR 		22
#define TOUSCRUINO_RD 		23
#define TOUSCRUINO_CS 		0xFF
#define TOUSCRUINO_RESET 	0xFF

#define SCREEN_ROTATION_0	0
#define SCREEN_ROTATION_90	1
#define SCREEN_ROTATION_180	2
#define SCREEN_ROTATION_270	3

class ScreenHAL: public Print {
    
	public:
		ScreenHAL();
	
		// required by the Print superclass
#if ARDUINO >= 100
		size_t write(uint8_t chr);
#else
		void write(uint8_t chr);
#endif
	
		// CAN BE implemented by the class inheriting from this class
		virtual void drawPixel(uint16_t x, uint16_t y, uint16_t color, bool grabAndReleaseBus = true);

		void setupScreen(uint8_t port = TOUSCRUINO_PORT, uint8_t cd = TOUSCRUINO_CD, uint8_t wr = TOUSCRUINO_WR, uint8_t rd = TOUSCRUINO_RD, uint8_t cs = TOUSCRUINO_CS, uint8_t reset = TOUSCRUINO_RESET);

		uint16_t getForegroundColor();

		void setForegroundColor(uint16_t color);		

		uint16_t getBackgroundColor();

		void setBackgroundColor(uint16_t color);

		uint16_t create565Color(uint8_t r, uint8_t g, uint8_t b);

		uint8_t getFontSize();

		void setFontSize(uint8_t size);

		uint16_t getWidth();

		uint16_t getHeight();
		
		void setRotation(uint8_t rotation);
		
		uint8_t getRotation();

		void setCursor(uint8_t x, uint8_t y);

		void drawChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize = 1, bool overlay = false, bool grabAndReleaseBus = true);

		void drawString(char *s, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize = 1, bool overlay = false, bool grabAndReleaseBus = true);

		void drawHorizontalLine(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color, uint8_t thickness = 1, bool grabAndReleaseBus = true);

		void drawVerticalLine(uint16_t x, uint16_t y1, uint16_t y2, uint16_t color, uint8_t thickness = 1, bool grabAndReleaseBus = true);

		void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness = 1, bool grabAndReleaseBus = true);
		
		void fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, bool grabAndReleaseBus = true);
		
	protected:
		volatile uint8_t *_outPort;
		volatile uint8_t *_inPort;
		volatile uint8_t *_portMode;
		volatile uint8_t *_rdPort;
		volatile uint8_t *_wrPort;
		volatile uint8_t *_cdPort;
		uint8_t _cs;
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
		
		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void initScreen(void);		

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void rotate(void);
		
		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void fillRectangleNoCheck(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);	

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void drawCharNoCheck(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, fontDefinition_t *fontDef, uint8_t fontScale, bool overlay, bool grabAndReleaseBus = true);

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void drawPixelNoCheck(uint16_t x, uint16_t y, uint16_t color);
		
		void grabBus();
		
		void releaseBus();

		void writeCommand(uint8_t command);

		void writeData(uint8_t data);

		void write16bData(uint16_t data);

		void write16bDataBuffer(uint16_t *data, uint32_t len);

		void read16bDataBuffer(uint16_t *data, uint32_t len);

};

#endif