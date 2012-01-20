#ifndef S6D0H0_HPP
#define S6D0H0_HPP

#include "ScreenHAL.hpp"

#define S6D04H0_WIDTH 240
#define S6D04H0_HEIGHT 320

class S6D04H0: public ScreenHAL {
    
	public:
		S6D04H0();
		
	protected:
		// required by the ScreenHAL superclass
		void initScreenImpl(void);
		
		// required by the ScreenHAL superclass
		void fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color);

		// required by the ScreenHAL superclass
		void retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

		// required by the ScreenHAL superclass
		void pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

		// required by the ScreenHAL superclass
		void setRotationImpl(uint8_t rotation);
		
		// required by the ScreenHAL superclass
		void drawPixelImpl(uint16_t x, uint16_t y, uint16_t color);

		// required by the ScreenHAL superclass
		uint16_t getWidthImpl();

		// required by the ScreenHAL superclass
		uint16_t getHeightImpl();
		
	private:
		void _setClippingRectangle(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy);
};

#endif