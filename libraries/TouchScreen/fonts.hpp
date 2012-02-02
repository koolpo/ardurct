#ifndef FONTS_HPP
#define FONTS_HPP

#include <avr/pgmspace.h>

#define FONT_SMALL 	1
#define FONT_MEDIUM 2
#define FONT_BIG 3
#define FONT_HUGE 4

#define FONT_PLAIN false
#define FONT_BOLD true

#define FONT_LAST_DEF 2					/* last definition */

#define FONT_MAX_SPACE		(9*14)		/* width x height of the biggest font including the charSpacing and lineSpacing */
#define FONT_MAX_PATTERN	12			/* max bytes one character definition can take */

/** Display fill direction
 *
 *  0 (HORIZONTAL)
 *     --------/
 *     --------/
 *
 *  1 (VERTICAL)
 *     | /|
 *     |/ |
 *
 **/

typedef struct {
	uint8_t width;
	uint8_t height;
	uint8_t charSpacing;
	uint8_t lineSpacing;
	uint8_t firstChar;
	uint8_t lastChar;
	uint8_t orientation;
} fontDefinition_t;

extern fontDefinition_t fontDefinition_small;
extern fontDefinition_t fontDefinition_medium;
extern const unsigned char PROGMEM font_small[][5];
extern const unsigned char PROGMEM font_medium[][12];

#endif