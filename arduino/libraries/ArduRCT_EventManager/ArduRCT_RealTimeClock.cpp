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

#include "ArduRCT_RealTimeClock.hpp"

#define RTC_MILLIS_TO_SEC 50

#define RTC_ALARM_MAX_SNOOZE    0x70
#define RTC_ALARM_STOPPED       (-1*RTC_ALARM_MAX_SNOOZE-1)
#define RTC_ALARM_RING_START    0x00
#define RTC_ALARM_RING_STARTED  0x01

#define RTC_TIME_SEPARATOR ':'

#define RTC_ALARM_MAX_RING 10

uint8_t rtc_dayOfWeekString[] = "SunMonTueWedThuFriSat";
uint8_t rtc_monthString[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

ArduRCT_RealTimeClock::ArduRCT_RealTimeClock() {
    for (int i=0; i<7; i++) {
        _alarm[i].hour = 0;
        _alarm[i].minute = 0;
    }
    setDateTime(2012, 01, 01, 0, 0, 0);        
}

ArduRCT_RealTimeClock::ArduRCT_RealTimeClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    for (int i=0; i<7; i++) {
        _alarm[i].hour = 0;
        _alarm[i].minute = 0;
    }
    setDateTime(year, month, day, hour, minute, second);
}

void ArduRCT_RealTimeClock::setDate(uint16_t year, uint8_t month, uint8_t day) {
    _year = year;
    _month = month % 13;
    if (_month == 0) _month = 1;
    _day = day % 32;
    if (_day == 0) _day = 1;
    int mld = getMonthLastDay(_year, _month);
    if (_day > mld) _day = mld;
    _dayOfWeek = getDayOfWeek(_year, _month, _day);
}

void ArduRCT_RealTimeClock::setTime(uint8_t hour, uint8_t minute, uint8_t second) {
    _hour = hour % 24;
    _minute = minute % 60;
    _second = second % 60;
    _timeDigit[RTC_HOUR10] = _hour / 10;
    _timeDigit[RTC_HOUR1] = _hour % 10;
    _timeDigit[RTC_MINUTE10] = _minute / 10;
    _timeDigit[RTC_MINUTE1] = _minute % 10;
    _timeDigit[RTC_SECOND10] = _second / 10;
    _timeDigit[RTC_SECOND1] = _second % 10;
    _nextSecond = millis() + RTC_MILLIS_TO_SEC;
    _alarmStatus = RTC_ALARM_STOPPED;
}


uint8_t ArduRCT_RealTimeClock::getDayOfWeek(uint16_t year, uint8_t month, uint8_t day) {
    uint8_t t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    uint16_t y = year;
    if (month < 3) y --;
    return (y + y/4 - y/100 + y/400 + t[month-1] + day) % 7;
}

void ArduRCT_RealTimeClock::setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    setDate(year, month, day);
    setTime(hour, minute, second);
}

bool ArduRCT_RealTimeClock::updateTime() {
    // check if we have to update the time
    if ((int32_t)(millis() - _nextSecond) < 0) return false;
    _nextSecond += RTC_MILLIS_TO_SEC;
    
    _timeDigit[RTC_SECOND1] ++;
    if (_timeDigit[RTC_SECOND1] > 9) {
        _timeDigit[RTC_SECOND1] = 0;
        _timeDigit[RTC_SECOND10] ++;
        if (_timeDigit[RTC_SECOND10] > 5) _timeDigit[RTC_SECOND10] = 0;
    }
    _second ++;
    if (_second < 60) return true;

    _second = 0;
    _timeDigit[RTC_MINUTE1] ++;
    if (_timeDigit[RTC_MINUTE1] > 9) {
        _timeDigit[RTC_MINUTE1] = 0;
        _timeDigit[RTC_MINUTE10] ++;
        if (_timeDigit[RTC_MINUTE10] > 5) _timeDigit[RTC_MINUTE10] = 0;
    }
    _minute ++;
    if (_alarmStatus != RTC_ALARM_STOPPED) _alarmStatus ++;
    if (_minute < 60) {
        _checkAlarms();
        return true;
    }

    _minute = 0;
    _hour ++;
    _timeDigit[RTC_HOUR1] ++;
    if (_timeDigit[RTC_HOUR1] > 9) {
        _timeDigit[RTC_HOUR1] = 0;
        _timeDigit[RTC_HOUR10] ++;
    }
    if (_hour < 24) {
        _checkAlarms();
        return true;
    }
    
    _hour = 0;
    _timeDigit[RTC_HOUR1] = 0;
    _timeDigit[RTC_HOUR10] = 0;
    _day ++;
    _dayOfWeek ++;
    if (_dayOfWeek > 6) _dayOfWeek = 0;
    _checkAlarms();
    if (_day <= getMonthLastDay(_year, _month)) return true;
            
    _day = 1;
    _month ++;
    if (_month <= 12) return true;
    
    _month = 1;
    _year ++;
    return true;
}

uint8_t ArduRCT_RealTimeClock::getMonthLastDay(uint8_t year, uint8_t month) {
    if ((month == JANUARY) || (month == MARCH) || (month == MAY) || (month == JULY) || (month == AUGUST) || 
            (month == OCTOBER) || (month == DECEMBER)) return 31;
    if ((month == APRIL) || (month == JUNE) || (month == SEPTEMBER) || (month == NOVEMBER)) return 30;
    if (((year % 100) == 0) && ((year / 100) != 4)) return 28;    // centuries are not leap years unless divided by 400
    if ((year % 4) != 0) return 28;
    return 29;
}

void ArduRCT_RealTimeClock::setAlarmTime(uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t turnOn) {
    if (dayOfWeek > 6) return;
    _alarm[dayOfWeek].hour = (hour % 24) + (turnOn != 0 ? RTC_ALARM_ON : 0);
    _alarm[dayOfWeek].minute = (minute % 60);
}
        
rtcAlarm_t *ArduRCT_RealTimeClock::getAlarmTime(uint8_t dayOfWeek) {
    if (dayOfWeek > 6) return 0;
    return &_alarm[dayOfWeek];
}
        
void ArduRCT_RealTimeClock::setAlarmOn(uint8_t dayOfWeek) {
    if (dayOfWeek > 6) return;
    _alarm[dayOfWeek].hour |= RTC_ALARM_ON;
}

bool ArduRCT_RealTimeClock::isAlarmOn(uint8_t dayOfWeek) {
    return ((_alarm[dayOfWeek].hour & RTC_ALARM_ON) != 0);
}

void ArduRCT_RealTimeClock::setAlarmOff(uint8_t dayOfWeek) {
    if (dayOfWeek > 6) return;
    _alarm[dayOfWeek].hour &= ~RTC_ALARM_ON;
}

void ArduRCT_RealTimeClock::stopAlarm() {
    _alarmStatus = RTC_ALARM_STOPPED;
}
                
void ArduRCT_RealTimeClock::snoozeAlarm(uint8_t minutes) {
    if ((_alarmStatus == RTC_ALARM_STOPPED) || (_alarmStatus < RTC_ALARM_RING_START)) return;
    _alarmStatus = -minutes;
}
        
bool ArduRCT_RealTimeClock::isAlarmRinging() {
    return (_alarmStatus >= RTC_ALARM_RING_START);
}

bool ArduRCT_RealTimeClock::isAlarmStartingToRing() {
    if (_alarmStatus != RTC_ALARM_RING_START) return false;
    _alarmStatus = RTC_ALARM_RING_STARTED;
    return true;
}

bool ArduRCT_RealTimeClock::isAlarmSnoozing() {
    return ((_alarmStatus != RTC_ALARM_STOPPED) && (_alarmStatus < RTC_ALARM_RING_START));
}


uint8_t ArduRCT_RealTimeClock::getTimeDigit(uint8_t digit) {
    if (digit > RTC_SECOND10) return 0xFF;
    return _timeDigit[digit];
}

char *ArduRCT_RealTimeClock::getDayOfWeekAsString() {
    return getDayOfWeekAsString(_dayOfWeek);
}

char *ArduRCT_RealTimeClock::getDayOfWeekAsString(uint8_t dow) {
    for (int i=0; i<3; i++) _buffer[i] = rtc_dayOfWeekString[dow*3+i];
    _buffer[3] = 0;
    return _buffer;
}

char *ArduRCT_RealTimeClock::getDateAsString(uint16_t year, uint8_t month, uint8_t day, bool withDayOfWeek) {
    for (uint8_t i=0; i<(withDayOfWeek ? 15 : 11); i++) _buffer[i] = ' ';
    _buffer[withDayOfWeek ? 15 : 11] = 0;
    uint8_t i = 0;
    if (withDayOfWeek) {
        uint8_t dayOfWeek = getDayOfWeek(year, month, day);
        for (int j=0; j<3; j++) _buffer[j] = rtc_dayOfWeekString[dayOfWeek*3+j];
        i = 4;
    }
    if (day > 10) _buffer[i++] = (day / 10) + '0';
    else _buffer[i++] = ' ';
    _buffer[i++] = (day % 10) + '0';
    i++;
    for (uint8_t j=0; j<3; j++) _buffer[i+j] = rtc_monthString[(month-1)*3+j];
    i += 4;
    if (year < 0) {
        _buffer[i++] = '-';
        year = -year;
    }
    boolean yearStart = false;
    if (year > 1000) {
        _buffer[i++] = (year / 1000) + '0';
        year -= (year / 1000) * 1000;
        yearStart = true;
    }
    if ((year > 100) || yearStart) {
        _buffer[i++] = (year / 100) + '0';
        year -= (year / 100) * 100;
    }
    if ((year > 10) || yearStart) _buffer[i++] = (year / 10) + '0';
    _buffer[i++] = (year % 10) + '0';
    return _buffer;
    
}

char *ArduRCT_RealTimeClock::getDateAsString(bool withDayOfWeek) {
    return getDateAsString(_year, _month, _day, withDayOfWeek);
}

char *ArduRCT_RealTimeClock::getTimeAsString(bool withSeconds) {
    uint8_t j = 0;
    for (uint8_t i=0; i<6; i++) {
        _buffer[j++] = _timeDigit[i] + '0';
        if ((i == 1) || (i == 4)) _buffer[j++] = RTC_TIME_SEPARATOR;
    }
    if (withSeconds) _buffer[j] = 0;
    else _buffer[5] = 0;
    return _buffer;
}

char *ArduRCT_RealTimeClock::getAlarmTimeAsString(uint8_t dayOfWeek) {
    if (dayOfWeek > 6) return 0;
    uint8_t hour = (_alarm[dayOfWeek].hour & ~RTC_ALARM_ON);
    if (hour / 10 > 0) _buffer[0] = hour/10 + '0';
    else _buffer[0] = ' ';
    _buffer[1] = (hour % 10) + '0';
    _buffer[2] = RTC_TIME_SEPARATOR;
    _buffer[3] = (_alarm[dayOfWeek].minute / 10) + '0';
    _buffer[4] = (_alarm[dayOfWeek].minute % 10) + '0';
    _buffer[5] = 0;
    return _buffer;
}

uint16_t ArduRCT_RealTimeClock::getYear() {
    return _year;
}

uint8_t ArduRCT_RealTimeClock::getMonth() {
    return _month;
}

uint8_t ArduRCT_RealTimeClock::getDay() {
    return _day;
}

uint8_t ArduRCT_RealTimeClock::getDayOfWeek() {
    return _dayOfWeek;
}

uint8_t ArduRCT_RealTimeClock::getHour() {
    return _hour;
}

uint8_t ArduRCT_RealTimeClock::getMinute() {
    return _minute;
}

uint8_t ArduRCT_RealTimeClock::getSecond() {
    return _second;
}

void ArduRCT_RealTimeClock::_checkAlarms() {
    if (_alarmStatus > RTC_ALARM_MAX_RING) _alarmStatus = RTC_ALARM_STOPPED;
    if ((_alarm[_dayOfWeek].hour & RTC_ALARM_ON) == 0) return;
    if ((_alarm[_dayOfWeek].hour & ~RTC_ALARM_ON) != _hour) return;
    if (_alarm[_dayOfWeek].minute != _minute) return;
    _alarmStatus = RTC_ALARM_RING_START;
}