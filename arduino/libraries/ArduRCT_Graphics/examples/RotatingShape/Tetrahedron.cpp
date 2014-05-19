#include "Shape3D.h"

/**
    http://www.csee.umbc.edu/~squire/reference/polyhedra.shtml

    Vertices
        0,  x= 0.000, y= 0.000, z= 1.000 
        1,  x= 0.943, y= 0.000, z=-0.333 
        2,  x=-0.471, y= 0.816, z=-0.333 
        3,  x=-0.471, y=-0.816, z=-0.333
    Faces (normal not facing outwards)
        polygon(0,1,2);
        polygon(0,2,3);
        polygon(0,3,1);
        polygon(1,2,3);
**/

Tetrahedron::Tetrahedron(uint8_t style) {
    setStyle(style);
    setVerticesPerFace(3);

    //  used factor 1.7*1024
    setVertice(0, 0, 0, 1741);
    setVertice(1, 1642, 0, -580);
    setVertice(2, -820, 1420, -580);
    setVertice(3, -820, -1420, -580);
    
    setFace(0, 0, 1, 2);
    setFace(1, 0, 3, 1);
    setFace(2, 0, 2, 3);
    setFace(3, 1, 3, 2);
}

char *Tetrahedron::getName() {
    return "Tetrahedron";
}
