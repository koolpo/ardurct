/*
 * Box - Class to define a box
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
 
#include "Box.h"

// we increase the precision in the software by 16
#define toScreen(a) ((a) >> 4)
#define toSoftware(a) ((a) << 4)

Box::Box() {
}


void Box::init(uint16_t color, uint16_t size, int16_t xIn, int16_t yIn, int16_t xSpeedIn, int16_t ySpeedIn) {
    _color = color;
    _size = size;
    x = toSoftware(xIn);
    y = toSoftware(yIn);
    xSpeed = xSpeedIn;
    ySpeed = ySpeedIn;
    _xDraw = -1000;
    _yDraw = 0;
}


void Box::erase(ArduRCT_Graphics *graphics) {
    // only erase if already drawn
    if (_xDraw == -1000) return;
#if (BOX_FILLED == 1)
    graphics->fillRectangle(_xDraw, _yDraw, _size, _size, graphics->getBackgroundColor());
#else
    graphics->drawRectangle(_xDraw, _yDraw, _size, _size, graphics->getBackgroundColor(), BOX_THICKNESS);
#endif    
    _xDraw = -1000;
}


void Box::draw(ArduRCT_Graphics *graphics) {
    _xDraw = toScreen(x);
    _yDraw = toScreen(y);
#if (BOX_FILLED == 1)
    graphics->fillRectangle(_xDraw, _yDraw, _size, _size, _color);
#else
    graphics->drawRectangle(_xDraw, _yDraw, _size, _size, _color, BOX_THICKNESS);
#endif    
}


void Box::move() {
    x += xSpeed; 
    y += ySpeed; 
}


void Box::checkWallCollisions(uint16_t wallX, uint16_t wallY) {
    if (x < 0) {                                // if at or beyond left side
        x = 0;                                  // place against edge and
        xSpeed = -xSpeed;                       // reverse direction.
    } else if (x > toSoftware(wallX-_size)) {   // if at or beyond right side
        x = toSoftware(wallX-_size);            // place against right edge.
        xSpeed = -xSpeed;                       // reverse direction.
    }
    if (y < 0) {                                // if at or beyond top side
        y = 0;                                  // place against edge and
        ySpeed = -ySpeed;                       // reverse direction.
    } else if (y > toSoftware(wallY-_size)) {   // if at or beyond bottom side
        y = toSoftware(wallY-_size);            // place against bottom edge.
        ySpeed = -ySpeed;                       // reverse direction.
    }
}


void Box::checkBoxCollisions(Box *box) {
    uint16_t size = toSoftware(_size);
    if ((abs(x-box->x) < size) && (abs(y-box->y) < size)) {
        if (xSpeed * box->xSpeed < 0) {
            if (box->x > x) box->x = x + size;
            else box->x = x - size;
            xSpeed = -xSpeed;
            box->xSpeed = -box->xSpeed;
        } 
        if (ySpeed * box->ySpeed < 0) {
            if (box->y > y) box->y = y + size;
            else box->y = y - size;
            ySpeed = -ySpeed;
            box->ySpeed = -box->ySpeed;
        } 
    }
}