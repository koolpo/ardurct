/*
 * Sparkfun - Tests the macros (see Macros.cpp for manual)
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

#define SCALE 2
#define LOGO_WIDTH 57
#define LOGO_HEIGHT 88

TouchScreen touchscreen;

/**
 * Approximation of sparkfun logo, 57 x 88
 *    w 0                    write to eeprom slot 0
 *    pc F800                set color to red
 *    tf 0 42 44 42 0 88     fill bottom left triangle
 *    afe 27 35 30           fill right half circle
 *    pts 7                  set line thickness to 7, scalable
 *    annw 25 79 17          draw thick arc to join triangle and half circle
 *    afnw 17 42 17          fill middle left quarter circle
 *    rf 17 35 11 7          fill middle left rectangle
 *    cf 30 13 13            fill top circle
 *    pc FFFF                set color to white, to erase
 *    cf 20 32 9             fill circle at middle left
 *    cf 43 11 8             fill circle at top right
 *    pc F800                set color to red
 *    afnne 32 14 14         fill NNE arc at top right
 *    tf 49 18 49 14 51 17   fill triangle at top right
 **/
uint8_t sparkfun[] = "w 0 "\
    "pc F800 "\
    "pts 7 "\
    "tf 0 42 44 42 0 88 "\
    "afe 27 35 30 "\
    "annw 25 79 17 "\
    "afnw 17 42 17 "\
    "rf 17 35 11 7 "\
    "cf 30 13 13 "\
    "pc FFFF "\
    "cf 20 32 9 "\
    "cf 43 11 8 "\
    "pc F800 "\
    "afnne 32 14 14 "\
    "tf 49 18 49 14 51 17";
    
void setup() {
    Serial.begin(57600);
    
    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touchscreen.setBacklight(180);
    
    // execute the macro without the initial write in slot 0
    touchscreen.executeMacro(&sparkfun[3], (touchscreen.getWidth()-57*SCALE)/2, (touchscreen.getHeight()-88*SCALE)/2, SCALE, 1, true);
    delay(3000);

    // write the macro to eeprom slot 0
    touchscreen.executeMacro(sparkfun);
    // erase the drawing
    touchscreen.fillRectangle((touchscreen.getWidth()-57*SCALE)/2-2, (touchscreen.getHeight()-88*SCALE)/2-2, 57*SCALE+4, 88*SCALE+4, WHITE);

    // execute the macro stored in slot 0. Any time we need the logo, in whatever scale, we can reuse "e 0"
    touchscreen.executeMacro((uint8_t *)"e 0", (touchscreen.getWidth()-57*SCALE)/2, (touchscreen.getHeight()-88*SCALE)/2, SCALE, 1);
}

void loop() {
    delay(100);
}