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

//#include <ArduRCT_S6D04H0.h>
//ArduRCT_S6D04H0 tft(2, 21, 22, 23, 0xFF, 0xFF);

#include <ArduRCT_ST7735.h>
ArduRCT_ST7735 tft(10, 9, 8);

void setup() {
    Serial.begin(57600);
    
    touscruino.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    touscruino.setBacklight(180);

    touscruino.executeMacro(seven, 20, 20);
    touscruino.executeMacro(seven, 10, 100, 3);
    
    delay(3000);
    
    touscruino.setBacklight(0);
    touscruino.fillScreen(WHITE);
    touscruino.setBacklight(180);
}

void loop() {
    while (Serial.available()) buffer[bufferPtr++] = Serial.read();
    if (bufferPtr == 0) return;
    
    // wait for end of sentence
    if (buffer[bufferPtr-1] == '.') {
        // mark end of macro
        buffer[bufferPtr-1] = 0;
        bufferPtr = 0;
        // execute the macro
        touscruino.executeMacro(buffer, 10, 10);
    }
    delay(10);
}


