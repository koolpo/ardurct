/*
 * UIFramework - Demonstration of use of the touchscreen for a User Interface
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
 *  Demonstrates the use of the UI manager included in the Touscruino
 **/

#define GRAPHICS_UI_ELEMENT_FONT FONT_HUGE
  
#include <Wire.h>
#include <SPI.h>
#include <ArduRCT_Graphics.h>
#include <ArduRCT_EventManager.h>

// Change to your version: the following include will automatically create the proper 'Touscruino' object
#define TOUSCRUINO_VERSION 2
#include <ArduRCT_TouScruino.h>

// define a screen
ArduRCT_GraphicsUIScreen screen;
// define a button which will call uiHandler for every action on it
ArduRCT_GraphicsUIButton button(0, "Toggle RED", &uiHandler);

void setup() {
    // prepare the screen
    Touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    // enable the GraphicsUI
    Touscruino.enableGraphicsUI();
    // add the screen
    Touscruino.addScreen(&screen);
    // add a button on the screen
    screen.addElement(&button, 40, 20, 160, 40);
    // start the UI by showing screen
    Touscruino.setGraphicsUIScreen(&screen);
}

void loop() {
    Touscruino.manageEvents();
}

bool isRed = false;
bool uiHandler(uint8_t elementId, uint8_t state, int16_t value) {
    isRed = !isRed;
    Touscruino.fillRectangle(80, 200, 80, 80, isRed ? RED : WHITE);
}