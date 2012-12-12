/*
 * BouncingBoxes - 3 boxes are bounced against each other
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

//#define BOX_SIZE 20
//#define BOX_SPEED_X 2
//#define BOX_SPEED_Y 3

#include <ArduRCT_ST7735.h>
ArduRCT_ST7735 tft(10, 9, 8);

#define BOX_SIZE 12
#define BOX_SPEED_X 1
#define BOX_SPEED_Y 2

#include "Box.h"

#define BACKGROUND WHITE

#define REFRESH_SCREEN_DELAY 50

#define NB_BOXES 3

Box box[NB_BOXES];
int16_t maxX;
int16_t maxY;
uint32_t nextDraw;

void setup() {
    touscruino.begin(BLACK, BACKGROUND);
    touscruino.setBacklight(180);
    maxX = touscruino.getWidth();
    maxY = touscruino.getHeight();
    box[0].init(RED, BOX_SIZE, 0, 0, BOX_SPEED_X, BOX_SPEED_Y);
    box[1].init(BLUE, BOX_SIZE, touscruino.getWidth(), 0, BOX_SPEED_Y, BOX_SPEED_X);
    box[2].init(GREEN, BOX_SIZE, 0, touscruino.getHeight(), BOX_SPEED_Y, BOX_SPEED_X);
}

void loop() {
    // move the boxes
    for (uint8_t i=0; i<NB_BOXES; i++) box[i].move();
    // check inter box collisions
    for (uint8_t i=0; i<NB_BOXES-1; i++) {
        for (uint8_t j=i+1; j<NB_BOXES; j++) {
            box[i].checkBoxCollisions(&box[j]);
        }
    }    
    // bounce the box off the walls if necessary.
    for (uint8_t i=0; i<NB_BOXES; i++) box[i].checkWallCollisions(maxX, maxY);
	// leave time for other things to happen
	delay(3);

    // check if we have to refresh the screen
    if ((int32_t)(millis() - nextDraw) < 0) return;
    nextDraw = millis() + REFRESH_SCREEN_DELAY;

    // erase the boxes
    for (uint8_t i=0; i<NB_BOXES; i++) box[i].erase(&tft);
    // draw the boxes
    for (uint8_t i=0; i<NB_BOXES; i++) box[i].draw(&tft);
}