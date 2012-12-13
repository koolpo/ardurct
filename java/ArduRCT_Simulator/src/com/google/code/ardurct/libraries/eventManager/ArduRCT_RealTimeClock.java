package com.google.code.ardurct.libraries.eventManager;

import java.util.Date;

class rtcAlarm_t {
	int hour = 0;
	int minute = 0;
}

public class ArduRCT_RealTimeClock  
implements IEventDefines {
	
	public final static int RTC_MILLIS_TO_SEC = 50;

	private final static int RTC_ALARM_MAX_SNOOZE = 0x7E;
	private final static int RTC_ALARM_STOPPED = (-RTC_ALARM_MAX_SNOOZE-1);
	private final static int RTC_ALARM_RING_START = 0x00;
	private final static int RTC_ALARM_RING_STARTED = 0x01;

	private final static int RTC_TIME_SEPARATOR = ':';

	public final static int RTC_ALARM_MAX_RING = 10;
	
	String rtc_dayOfWeekString = "MonTueWedThuFriSatSun";
	String rtc_monthString = "JanFebMarAprMayJunJulAugSepOctNovDec";

	int _year, _month, _day, _dayOfWeek, _hour, _minute, _second;
	long _nextSecond;
	int _alarmStatus;
	int _timeDigit[] = new int[6];
	rtcAlarm_t _alarm[] = new rtcAlarm_t[7];
	int _buffer[] = new int[16];
	
	protected long millis() {
		return (new Date()).getTime();
	}

	public ArduRCT_RealTimeClock() {
		for (int i=0; i<7; i++) _alarm[i] = new rtcAlarm_t();
	    setTime(2012, 01, 01, SUNDAY, 0, 0, 0);		
	}
	
	public ArduRCT_RealTimeClock(int year, int month, int day, int dayOfWeek, int hour, int minute, int second) {
		for (int i=0; i<7; i++) _alarm[i] = new rtcAlarm_t();
	    setTime(year, month, day, dayOfWeek, hour, minute, second);
	}

	public void setTime(int year, int month, int day, int dayOfWeek, int hour, int minute, int second) {
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

	public boolean updateTime() {
	    // check if we have to update the time
	    if ((millis() - _nextSecond) < 0) return false;
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

	public void setAlarmTime(int dayOfWeek, int hour, int minute, int turnOn) {
	    if (dayOfWeek > 6) return;
	    _alarm[dayOfWeek].hour = (hour % 24) + (turnOn != 0 ? RTC_ALARM_ON : 0);
	    _alarm[dayOfWeek].minute = (minute % 60);
	}
	        
	public rtcAlarm_t getAlarmTime(int dayOfWeek) {
	    if (dayOfWeek > 6) return null;
	    return _alarm[dayOfWeek];
	}
	        
	public void setAlarmOn(int dayOfWeek) {
	    if (dayOfWeek > 6) return;
	    _alarm[dayOfWeek].hour |= RTC_ALARM_ON;
	}

	public boolean isAlarmOn(int dayOfWeek) {
		return ((_alarm[dayOfWeek].hour & RTC_ALARM_ON) != 0);
	}

	public void setAlarmOff(int dayOfWeek) {
	    if (dayOfWeek > 6) return;
	    _alarm[dayOfWeek].hour &= ~RTC_ALARM_ON;
	}

	public void stopAlarm() {
	    _alarmStatus = RTC_ALARM_STOPPED;
	}
	                
	public void snoozeAlarm(int minutes) {
		if ((_alarmStatus == RTC_ALARM_STOPPED) || (_alarmStatus < RTC_ALARM_RING_START)) return;
	    _alarmStatus = -minutes;
	}
	        
	public boolean isAlarmRinging() {
	    return (_alarmStatus >= RTC_ALARM_RING_START);
	}

	public boolean isAlarmStartingToRing() {
		if (_alarmStatus != RTC_ALARM_RING_START) return false;
		_alarmStatus = RTC_ALARM_RING_STARTED;
		return true;
	}

	public boolean isAlarmSnoozing() {
	    return ((_alarmStatus != RTC_ALARM_STOPPED) && (_alarmStatus < RTC_ALARM_RING_START));
	}


	public int getTimeDigit(int digit) {
	    if (digit > RTC_SECOND10) return 0xFF;
	    return _timeDigit[digit];
	}

	public int[] getDayOfWeekAsString() {
		for (int i=0; i<3; i++) _buffer[i] = rtc_dayOfWeekString.charAt(_dayOfWeek*3+i);
		_buffer[3] = 0;
	    return _buffer;
	}


	public int[] getDateAsString(boolean withDayOfWeek) {
	    for (int i=0; i<(withDayOfWeek ? 15 : 11); i++) _buffer[i] = ' ';
		_buffer[withDayOfWeek ? 15 : 11] = 0;
		int i = 0;
	    if (withDayOfWeek) {
	    	for (int j=0; j<3; j++) _buffer[j] = rtc_dayOfWeekString.charAt(_dayOfWeek*3+j);
	        i = 4;
	    }
	    if (_day > 10) _buffer[i++] = (_day / 10) + '0';
	    _buffer[i++] = (_day % 10) + '0';
		i++;
    	for (int j=0; j<3; j++) _buffer[i+j] = rtc_monthString.charAt((_month-1)*3+j);
	    i += 4;
	    int year = _year;
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


	public int[] getTimeAsString(boolean withSeconds) {
	    int j = 0;
	    for (int i=0; i<6; i++) {
	        _buffer[j++] = _timeDigit[i] + '0';
	        if ((i == 1) || (i == 4)) _buffer[j++] = RTC_TIME_SEPARATOR;
	    }
	    if (withSeconds) _buffer[j] = 0;
	    else _buffer[5] = 0;
	    return _buffer;
	}

	public int[] getAlarmTimeAsString(int dayOfWeek) {
	    if (dayOfWeek > 6) return null;
		int hour = (_alarm[dayOfWeek].hour & ~RTC_ALARM_ON);
	    if (hour / 10 > 0) _buffer[0] = hour/10 + '0';
	    else _buffer[0] = ' ';
	    _buffer[1] = (hour % 10) + '0';
	    _buffer[2] = RTC_TIME_SEPARATOR;
	    _buffer[3] = (_alarm[dayOfWeek].minute / 10) + '0';
	    _buffer[4] = (_alarm[dayOfWeek].minute % 10) + '0';
	    _buffer[5] = 0;
	    return _buffer;
	}

	int getYear() {
	    return _year;
	}

	int getMonth() {
	    return _month;
	}

	public int getDay() {
	    return _day;
	}

	public int getDayOfWeek() {
	    return _dayOfWeek;
	}

	int getHour() {
	    return _hour;
	}

	int getMinute() {
	    return _minute;
	}

	public int getSecond() {
	    return _second;
	}

	void _checkAlarms() {
		if (_alarmStatus > RTC_ALARM_MAX_RING) _alarmStatus = RTC_ALARM_STOPPED;
	    if ((_alarm[_dayOfWeek].hour & RTC_ALARM_ON) == 0) return;
	    if ((_alarm[_dayOfWeek].hour & ~RTC_ALARM_ON) != _hour) return;
	    if (_alarm[_dayOfWeek].minute != _minute) return;
	    _alarmStatus = RTC_ALARM_RING_START;
	}
}
