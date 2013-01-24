/*
 * FillScreen - Demonstration of colors
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
 *  Touscruino buttons control the color of the screen
 *  MENU    black
 *  ENTER   red
 *  UP      green
 *  DOWN    blue
 */

#include <SPI.h>
#include <ArduRCT_Graphics.h>
#include <ArduRCT_EventManager.h>

// Change to your version: the following include will automatically create the proper 'touscruino' object
#define TOUSCRUINO_VERSION 1
#include <ArduRCT_TouScruino.h>

ArduRCT_EventHandler buttonsHandler(EVENT_SWITCH, EVENT_ANY_VALUE, &handleButtons); 

void setup() {
    // prepare the screen
    touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    // register an EventHandler for the switches
    touscruino.registerEventHandler(&buttonsHandler);
}

void loop() {
    touscruino.manageEvents();
}

int8_t handleButtons(uint8_t eventType, uint8_t buttonId) {
    if (buttonId == TOUSCRUINO_MENU) touscruino.fillScreen(BLACK);
    else if (buttonId == TOUSCRUINO_ENTER) touscruino.fillScreen(RED);
    else if (buttonId == TOUSCRUINO_UP) touscruino.fillScreen(GREEN);
    else if (buttonId == TOUSCRUINO_DOWN) touscruino.fillScreen(BLUE);
    return EVENT_HANDLING_DONE;
}
