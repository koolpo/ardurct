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

#include "ArduRCT_Configuration.hpp"
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

ArduRCT_Switch touscruinoUp(CONFIGURATION_V1_UP);
ArduRCT_Switch touscruinoDown(CONFIGURATION_V1_DOWN);
ArduRCT_Switch touscruinoLeft(CONFIGURATION_V1_LEFT);
ArduRCT_Switch touscruinoRight(CONFIGURATION_V1_RIGHT);
ArduRCT_Switch touscruinoCenter(CONFIGURATION_V1_CENTER);

ArduRCT_TouScruinoV1 touscruino(CONFIGURATION_V1_CD, CONFIGURATION_V1_CS, CONFIGURATION_V1_RST, CONFIGURATION_V1_BL, &touscruinoRTC,
    &touscruinoUp, &touscruinoDown, &touscruinoLeft, &touscruinoRight, &touscruinoCenter);

int8_t handleSwitchesForGraphicsUI(uint8_t eventId, uint8_t switchId) {
    int actionId = 0;
    if ((switchId != TOUSCRUINO_MENU) && (switchId != TOUSCRUINO_ENTER) && (switchId != TOUSCRUINO_UP) && 
            (switchId != TOUSCRUINO_DOWN)&& (switchId != TOUSCRUINO_RIGHT)) return EVENT_HANDLING_DONE;
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
    if (touscruino.handleUI(actionId, 0, 0)) return EVENT_HANDLING_EXIT;
    return EVENT_HANDLING_DONE;
}

ArduRCT_EventHandler graphicsUIHandler(EVENT_SWITCH, EVENT_ANY_VALUE, &handleSwitchesForGraphicsUI);

// to enable the handling of the GraphicsUI, you will have to register the graphicsUIHandler 
// just call enableGraphicsUI()
#define enableGraphicsUI() registerEventHandler(&graphicsUIHandler)

#elif (TOUSCRUINO_VERSION == 2)
/**
 *    This version is based on:
 *        - a parallel screen
 *        - a touch panel
 **/
#include <ArduRCT_TouScruinoV2.hpp>
#include <ArduRCT_S6D04H0.h>

ArduRCT_TouScruinoV2 touscruino(CONFIGURATION_V2_PORT, CONFIGURATION_V2_CD, CONFIGURATION_V2_WR, CONFIGURATION_V2_RD, CONFIGURATION_V2_CS, CONFIGURATION_V2_RST, CONFIGURATION_V2_BL);

#else
#error TOUSCRUINO_VERSION needs to be defined before #include <ArduRCT_TouScruino.h>
#endif

#endif