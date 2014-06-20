#ifndef SHAPE3D_H
#define SHAPE3D_H 1

#include <ArduRCT_Graphics.h>

#define MAX_VERTICES 20
#define MAX_FACES 20
#define MAX_VERTICES_PER_FACE 5

#define WIREFRAME               0
#define WIREFRAME_HIDDEN_FACES  1
#define COLORIZED               2
#define GREY_SHADED             3

// value between 0 and 15 (the higher the brighter the camera facing face)
#define SHADING_LIGHT 7
#define LINE_THICKNESS 1
#define LINE_COLOR WHITE

#define X 0
#define Y 1
#define Z 2

class Shape3D {
    
    public:
        Shape3D();
        Shape3D(uint8_t style);
        
        void clearTransformations();
        
        void setStyle(uint8_t style);
        void setVerticesPerFace(uint8_t vpf);
        void setInitialVertice(uint8_t n, int32_t x, int32_t y, int32_t z);
        int32_t *getInitialVertice(uint8_t n);
        int32_t *getVertice(uint8_t n);
        void setFace(uint8_t n, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4 = 0, uint8_t v5 = 0);
        uint8_t *getFace(uint8_t n);

        void rotate(int16_t angleX, int16_t angleY, int16_t angleZ);
        void scale(int32_t scaleMul, int32_t scaleDiv);
        void translate(int32_t x, int32_t y, int32_t z);
        void draw(ArduRCT_Graphics *graphics, int32_t fov, int32_t viewDistance, uint16_t drawWidth, uint16_t drawHeight, uint16_t drawSize[] = 0);
 
        virtual char *getName();
        char *getStyle();

    protected:
        int32_t _initialVertice[MAX_VERTICES][3];
        uint8_t _face[MAX_FACES][MAX_VERTICES_PER_FACE];
        uint8_t _nbVertices;
        uint8_t _nbFaces;
        uint8_t _verticesPerFace;
    
    private:
        int32_t _vertice2D[MAX_VERTICES][2];
        int32_t _vertice[MAX_VERTICES][3];
        uint8_t _faceOrder[MAX_FACES];
        uint8_t _style;
        bool _transformed;
        bool _facesOrdered;

        void _rotateX(int32_t verticeIn[], int32_t cosa, int32_t sina, int32_t verticeOut[]);
        void _rotateY(int32_t verticeIn[], int32_t cosa, int32_t sina, int32_t verticeOut[]);
        void _rotateZ(int32_t verticeIn[], int32_t cosa, int32_t sina, int32_t verticeOut[]);
        void _project(uint8_t n, int16_t viewWidth, int16_t viewHeight, int32_t fov, int32_t viewDistance);
        void _orderFaces();
        int32_t _getFaceVisibility(uint8_t f, int32_t viewDistance);
        
};

class Tetrahedron : public Shape3D {
    public:
        Tetrahedron(uint8_t style);
        virtual char *getName();
};

class Cube : public Shape3D {
    public:
        Cube(uint8_t style);
        virtual char *getName();
};

class Octahedron : public Shape3D {
    public:
        Octahedron(uint8_t style);
        virtual char *getName();
};

class Dodecahedron : public Shape3D {
    public:
        Dodecahedron(uint8_t style);
        virtual char *getName();
};

class Isocahedron : public Shape3D {
    public:
        Isocahedron(uint8_t style);
        virtual char *getName();
};


#endif