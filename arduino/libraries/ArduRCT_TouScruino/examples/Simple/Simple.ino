/*
 * TouchPanel - demonstrate the use of a Graphics object and a TouchPanel
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
#include <ArduRCT_EventManager.h>
#include <ArduRCT_Graphics.h>
#include <ArduRCT_SPFD5408.h>
#include <ArduRCT_TouchPanel.h>
#include <SPI.h>

// define the graphics
ArduRCT_SPFD5408 graphics;

#define TP_INT_PIN 30
#define TP_POINT_IS_SAME 2
#define TP_WIDTH 240
#define TP_HEIGHT 320

// define the touchpanel
ArduRCT_TouchPanel touchpanel(TP_INT_PIN, TP_POINT_IS_SAME, TP_WIDTH, TP_HEIGHT);

int16_t x;
int16_t y;

void setup() {
    // setup the screen 
    graphics.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD);
}

void loop() {
    // get the state of the touchpanel
    uint8_t tpState = touchpanel.update();
    if (tpState == EVENT_STATE_PRESSED) {
        // if the pen is pressed on the screen, draw a big dot at the pen place
        touchpanel.getPenXYZ(&x, &y);
        graphics.fillRectangle(x-2, y-2, 4, 4, BLUE);
    } else if (tpState == EVENT_STATE_DRAGGED) {
        // if the pen is dragged on the screen, 
        // draw a line connecting the last dot drawn and the current pen place
        graphics.drawLine(x, y, touchpanel.getPenX(), touchpanel.getPenY(), BLUE, 4);
        touchpanel.getPenXYZ(&x, &y);
    }
}