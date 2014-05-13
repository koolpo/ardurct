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
    _widthImpl = TOUSCRUINO_V2_WIDTH; \
    _heightImpl = TOUSCRUINO_V2_HEIGHT; \
    setupScreen(TOUSCRUINO_V2_CD_PIN, TOUSCRUINO_V2_WR_PIN, TOUSCRUINO_V2_RD_PIN, TOUSCRUINO_V2_CS_PIN, TOUSCRUINO_V2_RESET_PIN, TOUSCRUINO_V2_SPI_ON_BUS);
    setupBacklight(TOUSCRUINO_V2_BACKLIGHT_PIN);
    registerRTC(rtc);
    if (tp != 0) registerTouchPanel(tp);
}

void ArduRCT_TouScruinoV2::setRotation(uint8_t rotation, bool selectAndUnselectScreen) {
    ArduRCT_Graphics::setRotation(rotation);
    if (getRegisteredTouchPanel() != 0) getRegisteredTouchPanel()->setRotation(rotation);
}