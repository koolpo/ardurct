#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include "TouScruino.hpp"

#define BLACK 	0x0000
#define RED		(0x01F << 11) 
#define GREEN	(0x03F << 5) 
#define BLUE	0x01F
#define CYAN    (GREEN+BLUE)
#define MAGENTA	(RED+BLUE)
#define YELLOW	(RED+GREEN)
#define WHITE	0xFFFF

#define TOUCHSCREEN_EQUAL 5
#define TOUCHSCREEN_NO_TOUCH 0xFFFF

#define isTouched() (getTouchedXYZ(0, 0, 0) != TOUCHSCREEN_NO_TOUCH)

#include "S6D04H0.hpp"

// TouchScreen extends the implementation of ScreenHAL, here it is S6D04H0
// To change the COG of your screen, include another header and replace S6D04H0 by your COG  
class TouchScreen: public S6D04H0 {
    
	public:
		TouchScreen(uint8_t port = TOUSCRUINO_PORT, uint8_t cd = TOUSCRUINO_CD, uint8_t wr = TOUSCRUINO_WR, uint8_t rd = TOUSCRUINO_RD, 
				uint8_t cs = TOUSCRUINO_CS, uint8_t reset = TOUSCRUINO_RESET);
		
		void setupTouchPanel(uint8_t xm = TOUSCRUINO_XM, uint8_t xp = TOUSCRUINO_XP, uint8_t ym = TOUSCRUINO_YM, uint8_t yp = TOUSCRUINO_YP, 
				uint16_t xPlaneResistance = TOUSCRUINO_X_PLANE_RESISTANCE, uint16_t pressureThreshold = TOUSCRUINO_PRESSURE_THRESHOLD);

		void begin(uint8_t fontSize = FONT_SMALL, uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK);

		void setBacklight(uint8_t value, uint8_t pin = TOUSCRUINO_BACKLIGHT);
		
		uint16_t getTouchedX();
		
		uint16_t getTouchedY();
		
		uint16_t getTouchedZ();
		
		uint16_t getTouchedXYZ(uint16_t *x, uint16_t *y, uint16_t *z);
		
	private:
		uint8_t _xm;
		uint8_t _xp;
		uint8_t _ym;
		uint8_t _yp;
		volatile uint8_t *_xmPort;
		volatile uint8_t *_xpPort;
		volatile uint8_t *_ymPort;
		volatile uint8_t *_ypPort;
		uint8_t _xmBitMask;
		uint8_t _xpBitMask;
		uint8_t _ymBitMask;
		uint8_t _ypBitMask;
		uint16_t _xPlaneResistance;
		uint16_t _pressureThreshold;	
};

#endif