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
 
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

// Hardware
// --------
#define CONFIGURATION_V1_CS 	10
#define CONFIGURATION_V1_CD 	9
#define CONFIGURATION_V1_RST 	8 
#define CONFIGURATION_V1_BL 	5 

#define CONFIGURATION_V2_CS 	0xFF
#define CONFIGURATION_V2_CD		21 
#define CONFIGURATION_V2_WR 	22
#define CONFIGURATION_V2_RD 	23
#define CONFIGURATION_V2_RST 	0xFF
#define CONFIGURATION_V2_PORT 	2
#define CONFIGURATION_V2_BL 	5 

#define CONFIGURATION_SPI_IS_SHARED 1

// Software
// --------
// Max space that a pattern or a bitmap that needs to be overlaid can occupy
// minimum is FONT_MAX_SPACE (from font.hpp)
// RAM taken can be up to 5 times this, so beware! 
#define CONFIGURATION_MAX_BITMAP_SPACE (9*14)		

// if defined, the screen will try to 
#define CONFIGURATION_12B_COLORS_ALLOWED 1

// Touch panel
// -----------
// max delta in measures for points to be considered equal 
#define CONFIGURATION_TOUCHPANEL_DISTANCE_EQUAL 4
// number of measures averaged to define the point, 2 or 3 is fine
#define CONFIGURATION_TOUCHPANEL_NB_MEASURES 2
// resistance of the X plane
#define CONFIGURATION_TOUCHPANEL_X_PLANE_RESISTANCE 300
// from which value the pressure is considered valid, not less than 1
#define CONFIGURATION_TOUCHPANEL_PRESSURE_THRESHOLD 10
// to which value the pressure is considered valid
#define CONFIGURATION_TOUCHPANEL_PRESSURE_MAX 2000

// the address in the eeprom where the calibration matrix is stored 
// 25 bytes are taken in descending order
#define CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_DONE 		E2END
#define CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_A 		(E2END-4)
#define CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_B 		(E2END-8)
#define CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_X_DIVIDER 	(E2END-12)
#define CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_A 		(E2END-16)
#define CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_B 		(E2END-20)
#define CONFIGURATION_EEPROM_ADDRESS_TOUCHPANEL_CALIBRATION_Y_DIVIDER 	(E2END-24)

// User Interface
// --------------
// Comment this if you don't want to use the pre-configured user interface
#define CONFIGURATION_HAS_UI 1
// minimum time in milliseconds separating the handling of the ui (used for debouncing)
#define CONFIGURATION_UI_LOOP_LENGTH 25
// how many times do we debounce
#define CONFIGURATION_UI_DEBOUNCE_STEPS 6
// max number of tabs for the UI
#define CONFIGURATION_UI_MAX_TABS 6
// max number of elements for all the tabs
#define CONFIGURATION_UI_MAX_ELEMENTS 30
// style of the UI: 1 is finger, commented out is pen
#define CONFIGURATION_UI_STYLE_FINGER 1

// Macros
//-------
// Comment this if you don't want to use macros
#define CONFIGURATION_HAS_MACROS 1
// the number of macros defined in eeprom
// each compressed macro can be up to SCREEN_MACRO_MAX_SIZE character long
// the eeprom taken will be SCREEN_MACRO_MAX_NUMBER * SCREEN_MACRO_MAX_SIZE bytes
#define CONFIGURATION_MACRO_MAX_NUMBER 16
// this number can not be bigger than 255
#define CONFIGURATION_MACRO_MAX_SIZE 128


#endif