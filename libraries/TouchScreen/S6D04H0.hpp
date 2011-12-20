#ifndef S6D0H0_H
#define S6D0H0_H

#include "ScreenHAL.hpp"


class S6D04H0: public ScreenHAL {
    
	public:
		S6D04H0(uint8_t port = TOUSCRUINO_PORT, uint8_t cd = TOUSCRUINO_CD, uint8_t wr = TOUSCRUINO_WR, uint8_t rd = TOUSCRUINO_RD, uint8_t cs = TOUSCRUINO_CS, uint8_t reset = TOUSCRUINO_RESET);

		void drawPixel(uint16_t x, uint16_t y, uint16_t color, boolean grabAndReleaseBus);
		
	protected:
		// required by the ScreenHAL superclass
		void initScreen(void);
		
		// required by the ScreenHAL superclass
		void fillRectangleNoCheck(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);

		// required by the ScreenHAL superclass
		void drawCharNoCheck(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, fontDefinition_t *fontDef, uint8_t fontScale, boolean overlay = false, boolean grabAndReleaseBus= true);

		// required by the ScreenHAL superclass
		void rotate(void);
		
		// required by the ScreenHAL superclass
		void drawPixelNoCheck(uint16_t x, uint16_t y, uint16_t color);
		
	private:
		void _setClippingRectangleNoCheck(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy);
		
		void _bufferScaledPixel(uint16_t *buffer, uint8_t x, uint8_t y, uint8_t unscaledBufferWidth, uint16_t color, uint8_t scale = 1);
		
		bool _charPixelIsOn(fontDefinition_t *fontDef, uint8_t *pattern, uint8_t l, uint8_t c); 
};

#endif