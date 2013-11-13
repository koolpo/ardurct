/*
 * TouScruino
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
 *    v0.1    Initial release
 */
 
#ifndef ARDURCT_TOUSCRUINO_H
#define ARDURCT_TOUSCRUINO_H 1

#include "ArduRCT_Touscruino_Configuration.hpp"
#include <ArduRCT_Graphics.h>
#include <ArduRCT_EventManager.h>

ArduRCT_RealTimeClock touscruinoRTC;

#if (TOUSCRUINO_VERSION == 1)
/**
 * This version is based on:
 * - an SPI screen from Adafruit
 * - a 5 ways switch from Adafruit
 **/

#include <ArduRCT_TouScruinoV1.hpp>
#include <ArduRCT_ST7735.h>

int8_t touscruinoHandleGraphicsUI(uint8_t eventId, uint8_t switchId);

ArduRCT_Switch touscruinoUp(TOUSCRUINO_V1_UP);
ArduRCT_Switch touscruinoDown(TOUSCRUINO_V1_DOWN);
ArduRCT_Switch touscruinoLeft(TOUSCRUINO_V1_LEFT);
ArduRCT_Switch touscruinoRight(TOUSCRUINO_V1_RIGHT);
ArduRCT_Switch touscruinoCenter(TOUSCRUINO_V1_CENTER);

ArduRCT_TouScruinoV1 Touscruino(&touscruinoRTC, &touscruinoUp, &touscruinoDown, &touscruinoLeft, &touscruinoRight, &touscruinoCenter);

int8_t ArduRCT_TouscruinoV1HandleGraphicsUI(uint8_t eventId, uint8_t switchId) {
    uint8_t actionId = 0;
    if ((switchId != TOUSCRUINO_MENU) && (switchId != TOUSCRUINO_ENTER) && (switchId != TOUSCRUINO_UP) && 
            (switchId != TOUSCRUINO_DOWN) && (switchId != TOUSCRUINO_RIGHT)) return EVENT_HANDLING_DONE;
    if (eventId == EVENT_SWITCH_RELEASED) {
        if (switchId == TOUSCRUINO_ENTER) actionId = GRAPHICS_UI_ACTION_RELEASE;
        else if (switchId == TOUSCRUINO_MENU) actionId = GRAPHICS_UI_ACTION_MENU;
    } else if (eventId == EVENT_SWITCH_REPEATING) {
        if (switchId == TOUSCRUINO_DOWN) actionId = GRAPHICS_UI_ACTION_DOWN;
        else if (switchId == TOUSCRUINO_UP) actionId = GRAPHICS_UI_ACTION_UP;
    } else if (eventId == EVENT_SWITCH_PRESSED) {
        if (switchId == TOUSCRUINO_DOWN) actionId = GRAPHICS_UI_ACTION_DOWN;
        else if (switchId == TOUSCRUINO_UP) actionId = GRAPHICS_UI_ACTION_UP;
        else if (switchId == TOUSCRUINO_ENTER) actionId = GRAPHICS_UI_ACTION_ENTER;
    }
    if (actionId == 0) return EVENT_HANDLING_DONE;
    if (Touscruino.handleGraphicsUI(actionId, 0, 0)) return EVENT_HANDLING_EXIT;
    return EVENT_HANDLING_DONE;
}

ArduRCT_EventHandler ArduRCT_TouscruinoGraphicsHandler(EVENT_SWITCH, EVENT_ANY_VALUE, &ArduRCT_TouscruinoV1HandleGraphicsUI);

#elif (TOUSCRUINO_VERSION == 2)
/**
 *    This version is based on:
 *        - a parallel screen
 *        - a touch panel, driven by the AR1021 controller
 **/
#include <ArduRCT_TouScruinoV2.hpp>
#include <ArduRCT_SPFD5408.h>

int8_t touscruinoHandleGraphicsUI(uint8_t type, uint8_t value, int16_t x, int16_t y);

#ifdef TOUCHPANEL_MATRIX_CALIBRATION
ArduRCT_TouchPanel touscruinoTouchPanel(TOUSCRUINO_V2_TP_INT, TOUSCRUINO_V2_DRAG_TRIGGER, TOUSCRUINO_V2_WIDTH, TOUSCRUINO_V2_HEIGHT, TOUSCRUINO_EEPROM_TOUCHPANEL);
#else
ArduRCT_TouchPanel touscruinoTouchPanel(TOUSCRUINO_V2_TP_INT, TOUSCRUINO_V2_DRAG_TRIGGER, TOUSCRUINO_V2_WIDTH, TOUSCRUINO_V2_HEIGHT);
#endif

ArduRCT_TouScruinoV2 Touscruino(&touscruinoRTC, &touscruinoTouchPanel);

int8_t ArduRCT_TouscruinoV2HandleGraphicsUI(uint8_t type, uint8_t value, int16_t x, int16_t y) {
    uint8_t actionId = 0;
#ifdef TOUCHPANEL_MATRIX_CALIBRATION
    if (type == EVENT_TOUCHPANEL_CALIBRATION) {
        if (value == 1) Touscruino.setRotation(GRAPHICS_ROTATION_0); 
        Touscruino.fillScreen(WHITE);
        Touscruino.drawLine(x-10, y, x+10, y, BLACK, 3);
        Touscruino.drawLine(x, y-10, x, y+10, BLACK, 3);
        Touscruino.drawString("Touch the cross", Touscruino.getWidth()-7*Touscruino.getFontCharWidth(FONT_MEDIUM), Touscruino.getHeight()/2, BLACK, FONT_MEDIUM);
        return EVENT_HANDLING_EXIT;
    }
#endif
    if (type == EVENT_TOUCHPANEL_PRESSED) actionId = GRAPHICS_UI_ACTION_TOUCH;
    else if (type == EVENT_TOUCHPANEL_RELEASED) actionId = GRAPHICS_UI_ACTION_UNTOUCH;
    if (actionId == 0) return EVENT_HANDLING_DONE;
    if (Touscruino.handleGraphicsUI(actionId, x, y)) return EVENT_HANDLING_EXIT;
    return EVENT_HANDLING_DONE;
}

ArduRCT_EventHandler ArduRCT_TouscruinoGraphicsHandler(EVENT_TOUCHPANEL, EVENT_ANY_VALUE, 0, 0, &ArduRCT_TouscruinoV2HandleGraphicsUI);

#else
#error TOUSCRUINO_VERSION needs to be defined before #include <ArduRCT_TouScruino.h>
#endif

#define enableGraphicsUI() registerEventHandler(&ArduRCT_TouscruinoGraphicsHandler);

#endif