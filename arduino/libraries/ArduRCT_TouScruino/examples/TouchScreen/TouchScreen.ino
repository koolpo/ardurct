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

/**
 *  Demonstrates the use of the EventManager included in the Touscruino
 *  to manage the touchpanel and the graphics
 *  This is a variation of the Insider sketch using Touscruino instead of its components
 *      with this, you don't have to deal with any hardware
 **/
  
#include <Wire.h>
#include <SPI.h>
#include <ArduRCT_Graphics.h>
#include <ArduRCT_EventManager.h>

// Change to your version: the following include will automatically create the proper 'Touscruino' object
#define TOUSCRUINO_VERSION 2
#include <ArduRCT_TouScruino.h>

int16_t penX;
int16_t penY;

// Define the function to call when there is an EVENT_TOUCHPANEL
ArduRCT_EventHandler touchpanelHandler(EVENT_TOUCHPANEL, EVENT_ANY_VALUE, 0, 0, &handleTouchPanel);

void setup() {
    Serial.begin(57600);
    // reset the calibration
    // Touscruino.resetCalibration();
    // prepare the screen
    Touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    // enable the graphics UI to calibrate if required
    Touscruino.enableGraphicsUI();
    // register the EventHandler
    Touscruino.registerEventHandler(&touchpanelHandler);
}

void loop() {
    Touscruino.manageEvents();
}

int8_t handleTouchPanel(uint8_t eventType, uint8_t z, int16_t x, int16_t y) {
    if (eventType == EVENT_TOUCHPANEL_PRESSED) {
        // if the pen is pressed on the screen, draw a big dot at the pen place
        Touscruino.fillRectangle(x-2, y-2, 4, 4, BLUE);
    } else if (eventType == EVENT_TOUCHPANEL_DRAGGED) {
        // if the pen is dragged on the screen, 
        // draw a line connecting the last dot drawn and the current pen place
        Touscruino.drawLine(penX, penY, x, y, BLUE, 4);
    } else return EVENT_HANDLING_EXIT;
    penX = x;
    penY = y;
    // return EVENT_HANDLING_EXIT to indicate that the EVENT has been dealt with
    return EVENT_HANDLING_EXIT;
}