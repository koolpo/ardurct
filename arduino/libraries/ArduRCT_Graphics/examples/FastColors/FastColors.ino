/*
 * FastColors - Display all available "fast" colors
 *
 * Copyright (c) 2012 Laurent Wibaux <lm.wibaux@gmail.com>
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
//ArduRCT_S6D04H0 graphics;

#include <ArduRCT_SPFD5408.h>
ArduRCT_SPFD5408 graphics;

/* 
 * This program displays all the fast colors on the screen
 *
 * Because we use a 8 bits wide bus, we can save some time (30%) if we use 
 * 16 bits colors (RGB_565) where the high byte and the low byte are the same
 *    Such colors are:
 *        0x0000    BLACK
 *        0xFFFF    WHITE
 *        0x9494    GREY
 */ 

void setup() {
    graphics.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    graphics.setBacklight(180);
    
    uint8_t s[3];
    s[2] = 0;
    for (uint16_t i=0; i<8; i++) {
        for (uint16_t j=0; j<32; j++) {
            uint16_t colorL = i*32+j;
            uint16_t color = (colorL << 8) + colorL;
            s[0] = colorL >> 4;
            s[1] = colorL & 0x0F;
            s[0] += (s[0] < 10 ? '0' : 'A' - 10);
            s[1] += (s[1] < 10 ? '0' : 'A' - 10);
            int16_t x = i * 30;
            int16_t y = j * 10; 
            graphics.fillRectangle(x+1, y+1, 14, 8, color);
            graphics.drawRectangle(x+1, y+1, 14, 8, BLACK, 1);
            graphics.drawString((char *)s, x+17, y+1, BLACK, FONT_SMALL, false, false);
        }
    }
}

void loop() {
    delay(100);
}
