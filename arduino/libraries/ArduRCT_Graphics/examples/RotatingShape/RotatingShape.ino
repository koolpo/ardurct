/*
 * RotatingShape - Shows a 3D rotating shape on the screen
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
#include <ArduRCT_Graphics.h>

#include "Shape3D.h"

#define BUFFER_WIDTH 160
#define BUFFER_HEIGHT 160
#define ROTATIONS_PER_SHAPE 2
#define ROTATIONS_STEPS_PER_DEGREE 2
#define FIELD_OF_VIEW 300
#define VIEW_DISTANCE 7

//ArduRCT_S6D04H0 graphics;
//ArduRCT_SPFD5408 graphics;
ArduRCT_ILI9340 graphics;
//ArduRCT_ST7735 graphics;

#define NB_SHAPES 5
#define NB_STYLES 3
#define STARTING_STYLE WIREFRAME
Tetrahedron tetrahedron(STARTING_STYLE);
Cube cube(STARTING_STYLE);
Octahedron octahedron(STARTING_STYLE);
Dodecahedron dodecahedron(STARTING_STYLE);
Isocahedron isocahedron(STARTING_STYLE);
Shape3D *shape[] = { &tetrahedron, &cube, &octahedron, &dodecahedron, &isocahedron };

uint16_t buffer[BUFFER_WIDTH*BUFFER_HEIGHT];
uint16_t angle = 0;
uint8_t shapeNb = 0;
uint32_t startTime = 0;
uint16_t maxSize[2];

void setup() {
    maxSize[X] = 0;
    maxSize[Y] = 0;
    Serial.begin(57600);
    graphics.begin(WHITE, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
    graphics.fillScreen(BLACK);
    // display the shape and style
    showShapeNameAndStyle();
    // start the double buffering
    graphics.startDoubleBuffering(buffer, BUFFER_WIDTH, BUFFER_HEIGHT);
}

void loop() {
    if (startTime == 0) startTime = micros();
    // fill the buffer with black
    graphics.fillRectangle(0, 0, BUFFER_WIDTH, BUFFER_HEIGHT, BLACK);
    // rotate the cube
    shape[shapeNb/NB_STYLES]->rotate(angle/ROTATIONS_STEPS_PER_DEGREE, angle, 0);
    // draw it in the buffer
    shape[shapeNb/NB_STYLES]->draw(&graphics, FIELD_OF_VIEW, VIEW_DISTANCE, BUFFER_WIDTH, BUFFER_HEIGHT, maxSize);
    // paste the buffer in the middle of the screen
    graphics.pasteDoubleBuffer((graphics.getWidth()-BUFFER_WIDTH)/2,(graphics.getHeight()-BUFFER_HEIGHT)/2);
    
    angle += 1;  
    // when ROTATIONS_PER_SHAPE have been performed
    if (angle == 360*ROTATIONS_PER_SHAPE*ROTATIONS_STEPS_PER_DEGREE) {
        // show a performance measure
        Serial.print(shape[shapeNb/NB_STYLES]->getName()); Serial.print(" "); Serial.print(shape[shapeNb/NB_STYLES]->getStyle()); 
        Serial.print(": size "); Serial.print(maxSize[X]); Serial.print("x"); Serial.print(maxSize[Y]);
        Serial.print(", "); Serial.print(ROTATIONS_PER_SHAPE*ROTATIONS_STEPS_PER_DEGREE*360L*1000000L/(micros()-startTime)); Serial.println("fps");
        startTime = 0;    
        angle = 0;
        // change the shape to display, or its style
        shapeNb ++;
        if (shapeNb >= NB_SHAPES*NB_STYLES) shapeNb = 0;
        // shange the style of the shape
        shape[shapeNb/NB_STYLES]->setStyle((shapeNb%NB_STYLES) + STARTING_STYLE);
        maxSize[X] = 0;
        maxSize[Y] = 0;
        // stop the double buffering
        graphics.stopDoubleBuffering();
        // display the shape and style
        showShapeNameAndStyle();
        // re-start the double buffering
        graphics.startDoubleBuffering(buffer, BUFFER_WIDTH, BUFFER_HEIGHT);
    }  
}

void showShapeNameAndStyle() {
    graphics.fillRectangle(10, 300, 230, 20, BLACK);
    graphics.setCursor(10, 300);
    graphics.print(shape[shapeNb/NB_STYLES]->getName());
    graphics.print(" ");
    graphics.print(shape[shapeNb/NB_STYLES]->getStyle());
}