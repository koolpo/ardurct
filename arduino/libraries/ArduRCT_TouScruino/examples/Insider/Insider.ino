/*
 * Insider - demonstrate the use of a Graphics object and a TouchPanel
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

/**
 *  See TouchScreen for a proper use of TouScruino
 *  This sketch shows how the hardware inside TouScruino works
 */
 
#include <Wire.h>
#include <ArduRCT_EventManager.h>
#include <ArduRCT_Graphics.h>
#include <ArduRCT_TouchPanel.hpp>
#include <SPI.h>

#define TP_INT_PIN 30
#define TP_DRAG_TRIGGER 4
#define TP_XP 24
#define TP_YP 33
#define TP_XM 26
#define TP_YM 31
#define TP_WIDTH 240
#define TP_HEIGHT 320

// define the graphics
ArduRCT_ILI9340 graphics;
// define the touchpanel
ArduRCT_TouchPanel touchpanel(TP_XP, TP_XM, TP_YP, TP_YM, TP_DRAG_TRIGGER, TP_WIDTH, TP_HEIGHT);
//ArduRCT_TouchPanel touchpanel(TP_INT_PIN, TP_DRAG_TRIGGER, TP_WIDTH, TP_HEIGHT);

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