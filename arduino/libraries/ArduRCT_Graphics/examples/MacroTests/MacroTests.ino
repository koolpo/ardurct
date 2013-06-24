/*
 * MacroTests - Tests the macros (see TouchScreen_Macros.cpp for manual)
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

#include <ArduRCT_S6D04H0.h>
ArduRCT_S6D04H0 graphics(21, 0xFF, 0xFF, 5);   // graphics(CD, CS, RESET, BACKLIGHT)

//#include <ArduRCT_ST7735.h>
//ArduRCT_ST7735 graphics(10, 9 , 8, 5);         // graphics(CD, CS, RESET, BACKLIGHT)

void setup() {
    Serial.begin(57600);  
    graphics.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    graphics.setupBacklight(5);
    graphics.setBacklight(180);
}

void loop() {
    while (Serial.available()) buffer[bufferPtr++] = Serial.read();
    if (bufferPtr == 0) return;
    
    // wait for end of sentence
    if ((buffer[bufferPtr-1] == '.') || (buffer[bufferPtr-1] == '\n')) {
        // mark end of macro
        buffer[bufferPtr-1] = 0;
        bufferPtr = 0;
        // execute the macro
        graphics.executeMacro(buffer, 10, 10);
    }
    delay(10);
}


