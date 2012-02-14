/*
 * Fonts - definitions for fonts
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#ifndef FONTS_HPP
#define FONTS_HPP

#include <avr/pgmspace.h>

#define FONT_SMALL 	1
#define FONT_MEDIUM 2
#define FONT_BIG 3
#define FONT_HUGE 4
#define FONT_BIGGER 5
#define FONT_BIGGEST 6

#define FONT_PLAIN false
#define FONT_BOLD true

#define FONT_LAST_DEF 2					/* last definition */

#define FONT_MAX_SPACE		(9*14)		/* width x height of the biggest font including the charSpacing and lineSpacing */
#define FONT_MAX_PATTERN	12			/* max bytes one character definition can take */

/** Display fill orientation
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
extern const unsigned char PROGMEM font_big[][1];

#endif