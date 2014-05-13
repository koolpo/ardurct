#include "Shape3D.h"

/**
    http://www.csee.umbc.edu/~squire/reference/polyhedra.shtml

    Vertices
        0,  x= 0.607, y= 0.000, z= 0.795 
        1,  x= 0.188, y= 0.577, z= 0.795 
        2,  x=-0.491, y= 0.357, z= 0.795 
        3,  x=-0.491, y=-0.357, z= 0.795 
        4,  x= 0.188, y=-0.577, z= 0.795 
        5,  x= 0.982, y= 0.000, z= 0.188 
        6,  x= 0.304, y= 0.934, z= 0.188 
        7,  x=-0.795, y= 0.577, z= 0.188 
        8,  x=-0.795, y=-0.577, z= 0.188 
        9,  x= 0.304, y=-0.934, z= 0.188 
       10,  x= 0.795, y= 0.577, z=-0.188 
       11,  x=-0.304, y= 0.934, z=-0.188 
       12,  x=-0.982, y= 0.000, z=-0.188 
       13,  x=-0.304, y=-0.934, z=-0.188 
       14,  x= 0.795, y=-0.577, z=-0.188 
       15,  x= 0.491, y= 0.357, z=-0.795 
       16,  x=-0.188, y= 0.577, z=-0.795 
       17,  x=-0.607, y= 0.000, z=-0.795 
       18,  x=-0.188, y=-0.577, z=-0.795 
       19,  x= 0.491, y=-0.357, z=-0.795 
       
    Faces (normal not facing outwards)
        polygon(0,1,2,3,4);
        polygon(0,1,6,10,5);
        polygon(1,2,7,11,6);
        polygon(2,3,8,12,7);
        polygon(3,4,9,13,8);
        polygon(4,0,5,14,9);
        polygon(15,16,11,6,10);
        polygon(16,17,12,7,11);
        polygon(17,18,13,8,12);
        polygon(18,19,14,9,13);
        polygon(19,15,10,5,14);
        polygon(15,16,17,18,19);
**/

Dodecahedron::Dodecahedron(uint8_t style) {
    setStyle(style);
    setVerticesPerFace(5);
    initialize();
}

void Dodecahedron::initialize() {
    //  used factor 1.7*1024
    setVertice(0, 1057, 0, 1384);
    setVertice(1, 327, 1004, 1384);
    setVertice(2, -855, 621, 1384);
    setVertice(3, -855, -621, 1384);
    setVertice(4, 327, -1004, 1384);
    setVertice(5, 1709, 0, 327);
    setVertice(6, 529, 1626, 327);
    setVertice(7, -1384, 1004, 327);
    setVertice(8, -1384, -1004, 327);
    setVertice(9, 529, -1626, 327);
    setVertice(10, 1384, 1004, -327);
    setVertice(11, -529, 1626, -327);
    setVertice(12, -1709, 0, -327);
    setVertice(13, -529, -1626, -327);
    setVertice(14, 1384, -1004, -327);
    setVertice(15, 855, 621, -1384);
    setVertice(16, -327, 1004, -1384);
    setVertice(17, -1057, 0, -1384);
    setVertice(18, -327, -1004, -1384);
    setVertice(19, 855, -621, -1384);

    setFace(0,   0,  1,  2,  3,  4);
    setFace(1,   0,  5, 10,  6,  1);
    setFace(2,   1,  6, 11,  7,  2);
    setFace(3,   2,  7, 12,  8,  3);
    setFace(4,   3,  8, 13,  9,  4);
    setFace(5,   4,  9, 14,  5,  0);
    setFace(6,  15, 16, 11,  6, 10);
    setFace(7,  16, 17, 12,  7, 11);
    setFace(8,  17, 18, 13,  8, 12);
    setFace(9,  18, 19, 14,  9, 13);
    setFace(10, 19, 15, 10,  5, 14);
    setFace(11, 15, 19, 18, 17, 16);
}

char *Dodecahedron::getName() {
    return "Dodecahedron";
}