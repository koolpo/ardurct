#include "S6D04H0.hpp"

#define S6D04H0_CASET	0x2a
#define S6D04H0_PASET	0x2b
#define S6D04H0_RAMWR	0x2c
#define S6D04H0_RAMRD	0x2e
#define S6D04H0_MADCTL	0x36

#define S6D04H0_R0		0x08
#define S6D04H0_R90		0x38
#define S6D04H0_R180	0x68
#define S6D04H0_R270	0x58

const unsigned char PROGMEM S6D04H0_initialization_code[] = {
	0, 	0x11,			/* SLPOFF: SLeeP OFF */
	0xFE,				/* delay(100) */
	0, 	0x13,			/* NORON: NORmal mode ON */
	0xFE,				/* delay(100) */
	5, 	0xf3, 0x01, 0xff, 0x1f, 0x00, 0x03,
	17, 0xf2, 0x28, 0x60, 0x7f, 0x08, 0x08, 0x00, 0x00, 0x15, 0x48, 0x00, 0x07, 0x01, 0x00, 0x00, 0x94, 0x08, 0x08,
	20, 0xf4, 0x09, 0x00, 0x00, 0x00, 0x21, 0x47, 0x01, 0x02, 0x2A, 0x5D, 0x07, 0x2A, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	12, 0xf5, 0x00, 0x3c, 0x30, 0x00, 0x00, 0x12, 0x00, 0x00, 0x0d, 0x01, 0x00, 0x00,
	9, 	0xf6, 0x01, 0x01, 0x07, 0x00, 0x02, 0x0c, 0x02, 0x08, 0x03,
	4,	0xf7, 0x01, 0x00, 0x10, 0x00,
	3,	0xf8, 0x99, 0x00, 0x00,
	1,	0xf9, 0x01,
	1, 	0x26, 0x02,		/* GASET: Gamma Set */
	1, 	0x35, 0x00,		/* TEON: Tearing on */
	1,	0x36, 0x00,		/* MADCTL: Memory ADdress ConTroL, B7,B6,B5=0 => no rotation, B3=0 => no BGR filter */
	1, 	0x3a, 0x55, 	/* COLMOD: COLor MODe: 16 bits/pixel */
	4,	0x2a, 0x00, 0x00, 0x00, 0xef,	/* CASET: Column Address SET */
	4,	0x2b, 0x00, 0x00, 0x01, 0x3f, 	/* PASET:Page Address SET */
	0,	0x29, 			/* DISPON:	DISplay ON */
	0,	0x2c,			/* RAMWR: RAM WRite */
	0xFE,				/* delay(100) */
	0xFF				/* End initialization */
};


S6D04H0::S6D04H0() {	
}
 
/* ---------------- Protected functions ------------------------ */

void S6D04H0::initScreenImpl(void) {
	uint8_t buffer[32];
	uint16_t i = 0;
	
	// init the screen
	while (1) {
		memcpy_P(buffer, &(S6D04H0_initialization_code[i]), 32);
		if (buffer[0] == 0xFF) break;
		if (buffer[0] == 0xFE) delay(100);
		else {
			writeCommand(buffer[1]);
			for (uint8_t j=0; j<buffer[0]; j++) writeData(buffer[2+j]);
			i += buffer[0]+1;
		}
		i++;
	}
}

void S6D04H0::drawPixelImpl(uint16_t x, uint16_t y, uint16_t color) {
	writeCommand(S6D04H0_CASET);
	write16bData(x);
	write16bData(x);
	writeCommand(S6D04H0_PASET);
	write16bData(y);
	write16bData(y);	
	writeCommand(S6D04H0_RAMWR);
	write16bData(color);
}


// Fill the rectangle lx,ly -> hx, hy
// this functions assumes that lx <= hx and ly <= hy
void S6D04H0::fillRectangleImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
	uint32_t nbPixels = (hx-lx+1)*(hy-ly+1);
	_setClippingRectangle(lx, hx, ly, hy);
	writeCommand(S6D04H0_RAMWR);
	while (nbPixels-- > 0) write16bData(color);
}


void S6D04H0::drawBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool hasTransparency, uint16_t transparentColor) {
	uint16_t buffer[SCREENHAL_MAX_BITMAP_SIZE];
	
	_setClippingRectangle(x, y, x+width, y+width); 
	if (hasTransparency) {
		// avoid buffer overflows...
		if (width * height > SCREENHAL_MAX_BITMAP_SIZE) return;
		writeCommand(S6D04H0_RAMRD);
		read16bDataBuffer(buffer, width * height);
		for (uint16_t i=0; i<width*height; i++) {
			if (bitmap[i] != transparentColor) buffer[i] = bitmap[i];
		}
	}
	writeCommand(S6D04H0_RAMWR);
	if (hasTransparency) write16bDataBuffer(buffer, width * height);
	else write16bDataBuffer(bitmap, width * height);
}


void S6D04H0::setRotationImpl() {
	if (_rotation == SCREEN_ROTATION_0) {
		writeCommand(S6D04H0_MADCTL);
		writeData(S6D04H0_R0);
	} else if (_rotation == SCREEN_ROTATION_90) {
		writeCommand(S6D04H0_MADCTL);
		writeData(S6D04H0_R90);
	} else if (_rotation == SCREEN_ROTATION_180) {
		writeCommand(S6D04H0_MADCTL);
		writeData(S6D04H0_R90);		
	} else if (_rotation == SCREEN_ROTATION_270) {
		writeCommand(S6D04H0_MADCTL);
		writeData(S6D04H0_R90);
	}
}

uint16_t S6D04H0::getWidthImpl() {
	return S6D04H0_WIDTH;
}


uint16_t S6D04H0::getHeightImpl() {
	return S6D04H0_HEIGHT;
}

/* ---------------- Private functions -------------------------- */

// define the zone we are going to write to
void S6D04H0::_setClippingRectangle(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy) {
	writeCommand(S6D04H0_CASET);
	write16bData(lx);
	write16bData(hx);
	writeCommand(S6D04H0_PASET);
	write16bData(ly);
	write16bData(hy);
}

