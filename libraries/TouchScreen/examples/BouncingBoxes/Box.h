/*
 * Box - Class to define a box
 *	Part of BoucingBoxes
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
 
#ifndef BOX_H
#define BOX_H

#include <TouchScreen.h>

#define BACKGROUND WHITE

#define BOX_THICKNESS 3

class Box {

    public:
        Box(uint16_t color = RED, uint16_t size = 10);
        
        void vectorize(int16_t x, int16_t y, int16_t x_speed, int16_t y_speed);

        void erase(TouchScreen *touchscreen);
        
        void draw(TouchScreen *touchscreen);
        
        void move();

        void checkWallCollisions(uint16_t wall_x, uint16_t wall_y);
        
        void checkBoxCollisions(Box *box);

    protected:
        int16_t _x;
        int16_t _y;
        int16_t _x_speed;
        int16_t _y_speed;
        uint16_t _size;

    private:
        uint16_t _color;
};

#endif