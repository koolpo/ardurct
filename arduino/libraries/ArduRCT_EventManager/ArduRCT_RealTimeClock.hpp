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

// comment this line if your setup never has a MCP7941X
#define RTC_MCP7941X   1

#define RTC_MCP7941X_YEAR_OFFSET    2000
#define RTC_EXTERNAL_MCP7941X       true
#define RTC_INTERNAL                false

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

#define RTC_DATE_STRING_DOW 0
#define RTC_DATE_STRING_DAY 4
#define RTC_DATE_STRING_MONTH 7
#define RTC_DATE_STRING_YEAR 11

typedef struct {
    uint8_t hour;
    uint8_t minute;
} rtcAlarm_t;

class ArduRCT_RealTimeClock {

    public:
        ArduRCT_RealTimeClock(bool hasMCP7941x = false);
        ArduRCT_RealTimeClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, bool hasMCP7941x = false);
        
        void useEEPROMToStoreAlarms(uint16_t eepromAddress);
        
        void setDate(uint16_t year, uint8_t month, uint8_t day);
        
        void setTime(uint8_t hour, uint8_t minute, uint8_t second);
                
        void setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
        
        bool updateTime();
        
        uint8_t getMonthLastDay(uint8_t year, uint8_t month);

        uint8_t getDOW(uint16_t year, uint8_t month, uint8_t day);
        
        void setDOWAlarmTime(uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t turnOn);
        
        rtcAlarm_t *getDOWAlarmTime(uint8_t dayOfWeek);
        
        void setDOWAlarmOn(uint8_t dayOfWeek);
        
        bool isDOWAlarmOn(uint8_t dayOfWeek);
        
        void setDOWAlarmOff(uint8_t dayOfWeek);
        
        void stopAlarm();
        
        void snoozeAlarm(uint8_t minutes);
        
        bool isAlarmRinging();
        
        bool isAlarmStartingToRing();
        
        bool isAlarmSnoozing();
        
        uint8_t getTimeDigit(uint8_t digit);
        
        uint8_t getDOWAlarmDigit(uint8_t dayOfWeek, uint8_t digit);
        
        char *getDOWAsString();
        
        char *getDOWAsString(uint8_t dow);
        
        char *getDateAsString(uint16_t year, uint8_t month, uint8_t day, bool withDayOfWeek = false, char separator = ' ');
        
        char *getDateAsString(bool withDayOfWeek = false);
        
        char *getTimeAsString(bool withSeconds = false);
        
        char *getDOWAlarmTimeAsString(uint8_t dayOfWeek, bool withDayOfWeek = false);
        
        char *getMonthAsString(uint8_t month);
        
        uint16_t getYear();
        
        uint8_t getMonth();
        
        uint8_t getDay();
        
        uint8_t getDOW();
        
        uint8_t getHour();
        
        uint8_t getMinute();
        
        uint8_t getSecond();
        
#ifdef RTC_MCP7941X
        uint8_t decToBcd(uint8_t val);
        
        uint8_t bcdToDec(uint8_t val);
        
        void setHasMCP7941x(bool hasMCP7941x = false);
#endif
        
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
        rtcAlarm_t _alarm[7];
        char _buffer[18];
        uint16_t _dayOfWeekAlarmsEEPROMAddress;

#ifdef RTC_MCP7941X
        uint8_t _mcp7941xStatus;

        bool _getMCP7941xTime();
#endif
        void _checkAlarms();
};

#endif
