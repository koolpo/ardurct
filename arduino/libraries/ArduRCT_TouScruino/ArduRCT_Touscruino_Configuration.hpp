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
 
#ifndef ARDURCT_TOUSCRUINO_CONFIGURATION_HPP
#define ARDURCT_TOUSCRUINO_CONFIGURATION_HPP

// Hardware
// --------

// V1
#define TOUSCRUINO_V1_UP     17         /* Up button pin */
#define TOUSCRUINO_V1_DOWN   16         /* Down button pin */
#define TOUSCRUINO_V1_LEFT   21         /* Left button pin */
#define TOUSCRUINO_V1_CENTER 14         /* Center button pin */
#define TOUSCRUINO_V1_RIGHT  15         /* Right button pin */
#define TOUSCRUINO_V1_CD_PIN 22
#define TOUSCRUINO_V1_CS_PIN 18
#define TOUSCRUINO_V1_RESET_PIN 23
#define TOUSCRUINO_V1_BACKLIGHT_PIN 5

// V2

//#define TOUSCRUINO_V2_TFT_CHIP ArduRCT_ILI9340
#define TOUSCRUINO_V2_TFT_CHIP ArduRCT_SPFD5408

#define TOUSCRUINO_V2_TP_INT        30   /* Interrupt pin for the TouchPanel */
#define TOUSCRUINO_V2_WIDTH         240  /* Width of the screen */
#define TOUSCRUINO_V2_HEIGHT        320  /* Height of the screen */
#define TOUSCRUINO_V2_DRAG_TRIGGER  3    /* Touch points are considered equal if no more distant than */
#define TOUSCRUINO_V2_TP_XP         24   /* X+ pin for the TouchPanel */
#define TOUSCRUINO_V2_TP_XM         26   /* X- pin for the TouchPanel */
#define TOUSCRUINO_V2_TP_YP         33   /* Y+ pin for the TouchPanel */
#define TOUSCRUINO_V2_TP_YM         31   /* Y- pin for the TouchPanel */

#ifdef __AVR__
#define TOUSCRUINO_V2_WR_PIN        23
#define TOUSCRUINO_V2_RD_PIN        21
#define TOUSCRUINO_V2_CD_PIN        22
#define TOUSCRUINO_V2_CS_PIN        31
#define TOUSCRUINO_V2_RESET_PIN     0xFF
#define TOUSCRUINO_V2_SPI_ON_BUS    true
#define TOUSCRUINO_V2_BACKLIGHT_PIN 5
// Data bus on port B
#endif

#if defined(__arm__) && defined(CORE_TEENSY)
#define TOUSCRUINO_V2_WR_PIN        29
#define TOUSCRUINO_V2_RD_PIN        27
#define TOUSCRUINO_V2_CD_PIN        28
#define TOUSCRUINO_V2_CS_PIN        0xFF
#define TOUSCRUINO_V2_RESET_PIN     30
#define TOUSCRUINO_V2_SPI_ON_BUS    false
#define TOUSCRUINO_V2_BACKLIGHT_PIN 32
// Data bus on port D
#endif

// Software
// --------
#define TOUSCRUINO_UP CONFIGURATION_V1_UP
#define TOUSCRUINO_DOWN CONFIGURATION_V1_DOWN
#define TOUSCRUINO_MENU CONFIGURATION_V1_LEFT
#define TOUSCRUINO_LEFT CONFIGURATION_V1_LEFT
#define TOUSCRUINO_ENTER CONFIGURATION_V1_CENTER
#define TOUSCRUINO_CENTER CONFIGURATION_V1_CENTER
#define TOUSCRUINO_RIGHT CONFIGURATION_V1_RIGHT

// the address in the eeprom where the calibration matrix is stored 
// 25 bytes are taken in descending order
// set to 0xFFFF to prevent calibration and to E2END to calibrate
#define TOUSCRUINO_EEPROM_TOUCHPANEL    (E2END-1)

#endif