#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#define BLACK 	0x0000
#define WHITE	0xFFFF
#define RED		(0x01F << 11) 
#define GREEN	(0x03F << 5) 
#define BLUE	0x01F

#define TOUSCRUINO_XM	A6
#define TOUSCRUINO_XP	15
#define TOUSCRUINO_YM	14
#define TOUSCRUINO_YP	A7

#include "S6D04H0.hpp"

class TouchScreen: public S6D04H0 {
    
	public:
		TouchScreen(uint8_t port = TOUSCRUINO_PORT, uint8_t cd = TOUSCRUINO_CD, uint8_t wr = TOUSCRUINO_WR, uint8_t rd = TOUSCRUINO_RD, uint8_t cs = TOUSCRUINO_CS, uint8_t reset = TOUSCRUINO_RESET);
		
		void defineTouchPanelPins(uint8_t xm = TOUSCRUINO_XM, uint8_t xp = TOUSCRUINO_XP, uint8_t ym = TOUSCRUINO_YM, uint8_t yp = TOUSCRUINO_YP);

		void begin(uint8_t fontSize = FONT_SMALL, uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK);

		uint16_t create565Color(uint8_t r, uint8_t g, uint8_t b);

		bool getPressedPoint(uint16_t *x, uint16_t *y);
		
	private:
		uint8_t _xm;
		uint8_t _xp;
		uint8_t _ym;
		uint8_t _yp;
};

#endif