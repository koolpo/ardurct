/*
 * ArduRCT_EventManager_Configuration - Manages events in a coordinated way
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

#ifndef ARDURCT_EVENT_MANAGER_CONFIGURATION
#define ARDURCT_EVENT_MANAGER_CONFIGURATION 1

// the lower the value, the better the reaction to switches and encoders
// a value betweeen 20 and 50 is recommended
#define EVENT_MANAGER_CYCLE 25

// number of samples to average while reading analog values
// the higher the value, the higher the dampening of change,
// value should be between 1 and 6
#define ANALOG_AVERAGING 2

// distance between 2 points to consider them equal
#define TOUCHPANEL_TOLERANCE 2
// touchpanel driver chip
#if !defined(TOUCHPANEL_AR1021) && !defined(TOUCHPANEL_AD7348)
#define TOUCHPANEL_AR1021 1
#endif
// comment out the following to disable matrix calibration
//#define TOUCHPANEL_MATRIX_CALIBRATION 1

// number of steps in one rotation of the encoder
// this is function of your hardware
#define ENCODER_STEPS 24

#endif
