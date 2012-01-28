#ifndef SCREEN_HAL_HPP
#define SCREEN_HAL_HPP

#include <Arduino.h>
#include <inttypes.h>

// Comment this if you don't want to use macros to draw on the screen
#define SCREEN_MACROS 1

#include "fonts.hpp"

// Max space that a pattern or a bitmap that needs to be overlaid can occupy
// minimum is FONT_MAX_SPACE
// RAM taken can be up to 5 times this, so beware! 
#define SCREENHAL_MAX_BITMAP_SPACE (32*32)		

#define SCREEN_ROTATION_0	0
#define SCREEN_ROTATION_90	1
#define SCREEN_ROTATION_180	2
#define SCREEN_ROTATION_270	3

#define SCREEN_ARC_SSE (1 << 0)
#define SCREEN_ARC_SEE (1 << 1)
#define SCREEN_ARC_NEE (1 << 2)
#define SCREEN_ARC_NNE (1 << 3)
#define SCREEN_ARC_SSW (1 << 4)
#define SCREEN_ARC_SWW (1 << 5)
#define SCREEN_ARC_NWW (1 << 6)
#define SCREEN_ARC_NNW (1 << 7)

#define SCREEN_ARC_NE (SCREEN_ARC_NNE+SCREEN_ARC_NEE)
#define SCREEN_ARC_SE (SCREEN_ARC_SSE+SCREEN_ARC_SEE)
#define SCREEN_ARC_SW (SCREEN_ARC_SSW+SCREEN_ARC_SWW)
#define SCREEN_ARC_NW (SCREEN_ARC_NNW+SCREEN_ARC_NWW)

#define SCREEN_ARC_N (SCREEN_ARC_NE+SCREEN_ARC_NW)
#define SCREEN_ARC_S (SCREEN_ARC_SE+SCREEN_ARC_SW)
#define SCREEN_ARC_E (SCREEN_ARC_SE+SCREEN_ARC_NE)
#define SCREEN_ARC_W (SCREEN_ARC_NW+SCREEN_ARC_SW)

#define SCREEN_ARC_ALL (SCREEN_ARC_N+SCREEN_ARC_S)

#define PATTERN_ORIENTATION_HORIZONTAL 0
#define PATTERN_ORIENTATION_VERTICAL 1

#define OVERLAY true
#define NO_OVERLAY false

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

		uint16_t getMargin();

		void setMargin(uint16_t color);

		uint8_t getFontSize();
		
		bool isFontBold();
		
		bool isFontOverlay();

		void setFont(uint8_t size, bool isBold = false, bool isOverlay = false);

		uint16_t getWidth();

		uint16_t getHeight();
		
		void setRotation(uint8_t rotation);
		
		uint8_t getRotation();
		
		void getRotatedXY(int16_t *x, int16_t *y, uint8_t rotation);
		
		bool isVertical();

		void setCursor(uint8_t x, uint8_t y);

		void drawChar(uint8_t chr, int16_t x, int16_t y, uint16_t color, uint8_t fontSize = 1, bool isBold = false, bool overlay = false, bool grabAndReleaseBus = true);

		void drawString(char *s, int16_t x, int16_t y, uint16_t color, uint8_t fontSize = 1, bool isBold = false, bool overlay = false, bool grabAndReleaseBus = true);

		void drawPixel(int16_t x, int16_t y, uint16_t color, bool grabAndReleaseBus = true);

		void drawHorizontalLine(int16_t x1, int16_t x2, int16_t y, uint16_t color, int8_t thickness = 1, bool grabAndReleaseBus = true);

		void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int8_t thickness = 1, bool grabAndReleaseBus = true);

		void drawVerticalLine(int16_t x, int16_t y1, int16_t y2, uint16_t color, int8_t thickness = 1, bool grabAndReleaseBus = true);

		void drawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color, int8_t thickness = 1, bool grabAndReleaseBus = true);
		
		void fillRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color, bool grabAndReleaseBus = true);
				
		void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int8_t thickness = 1, bool grabAndReleaseBus = true);
	
		void fillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color, bool grabAndReleaseBus = true);
		
		void drawArc(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, int8_t thickness = 1, bool grabAndReleaseBus = true);

		void fillArc(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, bool grabAndReleaseBus = true);

		void drawCircle(int16_t x0, int16_t y0, uint16_t r, uint16_t color, int8_t thickness = 1, bool grabAndReleaseBus = true);

		void fillCircle(int16_t x0, int16_t y0, uint16_t r, uint16_t color, bool grabAndReleaseBus = true);
		
		void drawRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t r, uint16_t color, int8_t thickness = 1, bool grabAndReleaseBus = true);
	
		void fillRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t r, uint16_t color, bool grabAndReleaseBus = true);
		
		uint16_t *retrieveBitmap(uint16_t *bitmap, int16_t x, int16_t y, uint16_t width, uint16_t height, bool grabAndReleaseBus = true);

		void pasteBitmap(uint16_t *bitmap, int16_t x, int16_t y, uint16_t width, uint16_t height, bool hasTransparency = false, uint16_t transparentColor = 0xFFFF, bool grabAndReleaseBus = true);

		void drawPattern(uint8_t *pattern, uint8_t orientation, int16_t x, int16_t y, uint8_t width, uint8_t height, uint16_t color, uint16_t backColor, uint8_t scale = 1, bool overlay = false, bool grabAndReleaseBus = true);

	protected:
		volatile uint8_t *_outPort;
		volatile uint8_t *_inPort;
		volatile uint8_t *_portMode;
		volatile uint8_t *_rdPort;
		volatile uint8_t *_wrPort;
		volatile uint8_t *_cdPort;
		uint8_t _rd;
		uint8_t _wr;
		uint8_t _cd;		
		uint8_t _rdHigh;
		uint8_t _rdLow;
		uint8_t _wrHigh;
		uint8_t _wrLow;
		uint8_t _cdBitMask;
		
		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void initScreenImpl(void);		

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void setRotationImpl(uint8_t rotation);
		
		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);	

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void drawPixelImpl(uint16_t x, uint16_t y, uint16_t color);

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual void retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual uint16_t getWidthImpl();

		// NEEDS TO BE implemented by the class inheriting from this class
		virtual uint16_t getHeightImpl();		
		
		void writeCommand(uint8_t command);

		void writeData(uint8_t data);

		void write16bData(uint16_t data);

		void write16bDataBuffer(uint16_t *data, uint32_t len);

		uint8_t readData();

	private:
		uint8_t _cs;
		uint8_t _reset;
		uint8_t _spiUsed;
		uint8_t _busTaken;
		uint16_t _width;
		uint16_t _height;
		uint16_t _x;
		uint16_t _y;
		uint16_t _margin;
		uint8_t _fontSize;
		uint16_t _backgroundColor;
		uint16_t _foregroundColor;
		fontDefinition_t *_fontDef;
		uint8_t _fontScale;
		bool _isFontBold;
		uint8_t _rotation;
		int16_t _thickness;
		bool _isFontOverlay;

		void _grabBus();
		
		void _releaseBus();
		
		void _drawValidChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, fontDefinition_t *fontDef, uint8_t fontScale, bool fontBold, bool overlay);

		void _unpackPattern(uint8_t *pattern, uint8_t *unpacked, uint8_t width, uint8_t height, uint8_t borderRight, uint8_t borderBottom, uint8_t orientation);
		
		void _scaleShiftAndColorizeUnpackedPattern(uint8_t *unpacked, uint16_t *result, uint16_t onColor, uint16_t offColor, uint8_t width, uint8_t height, uint8_t scale, 
			uint8_t xShift = 0, uint8_t yShift = 0, bool blankFirst = true);
		
		void _fillBoundedArea(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

		void _drawBoundedPixel(int16_t x, int16_t y, uint16_t color);

		void _drawOrFillOctant(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, int8_t thickness, bool fill);

};

#endif