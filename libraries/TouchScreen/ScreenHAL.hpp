#ifndef SCREEN_HAL_HPP
#define SCREEN_HAL_HPP

#include <Arduino.h>
#include <inttypes.h>

#include "fonts.hpp"

#define SCREENHAL_MAX_BITMAP_SIZE (16*16)	/* RAM taken is twice this, so beware */

#define SCREEN_ROTATION_0	0
#define SCREEN_ROTATION_90	1
#define SCREEN_ROTATION_180	2
#define SCREEN_ROTATION_270	3

#include 
class ScreenHAL: public Print {
    
	public:
		ScreenHAL();
			
		// required by the Print superclass
#if ARDUINO >= 100
		size_t write(uint8_t chr);
#else
		void write(uint8_t chr);
#endif
		
		void initScreen();

		void setupScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset);

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
		
		void getTransformedXY(uint16_t *x, uint16_t *y, uint16_t maxX, uint16_t maxY, uint8_t rotation);
		
		bool isVertical();

		void setCursor(uint8_t x, uint8_t y);

		void drawChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize = 1, bool overlay = false, bool grabAndReleaseBus = true);

		void drawString(char *s, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize = 1, bool overlay = false, bool grabAndReleaseBus = true);

		void drawPixel(uint16_t x, uint16_t y, uint16_t color, bool grabAndReleaseBus = true);

		void drawBitmap(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool hasTransparency = false, uint16_t transparentColor = 0xFFFF, bool grabAndReleaseBus = true);

		void drawScaledPattern(uint8_t *pattern, uint8_t orientation, uint16_t x, uint16_t y, uint8_t width, uint8_t height, uint16_t color, uint16_t backColor, uint8_t scale = 1, bool overlay = false, bool grabAndReleaseBus = true);
		
		void drawHorizontalLine(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color, uint8_t thickness = 1, bool grabAndReleaseBus = true);

		void drawVerticalLine(uint16_t x, uint16_t y1, uint16_t y2, uint16_t color, uint8_t thickness = 1, bool grabAndReleaseBus = true);

		void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness = 1, bool grabAndReleaseBus = true);
		
		void fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, bool grabAndReleaseBus = true);
		
		void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t thickness = 1, bool grabAndReleaseBus = true)
		
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
		virtual void initScreenImpl(void);		

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void setRotationImpl(void);
		
		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void fillRectangleImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);	

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void drawPixelImpl(uint16_t x, uint16_t y, uint16_t color);

		// NEEDS TO BE implemented by the class inheriting from this class
		void drawBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool hasTransparency = false, uint16_t transparentColor = 0xFFFF);

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual uint16_t getWidthImpl();

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual uint16_t getHeightImpl();		
		
		void writeCommand(uint8_t command);

		void writeData(uint8_t data);

		void write16bData(uint16_t data);

		void write16bDataBuffer(uint16_t *data, uint32_t len);

		void read16bDataBuffer(uint16_t *data, uint32_t len);

	private:
		void _grabBus();
		
		void _releaseBus();
		
		void _drawValidChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, fontDefinition_t *fontDef, uint8_t fontScale, boolean overlay);

		void _unpackPattern(uint8_t *pattern, uint8_t *unpacked, uint8_t width, uint8_t height, uint8_t borderRight, uint8_t borderBottom, uint8_t orientation);
		
		void _scaleAndColorizeUnpackedPattern(uint8_t *unpacked, uint16_t *scaled, uint16_t onColor, uint16_t offColor, uint8_t width, uint8_t height, uint8_t scale);

};

#endif