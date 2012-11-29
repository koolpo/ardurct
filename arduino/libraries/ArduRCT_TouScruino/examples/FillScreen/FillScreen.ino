/*
 * FillScreen - Simple speed test to fill the screen
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


#include <SPI.h>

// Change to your version: the following include will automatically create the proper tft object
#define TOUSCRUINO_VERSION 1
#include <ArduRCT_TouScruino.h>

void setup() {
    tft.begin(WHITE, BLACK, FONT_MEDIUM, FONT_BOLD);
}

void loop() {
    uint32_t duration = millis();
    delay(500);
    tft.fillScreen(RED);
    delay(500);
    tft.fillScreen(GREEN);
    delay(500);
    tft.fillScreen(BLUE);
    delay(500);
    tft.fillScreen(WHITE);
    delay(500);
    tft.fillScreen(BLACK);
    duration = (millis() - duration)/5 - 500;
    tft.setCursor(20, 20);
    tft.print(duration);
    tft.print("ms");
}