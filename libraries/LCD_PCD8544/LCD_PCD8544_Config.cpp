/*
 * LCD_PCD8544_Config - Interface with Nokia 5110 LCDs (or compatible) 
 *	[Philips PCD8544 controller].
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

#ifndef LCD_PCD8544_CONFIG
#define LCD_PCD8544_CONFIG

// ------ Software descriptions ---------
// no need to change these in most cases
// --------------------------------------

// number of user glyphs: from 1 to 127
// these are displayed when using ascii codes >= 128
#define LCD_PCD8544_MAX_USER_GLYPHS 4


// ------ Hardware descriptions ---------
// no need to change these in most cases
// --------------------------------------

// screen size
#define LCD_PCD8544_WIDTH 84
#define LCD_PCD8544_HEIGHT 48

// default pin allocations
#define LCD_PCD8544_SCLK 3
#define LCD_PCD8544_SDIN 4
#define LCD_PCD8544_DC 5
#define LCD_PCD8544_RESET 6
#define LCD_PCD8544_SCE 7

#endif