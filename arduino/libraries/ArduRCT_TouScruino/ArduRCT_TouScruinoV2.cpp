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

#include "ArduRCT_TouScruinoV2.hpp"

ArduRCT_TouScruinoV2::ArduRCT_TouScruinoV2(ArduRCT_RealTimeClock *rtc, ArduRCT_TouchPanel *tp) {
    setupScreen(SPFD5408_CD_PIN, SPFD5408_WR_PIN, SPFD5408_RD_PIN, SPFD5408_CS_PIN, SPFD5408_RESET_PIN, SPFD5408_SPI_ON_BUS);
    setupBacklight(SPFD5408_BACKLIGHT_PIN);
    registerRTC(rtc);
    if (tp != 0) registerTouchPanel(tp);
}

void ArduRCT_TouScruinoV2::setRotation(uint8_t rotation) {
    ArduRCT_Graphics::setRotation(rotation);
    if (getRegisteredTouchPanel() != 0) getRegisteredTouchPanel()->setRotation(rotation);
}