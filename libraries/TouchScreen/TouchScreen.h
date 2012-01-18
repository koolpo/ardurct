#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

// TouchScreen extends the implementation of ScreenHAL, here it is S6D04H0
// To change the COG of your screen, include another header and replace S6D04H0 by your COG  
#include "S6D04H0.hpp"

#include "TouScruino.hpp"

#define BLACK 	0x0000
#define RED		(0x01F << 11) 
#define GREEN	(0x03F << 5) 
#define BLUE	0x01F
#define CYAN    (GREEN+BLUE)
#define MAGENTA	(RED+BLUE)
#define YELLOW	(RED+GREEN)
#define WHITE	0xFFFF

#define TOUCHSCREEN_EQUAL 20
#define TOUCHSCREEN_NO_TOUCH 0xFFFF

#define TOUCHSCREEN_EEPROM_CALIBRATED 0x0000		/* the address in the eeprom where the calibration matrix is stored */
#define TOUCHSCREEN_EEPROM_AX (TOUCHSCREEN_EEPROM_CALIBRATED+1)

#define isTouched() (getTouchXYZ(0, 0, 0) != TOUCHSCREEN_NO_TOUCH)

typedef struct {
	int32_t Ax;
	int32_t Bx;
	int32_t Dx;
	int32_t Ay;
	int32_t By;
	int32_t Dy;
} tsCalibrationMatrix_t;
	


class TouchScreen: public S6D04H0 {
    
	public:
		TouchScreen(uint8_t port = TOUSCRUINO_PORT, uint8_t cd = TOUSCRUINO_CD, uint8_t wr = TOUSCRUINO_WR, uint8_t rd = TOUSCRUINO_RD, 
				uint8_t cs = TOUSCRUINO_CS, uint8_t reset = TOUSCRUINO_RESET, uint8_t backlightPin = TOUSCRUINO_BACKLIGHT);
		
		void setupTouchPanel(uint8_t xm = TOUSCRUINO_XM, uint8_t xp = TOUSCRUINO_XP, uint8_t ym = TOUSCRUINO_YM, uint8_t yp = TOUSCRUINO_YP, 
				uint16_t xPlaneResistance = TOUSCRUINO_X_PLANE_RESISTANCE, uint16_t pressureThreshold = TOUSCRUINO_PRESSURE_THRESHOLD);

		void begin(uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK, uint8_t fontSize = FONT_SMALL, bool fontBold = false);

		void setupBacklight(uint8_t backlightPin);
		
		void setBacklight(uint8_t value);
		
		uint8_t getBacklight();
		
		uint16_t getTouchX();
		
		uint16_t getTouchY();
		
		uint16_t getTouchZ();
		
		uint16_t getTouchXYZ(int16_t *x, int16_t *y, int16_t *z);
		
		bool calibrateTouchPanel(bool force = false);
		
	private:
		uint8_t _xm;
		uint8_t _xp;
		uint8_t _ym;
		uint8_t _yp;
		uint8_t _backlightPin;
		uint8_t _backlight;
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
		tsCalibrationMatrix_t _calibrationMatrix;
		
		bool _calibrateTouchPanelPoint(int32_t dX, int32_t dY, int32_t *tsX, int32_t *tsY);
		
		void _getDisplayXY(int16_t *x, int16_t *y);
};

#endif