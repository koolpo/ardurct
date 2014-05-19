#include "Shape3D.h"

static int16_t _COS[] = {
    1024, 1024, 1023, 1023, 1022, 1020, 1018, 1016, 1014, 1011,
    1008, 1005, 1002, 998, 994, 989, 984, 979, 974, 968,
    962, 956, 949, 943, 935, 928, 920, 912, 904, 896,
    887, 878, 868, 859, 849, 839, 828, 818, 807, 796,
    784, 773, 761, 749, 737, 724, 711, 698, 685, 672,
    658, 644, 630, 616, 602, 587, 573, 558, 543, 527,
    512, 496, 481, 465, 449, 433, 416, 400, 384, 367,
    350, 333, 316, 299, 282, 265, 248, 230, 213, 195,
    178, 160, 143, 125, 107, 89, 71, 54, 36, 18, 0 };

static int16_t _cos(int16_t angle) {
    angle = angle % 360;
    if (angle < 0) angle = 360+angle;
    if (angle <= 90) return _COS[angle];
    if (angle <= 180) return -_COS[180-angle];
    if (angle <= 270) return -_COS[angle-180];
    return _COS[360-angle];
}

static uint16_t _sqrt(uint32_t n) {
    uint32_t op = n;
    uint32_t res = 0;
    uint32_t one = 1 << 30;
    while (one > op) one >>= 2;
    while (one != 0) {
        if (op >= res + one) {
            op = op - (res + one);
            res = res + (one << 1);
        }
        res >>= 1;
        one >>= 2;
    }
    return res;	
}


static uint16_t GREY565[] = { 
    0x0000, 0x0841, 0x1082, 0x18C3, 0x2104, 0x2945, 0x3186, 0x39C7, 0x4208, 0x4A49,
    0x528A, 0x5ACB, 0x630C, 0x6B4D, 0x738E, 0x7BCF, 0x8410, 0x8C51, 0x9492, 0x9CD3,
    0xA514, 0xAD55, 0xB596, 0xBDD7, 0xC618, 0xCE59, 0xD69A, 0xDEDB, 0xE71C, 0xEF5D,
    0xF79E, 0xFFDF };

static uint16_t COLORS[] = { 
    VIOLET, YELLOW, MAGENTA, GREEN, CYAN, RED, ORANGE, GREY, FUSCHIA, WHITE, 
    BLUE, BROWN, DARK_GREEN, DARK_RED, DARK_GREY, BLUE, YELLOW, DARK_CYAN, MAGENTA, CYAN, 
    RED, GREEN, ORANGE, GREY };    


Shape3D::Shape3D() {
    Shape3D(WIREFRAME_HIDDEN_FACES);
}

Shape3D::Shape3D(uint8_t style) {
    setStyle(style);
    _rotated = false;
    _facesOrdered = false;
}

void Shape3D::setStyle(uint8_t style) {
    _style = style;
}

char *Shape3D::getStyle() {
    if (_style == WIREFRAME) return "wireframe";
    if (_style == WIREFRAME_HIDDEN_FACES) return "hidden faces";
    if (_style == COLORIZED) return "colorized";
    return "shaded";
}

char *Shape3D::getName() {
    return "Polyhedron";
}

void Shape3D::initialize() {
}

void Shape3D::setVerticesPerFace(uint8_t vpf) {
    _verticesPerFace = vpf;
}

// all vertices are stored as integer, 1.0f becomes 1024 (1 << 10)
void Shape3D::setVertice(uint8_t n, int32_t x, int32_t y, int32_t z) {
    _vertice[n][X] = x;
    _vertice[n][Y] = y;
    _vertice[n][Z] = z;
    if (n+1 > _nbVertices) _nbVertices = n+1;
}

int32_t *Shape3D::getVertice(uint8_t n) {
    return _vertice[n];
}

int32_t *Shape3D::getRotatedVertice(uint8_t n) {
    return _rVertice[n];
}

void Shape3D::setFace(uint8_t n, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5) {
    _face[n][0] = v1;
    _face[n][1] = v2;
    _face[n][2] = v3;
    _face[n][3] = v4;
    _face[n][4] = v5;
    if (n+1 > _nbFaces) _nbFaces = n+1;
}

uint8_t *Shape3D::getFace(uint8_t n) {
    return _face[n];
}

void Shape3D::_rotateX(int32_t verticeIn[], int32_t cosa, int32_t sina, int32_t verticeOut[]) {
    verticeOut[X] = verticeIn[X];
    verticeOut[Y] = (verticeIn[Y] * cosa - verticeIn[Z] * sina) >> 10;
    verticeOut[Z] = (verticeIn[Y] * sina + verticeIn[Z] * cosa) >> 10;
}

void Shape3D::_rotateY(int32_t verticeIn[], int32_t cosa, int32_t sina, int32_t verticeOut[]) {
    verticeOut[Y] = verticeIn[Y];
    verticeOut[Z] = (verticeIn[Z] * cosa - verticeIn[X] * sina) >> 10;
    verticeOut[X] = (verticeIn[Z] * sina + verticeIn[X] * cosa) >> 10;
}
    
void Shape3D::_rotateZ(int32_t verticeIn[], int32_t cosa, int32_t sina, int32_t verticeOut[]) {
    verticeOut[Z] = verticeIn[Z];
    verticeOut[X] = (verticeIn[X] * cosa - verticeIn[Y] * sina) >> 10;
    verticeOut[Y] = (verticeIn[X] * sina + verticeIn[Y] * cosa) >> 10;
}

void Shape3D::_project(uint8_t n, int16_t viewWidth, int16_t viewHeight, int32_t fov, int32_t viewDistance) {
    int32_t factor = fov / (viewDistance + _rVertice[n][Z]);
    _vertice2D[n][X] = (((_rVertice[n][X] * factor) >> 10) + viewWidth / 2);
    _vertice2D[n][Y] = (((_rVertice[n][Y] * factor) >> 10) + viewHeight / 2);
}
        
void Shape3D::rotate(int16_t angleX, int16_t angleY, int16_t angleZ) {
    int32_t tempVertice[3];
    int32_t cosX = _cos(angleX);
    int32_t sinX = _cos(angleX+90);
    int32_t cosY = _cos(angleY);
    int32_t sinY = _cos(angleY+90);
    int32_t cosZ = _cos(angleZ);
    int32_t sinZ = _cos(angleZ+90);
    for (uint8_t i=0; i<_nbVertices; i++) {
        _rotateX(_vertice[i], cosX, sinX, tempVertice);
        _rotateY(tempVertice, cosY, sinY, _rVertice[i]);
        _rotateZ(_rVertice[i], cosZ, sinZ, tempVertice);
        _rVertice[i][X] = tempVertice[X];
        _rVertice[i][Y] = tempVertice[Y];
        _rVertice[i][Z] = tempVertice[Z];
    }
    _rotated = true;
    _facesOrdered = false;
}

void Shape3D::scale(int32_t scaleMul, int32_t scaleDiv) {
    for (uint8_t i=0; i<_nbVertices; i++) {
        _rVertice[i][X] = _rVertice[i][X] * scaleMul / scaleDiv;
        _rVertice[i][Y] = _rVertice[i][Y] * scaleMul / scaleDiv;
        _rVertice[i][Z] = _rVertice[i][Z] * scaleMul / scaleDiv;
    }    	
    _facesOrdered = false;
}

void Shape3D::translate(int32_t x, int32_t y, int32_t z) {
    for (uint8_t i=0; i<_nbVertices; i++) {
        _rVertice[i][X] += x;
        _rVertice[i][Y] += y;
        _rVertice[i][Z] += z;    		
    }    	
    _facesOrdered = false;
}

void Shape3D::_orderFaces() {
    // calculate the average z of the face
    int16_t faceZ[MAX_FACES];
    for (uint8_t i=0; i<_nbFaces; i++) {
        uint8_t *p = _face[i];
        faceZ[i] = _rVertice[p[0]][Z];
        for (uint8_t j=1; j<_verticesPerFace; j++) faceZ[i] += _rVertice[p[j]][Z];
        faceZ[i] = faceZ[i] / _verticesPerFace;
        _faceOrder[i] = i;
    }
    // order faces: bigger Z is in front
    for (uint8_t i=0; i<_nbFaces; i++) {
        for (uint8_t j=i+1; j<_nbFaces; j++) {
            if (faceZ[_faceOrder[j]] > faceZ[_faceOrder[i]]) {
                uint8_t temp = _faceOrder[i];
                _faceOrder[i] = _faceOrder[j];
                _faceOrder[j] = temp;
            }
        }
    }
    _facesOrdered = true;
}


// Back face culling, with grey shade calculation
int32_t Shape3D::_getFaceVisibility(uint8_t f, int32_t viewDistance) {
    uint8_t *p = _face[f];
    // face normal vector: cross product of p0-p1 and p0-p2
    int32_t v1x = _rVertice[p[1]][X] - _rVertice[p[0]][X];
    int32_t v1y = _rVertice[p[1]][Y] - _rVertice[p[0]][Y];
    int32_t v1z = _rVertice[p[1]][Z] - _rVertice[p[0]][Z];
    int32_t v2x = _rVertice[p[2]][X] - _rVertice[p[0]][X];
    int32_t v2y = _rVertice[p[2]][Y] - _rVertice[p[0]][Y];
    int32_t v2z = _rVertice[p[2]][Z] - _rVertice[p[0]][Z];
    int32_t nX = v1y * v2z - v2y * v1z;
    int32_t nY = v2x * v1z - v1x * v2z;
    int32_t nZ = v1x * v2y - v2x * v1y;
    if (_style == GREY_SHADED) {
        // for shaded, we calculate the unit vector
        int32_t nXs = nX >> 10;
        int32_t nYs = nY >> 10;
        int32_t nZs = nZ >> 10;
        int32_t nModulus = _sqrt(nXs*nXs + nYs*nYs + nZs*nZs);
        nX = nX / nModulus;
        nY = nY / nModulus;
        nZ = nZ / nModulus;
    } else {
        nX = nX >> 10;
        nY = nY >> 10;
        nZ = nZ >> 10;
    }
    // view vector: from first point of face to point of origin (0, 0, -viewDistance) 
    int32_t vX = 0 - _rVertice[p[0]][X];
    int32_t vY = 0 - _rVertice[p[0]][Y];
    int32_t vZ = -viewDistance - _rVertice[p[0]][Z];
    if (_style == GREY_SHADED) {
        // for shaded, we calculate the unit vector
        int32_t vModulus = _sqrt(vX*vX + vY*vY + vZ*vZ);
        vX = (vX << 10) / vModulus;
        vY = (vY << 10) / vModulus;
        vZ = (vZ << 10) / vModulus;
    }
    // dot product of normal and view
    int32_t dp = vX * nX + vY * nY + vZ * nZ;
    // if value > 0, face is pointing toward the view: it is visible
    // for shaded, the returned value is the cos of the angle between view and normal
    return dp >> 10;
}
    
void Shape3D::draw(ArduRCT_Graphics *graphics, int32_t fov, int32_t viewDistance, uint16_t drawWidth, uint16_t drawHeight, uint16_t drawSize[]) {
    if (!_rotated) rotate(0, 0, 0);
    if (!_facesOrdered) _orderFaces();
    fov = fov << 10;
    viewDistance = viewDistance << 10;
    // project vertices
    int32_t maxX = 0;
    int32_t maxY = 0;
    int32_t minX = graphics->getWidth();
    int32_t minY = graphics->getHeight();
    // project the vertices and define the extent of the projected shape
    for (uint8_t i=0; i<_nbVertices; i++) {
        _project(i, drawWidth, drawHeight, fov, viewDistance);
        if (drawSize) {
            if (_vertice2D[i][X] < minX) minX = _vertice2D[i][X];
            if (_vertice2D[i][X] > maxX) maxX = _vertice2D[i][X];
            if (_vertice2D[i][Y] < minY) minY = _vertice2D[i][Y];
            if (_vertice2D[i][Y] > maxY) maxY = _vertice2D[i][Y];
        }
    }
    // return the drawSize
    if (drawSize) {
        drawSize[X] = max(drawSize[X], 2*max(maxX-drawWidth/2, drawWidth/2-minX)+4);
        drawSize[Y] = max(drawSize[Y], 2*max(maxY-drawHeight/2, drawHeight/2-minY)+4);
    }
    // draw the faces
    for (uint8_t i=0; i<_nbFaces; i++) {
        uint8_t *p = _face[_faceOrder[i]];
        int32_t faceVisibility = 0;
        if (_style != WIREFRAME) {
            faceVisibility = _getFaceVisibility(_faceOrder[i], viewDistance);
            if (faceVisibility < 0) continue;
            uint16_t color = BLACK;
            // we take the cos of normal,view to calculate the light on the face
            if (_style == GREY_SHADED) color = GREY565[SHADING_LIGHT + (faceVisibility >> 6)];
            // or a basic color
            else if (_style == COLORIZED) color = COLORS[_faceOrder[i]];
            // if 3 vertices per face
            graphics->fillTriangle(_vertice2D[p[0]][X], _vertice2D[p[0]][Y], 
                    _vertice2D[p[1]][X], _vertice2D[p[1]][Y], 
                    _vertice2D[p[2]][X], _vertice2D[p[2]][Y], color);
            // if 4 vertices per face
            if (_verticesPerFace == 4) {
                graphics->fillTriangle(_vertice2D[p[0]][X], _vertice2D[p[0]][Y], 
                        _vertice2D[p[2]][X], _vertice2D[p[2]][Y], 
                        _vertice2D[p[3]][X], _vertice2D[p[3]][Y], color);
            } else if (_verticesPerFace == 5) {
                graphics->fillTriangle(_vertice2D[p[0]][X], _vertice2D[p[0]][Y], 
                        _vertice2D[p[2]][X], _vertice2D[p[2]][Y], 
                        _vertice2D[p[4]][X], _vertice2D[p[4]][Y], color);
                graphics->fillTriangle(_vertice2D[p[2]][X], _vertice2D[p[2]][Y], 
                        _vertice2D[p[3]][X], _vertice2D[p[3]][Y], 
                        _vertice2D[p[4]][X], _vertice2D[p[4]][Y], color);
            }
        }
        if (_style < COLORIZED) {
           for (uint8_t j=0; j<_verticesPerFace-1; j++) {
                graphics->drawLine(_vertice2D[p[j]][X], _vertice2D[p[j]][Y], 
                        _vertice2D[p[j+1]][X], _vertice2D[p[j+1]][Y], LINE_COLOR, LINE_THICKNESS);
            }
            graphics->drawLine(_vertice2D[p[0]][X], _vertice2D[p[0]][Y], 
                    _vertice2D[p[_verticesPerFace-1]][X], _vertice2D[p[_verticesPerFace-1]][Y], LINE_COLOR, LINE_THICKNESS);
        }
    }
}