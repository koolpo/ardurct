/*
 * Colors - Display all the colors available on the screen
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
//ArduRCT_S6D04H0 graphics(2, 21, 22, 23, 0xFF, 0xFF);		// graphics(PORT, CD, WR, RD, CS, RESET)

#include <ArduRCT_ST7735.h>
ArduRCT_ST7735 graphics(10, 9 , 8);							// graphics(CD, CS, RESET)

uint8_t rotation = GRAPHICS_ROTATION_0;

void setup() {
    graphics.begin(WHITE, BLACK, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    graphics.setBacklight(180);
}

void loop() {
    drawScreen();
    delay(5000);
    rotation ++;
    if (rotation > GRAPHICS_ROTATION_270) rotation = GRAPHICS_ROTATION_0;
    graphics.setRotation(rotation);
}


void drawScreen() {
    uint32_t duration = millis();
    uint16_t height = graphics.getHeight()/3;
    uint16_t width = graphics.getWidth();
    uint16_t *buffer;
    for (uint8_t band=0; band<3; band++) buffer = drawBand(band, width, height);
    // paste the last buffer to complete the screen
    for (uint16_t y=3*height; y<graphics.getHeight(); y++) graphics.pasteBitmap(buffer, 0, y, width, 1);
    duration = millis()-duration;
    graphics.setCursor((width-graphics.getStringWidth("000ms", FONT_MEDIUM))/2, 3*height/2);
    graphics.print(duration);
    graphics.print("ms");
}

uint16_t *drawBand(uint8_t band, uint16_t width, uint16_t height) {
    uint16_t buffer[320];
    uint16_t wd2 = width/2;

    for (int16_t y=0; y<height; y++) {
        int32_t Y = (y << 8)/height;
        // for first half of width, we go from black to color
        // along y, we go from full c1 to null c1 and null c2 to full c2
        for (int16_t x=0; x<=wd2; x++) {
            int32_t X = (x << 8)/wd2;
            int32_t c1 = (0xFF * X * (0xFF-Y)) >> 16;
            int32_t c2 = (0xFF * X * Y) >> 16;
            int32_t c3 = 0x00;
            if (band == 0) buffer[x] = COLOR_565(c1, c2, c3);
            else if (band == 1) buffer[x] = COLOR_565(c3, c1, c2);
            else buffer[x] = COLOR_565(c2, c3, c1);
        }
        // for second half of width, we go from color to white
        // along y, we go from full c1 to null c1 and null c2 to full c2
        for (int16_t x=wd2; x<width; x++) {
            int32_t X = ((x - wd2) << 7)/wd2;                        
            int32_t c1 = (0xFF * (0xFF-Y + X)) >> 8;
            if (c1 > 0xFF) c1 = 0xFF;
            int32_t c2 = (0xFF * (Y + X)) >> 8;
            if (c2 > 0xFF) c2 = 0xFF;
            int32_t c3 = (0xFF * X) >> 8;
            if (band == 0) buffer[x] = COLOR_565(c1, c2, c3);
            else if (band == 1) buffer[x] = COLOR_565(c3, c1, c2);
            else buffer[x] = COLOR_565(c2, c3, c1);
        }
        graphics.pasteBitmap(buffer, 0, y+height*band, width, 1);
    }
    return buffer;
}