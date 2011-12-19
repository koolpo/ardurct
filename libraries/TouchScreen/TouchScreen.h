#ifndef TOUCH_SCREEN_H
#define TOUCH_SCREEN_H

#define BLACK 	0x0000
#define WHITE	0xFFFF
#define RED		(0x01F << 11) 
#define GREEN	(0x03F << 5) 
#define BLUE	0x01F


#include "S6D04H0.hpp"

class TouchScreen: public S6D04H0 {
    
	public:
		void TouchScreen(uint8_t xm = A6, uint8_t xp = 15, uint8_t ym = 14, uint8_t yp = A7, 
				uint8_t port = PB, uint8_t cd = 21, uint8_t wr = 22, uint8_t rd = 23, uint8_t cs = 0xFF, uint8_t reset = 0xFF);

		void TouchScreen(uint8_t port = PB, uint8_t cd = 21, uint8_t wr = 22, uint8_t rd = 23, uint8_t cs = 0xFF, uint8_t reset = 0xFF);

		void TouchScreen:begin(uint8_t fontSize = 1, uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK) {

		uint16_t get565Color(uint8_t r, uint8_t g, uint8_t b);

		bool TouchScreen::getPressedPoint(uint16_t *x, uint16_t *y);
		
	private:
		uint8_t _xm;
		uint8_t _xp;
		uint8_t _ym;
		uint8_t _yp;
}

#endif