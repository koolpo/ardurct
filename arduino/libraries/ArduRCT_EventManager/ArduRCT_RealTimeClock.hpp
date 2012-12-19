/*
 * RealTimeClock - Emulates a Real Time Clock in software
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
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

#ifndef ARDURCT_RTC
#define ARDURCT_RTC 1

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <inttypes.h>

#define RTC_HOUR10     0
#define RTC_HOUR1      1
#define RTC_MINUTE10   2
#define RTC_MINUTE1    3
#define RTC_SECOND10   4
#define RTC_SECOND1    5

#define SUNDAY    0
#define MONDAY    1
#define TUESDAY   2
#define WEDNESDAY 3
#define THURSDAY  4
#define FRIDAY    5
#define SATURDAY  6

#define JANUARY   1
#define FEBRUARY  2
#define MARCH     3
#define APRIL     4
#define MAY       5
#define JUNE      6
#define JULY      7
#define AUGUST    8
#define SEPTEMBER 9
#define OCTOBER   10
#define NOVEMBER  11
#define DECEMBER  12

#define RTC_WITH_DAY_OF_WEEK true
#define RTC_WITHOUT_DAY_OF_WEEK false
#define RTC_WITH_SECONDS true
#define RTC_WITHOUT_SECONDS false

#define RTC_ALARM_ON 0x80
#define RTC_ALARM_OFF 0x00

typedef struct {
    uint8_t hour;
    uint8_t minute;
} rtcAlarm_t;

class ArduRCT_RealTimeClock {

    public:
        ArduRCT_RealTimeClock();
        ArduRCT_RealTimeClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
        
        void setDate(uint16_t year, uint8_t month, uint8_t day);
        
        void setTime(uint8_t hour, uint8_t minute, uint8_t second);
        
        uint8_t getDayOfWeek(uint16_t year, uint8_t month, uint8_t day);
        
        void setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
        
        bool updateTime();
        
        uint8_t getMonthLastDay(uint8_t year, uint8_t month);
        
        void setAlarmTime(uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t turnOn);
        
        rtcAlarm_t *getAlarmTime(uint8_t dayOfWeek);
        
        void setAlarmOn(uint8_t dayOfWeek);
        
        bool isAlarmOn(uint8_t dayOfWeek);
        
        void setAlarmOff(uint8_t dayOfWeek);
        
        void stopAlarm();
        
        void snoozeAlarm(uint8_t minutes);
        
        bool isAlarmRinging();
        
        bool isAlarmStartingToRing();
        
        bool isAlarmSnoozing();
        
        uint8_t getTimeDigit(uint8_t digit);
        
        char *getDayOfWeekAsString();
        
        char *getDayOfWeekAsString(uint8_t dow);
        
        char *getDateAsString(uint16_t year, uint8_t month, uint8_t day, bool withDayOfWeek);
        
        char *getDateAsString(bool withDayOfWeek);
        
        char *getTimeAsString(bool withSeconds);
        
        char *getAlarmTimeAsString(uint8_t dayOfWeek);
        
        uint16_t getYear();
        
        uint8_t getMonth();
        
        uint8_t getDay();
        
        uint8_t getDayOfWeek();
        
        uint8_t getHour();
        
        uint8_t getMinute();
        
        uint8_t getSecond();
        
    private:
        uint16_t _year;
        uint8_t _month;
        uint8_t _day;
        uint8_t _dayOfWeek;
        uint8_t _hour;
        uint8_t _minute;
        uint8_t _second;
        uint32_t _nextSecond;
        int8_t _alarmStatus;
        uint8_t _timeDigit[6];
        rtcAlarm_t _alarm[7];
        char _buffer[18];

        void _checkAlarms();
};

#endif
