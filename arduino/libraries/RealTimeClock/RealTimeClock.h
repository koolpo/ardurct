/*
 * RealTimeClock - A class to maintain time
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

#ifndef REAL_TIME_CLOCK_HPP
#define REAL_TIME_CLOCK_HPP 1

#include <Arduino.h>

#define RTC_HOUR10    0
#define RTC_HOUR1     1
#define RTC_MINUTE10  2
#define RTC_MINUTE1   3
#define RTC_SECOND10  4
#define RTC_SECOND1   5

#define RTC_TIME_SEPARATOR ':'

#define MONDAY      0
#define TUESDAY     1
#define WEDNESDAY   2
#define THURSDAY    3
#define FRIDAY      4
#define SATURDAY    5
#define SUNDAY      6

#define JANUARY     1
#define FEBRUARY    2
#define MARCH       3
#define APRIL       4
#define MAY         5
#define JUNE        6
#define JULY        7
#define AUGUST      8
#define SEPTEMBER   9
#define OCTOBER     10
#define NOVEMBER    11
#define DECEMBER    12

#define RTC_WITH_DAY_OF_WEEK true
#define RTC_WITHOUT_DAY_OF_WEEK false
#define RTC_WITH_SECONDS true
#define RTC_WITHOUT_SECONDS false

#define RTC_ALARM_ON 0x80
#define RTC_ALARM_OFF 0x00

#define RTC_ALARM_MAX_RING 10

typedef struct {
    uint8_t hour;
    uint8_t minute;
} rtcAlarm_t;

class RealTimeClock {

    public:
        RealTimeClock(int16_t year, uint8_t month, uint8_t day, uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t second = 0);
        
        void set(int16_t year, uint8_t month, uint8_t day, uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t second = 0);
        
        // should be called any time you want, but at least once per second
        // this updates the time and returns true if anything has changed
        bool update();
        
        void setAlarmTime(uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t turnOn = RTC_ALARM_OFF);
        
        rtcAlarm_t *getAlarmTime(uint8_t dayOfWeek);
        
        void setAlarmOn(uint8_t dayOfWeek);

        void setAlarmOff(uint8_t dayOfWeek);
        
		bool isAlarmOn(uint8_t dayOfWeek);
		
        void stopAlarm();

        void snoozeAlarm(uint8_t minutes);
        
        bool isAlarmRinging();
        
        bool isAlarmSnoozing();
        
        uint16_t getYear();
        uint8_t getMonth();
        uint8_t getDay();
        uint8_t getDayOfWeek();
        uint8_t getHour();
        uint8_t getMinute();
        uint8_t getSecond();
        
        uint8_t getTimeDigit(uint8_t digit);
        
        char *getDayOfWeekAsString();
        char *getDateAsString(bool withDayOfWeek = true);
        char *getTimeAsString(bool withSecond = true);
        char *getAlarmTimeAsString(uint8_t dayOfWeek);
        
    private:
        uint8_t _buffer[16];
        uint8_t _timeDigit[6];
        int16_t _year;
        uint8_t _month;
        uint8_t _day;
        uint8_t _dayOfWeek;
        uint8_t _hour;
        uint8_t _minute;
        uint8_t _second;
        uint32_t _nextSecond;
        rtcAlarm_t _alarm[7];    // one alarm per day of week
        int16_t _alarmStatus;
        
        void _checkAlarms();
};

#endif