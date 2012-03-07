/*
 * UserInterface - User Interface elements
 *	Part of Touchscreen class
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
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
 
#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP 1

#include <inttypes.h>

#define UI_COLOR_BACKGROUND WHITE
#define UI_COLOR_TAB_UNSELECTED LIGHT_GREY
#define UI_COLOR_BUTTON_PUSHED 0x9494
#define UI_COLOR_BUTTON_RELEASED LIGHT_GREY
#define UI_COLOR_GAUGE 0x7C7C

#define UI_TAB_FONT FONT_MEDIUM
#define UI_TAB_FONT_IS_BOLD true
#define UI_TAB_TOP_MARGIN 6
#define UI_TAB_LEFT_MARGIN 7

#define UI_ELEMENT_FONT FONT_MEDIUM
#define UI_ELEMENT_FONT_IS_BOLD true

#define UI_ELEMENT_TOP_MARGIN 5
#define UI_ELEMENT_LEFT_MARGIN 8

#define UI_SLIDER_KNOB_SIZE 12

#define UI_PUSHED 1
#define UI_RELEASED 0

#define UI_ERROR 0

#define UI_AUTO_SIZE 0
#define UI_SELECTED 1
#define UI_UNSELECTED 0
#define UI_NO_GROUP -1

#define UI_SAME_AS 0x4000
#define UI_WITH_MARGIN 0x0800
#define UI_LEFT_OF 0x2000
#define UI_RIGHT_OF 0x1000
#define UI_RIGHT_OF_WITH_MARGIN 0x1800
#define UI_TOP_OF 0x2000
#define UI_BOTTOM_OF 0x1000
#define UI_BOTTOM_OF_WITH_MARGIN 0x1800

#define UI_PLACE_MODIFIER_MASK 0x7800

typedef struct {
	uint8_t *text;
	int16_t x;
	int16_t y;
	uint16_t width;
	void (*drawCallback)(uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value);
	void (*handleCallback)(uint8_t id);
} touchScreen_UITab_t;

typedef struct {
	uint8_t type;
	uint8_t tab;
	uint8_t id;
	int8_t group;
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
	uint8_t *text;
	int16_t value;
	int16_t min;
	int16_t max;
	bool editable;
} touchScreen_UIElement_t;

#endif
