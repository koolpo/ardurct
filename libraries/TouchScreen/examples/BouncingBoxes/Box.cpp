#include "Box.h"

Box::Box(uint16_t color, uint16_t size) {
    _color = color;
    _size = size;
}


void Box::vectorize(int16_t x, int16_t y, int16_t x_speed, int16_t y_speed) {
    _x = x;
    _y = y;
    _x_speed = x_speed;
    _y_speed = y_speed;
}


void Box::erase(TouchScreen *touchscreen) {
    touchscreen->drawRectangle(_x, _y, _size, _size, BACKGROUND, BOX_THICKNESS);
}


void Box::draw(TouchScreen *touchscreen) {
    touchscreen->drawRectangle(_x, _y, _size, _size, _color, BOX_THICKNESS);
}


void Box::move() {
    _x += _x_speed; 
    _y += _y_speed; 
}


void Box::checkWallCollisions(uint16_t wallX, uint16_t wallY) {
    if (_x < 0) {                       // if at or beyond left side
        _x = 0;                         // place against edge and
        _x_speed = -_x_speed;           // reverse direction.
    } else if (_x > wallX-_size) {      // if at or beyond right side
        _x = wallX-_size;                // place against right edge.
        _x_speed = -_x_speed;           // reverse direction.
    }
    if (_y < 0) {                       // if at or beyond top side
        _y = 0;                         // place against edge and
        _y_speed = -_y_speed;           // reverse direction.
    } else if (_y > wallY-_size) {      // if at or beyond bottom side
        _y = wallY-_size;               // place against bottom edge.
        _y_speed = -_y_speed;           // reverse direction.
    }
}


void Box::checkBoxCollisions(Box *box) {
    if ((abs(_x-box->_x) < _size) && (abs(_y-box->_y) < _size)) {
        if (_x_speed * box->_x_speed < 0) {
            if (box->_x > _x) box->_x = _x + _size;
            else box->_x = _x - _size;
            _x_speed = -_x_speed;
            box->_x_speed = -box->_x_speed;
        } 
        if (_y_speed * box->_y_speed < 0) {
            if (box->_y > _y) box->_y = _y + _size;
            else box->_y = _y - _size;
            _y_speed = -_y_speed;
            box->_y_speed = -box->_y_speed;
        } 
    }
}