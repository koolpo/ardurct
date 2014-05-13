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
#include <ArduRCT_Graphics.h>

//ArduRCT_S6D04H0 graphics;
//ArduRCT_SPFD5408 graphics;
ArduRCT_ILI9340 graphics;
//ArduRCT_ST7735 graphics;

void setup() {
    graphics.begin(WHITE, BLACK, FONT_MEDIUM, FONT_BOLD);
}

void loop() {
    uint32_t duration = micros();
    
    delay(500);
    graphics.fillScreen(RED);
    delay(500);
    graphics.fillScreen(GREEN);
    delay(500);
    graphics.fillScreen(BLUE);    
    delay(500);
    graphics.fillScreen(WHITE);
    delay(500);
    graphics.fillScreen(BLACK);
	
    duration = (micros() - duration)/5 - 500000;
    graphics.setCursor(20, 20);
    graphics.print(duration);
    graphics.print("us");
    Serial.print(duration);
    Serial.println("us");
    delay(1000);
}