/*
 * Colors - Demonstration of colors
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

/**
 *  Touscruino buttons control the color of a square
 *  MENU    resets the square color to black
 *  ENTER   changes the RGB part controlled
 *  UP      increases the value of the RGB part
 *  DOWN    decreases the value of the RGB part
 */

#include <SPI.h>
#include <ArduRCT_Graphics.h>
#include <ArduRCT_EventManager.h>

// Change to your version: the following include will automatically create the proper 'touscruino' object
#define TOUSCRUINO_VERSION 1
#include <ArduRCT_TouScruino.h>

#define FILL_SIZE 40
#define BUTTON_FONT FONT_MEDIUM
#define BUTTON_FONT_WEIGHT FONT_BOLD

ArduRCT_EventHandler buttonsHandler(EVENT_SWITCH, EVENT_ANY_VALUE, &handleButtons); 

int16_t colorPart[] = { 0, 0, 0 };
uint8_t activeColor = 0;
uint8_t buttonWidth;

void setup() {
    touscruino.begin(BLACK, WHITE, FONT_SMALL, FONT_PLAIN, NO_OVERLAY);
    touscruino.setRotation(GRAPHICS_ROTATION_90);  
    buttonWidth = touscruino.getStringWidth("Reset", BUTTON_FONT) + 10;
    
    drawButton("Red", 0, false);
    drawButton("Up", 1, false);
    drawButton("Down", 2, false);
    drawButton("Reset", 3, false);
    drawColor(BLACK);
    
    touscruino.registerEventHandler(&buttonsHandler);
}

void loop() {
    touscruino.manageEvents();
}

int8_t handleButtons(uint8_t eventType, uint8_t buttonId) {
    if (eventType == EVENT_SWITCH_PRESSED) {
        if (buttonId == TOUSCRUINO_MENU) {
            drawButton("Reset", 3, true);
            for (uint8_t i=0; i< 3; i++) colorPart[i] = 0;
        } else if (buttonId == TOUSCRUINO_ENTER) {
            activeColor ++;
            if (activeColor > 2) activeColor = 0;
            if (activeColor == 0) drawButton("Red", 0, true);
            else if (activeColor == 1) drawButton("Green", 0, true);
            else  drawButton("Blue", 0, true);
        } else if (buttonId == TOUSCRUINO_UP) {
            colorPart[activeColor] += (activeColor == 1 ? 0x04 : 0x08);
            if (colorPart[activeColor] > 0xFF) colorPart[activeColor] = 0xFF;
            drawButton("Up", 1, true);
        } else if (buttonId == TOUSCRUINO_DOWN) {
            colorPart[activeColor] -= (activeColor == 1 ? 0x04 : 0x08);
            if (colorPart[activeColor] < 0) colorPart[activeColor] = 0;
            drawButton("Down", 2, true);
        }
    } else if (eventType == EVENT_SWITCH_REPEATING) {
        if (buttonId == TOUSCRUINO_UP) {
            colorPart[activeColor] += (activeColor == 1 ? 0x04 : 0x08);
            if (colorPart[activeColor] > 0xFF) colorPart[activeColor] = 0xFF;
        } else if (buttonId == TOUSCRUINO_DOWN) {
            colorPart[activeColor] -= (activeColor == 1 ? 0x04 : 0x08);
            if (colorPart[activeColor] < 0) colorPart[activeColor] = 0;
        }
    } else if (eventType == EVENT_SWITCH_RELEASED) {
        if (buttonId == TOUSCRUINO_MENU) drawButton("Reset", 3, false);
        else if (buttonId == TOUSCRUINO_UP) drawButton("Up", 1, false);
        else if (buttonId == TOUSCRUINO_DOWN) drawButton("Down", 2, false);
        else if (buttonId == TOUSCRUINO_ENTER) {
            if (activeColor == 0) drawButton("Red", 0, false);
            else if (activeColor == 1) drawButton("Green", 0, false);
            else  drawButton("Blue", 0, false);
        }
    }
    if (eventType != EVENT_SWITCH_RELEASED) {
        int color = COLOR_565(colorPart[0], colorPart[1], colorPart[2]);
        drawColor(color);
    }
    return EVENT_HANDLING_DONE;
}


void drawButton(char *label, uint8_t nb, bool active) {
    uint16_t height = touscruino.getHeight()/4; 
    uint16_t y = nb*height;
    touscruino.fillRectangle(touscruino.getWidth()-buttonWidth, y, buttonWidth, height, active ? GREY : LIGHT_GREY);
    touscruino.drawRectangle(touscruino.getWidth()-buttonWidth, y - (nb == 0 ? 0 : 1), buttonWidth, height + (nb == 0 ? 0 : 1), BLACK, 1);
    touscruino.setBackgroundColor(active ? GREY : LIGHT_GREY);
    uint16_t x = touscruino.getWidth()-buttonWidth+(buttonWidth-touscruino.getStringWidth(label, BUTTON_FONT))/2;
    y += (height-touscruino.getFontHeight(BUTTON_FONT))/2;
    touscruino.drawString(label, x, y, BLACK, BUTTON_FONT, BUTTON_FONT_WEIGHT, NO_OVERLAY);
}

void drawColor(uint16_t color) {
    uint16_t y = (touscruino.getHeight()-FILL_SIZE-24)/2;
    touscruino.fillRectangle((touscruino.getWidth()-FILL_SIZE-buttonWidth)/2, y, FILL_SIZE, FILL_SIZE, color);
    touscruino.drawRectangle((touscruino.getWidth()-FILL_SIZE-buttonWidth)/2, y, FILL_SIZE, FILL_SIZE, BLACK, 1);
    touscruino.setBackgroundColor(WHITE);
    touscruino.setCursor(8, y + FILL_SIZE + 10);
    touscruino.print("R="); 
    if (colorPart[0] < 0x10) touscruino.print(0);
    touscruino.print(colorPart[0], HEX); 
    touscruino.print(" G="); 
    if (colorPart[1] < 0x10) touscruino.print(0);
    touscruino.print(colorPart[1], HEX); 
    touscruino.print(" B="); 
    if (colorPart[2] < 0x10) touscruino.print(0);
    touscruino.print(colorPart[2], HEX); 
    touscruino.setCursor(16, y + FILL_SIZE + 24);
    touscruino.print("Color=");
    if (color < 0x1000) touscruino.print(0);
    if (color < 0x100) touscruino.print(0);
    if (color < 0x10) touscruino.print(0);
    touscruino.print(color, HEX); 
}
