/*
 * ArduRCT_EventManager - Manages events in a coordinated way
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

#ifndef ARDURCT_EVENTS
#define ARDURCT_EVENTS 1

// ----------------------------------------
// no need to modify the following defines  
// ----------------------------------------
#define EVENT_NO_VALUE 0xFF
#define EVENT_ANY_VALUE 0xFF
#define EVENT_ANY_CHANNEL 0xFF
#define EVENT_ANY_ID 0xFF

#define EVENT_CLASS_MASK 0xF0
    
#define EVENT_SYSTEM 0x10
#define EVENT_SYSTEM_TICK 0x11

#define EVENT_TIME 0x20    
#define EVENT_TIME_SECOND 0x22
#define EVENT_TIME_MINUTE 0x23
#define EVENT_TIME_HOUR 0x24
#define EVENT_TIME_DAY 0x25
#define EVENT_TIME_MONTH 0x26
#define EVENT_TIME_YEAR 0x27    
#define EVENT_TIME_ALARM 0x2F
    
#define EVENT_SWITCH 0x30    
#define EVENT_SWITCH_PRESSED 0x31
#define EVENT_SWITCH_RELEASED 0x32
#define EVENT_SWITCH_REPEATING 0x33

#define EVENT_TOUCHPANEL 0x40
#define EVENT_TOUCHPANEL_PRESSED 0x41
#define EVENT_TOUCHPANEL_REPEAT_PRESSED 0x42
#define EVENT_TOUCHPANEL_DRAGGED 0x43
#define EVENT_TOUCHPANEL_RELEASED 0x44
#define EVENT_TOUCHPANEL_CALIBRATION 0x45
    
#define EVENT_ANALOG 0x50
#define EVENT_ANALOG_DECREASE 0x51
#define EVENT_ANALOG_INCREASE 0x52

#define EVENT_ENCODER 0x60
#define EVENT_ENCODER_DECREASE 0x61
#define EVENT_ENCODER_INCREASE 0x62

#define TOUCHPANEL_NO_TOUCH -1

#define ANALOG_NO_VALUE 0xFFFF
#define ANALOG_RESOLUTION_16B 65536
#define ANALOG_RESOLUTION_12B 1024
#define ANALOG_RESOLUTION_8B 256
#define ANALOG_RESOLUTION_100 100
#define ANALOG_RESOLUTION_4B 16
#define ANALOG_HARDWARE_RESOLUTION ANALOG_RESOLUTION_12B

#define EVENT_STATE_DEBOUNCING_UP 0
#define EVENT_STATE_RELEASED 2
#define EVENT_STATE_IDLE 3
#define EVENT_STATE_DEBOUNCING_DOWN 4
#define EVENT_STATE_PRE_PRESSED 6
#define EVENT_STATE_PRESSED 7
#define EVENT_STATE_PRESSED_MOTIONLESS 8
#define EVENT_STATE_DRAGGED 9
#define EVENT_STATE_REPEATING 20
#define EVENT_STATE_FAST_REPEATING 80

#endif
