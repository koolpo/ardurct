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

#define REPORT_PERFORMANCE 1

#include "Shape3D.h"

//ArduRCT_S6D04H0 graphics;
//ArduRCT_SPFD5408 graphics;
ArduRCT_ILI9340 graphics;
//ArduRCT_ST7735 graphics;

#define BUFFER_WIDTH 160
#define BUFFER_HEIGHT 160
#define ROTATIONS_PER_SHAPE 2
#define ROTATIONS_STEPS_PER_DEGREE 2
#define FIELD_OF_VIEW 300
#define VIEW_DISTANCE 7

#define NB_SHAPES 6
#define NB_STYLES 4
#define POLYHEDRON 5

Tetrahedron tetrahedron(WIREFRAME);
Cube cube(WIREFRAME);
Octahedron octahedron(WIREFRAME);
Dodecahedron dodecahedron(WIREFRAME);
Isocahedron isocahedron(WIREFRAME);
Shape3D polyhedron(WIREFRAME);
Shape3D *shape[] = { &tetrahedron, &cube, &octahedron, &dodecahedron, &isocahedron, &polyhedron };

uint16_t buffer[BUFFER_WIDTH*BUFFER_HEIGHT];
uint16_t angle = 0;
uint8_t shapeNb = 0;
uint8_t style = WIREFRAME;
uint32_t startTime = 0;
uint16_t maxSize[2];

void setup() {
    maxSize[X] = 0;
    maxSize[Y] = 0;
#ifdef REPORT_PERFORMANCE
    Serial.begin(57600);
#endif
    graphics.begin(WHITE, BLACK, FONT_MEDIUM, FONT_PLAIN, NO_OVERLAY);
    graphics.fillScreen(BLACK);
    // display the shape and style
    showShapeNameAndStyle();
    // start the double buffering
    graphics.startDoubleBuffering(buffer, BUFFER_WIDTH, BUFFER_HEIGHT);
    // create the polyedron faces
    calculatePolyhedronFaces();
}

void loop() {
    if (startTime == 0) startTime = micros();
    // fill the buffer with black
    graphics.fillRectangle(0, 0, BUFFER_WIDTH, BUFFER_HEIGHT, BLACK);
    // build the shape if required
    if (shapeNb == POLYHEDRON) calculatePolyhedronVertices();
    // rotate the shape, starting from the initial position to suppress the fixed point math imprecision on rotations
    shape[shapeNb]->clearTransformations();
    shape[shapeNb]->rotate(angle/ROTATIONS_STEPS_PER_DEGREE, angle, 0);
    // draw it in the buffer
    shape[shapeNb]->draw(&graphics, FIELD_OF_VIEW, VIEW_DISTANCE, BUFFER_WIDTH, BUFFER_HEIGHT, maxSize);
    // paste the buffer in the middle of the screen
    graphics.pasteDoubleBuffer((graphics.getWidth()-BUFFER_WIDTH)/2,(graphics.getHeight()-BUFFER_HEIGHT)/2);
    angle += 1;  
    // when ROTATIONS_PER_SHAPE have been performed
    if (angle == 360*ROTATIONS_PER_SHAPE*ROTATIONS_STEPS_PER_DEGREE) {
#ifdef REPORT_PERFORMANCE
        // show a performance measure
        Serial.print(shape[shapeNb]->getName()); Serial.print(" "); Serial.print(shape[shapeNb]->getStyle()); 
        Serial.print(": size "); Serial.print(maxSize[X]); Serial.print("x"); Serial.print(maxSize[Y]);
        Serial.print(", "); Serial.print(ROTATIONS_PER_SHAPE*ROTATIONS_STEPS_PER_DEGREE*360L*1000000L/(micros()-startTime)); Serial.println("fps");
#endif
        startTime = 0;    
        angle = 0;
        // change the style
        style ++;
        // when all styles have been shown change the shape
        if (style > GREY_SHADED) {
            style = WIREFRAME;
            shapeNb ++;
            if (shapeNb >= NB_SHAPES) shapeNb = 0;
        }
        // change the style of the shape
        shape[shapeNb]->setStyle(style);
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
    graphics.print(shape[shapeNb]->getName());
    graphics.print(" ");
    graphics.print(shape[shapeNb]->getStyle());
}


// the polyhedron contains 4 tetrahedrons
void calculatePolyhedronFaces() {
    polyhedron.setVerticesPerFace(3);
    for (uint8_t i=0; i<4; i++) {
        for (uint8_t j=0; j<4; j++) {
            uint8_t *face = tetrahedron.getFace(j);
            polyhedron.setFace(i*4+j, i*4+face[0], i*4+face[1], i*4+face[2], i*4+face[3], i*4+face[4]);
        }
    }
}

void calculatePolyhedronVertices() {
    // save the vertices positions of a scaled down tetrahedron
    int32_t translations[4][3];
    tetrahedron.clearTransformations();
    tetrahedron.scale(3, 5);
    for (uint8_t i=0; i<4; i++) {
        int32_t *vertice = tetrahedron.getVertice(i);
        translations[i][X] = vertice[X];
        translations[i][Y] = vertice[Y];
        translations[i][Z] = vertice[Z];
    }
    // we rotate the small tetrahedrons, scale them and translate them to the vertices of the bigger tetrahedron defined above 
    for (uint8_t i=0; i<4; i++) {
        tetrahedron.clearTransformations();
        if (i == 0) tetrahedron.rotate(angle/2, angle, 0);
        else if (i == 1) tetrahedron.rotate(angle, angle/2, 0);
        else if (i == 2) tetrahedron.rotate(-angle/2, -angle, 0);
        else tetrahedron.rotate(-angle, -angle/2, 0);
        tetrahedron.scale(1, 2);
        tetrahedron.translate(translations[i][X], translations[i][Y], translations[i][Z]);
        for (uint8_t j=0; j<4; j++) {
            int32_t *vertice = tetrahedron.getVertice(j);
            polyhedron.setInitialVertice(i*4+j, vertice[X], vertice[Y], vertice[Z]);
        }
    }
}