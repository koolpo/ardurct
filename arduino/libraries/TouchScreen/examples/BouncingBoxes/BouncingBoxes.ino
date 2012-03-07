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
 
#include <TouchScreen.h>
#include "Box.h"

#define REFRESH 30

#define BOX_SPEED_X 2
#define BOX_SPEED_Y 3
#define BOX_SIZE 20

TouchScreen touchscreen;
Box box1(RED, BOX_SIZE);
Box box2(BLUE, BOX_SIZE);
Box box3(GREEN, BOX_SIZE);
int16_t maxX;
int16_t maxY;


void setup() {
    touchscreen.begin(BLACK, BACKGROUND);
    touchscreen.setBacklight(180);
    box1.vectorize(0, 0, BOX_SPEED_X, BOX_SPEED_Y);
    box2.vectorize(touchscreen.getWidth(), 0, BOX_SPEED_Y, BOX_SPEED_X);
    box3.vectorize(0, touchscreen.getHeight(), BOX_SPEED_Y, BOX_SPEED_X);
    maxX = touchscreen.getWidth();
    maxY = touchscreen.getHeight();
}

void loop() {
    // erase the boxes
    box1.erase(&touchscreen);
    box2.erase(&touchscreen);
    box3.erase(&touchscreen);
    
    // move the boxes
    box1.move();
    box2.move();
    box3.move();
    
    // check inter box collisions
    box1.checkBoxCollisions(&box2);
    box1.checkBoxCollisions(&box3);
    box2.checkBoxCollisions(&box3);
    
    // bounce the box off the walls if necessary.
    box1.checkWallCollisions(maxX, maxY);
    box2.checkWallCollisions(maxX, maxY);
    box3.checkWallCollisions(maxX, maxY);
    
    // draw the ball
    box1.draw(&touchscreen);
    box2.draw(&touchscreen);
    box3.draw(&touchscreen);
    
    // wait
    delay(REFRESH);
}
