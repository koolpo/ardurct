#include "Shape3D.h"

Cube::Cube(uint8_t style) {
    setStyle(style);
    setVerticesPerFace(4);

    setVertice( 0, -1024, +1024, -1024);
    setVertice( 1, +1024, +1024, -1024);
    setVertice( 2, +1024, -1024, -1024);
    setVertice( 3, -1024, -1024, -1024);
    setVertice( 4, -1024, +1024, +1024);
    setVertice( 5, +1024, +1024, +1024);
    setVertice( 6, +1024, -1024, +1024);
    setVertice( 7, -1024, -1024, +1024);
    
    setFace(0, 0, 1, 2, 3);
    setFace(1, 1, 5, 6, 2);
    setFace(2, 5, 4, 7, 6);
    setFace(3, 4, 0, 3, 7);
    setFace(4, 0, 4, 5, 1);
    setFace(5, 3, 2, 6, 7);
}

char *Cube::getName() {
    return "Cube";
}
