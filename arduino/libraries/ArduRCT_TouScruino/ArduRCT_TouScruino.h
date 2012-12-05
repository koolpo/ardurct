/*
 * TouScruino
 *
 * Copyright (c) 2012 Laurent Wibaux <lm.wibaux@gmail.com>
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

/* 
 * Versions
 *	v0.1	Initial release
 */
 
#ifndef TOUSCRUINO_H
#define TOUSCRUINO_H 1

#if (TOUSCRUINO_VERSION == 1)
/**
 *	This version is based on:
 *		- an SPI screen from Adafruit
 *		- a 5 ways switch from Adafruit
 **/

#include <ArduRCT_TouScruinoV1.hpp>

ArduRCT_TouScruinoV1 tft(CONFIGURATION_V1_CD, CONFIGURATION_V1_CS, CONFIGURATION_V1_RST, CONFIGURATION_V1_BL);

#elif (TOUSCRUINO_VERSION == 2)
/**
 *	This version is based on:
 *		- a parallel screen
 *		- a touch panel
 **/
#include <ArduRCT_TouScruinoV2.hpp>

ArduRCT_TouScruinoV2 tft(CONFIGURATION_V2_PORT, CONFIGURATION_V2_CD, CONFIGURATION_V2_WR, CONFIGURATION_V2_RD, CONFIGURATION_V2_CS, CONFIGURATION_V2_RST, CONFIGURATION_V2_BL);

#else
#error TOUSCRUINO_VERSION needs to be defined before #include <ArduRCT_TouScruino.h>
#endif

#endif