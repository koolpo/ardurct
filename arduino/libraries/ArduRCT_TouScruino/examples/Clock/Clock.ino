/*
 * Terminal - Most simple terminal: all received chars are shown on screen
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

// Put 1 for roundish and 0 for oldish
#define DIGIT_STYLE 0
#define THICKNESS 7

#define DIGIT_WIDTH 30
#define DIGIT_HEIGHT 80
#define SECONDS_WIDTH 120
#define SECONDS_HEIGHT 5

#define TIME_Y 0
#define DATE_Y 89
#define ALARM_Y 114
#define SECONDS_Y 105
#define H1_X 0
#define H2_X 38
#define M1_X 88
#define M2_X 130
#define COLUMN_X 79

#if (DIGIT_STYLE == 0)
uint8_t DIGIT_SEGMENTS[] = { 
    0x3F,    // b00111111    0
    0x30,    // b00110000    1
    0x6D,    // b01101101    2
    0x79,    // b01111001    3
    0x72,    // b01110010    4
    0x5B,    // b01011011    5
    0x5F,    // b01011111    6
    0x31,    // b00110001    7
    0x7F,    // b01111111    8
    0x7B     // b01111011    9
};  
#endif

uint8_t seconds = 0;
uint8_t minutes = 56;
uint8_t hours = 19;
    
uint8_t dig1 = 70, dig2 = 70, dig3 = 70, dig4 = 70;
uint32_t nextSecond = 0;
    
void setup() {
    tft.begin(WHITE, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    tft.fillScreen(BLACK);
    tft.setRotation(GRAPHICS_ROTATION_90);
    tft.drawString("Tue 22 Apr 2012", 12, DATE_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    tft.drawString("Alarm 22:40", 30, ALARM_Y, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
}

void loop() {
    delay(5);
    
    // check if we have to refresh the screen
    if ((int32_t)(millis() - nextSecond) < 0) return;
    nextSecond = millis() + 1000;

    seconds ++;
    if (seconds == 60) {
        seconds = 0;
        minutes ++;
        if (minutes == 60) {
            minutes = 0;
            hours ++;
            if (hours == 24) hours = 0;
        }
    }
    
    // draw the time
    refreshTime();
    
    // blink the seconds
    if ((seconds % 2) == 0) drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, BLACK, THICKNESS);
    else drawColumn(COLUMN_X, 0, DIGIT_HEIGHT, WHITE, THICKNESS);
    
    // draw a progress bar for the seconds
    if (seconds == 0) tft.fillRectangle((tft.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, SECONDS_WIDTH, SECONDS_HEIGHT, BLACK);
    else tft.fillRectangle((tft.getWidth() - SECONDS_WIDTH)/2, SECONDS_Y, seconds * SECONDS_WIDTH / 60, SECONDS_HEIGHT, YELLOW);
}

void drawColumn(uint16_t x, uint16_t y, uint16_t height, uint16_t color, uint16_t  thickness) {
    uint16_t ht = thickness/2;
    tft.fillRectangle(x - ht , y + height/3 - ht, thickness, thickness, color); 
    tft.fillRectangle(x - ht , y + 2*height/3 - ht, thickness, thickness, color); 
}

void refreshTime() {
    if (dig1 != (hours / 10)) {
        drawDigit(dig1, H1_X, 0, DIGIT_WIDTH, DIGIT_HEIGHT, BLACK, THICKNESS);
        dig1 = hours / 10;
        if (dig1 != 0) drawDigit(dig1, H1_X, 0, DIGIT_WIDTH, DIGIT_HEIGHT, WHITE, THICKNESS);
    }
    if (dig2 != (hours % 10)) {
        drawDigit(dig2, H2_X, 0, DIGIT_WIDTH, DIGIT_HEIGHT, BLACK, THICKNESS);
        dig2 = hours % 10;
        drawDigit(dig2, H2_X, 0, DIGIT_WIDTH, DIGIT_HEIGHT, WHITE, THICKNESS);
    }
    if (dig3 != (minutes / 10)) {
        drawDigit(dig3, M1_X, 0, DIGIT_WIDTH, DIGIT_HEIGHT, BLACK, THICKNESS);
        dig3 = minutes / 10;
        drawDigit(dig3, M1_X, 0, DIGIT_WIDTH, DIGIT_HEIGHT, WHITE, THICKNESS);
    }
    if (dig4 != (minutes % 10)) {
        drawDigit(dig4, M2_X, 0, DIGIT_WIDTH, DIGIT_HEIGHT, BLACK, THICKNESS);
        dig4 = minutes % 10;
        drawDigit(dig4, M2_X, 0, DIGIT_WIDTH, DIGIT_HEIGHT, WHITE, THICKNESS);
    }
}

void drawDigit(uint8_t d, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, uint8_t thickness) {
    // should have odd thickness to look nice
    if ((thickness % 2) != 1) return;
#if (DIGIT_STYLE == 0)
    // draws 7 segment style digits
    if (d > 9) return;
    uint16_t h = (height + thickness)/2 - 1;
    uint16_t x1 = x + width - thickness;
    uint16_t y1 = y,
        y2 = y + h - thickness + 2,
        y3 = y + height - thickness;
    if ((DIGIT_SEGMENTS[d] & 0x01) != 0) {
        // horizontal top
        tft.fillRectangle(x+thickness-1, y1, width-2*thickness+2, thickness, color);
        for (uint8_t i=1; i<thickness; i++) {
            tft.drawHorizontalLine(x+thickness-i-1, x+thickness-2, y1+i, color, 1);
            tft.drawHorizontalLine(x+width-thickness+1, x+width-thickness+i, y1+i, color, 1);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x02) != 0) {
        // vertical top left
        tft.fillRectangle(x, y1+thickness-1, thickness, h-2*thickness+2, color);
        for (uint8_t i=1; i<thickness; i++) {
            tft.drawVerticalLine(x+i, y1+thickness-i-1, y1+thickness-2, color, 1);
            tft.drawVerticalLine(x+i, y1+h-thickness+1, y1+h-thickness+i, color, 1);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x04) != 0) {
        // vertical bottom left
        tft.fillRectangle(x, y2+thickness-1, thickness, h-2*thickness+2, color);
        for (uint8_t i=1; i<thickness; i++) {
            tft.drawVerticalLine(x+i, y2+thickness-i-1, y2+thickness-2, color, 1);
            tft.drawVerticalLine(x+i, y2+h-thickness+1, y2+h-thickness+i, color, 1);
        }
        if (DIGIT_SEGMENTS[d] == 0x3F || DIGIT_SEGMENTS[d] == 0x5F) tft.fillRectangle(x, y2-1, thickness, thickness, color);
    }
    if ((DIGIT_SEGMENTS[d] & 0x08) != 0) {
        // horizontal bottom
        tft.fillRectangle(x+thickness-1, y3, width-2*thickness+2, thickness, color);
        for (uint8_t i=1; i<thickness; i++) {
            tft.drawHorizontalLine(x+thickness-i-1, x+thickness-2, y3+thickness-i-1, color, 1);
            tft.drawHorizontalLine(x+width-thickness+1, x+width-thickness+i, y3+thickness-i-1, color, 1);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x10) != 0) {
        // vertical bottom right
        tft.fillRectangle(x1, y2+thickness-1, thickness, h-2*thickness+2, color);
        for (uint8_t i=1; i<thickness; i++) {
            tft.drawVerticalLine(x1+thickness-i-1, y2+thickness-i-1, y2+thickness-2, color, 1);
            tft.drawVerticalLine(x1+thickness-i-1, y2+h-thickness+1, y2+h-thickness+i, color, 1);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x20) != 0) {
        // vertical top right
        tft.fillRectangle(x1, y1+thickness-1, thickness, h-2*thickness+2, color);
        for (uint8_t i=1; i<thickness; i++) {
            tft.drawVerticalLine(x1+thickness-i-1, y1+thickness-i-1, y1+thickness-2, color, 1);
            tft.drawVerticalLine(x1+thickness-i-1, y1+h-thickness+1, y1+h-thickness+i, color, 1);
        }                        
        if (DIGIT_SEGMENTS[d] == 0x3F || DIGIT_SEGMENTS[d] == 0x30 || DIGIT_SEGMENTS[d] == 0x72 || DIGIT_SEGMENTS[d] == 0x31 || DIGIT_SEGMENTS[d] == 0x7B) 
            tft.fillRectangle(x1, y2-1, thickness, thickness, color);
    }
    if ((DIGIT_SEGMENTS[d] & 0x40) != 0) {
        // horizontal middle
        tft.fillRectangle(x+thickness, y2-1, width-2*thickness, thickness, color);
        if (DIGIT_SEGMENTS[d] == 0x79) {
            for (uint8_t i=1; i<=thickness/2; i++) tft.drawVerticalLine(x+thickness-i, y2+i-1, y2+thickness-i-1, color, 1);
        }
   }
#else
// assume that height > 2*width
// thickness should be odd
    uint16_t hw = (width - 2 * thickness)/2;
    uint16_t hh = (height - 2 * thickness)/2;
    uint16_t ht = thickness/2;
    uint16_t r = hw + ht;
    uint16_t cx = x + thickness + hw;
    uint16_t c1y = y + thickness + hw - 1, 
        c2y = y + ht + hh - hw, 
        c3y = y + hh + ht + 1, 
        c4y = y + hh + hw + thickness, 
        c5y = y - thickness + height - hw,
        c6y = y + thickness + 2 * hw + 1,
        c9y = y - thickness + height - 2 * hw - 2;
    uint16_t l1x = x + ht,
        l2x = x + width - ht;
    uint16_t l1y = y + ht,
        l2y = y + height - ht - 1;

    switch (d) {
        case 0:
            tft.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            tft.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            tft.drawLine(l1x, c1y, l1x, c5y, color, thickness);
            tft.drawLine(l2x, c1y, l2x, c5y, color, thickness);
            break;
        case 1:
            tft.drawArc(l1x, l1y, r, GRAPHICS_ARC_SE, color, thickness);
            tft.drawLine(cx, l1y, cx, l2y, color, thickness);
            tft.drawLine(l1x, l2y, l2x, l2y, color, thickness);
            break;
        case 2:
            tft.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            tft.drawArc(cx, c2y, r, GRAPHICS_ARC_SE, color, thickness);
            tft.drawArc(cx, c4y, r, GRAPHICS_ARC_NW, color, thickness);
            tft.drawLine(l2x, c1y, l2x, c2y, color, thickness);
            tft.drawLine(l1x, c4y, l1x, l2y, color, thickness);
            tft.drawLine(l1x, l2y, l2x, l2y, color, thickness);
            break;
        case 3:
            tft.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            tft.drawArc(cx, c2y, r, GRAPHICS_ARC_SE, color, thickness);
            tft.drawArc(cx, c4y, r, GRAPHICS_ARC_NE, color, thickness);
            tft.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            tft.drawLine(l2x, c1y, l2x, c2y, color, thickness);
            tft.drawLine(l2x, c4y, l2x, c5y, color, thickness);
            break;
        case 4:
            tft.drawLine(cx, l1y, l1x, c4y, color, thickness);
            tft.drawLine(l1x, c4y, l2x, c4y, color, thickness);
            tft.drawLine(cx, c3y, cx, l2y, color, thickness);
            break;
        case 5:
            tft.drawArc(cx, c4y, r, GRAPHICS_ARC_NE, color, thickness);
            tft.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            tft.drawLine(l2x, c4y, l2x, c5y, color, thickness);
            tft.drawLine(l1x, l1y, l2x, l1y, color, thickness);
            tft.drawLine(l1x, l1y, l1x, c3y, color, thickness);
            tft.drawLine(l1x, c3y, cx, c3y, color, thickness);
            break;
        case 6:
            tft.drawArc(cx, c4y, r, GRAPHICS_ARC_NE, color, thickness);
            tft.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            tft.drawArc(l2x-ht, c6y, r*2, GRAPHICS_ARC_NW, color, thickness);
            tft.drawLine(l2x, c4y, l2x, c5y, color, thickness);
            tft.drawLine(l1x, c3y, cx, c3y, color, thickness);
            tft.drawLine(l1x, c2y, l1x, c5y, color, thickness);
            break;
        case 7:
            tft.drawArc(l2x, c3y, r, GRAPHICS_ARC_NW, color, thickness);
            tft.drawLine(l2x, l1y, l2x, c3y-r, color, thickness);
            tft.drawLine(l1x, l1y, l2x, l1y, color, thickness);
            tft.drawLine(cx, c3y, cx, l2y, color, thickness);
            break;
        case 8:
            tft.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            tft.drawArc(cx, c2y, r, GRAPHICS_ARC_S, color, thickness);
            tft.drawArc(cx, c4y, r, GRAPHICS_ARC_N, color, thickness);
            tft.drawArc(cx, c5y, r, GRAPHICS_ARC_S, color, thickness);
            tft.drawLine(l1x, c1y, l1x, c2y, color, thickness);
            tft.drawLine(l1x, c4y, l1x, c5y, color, thickness);
            tft.drawLine(l2x, c1y, l2x, c2y, color, thickness);
            tft.drawLine(l2x, c4y, l2x, c5y, color, thickness);
            break;
        case 9:
            tft.drawArc(cx, c2y, r, GRAPHICS_ARC_SW, color, thickness);
            tft.drawArc(cx, c1y, r, GRAPHICS_ARC_N, color, thickness);
            tft.drawArc(l1x+ht, c9y, r*2, GRAPHICS_ARC_SE, color, thickness);
            tft.drawLine(cx, c3y, l2x, c3y, color, thickness);
            tft.drawLine(l2x, c1y, l2x, c9y, color, thickness);
            tft.drawLine(l1x, c1y, l1x, c2y, color, thickness);
            break;
    }
#endif
}