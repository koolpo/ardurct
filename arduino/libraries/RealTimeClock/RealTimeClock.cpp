/*
 * Time - A class to maintain time
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

#include "RealTimeClock.h"
#include <avr/pgmspace.h>
#include <inttypes.h>

#define RTC_ALARM_MAX_SNOOZE 0xFF	/* uint8_t ... */
#define RTC_ALARM_STOPPED (-RTC_ALARM_MAX_SNOOZE-1)
#define RTC_ALARM_RING_START 0x01

#define RTC_MILLIS_TO_SEC 50

const unsigned char PROGMEM rtc_dayOfWeekString[] = "MonTueWedThuFriSatSun";
const unsigned char PROGMEM rtc_monthString[] = "JanFebMarAprMayJunJulAugSepOctNovDec";


RealTimeClock::RealTimeClock(int16_t year, uint8_t month, uint8_t day, uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t second) {
    set(year, month, day, dayOfWeek, hour, minute, second);
}

void RealTimeClock::set(int16_t year, uint8_t month, uint8_t day, uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t second) {
    _year = year;
    _month = month % 13;
    if (_month == 0) _month = 1;
    _day = day % 32;
    if (_day == 0) _day = 1;
    _dayOfWeek = dayOfWeek % 7;
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

bool RealTimeClock::update() {
    // check if we have to update the time
    if ((int32_t)(millis() - _nextSecond) < 0) return false;
    _nextSecond = millis() + RTC_MILLIS_TO_SEC;
    
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
    if (_day <= 28) {
        _checkAlarms();
        return true;
    }
    
    if (_month == FEBRUARY) {
        // allow 29 days for leap years
        if (_day <= 29) {
            if ((_year % 100) == 0) {                        // if a century, year is not a leap year
                if (((_year/100) % 4) == 0) return true;     //    unless the century can be divided by 4
            } else {                                         // else 
                if ((_year % 4) == 0) return true;           // else if year can be divided by 4, year is a leap year
            }
        }        
    } else if ((_month == APRIL) || (_month == JUNE) || (_month == SEPTEMBER) || (_month == NOVEMBER)) {
        if (_day <= 30) return true;
    } else { // JANUARY, MARCH, MAY, JULY, AUGUST, OCTOBER, DECEMBER
        if (_day <= 31) return true;
    }
    
    _day = 1;
    _month ++;
    if (_month <= 12) return true;
    
    _month = 1;
    _year ++;
	return true;
}

void RealTimeClock::setAlarmTime(uint8_t dayOfWeek, uint8_t hour, uint8_t minute, uint8_t turnOn) {
    if (dayOfWeek > 6) return;
    _alarm[dayOfWeek].hour = (hour % 24) + (turnOn != 0 ? RTC_ALARM_ON : 0);
    _alarm[dayOfWeek].minute = (minute % 60);
}
        
rtcAlarm_t *RealTimeClock::getAlarmTime(uint8_t dayOfWeek) {
    if (dayOfWeek > 6) return 0;
    return &_alarm[dayOfWeek];
}
        
void RealTimeClock::setAlarmOn(uint8_t dayOfWeek) {
    if (dayOfWeek > 6) return;
    _alarm[dayOfWeek].hour |= RTC_ALARM_ON;
}

bool RealTimeClock::isAlarmOn(uint8_t dayOfWeek) {
	return ((_alarm[dayOfWeek].hour & RTC_ALARM_ON) != 0);
}

void RealTimeClock::setAlarmOff(uint8_t dayOfWeek) {
    if (dayOfWeek > 6) return;
    _alarm[dayOfWeek].hour &= ~RTC_ALARM_ON;
}

void RealTimeClock::stopAlarm() {
    _alarmStatus = RTC_ALARM_STOPPED;
}
                
void RealTimeClock::snoozeAlarm(uint8_t minutes) {
 	if ((_alarmStatus == RTC_ALARM_STOPPED) || (_alarmStatus < RTC_ALARM_RING_START)) return;
   _alarmStatus = -minutes;
}
        
bool RealTimeClock::isAlarmRinging() {
    return (_alarmStatus >= RTC_ALARM_RING_START);
}

bool RealTimeClock::isAlarmSnoozing() {
    return ((_alarmStatus != RTC_ALARM_STOPPED) && (_alarmStatus < RTC_ALARM_RING_START));
}


uint8_t RealTimeClock::getTimeDigit(uint8_t digit) {
    if (digit > RTC_SECOND10) return 0xFF;
    return _timeDigit[digit];
}

char *RealTimeClock::getDayOfWeekAsString() {
    memcpy_P(_buffer, &(rtc_dayOfWeekString[_dayOfWeek*3]), 3);
    _buffer[4] = 0;
    return (char *)_buffer;
}


char *RealTimeClock::getDateAsString(bool withDayOfWeek) {
    for (int i=0; i<(withDayOfWeek ? 15 : 11); i++) _buffer[i] = ' ';
	_buffer[withDayOfWeek ? 15 : 11] = 0;
	uint8_t i = 0;
    if (withDayOfWeek) {
        memcpy_P(_buffer, &(rtc_dayOfWeekString[_dayOfWeek*3]), 3);
        i = 4;
    }
    if (_day > 10) _buffer[i++] = (_day / 10) + '0';
    _buffer[i++] = (_day % 10) + '0';
	i++;
    memcpy_P(&_buffer[i], &(rtc_monthString[(_month-1)*3]), 3);
    i += 4;
    int16_t year = _year;
    if (year < 0) {
        _buffer[i++] = '-';
        year = -year;
    }
    bool yearStart = false;
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
    return (char *)_buffer;
}


char *RealTimeClock::getTimeAsString(bool withSeconds) {
    uint8_t j = 0;
    for (uint8_t i=0; i<6; i++) {
        _buffer[j++] = _timeDigit[i] + '0';
        if ((i == 1) || (i == 4)) _buffer[j++] = RTC_TIME_SEPARATOR;
    }
    if (withSeconds) _buffer[j] = 0;
    else _buffer[5] = 0;
    return (char *)_buffer;
}

char *RealTimeClock::getAlarmTimeAsString(uint8_t dayOfWeek) {
    if (dayOfWeek > 6) return 0;
	uint8_t hour = (_alarm[dayOfWeek].hour & ~RTC_ALARM_ON);
    if (hour / 10 > 0) _buffer[0] = hour/10 + '0';
    else _buffer[0] = ' ';
    _buffer[1] = (hour % 10) + '0';
    _buffer[2] = RTC_TIME_SEPARATOR;
    _buffer[3] = (_alarm[dayOfWeek].minute / 10) + '0';
    _buffer[4] = (_alarm[dayOfWeek].minute % 10) + '0';
    _buffer[5] = 0;
    return (char *)_buffer;
}

uint16_t RealTimeClock::getYear() {
    return _year;
}

uint8_t RealTimeClock::getMonth() {
    return _month;
}

uint8_t RealTimeClock::getDay() {
    return _day;
}

uint8_t RealTimeClock::getDayOfWeek() {
    return _dayOfWeek;
}

uint8_t RealTimeClock::getHour() {
    return _hour;
}

uint8_t RealTimeClock::getMinute() {
    return _minute;
}

uint8_t RealTimeClock::getSecond() {
    return _second;
}

void RealTimeClock::_checkAlarms() {
	if (_alarmStatus > RTC_ALARM_MAX_RING) _alarmStatus = RTC_ALARM_STOPPED;
    if ((_alarm[_dayOfWeek].hour & RTC_ALARM_ON) == 0) return;
    if ((_alarm[_dayOfWeek].hour & ~RTC_ALARM_ON) != _hour) return;
    if (_alarm[_dayOfWeek].minute != _minute) return;
    _alarmStatus = RTC_ALARM_RING_START;
}