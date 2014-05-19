/*
 * TouchPanel - Manages a touch panel 
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
 
#ifndef ARDURCT_TOUCH_PANEL
#define ARDURCT_TOUCH_PANEL 1

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <inttypes.h>
#include "ArduRCT_EventManager_Configuration.hpp"

#define TOUCHPANEL_ROTATION_0   0
#define TOUCHPANEL_ROTATION_90  1
#define TOUCHPANEL_ROTATION_180 2
#define TOUCHPANEL_ROTATION_270 3

#define TOUCHPANEL_NOT_INITIALIZED                  0
#define TOUCHPANEL_CALIBRATION_REQUEST_CROSS_1      1
#define TOUCHPANEL_CALIBRATION_WAIT_FOR_PENDOWN_1   11
#define TOUCHPANEL_CALIBRATION_REQUEST_CROSS_2      2
#define TOUCHPANEL_CALIBRATION_WAIT_FOR_PENDOWN_2   12
#define TOUCHPANEL_CALIBRATION_REQUEST_CROSS_3      3
#define TOUCHPANEL_CALIBRATION_WAIT_FOR_PENDOWN_3   13
#define TOUCHPANEL_CALIBRATION_PREPARE_SAVE         4
#define TOUCHPANEL_CALIBRATION_SAVE                 5
#define TOUCHPANEL_CALIBRATED                       0x55
#define TOUCHPANEL_NOT_CALIBRATED                   0xFF

typedef struct {
	int32_t a;
	int32_t b;
	int32_t divider;
} tsCalibrationEquation_t;

class ArduRCT_TouchPanel {
	
	public:
	
#ifdef TOUCHPANEL_MATRIX_CALIBRATION
		ArduRCT_TouchPanel(uint8_t interruptPin = TOUCHPANEL_EVENT_PIN, uint8_t dragTrigger = TOUCHPANEL_DRAG_TRIGGER, 
                uint16_t width = TOUCHPANEL_WIDTH, uint16_t height = TOUCHPANEL_HEIGHT, uint16_t calibrationMatrixEepromAddress = 0xFFFF);

		ArduRCT_TouchPanel(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint8_t dragTrigger = TOUCHPANEL_DRAG_TRIGGER, 
                uint16_t width = TOUCHPANEL_WIDTH, uint16_t height = TOUCHPANEL_HEIGHT, uint16_t calibrationMatrixEepromAddress = 0xFFFF);
#else
		ArduRCT_TouchPanel(uint8_t interruptPin, uint8_t dragTrigger, uint16_t width, uint16_t height);

		ArduRCT_TouchPanel(uint8_t xp, uint8_t xm, uint8_t yp, uint8_t ym, uint8_t dragTrigger, uint16_t width, uint16_t height);
#endif
		int16_t getPenXYZ(int16_t *x = 0, int16_t *y = 0, int16_t *z = 0);
		
		uint8_t update();
		
		bool isPenPressed();
		
		bool isPenPressedMotionless();
		
		bool isPenReleased();
		
		bool isPenDragged();
		
		int16_t getPenX();
		
		int16_t getPenY();
		
		int16_t getPenZ();

        void setRotation(uint8_t rotation);

#ifdef TOUCHPANEL_MATRIX_CALIBRATION
        uint8_t getCalibrationCrossNumber();

        bool isCalibrating();
        
        void getCalibrationCrossXY(uint8_t number, uint16_t *x, uint16_t *y);
        
        void deleteCalibration();
#endif

    private:
		uint8_t _state;
        uint8_t _rotation;
		int16_t _penX;
		int16_t _penY;
		int16_t _penZ;
        int16_t _touchX;
        int16_t _touchY;
        int16_t _touchZ;
        uint8_t _calibrationStatus;
        uint8_t _interruptPin;
        uint8_t _dragTrigger;
        uint16_t _width;
        uint16_t _height;
   		uint8_t _xm;
		uint8_t _xp;
		uint8_t _ym;
		uint8_t _yp;

#ifdef TOUCHPANEL_MATRIX_CALIBRATION
        uint16_t _calibrationMatrixEepromAddress;
        tsCalibrationEquation_t _xCalibrationEquation;
        tsCalibrationEquation_t _yCalibrationEquation;
        uint16_t _calibrationX[3];
        uint16_t _calibrationY[3];
#endif
		int16_t _getTouchX();
		
		int16_t _getTouchY();
		
		int16_t _getTouchZ();
        
        boolean _adjustTouchWithCalibration();

        uint8_t _calibrate();
};

#endif
