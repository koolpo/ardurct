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

#include "ArduRCT_Events.hpp"
#include "ArduRCT_RealTimeClock.hpp"
#include "ArduRCT_Switch.hpp"
#include "ArduRCT_Analog.hpp"
#include "ArduRCT_Encoder.hpp"
#include "ArduRCT_EventHandler.hpp"
#include "ArduRCT_TouchPanel.hpp"

class ArduRCT_EventManager {
    
    public:
        ArduRCT_EventManager();

        ArduRCT_EventManager(ArduRCT_RealTimeClock *rtc);
        
        void setRTC(ArduRCT_RealTimeClock *rtc);
        
        ArduRCT_RealTimeClock *getRTC();
        
        void registerSwitch(ArduRCT_Switch *aSwitch);

        void registerEncoder(ArduRCT_Encoder *anEncoder);
        
        void registerAnalog(ArduRCT_Analog *anAnalog);
        
        void registerTouchPanel(ArduRCT_TouchPanel *touchPanel);
        
        void registerEventHandler(ArduRCT_EventHandler *handler);
        
        void manageEvents();
        
        int8_t processEvent(uint8_t type, uint8_t value);
        
        int8_t processEvent(uint8_t type, uint8_t value, uint16_t x, uint16_t y);

    private:
        uint32_t _nextUpdate;
        uint8_t _tick;
        ArduRCT_RealTimeClock *_rtc;
        ArduRCT_Switch *_switch;
        ArduRCT_Encoder *_encoder;
        ArduRCT_Analog *_analog;
        ArduRCT_TouchPanel *_touchPanel;
        ArduRCT_EventHandler *_handler;

        void _updateTime();
};

#endif
