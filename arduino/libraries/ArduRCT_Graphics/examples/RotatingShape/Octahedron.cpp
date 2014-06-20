#include "Shape3D.h"

/**
    http://www.csee.umbc.edu/~squire/reference/polyhedra.shtml

    Vertices
        0,   x= 0.000, y= 0.000, z= 1.000 
        1,   x= 1.000, y= 0.000, z= 0.000 
        2,   x= 0.000, y= 1.000, z= 0.000 
        3,   x=-1.000, y= 0.000, z= 0.000 
        4,   x= 0.000, y=-1.000, z= 0.000 
        5,   x= 0.000, y= 0.000, z=-1.000 
    Faces (normal not facing outwards)
        polygon(0,1,2);
        polygon(0,2,3);
        polygon(0,3,4);
        polygon(0,4,1);
        polygon(5,1,2);
        polygon(5,2,3);
        polygon(5,3,4);
        polygon(5,4,1);
**/


Octahedron::Octahedron(uint8_t style) {
    setStyle(style);
    setVerticesPerFace(3);

    //  used factor 1.7*1024
    setInitialVertice(0, 0, 0, 1741);
    setInitialVertice(1, 1741, 0, 0);
    setInitialVertice(2, 0, 1741, 0);
    setInitialVertice(3, -1741, 0, 0);
    setInitialVertice(4, 0, -1741, 0);
    setInitialVertice(5, 0, 0, -1741);

    setFace(0, 0, 1, 2);
    setFace(1, 0, 2, 3);
    setFace(2, 0, 3, 4);
    setFace(3, 0, 4, 1);
    setFace(4, 5, 2, 1);
    setFace(5, 5, 3, 2);
    setFace(6, 5, 4, 3);
    setFace(7, 5, 1, 4);
}

char *Octahedron::getName() {
    return "Octahedron";
}