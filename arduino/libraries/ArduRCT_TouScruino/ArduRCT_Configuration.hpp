/*
 * Configuration - Configures the default options for software and hardware
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
 
#ifndef ARDURCT_CONFIGURATION_HPP
#define ARDURCT_CONFIGURATION_HPP

// Hardware
// --------

// V1
#define CONFIGURATION_V1_CS     10
#define CONFIGURATION_V1_CD     9
#define CONFIGURATION_V1_RST    8 
#define CONFIGURATION_V1_BL     5 
#define CONFIGURATION_V1_UP     17
#define CONFIGURATION_V1_DOWN   16
#define CONFIGURATION_V1_LEFT   21
#define CONFIGURATION_V1_CENTER 14
#define CONFIGURATION_V1_RIGHT  15

// V2
#define CONFIGURATION_V2_CS     0xFF
#define CONFIGURATION_V2_CD     21 
#define CONFIGURATION_V2_WR     22
#define CONFIGURATION_V2_RD     23
#define CONFIGURATION_V2_RST    0xFF
#define CONFIGURATION_V2_PORT   2
#define CONFIGURATION_V2_BL     5 

#endif