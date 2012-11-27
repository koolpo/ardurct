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
 
// Change the version to adapt to your screen
#define TOUSCRUINO_VERSION 1

#if (TOUSCRUINO_VERSION == 1)

#define BOX_SIZE 16
#define BOX_SPEED_X 1
#define BOX_SPEED_Y 2

#define TFT_CS 10
#define TFT_CD 9
#define TFT_RST 8

#include <SPI.h>
#include <ArduRCT_TouScruinoV1.h>

ArduRCT_TouScruinoV1 tft(TFT_CD, TFT_CS, TFT_RST);

#elif (TOUSCRUINO_VERSION == 2)

#define BOX_SIZE 20
#define BOX_SPEED_X 2
#define BOX_SPEED_Y 3

#define TFT_PORT 2 // PortB
#define TFT_CD     21
#define TFT_WR     22
#define TFT_RD     23
#define TFT_CS     0xFF
#define TFT_RST 0xFF
#define TFT_BL 5

#include <ArduRCT_TouScruinoV2.h>

ArduRCT_TouScruinoV2 tft(TFT_PORT, TFT_CD, TFT_WR, TFT_RD, TFT_CS, TFT_RST, TFT_BL);

#endif

#include "Box.h"

#define BACKGROUND WHITE

#define REFRESH 30

#define NB_BOXES 3

Box box[NB_BOXES];
int16_t maxX;
int16_t maxY;

void setup() {
    tft.begin(BLACK, BACKGROUND);
    tft.setBacklight(180);
    maxX = tft.getWidth();
    maxY = tft.getHeight();
    box[0].init(RED, BOX_SIZE, 0, 0, BOX_SPEED_X, BOX_SPEED_Y);
    box[1].init(BLUE, BOX_SIZE, tft.getWidth(), 0, BOX_SPEED_Y, BOX_SPEED_X);
    box[2].init(GREEN, BOX_SIZE, 0, tft.getHeight(), BOX_SPEED_Y, BOX_SPEED_X);
}

void loop() {
    // erase the boxes
    for (uint8_t i=0; i<NB_BOXES; i++) box[i].erase(&tft);
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
    // draw the ball
    for (uint8_t i=0; i<NB_BOXES; i++) box[i].draw(&tft);
    
    // wait
    delay(REFRESH);
}