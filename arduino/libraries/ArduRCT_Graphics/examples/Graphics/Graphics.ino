/*
 * Graphics - Demonstrate the capacities of the screen in terms of graphics
 *    Part of BoucingBoxes
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

// Change to your version: the following include will automatically create the proper 'touscruino' object
#define TOUSCRUINO_VERSION 1
#include <ArduRCT_TouScruino.h>

uint16_t wait = 3000;

void setup() {
    touscruino.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    touscruino.setBacklight(180);
}

void loop() {
    // Fonts
    touscruino.drawString("SMALL", 5, 4, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawString("BOLD", 5, 14, BLACK, FONT_SMALL, FONT_BOLD, NO_OVERLAY);
    touscruino.drawString("MEDIUM", 5, 30, RED, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawString("BOLD", 5, 44, RED, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touscruino.drawString("BIG", 5, 64, BROWN, FONT_BIG, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawString("BOLD", 5, 84, BROWN, FONT_BIG, FONT_BOLD, NO_OVERLAY);
    touscruino.drawString("HUGE", 5, 110, BLUE, FONT_HUGE, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawString("BOLD", 5, 134, BLUE, FONT_HUGE, FONT_BOLD, NO_OVERLAY);
    delay(wait);
    
    // Lines
    touscruino.fillScreen(WHITE);
    touscruino.drawString("Lines", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touscruino.drawLine(10, 30, 104, 69, RED, 2);
    touscruino.drawLine(12, 92, 81, 81, BLUE, 1);
    touscruino.drawLine(15, 145, 120, 105, GREEN, 5);
    delay(wait);
    
    // Triangles
    touscruino.fillScreen(WHITE);
    touscruino.drawString("Triangles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touscruino.drawTriangle(7, 31, 71, 30, 20, 63, BLUE, 1);
    touscruino.drawString("Thickness 3", 50, 65, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawTriangle(17, 92, 87, 80, 109, 106, DARK_GREEN, 3);
    touscruino.drawString("Filled", 12, 145, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillTriangle(18, 112, 13, 136, 94, 128, ORANGE);
    delay(wait);
    
    // Corners
    touscruino.fillScreen(WHITE);
    touscruino.drawString("Corners", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touscruino.fillCorner(74, 90, 40, GRAPHICS_CORNER_SE, RED);
    touscruino.drawString("SE", 100, 120, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillCorner(54, 90, 35, GRAPHICS_CORNER_SW, ORANGE);
    touscruino.drawString("SW", 10, 115, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillCorner(74, 70, 45, GRAPHICS_CORNER_NE, BLUE);
    touscruino.drawString("NE", 110, 40, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillCorner(54, 70, 30, GRAPHICS_CORNER_NW, GREY);
    touscruino.drawString("NW", 20, 40, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    delay(wait);
    
    // Rectangle
    touscruino.fillScreen(WHITE);
    touscruino.drawString("Rectangles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touscruino.drawRectangle(12, 28, 40, 50, GREY, 3);
    touscruino.fillRectangle(65, 30, 50, 25, ORANGE);
    touscruino.drawString("Filled", 70, 60, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawRoundedRectangle(10, 97, 55, 32, 6, RED, 2);
    touscruino.drawString("Rounded", 15, 136, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillRoundedRectangle(80, 80, 33, 48, 8, BLUE);
    touscruino.drawString("Rounded", 78, 136, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawString("filled", 78, 146, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    delay(wait);
    
    // Arcs
    touscruino.fillScreen(WHITE);
    touscruino.drawString("Arcs", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touscruino.drawArc(55, 60, 20, GRAPHICS_ARC_NWW, RED, 3);
    touscruino.drawString("NWW", 15, 45, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawArc(62, 53, 20, GRAPHICS_ARC_NNW, ORANGE, 3);
    touscruino.drawString("NNW", 30, 27, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawArc(70, 55, 20, GRAPHICS_ARC_NE, BLUE, 3);
    touscruino.drawString("NE", 90, 33, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawArc(64, 65, 20, GRAPHICS_ARC_S, GREY, 3);
    touscruino.drawString("S", 90, 70, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillArc(55, 130, 20, GRAPHICS_ARC_NWW, GREEN);
    touscruino.drawString("NWW", 15, 115, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillArc(62, 123, 20, GRAPHICS_ARC_NNW, LIGHT_GREY);
    touscruino.drawString("NNW", 30, 97, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillArc(70, 125, 20, GRAPHICS_ARC_NE, ORANGE);
    touscruino.drawString("NE", 90, 103, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.fillArc(64, 115, 30, GRAPHICS_ARC_SSW+GRAPHICS_ARC_SSE, BLUE);
    touscruino.drawString("SSW+SSE", 80, 147, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.drawString("Filled", 10, 150, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    delay(wait);
    
    // Circles
    touscruino.fillScreen(WHITE);
    touscruino.drawString("Circles", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touscruino.drawCircle(64, 55, 25, BLUE, 3);
    touscruino.fillCircle(64, 115, 25, BROWN);
    touscruino.drawString("Filled", 50, 145, BLACK, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    delay(wait);
    
    // Big digits
    touscruino.fillScreen(WHITE);
    touscruino.drawString("Big digits", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touscruino.drawBigDigit(6, 10, 25, 40, 60, BLUE, 6, GRAPHICS_STYLE_NORMAL);
    touscruino.drawBigDigit(4, 70, 35, 45, 55, RED, 10, GRAPHICS_STYLE_ADVANCED);
    touscruino.drawBigDigit(8, 30, 100, 60, 50, BROWN, 12, GRAPHICS_STYLE_ADVANCED);
    delay(wait);
    
    // Rotations
    touscruino.fillScreen(WHITE);
    touscruino.drawString("Rotations", 3, 3, BLACK, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    touscruino.setRotation(GRAPHICS_ROTATION_90);
    touscruino.drawString("Rotation 90d", 15, 15, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
    touscruino.setRotation(GRAPHICS_ROTATION_180);
    touscruino.drawString("Rotation 180d", 5, 15, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
    touscruino.setRotation(GRAPHICS_ROTATION_270);
    touscruino.drawString("Rotation 270d", 15, 15, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
    touscruino.setRotation(GRAPHICS_ROTATION_0);
    delay(wait);

    touscruino.fillScreen(WHITE);
    wait = 1500;
}