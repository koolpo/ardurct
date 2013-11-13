/*
 * TouchScreen - Demonstration of use of the touchscreen
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

#include <Wire.h>
#include <SPI.h>
#include <ArduRCT_Graphics.h>
#include <ArduRCT_EventManager.h>

// Change to your version: the following include will automatically create the proper 'touscruino' object
#define TOUSCRUINO_VERSION 2
#include <ArduRCT_TouScruino.h>

int16_t penX;
int16_t penY;

void setup() {
    // prepare the screen
    touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    // register an EventHandler for the panel
    touscruino.registerEventHandler(new ArduRCT_EventHandler(EVENT_TOUCHPANEL, EVENT_ANY_VALUE, 0, 0, &touchpanelHandler));
}

void loop() {
    touscruino.manageEvents();
}

int8_t touchpanelHandler(uint8_t eventType, uint8_t z, int16_t x, int16_t y) {
    if (eventType == EVENT_TOUCHPANEL_PRESSED) {
        // if the pen is pressed on the screen, draw a big dot at the pen place
        touscruino.fillRectangle(x-2, y-2, 4, 4, BLUE);
    } else if (eventType == EVENT_TOUCHPANEL_DRAGGED) {
        // if the pen is dragged on the screen, 
        // draw a line connecting the last dot drawn and the current pen place
        touscruino.drawLine(penX, penY, x, y, BLUE, 4);
    } else return EVENT_HANDLING_EXIT;
    penX = x;
    penY = y;
    return EVENT_HANDLING_EXIT;
}
