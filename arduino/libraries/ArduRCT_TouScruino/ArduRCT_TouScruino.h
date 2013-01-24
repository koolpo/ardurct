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
//ArduRCT_EventManager touscruinoEventManager(&touscruinoRTC);

#if (TOUSCRUINO_VERSION == 1)
/**
 * This version is based on:
 * - an SPI screen from Adafruit
 * - a 5 ways switch from Adafruit
 **/

#include <ArduRCT_TouScruinoV1.hpp>
#include <ArduRCT_ST7735.h>

#define TOUSCRUINO_UP CONFIGURATION_V1_UP
#define TOUSCRUINO_DOWN CONFIGURATION_V1_DOWN
#define TOUSCRUINO_MENU CONFIGURATION_V1_LEFT
#define TOUSCRUINO_LEFT CONFIGURATION_V1_LEFT
#define TOUSCRUINO_ENTER CONFIGURATION_V1_CENTER
#define TOUSCRUINO_CENTER CONFIGURATION_V1_CENTER
#define TOUSCRUINO_RIGHT CONFIGURATION_V1_RIGHT

ArduRCT_Switch touscruinoUp(CONFIGURATION_V1_UP);
ArduRCT_Switch touscruinoDown(CONFIGURATION_V1_DOWN);
ArduRCT_Switch touscruinoLeft(CONFIGURATION_V1_LEFT);
ArduRCT_Switch touscruinoRight(CONFIGURATION_V1_RIGHT);
ArduRCT_Switch touscruinoCenter(CONFIGURATION_V1_CENTER);

ArduRCT_TouScruinoV1 touscruino(CONFIGURATION_V1_CD, CONFIGURATION_V1_CS, CONFIGURATION_V1_RST, CONFIGURATION_V1_BL, &touscruinoRTC,
    &touscruinoUp, &touscruinoDown, &touscruinoLeft, &touscruinoRight, &touscruinoCenter);


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