/*
 * Colors - Defines RGB565 colors
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
 
#ifndef COLORS_HPP
#define COLORS_HPP

#include <inttypes.h>

/*        NAME          RGB565      RGB      */

// fast colors
#define BLACK           0x0000      /* 000000 */
#define WHITE           0xFFFF      /* FFFFFF */
#define FAST_BLUE       0x5A5A      /*        */
#define FAST_RED        0xE8E8      /*        */
#define FAST_GREEN      0x2727      /*        */
#define F_LIGHT_GREY    0xDEDE      /* DEDAF6 */
#define F_GREY          0x9494      /* 9491A4 */

// normal colors
#define RED             0xF800      /* FF0000 */
#define GREEN           0x07E0      /* 00FF00 */
#define BLUE            0x001F      /* 0000FF */
#define CYAN            0x07FF      /* 00FFFF */
#define MAGENTA         0xF81F      /* FF00FF */
#define YELLOW          0xFFE0      /* FFFF00 */
#define LIGHT_GREY      0xCE79      /* CCCCCC */
#define VLIGHT_GREY     0xEF7D      /* EEEEEE */

#define GREY            0x9CD3      /* 999999 */
#define DARK_GREY       0x632C      /* 666666 */
#define ORANGE          0xFBE0      /* FF7F00 */
#define FUSCHIA         0xF80F      /* FF007F */
#define VIOLET          0x780F      /* 7F007F */
#define BROWN           0x83E0      /* 827F00 */
#define DARK_GREEN      0x03E0      /* 007F00 */
#define DARK_BLUE       0x000F      /* 00007F */
#define DARK_RED        0x7800      /* 7F0000 */
#define DARK_CYAN       0x03EF      /* 007F7F */

#define COLOR_565(r, g, b) ((((((uint16_t)(r)) & 0x0FF) >> 3) << 11) | (((((uint16_t)(g)) & 0x0FF) >> 2) << 5) | ((((uint16_t)(b)) & 0x0FF) >> 3))

#endif