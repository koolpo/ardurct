/*
 * ArduRCT_Graphics - Screen Hardware Abstraction Layer
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
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

#ifndef ARDURCT_GRAPHICS_HPP
#define ARDURCT_GRAPHICS_HPP 1

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <inttypes.h>
#include <avr/pgmspace.h>

#include "configuration.hpp"
#include "fonts.hpp"
#include "colors.hpp"

#if defined(CONFIGURATION_HAS_MACROS)
#include "Graphics_Macros.hpp"
#endif

#define GRAPHICS_ARC_SSE (1 << 0)
#define GRAPHICS_ARC_SEE (1 << 1)
#define GRAPHICS_ARC_NEE (1 << 2)
#define GRAPHICS_ARC_NNE (1 << 3)
#define GRAPHICS_ARC_SSW (1 << 4)
#define GRAPHICS_ARC_SWW (1 << 5)
#define GRAPHICS_ARC_NWW (1 << 6)
#define GRAPHICS_ARC_NNW (1 << 7)

#define GRAPHICS_ARC_NE (GRAPHICS_ARC_NNE+GRAPHICS_ARC_NEE)
#define GRAPHICS_ARC_SE (GRAPHICS_ARC_SSE+GRAPHICS_ARC_SEE)
#define GRAPHICS_ARC_SW (GRAPHICS_ARC_SSW+GRAPHICS_ARC_SWW)
#define GRAPHICS_ARC_NW (GRAPHICS_ARC_NNW+GRAPHICS_ARC_NWW)

#define GRAPHICS_ARC_N (GRAPHICS_ARC_NE+GRAPHICS_ARC_NW)
#define GRAPHICS_ARC_S (GRAPHICS_ARC_SE+GRAPHICS_ARC_SW)
#define GRAPHICS_ARC_E (GRAPHICS_ARC_SE+GRAPHICS_ARC_NE)
#define GRAPHICS_ARC_W (GRAPHICS_ARC_NW+GRAPHICS_ARC_SW)

#define GRAPHICS_ARC_ALL (GRAPHICS_ARC_N+GRAPHICS_ARC_S)

#define GRAPHICS_ROTATION_0		0
#define GRAPHICS_ROTATION_90	1
#define GRAPHICS_ROTATION_180	2
#define GRAPHICS_ROTATION_270	3

#define PATTERN_ORIENTATION_HORIZONTAL 0
#define PATTERN_ORIENTATION_VERTICAL 1

#define OVERLAY true
#define NO_OVERLAY false

class ArduRCT_Graphics: public Print {
   
	public:
		ArduRCT_Graphics();
			
#if ARDUINO >= 100
		size_t write(uint8_t chr);
#else
		void write(uint8_t chr);
#endif

		// used for parallel port screens
		void setupScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset, bool spiOnBus = false);

		// used for spi screens
		void setupScreen(uint8_t cd, uint8_t cs, uint8_t reset);

		void begin(uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK, uint8_t fontSize = FONT_SMALL, bool fontBold = false, bool fontOverlay = false);

		void setupBacklight(uint8_t backlightPin);
		
		void setBacklight(uint8_t value);
		
		uint8_t getBacklight();

		uint16_t getForegroundColor();

		void setForegroundColor(uint16_t color);		

		uint16_t getBackgroundColor();

		void setBackgroundColor(uint16_t color);
		
		void setThickness(int8_t thickness);

		int8_t getThickness();
		
		uint16_t getMargin();

		void setMargin(uint16_t color);

		uint8_t getFontSize();
		
		bool isFontBold();
		
		bool isFontOverlay();

		void setFont(uint8_t size, bool isBold = false, bool isOverlay = false);
		
		uint16_t getStringWidth(char *s, uint8_t fontSize);
		
		uint8_t getFontHeight(uint8_t fontSize);

		uint8_t getFontCharWidth(uint8_t fontSize);
		
		uint16_t getWidth();

		uint16_t getHeight();
		
		void setRotation(uint8_t rotation, bool selectAndUnselectScreen = true);
		
		uint8_t getRotation();
		
		void getRotatedXY(int16_t *x, int16_t *y, uint8_t rotation);
		
		bool isVertical();

		void setCursor(int16_t x, int16_t y);

		void drawChar(uint8_t chr, int16_t x, int16_t y, uint16_t color, uint8_t fontSize = 1, bool isBold = false, bool overlay = false, bool selectAndUnselectScreen = true);

		void drawString(char *s, int16_t x, int16_t y, uint16_t color, uint8_t fontSize = 1, bool isBold = false, bool overlay = false, bool selectAndUnselectScreen = true);

		void drawPixel(int16_t x, int16_t y, uint16_t color, bool selectAndUnselectScreen = true);

		void drawHorizontalLine(int16_t x1, int16_t x2, int16_t y, uint16_t color, int8_t thickness = 1, bool selectAndUnselectScreen = true);

		void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int8_t thickness = 1, bool selectAndUnselectScreen = true);

		void drawVerticalLine(int16_t x, int16_t y1, int16_t y2, uint16_t color, int8_t thickness = 1, bool selectAndUnselectScreen = true);

		void drawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color, int8_t thickness = 1, bool selectAndUnselectScreen = true);
		
		void fillRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color, bool selectAndUnselectScreen = true);
				
		void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int8_t thickness = 1, bool selectAndUnselectScreen = true);
	
		void fillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color, bool selectAndUnselectScreen = true);
		
		void drawArc(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, int8_t thickness = 1, bool selectAndUnselectScreen = true);

		void fillArc(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, bool selectAndUnselectScreen = true);

		void drawCircle(int16_t x0, int16_t y0, uint16_t r, uint16_t color, int8_t thickness = 1, bool selectAndUnselectScreen = true);

		void fillCircle(int16_t x0, int16_t y0, uint16_t r, uint16_t color, bool selectAndUnselectScreen = true);
		
		void drawRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t r, uint16_t color, int8_t thickness = 1, bool selectAndUnselectScreen = true);
	
		void fillRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t r, uint16_t color, bool selectAndUnselectScreen = true);
		
		uint16_t *retrieveBitmap(uint16_t *bitmap, int16_t x, int16_t y, uint16_t width, uint16_t height, bool selectAndUnselectScreen = true);

		void pasteBitmap(uint16_t *bitmap, int16_t x, int16_t y, uint16_t width, uint16_t height, bool hasTransparency = false, uint16_t transparentColor = 0xFFFF, bool selectAndUnselectScreen = true);

		void drawPattern(uint8_t *pattern, uint8_t orientation, int16_t x, int16_t y, uint8_t width, uint8_t height, uint16_t color, uint16_t backColor, uint8_t scale = 1, bool overlay = false, bool selectAndUnselectScreen = true);

		void fillScreen(uint16_t color, bool selectAndUnselectScreen = true);

#if defined(CONFIGURATION_HAS_MACROS)
		uint8_t *executeMacro(uint8_t *macro, int16_t x = 0, int16_t y = 0, uint16_t scaleMul = 1, uint16_t scaleDiv = 1, 
			bool continueLastMacro = false, bool selectAndUnselectScreen = true);

		void executeMacroCommand(graphics_macroCommand_t *mc, int16_t x = 0, int16_t y = 0, uint16_t scaleMul = 1, uint16_t scaleDiv = 1,
			bool continueLastMacro = false, bool selectAndUnselectScreen = true);
		
		void executeEepromMacro(uint8_t macroNb, int16_t x = 0, int16_t y = 0, uint16_t scaleMul = 1, uint16_t scaleDiv = 1,
			bool continueLastMacro = false, bool selectAndUnselectScreen = true);
#endif		
		
	protected:
		uint16_t _backgroundColor;
		uint16_t _foregroundColor;
		uint8_t _fontSize;
		bool _isFontBold;
		bool _isFontOverlay;
		int8_t _thickness;
		volatile uint8_t *_outPort;
		volatile uint8_t *_inPort;
		volatile uint8_t *_portMode;
		volatile uint8_t *_rdPort;
		volatile uint8_t *_wrPort;
		volatile uint8_t *_cdPort;
		volatile uint8_t *_csPort;
		uint8_t _rd;
		uint8_t _wr;
		uint8_t _cd;		
		uint8_t _rdHigh;
		uint8_t _rdLow;
		uint8_t _wrHigh;
		uint8_t _wrLow;
		uint8_t _cdBitMask;
		uint8_t _csBitMask;
		uint8_t _cs;
		uint8_t _reset;
		uint8_t _backlightPin;
		uint8_t _backlight;
		bool _spiUsed;
		bool _spiOnBus;
		bool _screenSelected;
		uint16_t _widthImpl;
		uint16_t _heightImpl;

		// the following functions are overwritten by the implementing class
		virtual void initScreenImpl();
		virtual void fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);
		virtual uint16_t *retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
		virtual void pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
		virtual void setRotationImpl(uint8_t rotation);
		virtual void drawPixelImpl(uint16_t x, uint16_t y, uint16_t color);
		virtual void selectScreenImpl();
		virtual void unselectScreenImpl();		
		
	private:
		uint16_t _width;
		uint16_t _height;
		int16_t _x;
		int16_t _y;
		uint16_t _margin;
		uint8_t _rotation;
		fontDefinition_t *_fontDef;
		uint8_t _fontScale;

#if defined(CONFIGURATION_HAS_MACROS)
		int16_t _mThickness;
		int16_t _mX;
		int16_t _mY;
		int16_t _mScaleMul;
		int16_t _mScaleDiv;
		bool _mIsThicknessScalable;
		uint16_t _mBackgroundColor;
		uint16_t _mForegroundColor;
		uint8_t _mFontSize;
		bool _mIsFontBold;
		bool _mIsFontOverlay;

		void _formatMacroSentence(uint8_t *s);

		void _initializeMacros();
		
		int16_t _parseMacroCommandParameters(uint8_t *s, graphics_macroCommand_t *mc);

		int8_t _parseMacroCommandHexColor(uint8_t *s, int16_t n, graphics_macroCommand_t *mc);

		int8_t _parseMacroCommandParameter(uint8_t *s, int16_t n, graphics_macroCommand_t *mc, uint8_t paramId);
		
		void _executeMacroCommand(graphics_macroCommand_t *mc, int16_t x = 0, int16_t y = 0, uint16_t scaleMul = 1, uint16_t scaleDiv = 1);
		
		int32_t _getArcEnd(uint32_t radius, uint8_t octant, bool isReversed, bool isX);
		
		int16_t _compressMacroCommand(graphics_macroCommand_t *mc, uint8_t *buffer, uint16_t bufferPtr);
		
		int16_t _uncompressMacroCommand(uint8_t *buffer, uint16_t n, graphics_macroCommand_t *mc);
		
		int8_t _compressNumber(int16_t in, uint8_t *out, uint16_t n);
		
		int8_t _uncompressNumber(uint8_t *in, uint16_t n,  graphics_macroCommand_t *mc, uint8_t paramId);
#endif
		
		void _drawValidChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, fontDefinition_t *fontDef, uint8_t fontScale, bool fontBold, bool overlay);

		void _unpackPattern(uint8_t *pattern, uint8_t *unpacked, uint8_t width, uint8_t height, uint8_t borderRight, uint8_t borderBottom, uint8_t orientation);
		
		void _scaleShiftAndColorizeUnpackedPattern(uint8_t *unpacked, uint16_t *result, uint16_t onColor, uint16_t offColor, uint8_t width, uint8_t height, uint8_t scale, 
			uint8_t xShift = 0, uint8_t yShift = 0, bool blankFirst = true);
			
		void _drawPatternPixelPerPixel(uint16_t *buffer, int16_t x, int16_t y, uint16_t onColor, uint8_t width, uint8_t height);
		
		void _fillBoundedArea(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

		void _drawBoundedPixel(int16_t x, int16_t y, uint16_t color);

		void _drawOrFillOctant(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, int8_t thickness, bool fill);

};


#endif