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

#define X 0
#define Y 1
#define Z 2

class Shape3D {
    
    public:
        Shape3D();
        Shape3D(uint8_t style);
        
        virtual void initialize();
        void setStyle(uint8_t style);
        void setVerticesPerFace(uint8_t vpf);
        void setVertice(uint8_t n, int32_t x, int32_t y, int32_t z);
        void setFace(uint8_t n, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4 = 0, uint8_t v5 = 0);
        
        int16_t cos(int16_t angle);
        uint16_t sqrt(uint32_t n);
        
        void rotate(int16_t angleX, int16_t angleY, int16_t angleZ);
        void draw(ArduRCT_Graphics *graphics, int32_t fov, int32_t viewDistance, uint16_t drawWidth, uint16_t drawHeight, uint16_t drawSize[] = 0);
 
        virtual char *getName();
        virtual char *getStyle();

    protected:
        int32_t _vertice[MAX_VERTICES][3];
        uint8_t _face[MAX_FACES][MAX_VERTICES_PER_FACE];
        uint8_t _nbVertices;
        uint8_t _nbFaces;
        uint8_t _verticesPerFace;
    
    private:
        int32_t _vertice2D[MAX_VERTICES][2];
        int32_t _rVertice[MAX_VERTICES][3];
        uint8_t _faceOrder[MAX_FACES];
        uint8_t _style;
        bool _rotated;
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
        virtual void initialize();
        virtual char *getName();
};

class Cube : public Shape3D {
    public:
        Cube(uint8_t style);
        virtual void initialize();
        virtual char *getName();
};

class Octahedron : public Shape3D {
    public:
        Octahedron(uint8_t style);
        virtual void initialize();
        virtual char *getName();
};

class Dodecahedron : public Shape3D {
    public:
        Dodecahedron(uint8_t style);
        virtual void initialize();
        virtual char *getName();
};

class Isocahedron : public Shape3D {
    public:
        Isocahedron(uint8_t style);
        virtual void initialize();
        virtual char *getName();
};


#endif