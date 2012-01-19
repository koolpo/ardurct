#ifndef BOX_H
#define BOX_H

#include <TouchScreen.h>

#define BACKGROUND WHITE

#define BOX_THICKNESS 3

class Box {

    public:
        Box(uint16_t color = RED, uint16_t size = 10);
        
        void vectorize(int16_t x, int16_t y, int16_t x_speed, int16_t y_speed);

        void erase(TouchScreen *touchscreen);
        
        void draw(TouchScreen *touchscreen);
        
        void move();

        void checkWallCollisions(uint16_t wall_x, uint16_t wall_y);
        
        void checkBoxCollisions(Box *box);

    protected:
        int16_t _x;
        int16_t _y;
        int16_t _x_speed;
        int16_t _y_speed;
        uint16_t _size;

    private:
        uint16_t _color;
};

#endif