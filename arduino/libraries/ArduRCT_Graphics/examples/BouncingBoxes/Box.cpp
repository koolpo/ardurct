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


void Box::init(uint16_t color, uint16_t size, int16_t x, int16_t y, int16_t xSpeed, int16_t ySpeed) {
    _color = color;
    _size = size;
    _x = toSoftware(x);
    _y = toSoftware(y);
    _xSpeed = xSpeed;
    _ySpeed = ySpeed;
	_xDraw = -1000;
	_yDraw = 0;
}


void Box::erase(ArduRCT_Graphics *display) {
	// only erase if already drawn
	if (_xDraw == -1000) return;
#if (BOX_FILLED == 1)
    display->fillRectangle(_xDraw, _yDraw, _size, _size, display->getBackgroundColor());
#else
    display->drawRectangle(_xDraw, _yDraw, _size, _size, display->getBackgroundColor(), BOX_THICKNESS);
#endif	
	_xDraw = -1000;
}


void Box::draw(ArduRCT_Graphics *display) {
	_xDraw = toScreen(_x);
	_yDraw = toScreen(_y);
#if (BOX_FILLED == 1)
    display->fillRectangle(_xDraw, _yDraw, _size, _size, _color);
#else
    display->drawRectangle(_xDraw, _yDraw, _size, _size, _color, BOX_THICKNESS);
#endif	
}


void Box::move() {
    _x += _xSpeed; 
    _y += _ySpeed; 
}


void Box::checkWallCollisions(uint16_t wallX, uint16_t wallY) {
    if (_x < 0) {                       // if at or beyond left side
        _x = 0;                         // place against edge and
        _xSpeed = -_xSpeed;           // reverse direction.
    } else if (_x > toSoftware(wallX-_size)) {      // if at or beyond right side
        _x = toSoftware(wallX-_size);        // place against right edge.
        _xSpeed = -_xSpeed;           // reverse direction.
    }
    if (_y < 0) {                       // if at or beyond top side
        _y = 0;                         // place against edge and
        _ySpeed = -_ySpeed;           // reverse direction.
    } else if (_y > toSoftware(wallY-_size)) {      // if at or beyond bottom side
        _y = toSoftware(wallY-_size);               // place against bottom edge.
        _ySpeed = -_ySpeed;           // reverse direction.
    }
}


void Box::checkBoxCollisions(Box *box) {
	uint16_t size = toSoftware(_size);
    if ((abs(_x-box->_x) < size) && (abs(_y-box->_y) < size)) {
        if (_xSpeed * box->_xSpeed < 0) {
            if (box->_x > _x) box->_x = _x + size;
            else box->_x = _x - size;
            _xSpeed = -_xSpeed;
            box->_xSpeed = -box->_xSpeed;
        } 
        if (_ySpeed * box->_ySpeed < 0) {
            if (box->_y > _y) box->_y = _y + size;
            else box->_y = _y - size;
            _ySpeed = -_ySpeed;
            box->_ySpeed = -box->_ySpeed;
        } 
    }
}