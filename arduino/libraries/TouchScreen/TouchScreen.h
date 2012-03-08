/*
 * Touchscreen - Screen and Touchpanel management
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

/* 
 * Versions
 *	v1.0	Initial release
 */
 
#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include "colors.hpp"
#include "configuration.hpp"
#include "ScreenHAL.hpp"

//#include "../Printf/Printf.h"

#if defined(CONFIGURATION_HAS_MACROS)
#include "TouchScreen_Macros.hpp"
#endif

#if defined(CONFIGURATION_HAS_UI)
#include "TouchScreen_UserInterface.hpp"
#endif

#define TOUCHSCREEN_NO_TOUCH 0

typedef struct {
	int32_t a;
	int32_t b;
	int32_t divider;
} tsCalibrationEquation_t;

class TouchScreen: public ScreenHAL {

	public:
		TouchScreen(uint8_t port = CONFIGURATION_PORT, uint8_t cd = CONFIGURATION_CD, uint8_t wr = CONFIGURATION_WR, uint8_t rd = CONFIGURATION_RD, 
				uint8_t cs = CONFIGURATION_CS, uint8_t reset = CONFIGURATION_RESET, uint8_t backlightPin = CONFIGURATION_BACKLIGHT);
		
		void setupTouchPanel(uint8_t xm = CONFIGURATION_XM, uint8_t xp = CONFIGURATION_XP, uint8_t ym = CONFIGURATION_YM, uint8_t yp = CONFIGURATION_YP);

		void begin(uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK, uint8_t fontSize = FONT_SMALL, bool fontBold = false, bool fontOverlay = false);

		void setupBacklight(uint8_t backlightPin = CONFIGURATION_BACKLIGHT);
		
		void setBacklight(uint8_t value);
		
		uint8_t getBacklight();
		
		int16_t getTouchX();
		
		bool isTouched();

		int16_t getTouchY();
		
		int16_t getTouchZ();
		
		int16_t getTouchXYZ(int16_t *x, int16_t *y, int16_t *z);
		
		bool calibrateTouchPanel(bool force = false);
		
		void resetTouchPanelCalibration();

#if defined(CONFIGURATION_HAS_MACROS)
		uint8_t *executeMacro(uint8_t *macro, int16_t x = 0, int16_t y = 0, uint16_t scaleMul = 1, uint16_t scaleDiv = 1, 
			bool continueLastMacro = false, bool selectAndUnselectScreen = true);

		void executeMacroCommand(touchScreen_macroCommand_t *mc, int16_t x = 0, int16_t y = 0, uint16_t scaleMul = 1, uint16_t scaleDiv = 1,
			bool continueLastMacro = false, bool selectAndUnselectScreen = true);
		
		void executeEepromMacro(uint8_t macroNb, int16_t x = 0, int16_t y = 0, uint16_t scaleMul = 1, uint16_t scaleDiv = 1,
			bool continueLastMacro = false, bool selectAndUnselectScreen = true);
#endif

#if defined(CONFIGURATION_HAS_UI)
		void setupUI(int16_t x = 0, int16_t y = 0, uint16_t width = CONFIGURATION_WIDTH, uint16_t height = CONFIGURATION_HEIGHT, void (*handleCallback)(uint8_t id) = 0, 
			void (*drawCallback)(uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value) = 0);
		
		int16_t addUITab(char *text, void (*handleCallback)(uint8_t id) = 0, void (*drawCallback)(uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value) = 0);
		
		int16_t addUIButton(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, char *text);
		
		int16_t addUILabel(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, char *text);
		
		int16_t addUIPushButton(int16_t tab, uint8_t id, int16_t group, int16_t x, int16_t y, uint16_t width, uint16_t height, char *text, uint16_t state);
		
		int16_t addUIArea(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height);
		
		int16_t addUIGauge(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value, int16_t min, int16_t max);
		
		int16_t addUISlider(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value, int16_t min, int16_t max);
		
		void setUITab(uint8_t tabId);
		
		void stopUI();
		
		void handleUI();
		
		uint16_t getUIElementWidth(uint8_t id);
		
		void setUIElementValue(uint8_t id, int16_t value);
		
		int16_t getUIElementValue(uint8_t id);
		
		void setUIElementEditable(uint8_t id, bool editable);
		
		void setUIElementTab(uint8_t id, uint8_t tab);
#endif
		
	private:
		uint8_t _xm;
		uint8_t _xp;
		uint8_t _ym;
		uint8_t _yp;
		uint8_t _backlightPin;
		uint8_t _backlight;
		volatile uint8_t *_xmPort;
		volatile uint8_t *_xpPort;
		volatile uint8_t *_ymPort;
		volatile uint8_t *_ypPort;
		uint8_t _xmBitMask;
		uint8_t _xpBitMask;
		uint8_t _ymBitMask;
		uint8_t _ypBitMask;
		uint16_t _xPlaneResistance;
		uint16_t _pressureThreshold;	
		tsCalibrationEquation_t _xCalibrationEquation;
		tsCalibrationEquation_t _yCalibrationEquation;
		
#if defined(CONFIGURATION_HAS_UI)
		uint32_t _uiNextHandleTime;
		touchScreen_UIElement_t _uiElement[CONFIGURATION_UI_MAX_ELEMENTS];
		uint8_t _uiNbElements;
		int16_t _uiActiveElement;
		int16_t _uiPotentialActiveElement;
		uint8_t _uiDebounceSteps;
		touchScreen_UITab_t _uiTab[CONFIGURATION_UI_MAX_TABS];
		uint8_t _uiNbTabs;
		int8_t _uiActiveTab;
		int16_t _uiX;
		int16_t _uiY;
		uint16_t _uiWidth;
		uint16_t _uiHeight;
		uint8_t _uiTabHeight;
		void (*_uiDrawCallback)(uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value);
		void (*_uiHandleCallback)(uint8_t id);
#endif
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
		
		int16_t _parseMacroCommandParameters(uint8_t *s, touchScreen_macroCommand_t *mc);

		int8_t _parseMacroCommandHexColor(uint8_t *s, int16_t n, touchScreen_macroCommand_t *mc);

		int8_t _parseMacroCommandParameter(uint8_t *s, int16_t n, touchScreen_macroCommand_t *mc, uint8_t paramId);
		
		void _executeMacroCommand(touchScreen_macroCommand_t *mc, int16_t x = 0, int16_t y = 0, uint16_t scaleMul = 1, uint16_t scaleDiv = 1);
		
		int32_t _getArcEnd(uint32_t radius, uint8_t octant, bool isReversed, bool isX);
		
		int16_t _compressMacroCommand(touchScreen_macroCommand_t *mc, uint8_t *buffer, uint16_t bufferPtr);
		
		int16_t _uncompressMacroCommand(uint8_t *buffer, uint16_t n, touchScreen_macroCommand_t *mc);
		
		int8_t _compressNumber(int16_t in, uint8_t *out, uint16_t n);
		
		int8_t _uncompressNumber(uint8_t *in, uint16_t n,  touchScreen_macroCommand_t *mc, uint8_t paramId);
#endif
#if defined(CONFIGURATION_HAS_UI)
		int16_t _addUIElement(int16_t type, int16_t tab, uint8_t id, int16_t group, int16_t x, int16_t y, int16_t width, int16_t height, int16_t value, uint8_t *text, int16_t min, int16_t max);
		
		void _drawUIElement(touchScreen_UIElement_t *elt);
		
		bool _isTouchInUITab(touchScreen_UITab_t *tab, int16_t x, int16_t y);
		
		bool _isTouchInUIElement(touchScreen_UIElement_t *elt, int16_t x, int16_t y);
		
		bool _updateUIElementValue(touchScreen_UIElement_t *elt, int16_t value, int16_t x, int16_t y) ;
		
		int16_t _getUIElementIndex(uint8_t id);
#endif
		int16_t _analogAverage(uint8_t pin);
		
		bool _calibrateTouchPanelPoint(int32_t dX, int32_t dY, int32_t *tsX, int32_t *tsY);
		
		void _getDisplayXY(int16_t *x, int16_t *y);
};

#endif