/*
 * TouchPanel - Demonstrates the use of the touch panel
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
 
#include <TouchScreen.h>
#include <Printf.h>

TouchScreen touchscreen;

void setup() {
    Serial.begin(57600);
    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD);
	// uncomment the following line if you want to recalibrate the touch panel
    //touchscreen.resetTouchPanelCalibration();
    touchscreen.setupTouchPanel();
    touchscreen.setBacklight(180);
}

void loop() {
    int16_t x, y, z;
    
    if (touchscreen.getTouchXYZ(&x, &y, &z)) {
        Serial_printf("x=%d, y=%d, z=%d\n", x, y, z);
        touchscreen.fillRectangle(x-5, y-5, 10, 10, BLUE);
    }
    delay(100);
}