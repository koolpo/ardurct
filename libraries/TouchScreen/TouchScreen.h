#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include "colors.hpp"
#include "hardware.hpp"

#if defined(HARDWARE_S6D04H0)
#include "S6D04H0.hpp"
#elif defined(HARDWARE_ILI932X)
#include "ILI932X.hpp"
#endif

#define TOUCHSCREEN_NO_TOUCH 0xFFFF

// the address in the eeprom where the calibration matrix is stored 
// 25 bytes are taken in descending order
#define TOUCHSCREEN_CALIBRATED_EEPROM_ADDRESS 	E2END
#define TOUCHSCREEN_X_A_EEPROM_ADDRESS 			(E2END-1)
#define TOUCHSCREEN_X_B_EEPROM_ADDRESS 			(E2END-5)
#define TOUCHSCREEN_X_DIVIDER_EEPROM_ADDRESS 	(E2END-9)
#define TOUCHSCREEN_Y_A_EEPROM_ADDRESS 			(E2END-13)
#define TOUCHSCREEN_Y_B_EEPROM_ADDRESS 			(E2END-17)
#define TOUCHSCREEN_Y_DIVIDER_EEPROM_ADDRESS 	(E2END-21)

typedef struct {
	int32_t a;
	int32_t b;
	int32_t divider;
} tsCalibrationEquation_t;

#if defined(HARDWARE_S6D04H0)
class TouchScreen: public S6D04H0 {
#elif defined(HARDWARE_ILI932X)
class TouchScreen: public ILI932X {
#endif    

	public:
		TouchScreen(uint8_t port = HARDWARE_PORT, uint8_t cd = HARDWARE_CD, uint8_t wr = HARDWARE_WR, uint8_t rd = HARDWARE_RD, 
				uint8_t cs = HARDWARE_CS, uint8_t reset = HARDWARE_RESET, uint8_t backlightPin = HARDWARE_BACKLIGHT);
		
		void setupTouchPanel(uint8_t xm = HARDWARE_XM, uint8_t xp = HARDWARE_XP, uint8_t ym = HARDWARE_YM, uint8_t yp = HARDWARE_YP);

		void begin(uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK, uint8_t fontSize = FONT_SMALL, bool fontBold = false, bool fontOverlay = false);

		void setupBacklight(uint8_t backlightPin = HARDWARE_BACKLIGHT);
		
		void setBacklight(uint8_t value);
		
		uint8_t getBacklight();
		
		uint16_t getTouchX();
		
		bool isTouched();

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
		tsCalibrationEquation_t _xCalibrationEquation;
		tsCalibrationEquation_t _yCalibrationEquation;
		
		bool _calibrateTouchPanelPoint(int32_t dX, int32_t dY, int32_t *tsX, int32_t *tsY);
		
		void _getDisplayXY(int16_t *x, int16_t *y);
};

#endif