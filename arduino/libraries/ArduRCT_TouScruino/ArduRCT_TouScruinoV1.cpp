/*
 * TouScruinoV1 - Screen and 5 ways switch management
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

#include <ArduRCT_Graphics.h>
#include <ArduRCT_EventManager.h>
#include "ArduRCT_TouScruinoV1.hpp"

ArduRCT_TouScruinoV1::ArduRCT_TouScruinoV1(ArduRCT_RealTimeClock *rtc,
            ArduRCT_Switch *up, ArduRCT_Switch *down, ArduRCT_Switch *left, ArduRCT_Switch *right, ArduRCT_Switch *center) {
    setupScreen(ST7735_CD_PIN, ST7735_CS_PIN, ST7735_RESET_PIN);
    setupBacklight(ST7735_BACKLIGHT_PIN);
    registerSwitch(up);
    registerSwitch(down);
    registerSwitch(right);
    registerSwitch(left);
    registerSwitch(center);
    registerRTC(rtc);
}
