#ifndef S6D0H0_H
#define S6D0H0_H

#include "ScreenHAL.hpp"

class S6D0H0: public ScreenHAL {
    
	public:
		S6D0H0(uint8_t port = PB, uint8_t cd = 21, uint8_t wr = 22, uint8_t rd = 23, uint8_t cs = 0xFF, uint8_t reset = 0xFF);

		// required by the ScreenHAL superclass
		void initScreen(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset);
	
		// required by the ScreenHAL superclass
		void drawChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, fontDefinition_t *fontDef, uint8_t fontScale, boolean overlay, boolean grabAndRelease);

		// required by the ScreenHAL superclass
		void drawPixel(uint16_t x, uint16_t y, uint16_t color);
		
		// required by the ScreenHAL superclass
		void fillRectangleNoCheck(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);
	
	protected:
		// required by the ScreenHAL superclass
		void rotate();
		
	private:
		void setClippingRectangleNoCheck(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy);
		
		void bufferCharPixel(char *buffer, uint8_t x, uint8_t y, uint8_t fontTotalWidth, uint16_t color, uint8_t scale = 1);
		
		bool charPixelIsOn(fontDefinition_t *fDef, uint8_t *pattern, uint8_t l, uint8_t c); 
}

#endif