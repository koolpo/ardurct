#include "Shape3D.h"

/**
    http://www.csee.umbc.edu/~squire/reference/polyhedra.shtml

    Vertices
        0,  x= 0.000, y= 0.000, z= 1.000 
        1,  x= 0.894, y= 0.000, z= 0.447 
        2,  x= 0.276, y= 0.851, z= 0.447 
        3,  x=-0.724, y= 0.526, z= 0.447 
        4,  x=-0.724, y=-0.526, z= 0.447 
        5,  x= 0.276, y=-0.851, z= 0.447 
        6,  x= 0.724, y= 0.526, z=-0.447 
        7,  x=-0.276, y= 0.851, z=-0.447 
        8,  x=-0.894, y= 0.000, z=-0.447 
        9,  x=-0.276, y=-0.851, z=-0.447 
        10,  x= 0.724, y=-0.526, z=-0.447 
        11,  x= 0.000, y= 0.000, z=-1.000 
    Faces (normal not facing outwards)
        polygon(0,1,2);
        polygon(0,2,3);
        polygon(0,3,4);
        polygon(0,4,5);
        polygon(0,5,1);
        polygon(11,6,7);
        polygon(11,7,8);
        polygon(11,8,9);
        polygon(11,9,10);
        polygon(11,10,6);
        polygon(1,2,6);
        polygon(2,3,7);
        polygon(3,4,8);
        polygon(4,5,9);
        polygon(5,1,10);
        polygon(6,7,2);
        polygon(7,8,3);
        polygon(8,9,4);
        polygon(9,10,5);
        polygon(10,6,1);
**/

Isocahedron::Isocahedron(uint8_t style) {
    setStyle(style);
    setVerticesPerFace(3);

    //  used factor 1.7*1024
    setInitialVertice(0, 0, 0, 1741);
    setInitialVertice(1, 1556, 0, 778);
    setInitialVertice(2, 480, 1481, 778);
    setInitialVertice(3, -1260, 916, 778);
    setInitialVertice(4, -1260, -916, 778);
    setInitialVertice(5, 480, -1481, 778);
    setInitialVertice(6, 1260, 916, -778);
    setInitialVertice(7, -480, 1481, -778);
    setInitialVertice(8, -1556, 0, -778);
    setInitialVertice(9, -480, -1481, -778);
    setInitialVertice(10, 1260, -916, -778);
    setInitialVertice(11, 0, 0, -1741);

    setFace(0,  0,  1,  2);
    setFace(1,  0,  2,  3);
    setFace(2,  0,  3,  4);
    setFace(3,  0,  4,  5);
    setFace(4,  0,  5,  1);
    setFace(5, 11,  7,  6);
    setFace(6, 11,  8,  7);
    setFace(7, 11,  9,  8);
    setFace(8, 11, 10,  9);
    setFace(9, 11,  6, 10);
    setFace(10, 1,  6,  2);
    setFace(11, 2,  7,  3);
    setFace(12, 3,  8,  4);
    setFace(13, 4,  9,  5);
    setFace(14, 5, 10,  1);
    setFace(15, 6,  7,  2);
    setFace(16, 7,  8,  3);
    setFace(17, 8,  9,  4);
    setFace(18, 9, 10,  5);
    setFace(19, 10, 6,  1);
}

char *Isocahedron::getName() {
    return "Isocahedron";
}