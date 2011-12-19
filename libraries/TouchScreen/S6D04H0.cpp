#include "S6D04H0.hpp"
#include "font_5x7.hpp"

#define S6D04H0_CASET	0x2a
#define S6D04H0_PASET	0x2b
#define S6D04H0_RAMWR	0x2c
#define S6D04H0_RAMRD	0x2e
#define S6D04H0_MADCTL	0x36

#define S6D04H0_R0		0x08
#define S6D04H0_R90		0x38
#define S6D04H0_R180	0x68
#define S6D04H0_R270	0x58


const PROGMEM unsigned char S6D04H0_initialization_code[] {
	0, 	0x11,			// SLPOFF
	0xFF,
	0, 	0x13,			// NORON
	0xFF,
	5, 	0xf3, 0x01, 0xff, 0x1f, 0x00, 0x03,
	17, 0xf2, 0x28, 0x60, 0x7f, 0x08, 0x08, 0x00, 0x00, 0x15, 0x48, 0x00, 0x07, 0x01, 0x00, 0x00, 0x94, 0x08, 0x08,
	20, 0xf4, 0x09, 0x00, 0x00, 0x00, 0x21, 0x47, 0x01, 0x02, 0x2A, 0x5D, 0x07, 0x2A, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	12, 0xf5, 0x00, 0x3c, 0x30, 0x00, 0x00, 0x12, 0x00, 0x00, 0x0d, 0x01, 0x00, 0x00,
	9, 	0xf6, 0x01, 0x01, 0x07, 0x00, 0x02, 0x0c, 0x02, 0x08, 0x03,
	4,	0xf7, 0x01, 0x00, 0x10, 0x00,
	3,	0xf8, 0x99, 0x00, 0x00,
	1,	0xf9, 0x01,
	1, 	0x26, 0x02,		// Gamma Set
	1, 	0x35, 0x00,		// TEON: Tearing on
	1,	0x36, 0x08,		// MADCTL : Memory Data Access Control, B7=0 => , B3=1 => BGR filter
	1, 	0x3a, 0x55, 	// COLMOD: 16 bits/pixel
	4,	0x2a, 0x00, 0x00, 0x00, 0xef,	// CASET: Column Address Set
	4,	0x2b, 0x00, 0x00, 0x01, 0x3f, 	// PASET:Page Address Set
	0,	0x29, 			// DISPON:	Display On
	0,	0x2c,			// RAMWR: Memory write
	0xFF
}

S6D04H0::S6D04H0(uint8_t port, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset) {	
	setupScreen(port, cd, wr, rd, cs, reset);
}
 

void S6D04H0::initScreen() {
	uint8_t buffer[32];
	
	// init the screen
	grabBus();
	for (uint8_t i=0; i<sizeof(S6D04H0_initialization_code); i++) {
		memcpy_P(buffer, &S6D04H0_initialization_code[i], 32);
		if (buffer[0] == 0xFF) delay(100);
		else {
			writeCommand(buffer[1]);
			for (uint8_t j=0; j<buffer[0]; j++) writeData(buffer[2+j]);
			i += buffer[0]+1;
		}
	}
	releaseBus();
}


void S6D04H0::rotate() {
	if (_rotation == SCREEN_ROTATE_0) {
		writeCommand(S6D04H0_MADCTL);
		writeData(S6D04H0_R0);
	} else if (_rotation == SCREEN_ROTATE_90) {
		writeCommand(S6D04H0_MADCTL);
		writeData(S6D04H0_R90);
	} else if (_rotation == SCREEN_ROTATE_180) {
		writeCommand(S6D04H0_MADCTL);
		writeData(S6D04H0_R90);		
	} else if (_rotation == SCREEN_ROTATE_270) {
		writeCommand(S6D04H0_MADCTL);
		writeData(S6D04H0_R90);
	}
}

void S6D04H0::drawChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, fontDefinition_t *fontDef, uint8_t fontScale, boolean overlay, boolean grabAndRelease) {
	uint8_t pattern[FONT_MAX_PATTERN];
	uint16_t buffer[FONT_MAX_SPACE*4];

	uint16_t charSpaceUsed = (fDef->width+fDef->charSpacing)*(fDef->height+fDef->lineSpacing)*scale*scale;
	if (grabAndRelaseBus) grabBus();
	clipRectangleNoCheck(x, y, x+(fDef->width+fDef->charSpacing)*scale, y+(fDef->height+fDef->lineSpacing)*scale); 
	if (overlay) {
		writeCommand(S6D04H0_RAMRD);
		read16bDataBuffer(buffer, charSpaceUsed);
	}
	if (fs == 1) memcpy_P(pattern, &font_small[chr - fDef->firstChar], fDef->orientation == 0 ? fDef->height : fDef->width);
	else if (fs == 2) memcpy_P(pattern, &font_medium[chr - fDef->firstChar], fDef->orientation == 0 ? fDef->height : fDef->width);
	if (!overlay) {
		// prepare the horizontal line at bottom
		for (uint8_t l=0; l<fDef->lineSpacing; l++) {
			for (uint8_t c=0; c<fDef->width+fDef->charSpacing; c++) bufferCharPixel(buffer, c, fDef->height+l, fDef->width+fDef->charSpacing, _backgroundColor, scale);
		}
		// prepare the vertical line at right
		for (uint8_t c=0; c<fDef->charSpacing; c++) {
			for (uint8_t l=0; l<8; l++) bufferCharPixel(buffer, fDef->width+c, l, fDef->width+fDef->charSpacing, _backgroundColor, scale);
		}
	}
	// prepare the dots
	for (uint8_t l=0; l<7; l++) {
		for (uint8_t c=0; c<5; c++) {
			if (charPixelIsOn(fDef, pattern, l, c)) bufferCharPixel(buffer, c, l, fDef->width+fDef->charSpacing, color, scale);
			else if (!overlay) bufferCharPixel(buffer, c, l, fDef->width+fDef->charSpacing, _backgroundColor, fs, scale);
		}
	}
	// draw the char 
	writeCommand(S6D04H0_RAMWR);
	write16bDataBuffer(buffer, charSpaceUsed);
	if (grabAndRelaseBus) releaseBus();
}


void S6D04H0::drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	grabBus();
	fillRectangleNoCheck(x, y, x, y, color);
	releaseBus();
}

// Fill the rectangle lx,ly -> hx, hy
// this functions assumes that lx <= hx and ly <= hy
void S6D04H0::fillRectangleNoCheck(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
	uint32_t nbPixels = (hx-lx+1)*(hy-ly+1);
	setClippingRectangleNoCheck(lx, hx, ly, hy);
	writeCommand(S6D04H0_RAMWR);
	while (nbPixels-- > 0) write16bData(color);
}

/* ---------------- Private functions -------------------------- */

// define the zone we are going to write to
void S6D04H0::setClippingRectangleNoCheck(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy) {
	writeCommand(S6D04H0_CASET);
	write16bData(lx);
	write16bData(hx);
	writeCommand(S6D04H0_PASET);
	write16bData(ly);
	write16bData(hy);
}


void S6D04H0::bufferCharPixel(char *buffer, uint8_t x, uint8_t y, uint8_t fontTotalWidth, uint16_t color, uint8_t scale) {
	if (scale == 1) {
		buffer[y*fontTotalWidth+x] = color;
		return;
	}
	for (uint8_t l=0; l<scale; l++) {
		for (uint8_t c=0; c<scale; c++) {
			buffer[(y*scale+l)*fontTotalWidth+x*scale+c] = color;
		}
	}
}


bool S6DD040:charPixelIsOn(fontDefinition_t *fDef, uint8_t *pattern, uint8_t l, uint8_t c) {
	if (fDef->orientation == HORIZONTAL) return (pattern[l*(1+fDef->width/8)+c/8] & (0x01 << (c & 0x07));
	else return (pattern[c*(1+fDef->height/8)+l/8] & (0x01 << (l & 0x07));
}