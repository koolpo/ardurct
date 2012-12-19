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

#ifndef ARDURCT_EVENT_MANAGER
#define ARDURCT_EVENT_MANAGER 1

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <inttypes.h>

#include "ArduRCT_RealTimeClock.hpp"
#include "ArduRCT_Switch.hpp"
#include "ArduRCT_EventHandler.hpp"
#include "ArduRCT_TouchPanel.hpp"

// the lower the value, the better the reaction to switches
#define EVENT_MANAGER_CYCLE 25
  
#define EVENT_NO_VALUE 0xFF
#define EVENT_ANY_VALUE 0xFF

#define EVENT_CLASS_MASK 0xFF
    
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
#define EVENT_TOUCHPANEL_DRAGGED 0x42    
#define EVENT_TOUCHPANEL_RELEASED 0x43    
    
#define TOUCHPANEL_NO_TOUCH 0xFFFF
#define TOUCHPANEL_TOLERANCE 2
    
#define EVENT_STATE_DEBOUNCING_UP 0
#define EVENT_STATE_RELEASED 2
#define EVENT_STATE_IDLE 3
#define EVENT_STATE_DEBOUNCING_DOWN 4
#define EVENT_STATE_PRESSED 6
#define EVENT_STATE_PRESSED_MOTIONLESS 7
#define EVENT_STATE_DRAGGED 8
#define EVENT_STATE_REPEATING 20
#define EVENT_STATE_FAST_REPEATING 80

class ArduRCT_EventManager {
    
    public:
        ArduRCT_EventManager();

        ArduRCT_EventManager(ArduRCT_RealTimeClock *rtc);
        
        void setRTC(ArduRCT_RealTimeClock *rtc);
        
        ArduRCT_RealTimeClock *getRTC();
        
        void registerSwitch(ArduRCT_Switch *aSwitch);
        
        void registerTouchPanel(ArduRCT_TouchPanel *touchPanel);
        
        void registerEventHandler(ArduRCT_EventHandler *handler);
        
        void update();
                
        bool addEvent(uint8_t type, uint8_t value);
                
        bool addEvent(uint8_t type, uint8_t value, uint16_t x, uint16_t y);

    private:
        uint32_t _nextUpdate;
        uint8_t _tick;
        ArduRCT_Switch *_switch;
        ArduRCT_EventHandler *_handler;
        ArduRCT_RealTimeClock *_rtc;
        ArduRCT_TouchPanel *_touchPanel;

        void _updateTime();
};

#endif
