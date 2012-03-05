/*
 * TouchScreen_Macros - Macro language to draw on the screen
 *	Part of Touchscreen class
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
 
#ifndef TOUCHSCREEN_MACROS_HPP
#define TOUCHSCREEN_MACROS_HPP 1

#include <inttypes.h>

#define SCREEN_MACRO_CMD_NONE						0x00
#define SCREEN_MACRO_CMD_PRESET_FOREGROUND			0x11
#define SCREEN_MACRO_CMD_PRESET_BACKGROUND			0x12
#define SCREEN_MACRO_CMD_PRESET_FONT				0x13
#define SCREEN_MACRO_CMD_PRESET_THICKNESS			0x14
#define SCREEN_MACRO_CMD_PRESET_SCALE				0x15
#define SCREEN_MACRO_CMD_PRESET_ERASE				0x16
#define SCREEN_MACRO_CMD_LINE 						0x20
#define SCREEN_MACRO_CMD_LINE_DELTA 				0x21
#define SCREEN_MACRO_CMD_ARC 						0x30
#define SCREEN_MACRO_CMD_ARC_FILLED 				0x31
#define SCREEN_MACRO_CMD_ARC_REVERSED				0x32
#define SCREEN_MACRO_CMD_ARC_FILLED_REVERSED		0x33
#define SCREEN_MACRO_CMD_RECTANGLE					0x40
#define SCREEN_MACRO_CMD_RECTANGLE_FILLED			0x41
#define SCREEN_MACRO_CMD_RECTANGLE_ROUNDED			0x42
#define SCREEN_MACRO_CMD_RECTANGLE_ROUNDED_FILLED	0x43
#define SCREEN_MACRO_CMD_TRIANGLE					0x50
#define SCREEN_MACRO_CMD_TRIANGLE_FILLED			0x51
#define SCREEN_MACRO_CMD_CIRCLE						0x60
#define SCREEN_MACRO_CMD_CIRCLE_FILLED				0x61
#define SCREEN_MACRO_CMD_STRING						0x70
#define SCREEN_MACRO_CMD_WRITE						0xE0
#define SCREEN_MACRO_CMD_EXECUTE					0xF0
#define SCREEN_MACRO_CMD_EXECUTE_WITH_RESET			0xF1

typedef struct {
	uint8_t cmd;
	uint8_t nbParams;
	int16_t param[6];
	uint16_t color;
	uint8_t *text;
	uint8_t textLen;
} touchScreen_macroCommand_t;

#endif