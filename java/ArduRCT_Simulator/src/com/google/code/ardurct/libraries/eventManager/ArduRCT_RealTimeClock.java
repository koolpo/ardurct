package com.google.code.ardurct.libraries.eventManager;

import java.util.Date;


public class ArduRCT_RealTimeClock  
implements IEventDefines {

	public class rtcAlarm_t {
		public int hour = 0;
		public int minute = 0;
	}
	
	public final static int RTC_MILLIS_TO_SEC = 1000;

	private final static int RTC_ALARM_MAX_SNOOZE = 0x7E;
	private final static int RTC_ALARM_STOPPED = (-RTC_ALARM_MAX_SNOOZE-1);
	private final static int RTC_ALARM_RING_START = 0x00;
	private final static int RTC_ALARM_RING_STARTED = 0x01;

	private final static int RTC_TIME_SEPARATOR = ':';

	public final static int RTC_ALARM_MAX_RING = 10;
	
	String rtc_dayOfWeekString = "SunMonTueWedThuFriSat";
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
	    setDateTime(2012, 01, 01, 0, 0, 0);		
	}
	
	public ArduRCT_RealTimeClock(int year, int month, int day, int dayOfWeek, int hour, int minute, int second) {
		for (int i=0; i<7; i++) _alarm[i] = new rtcAlarm_t();
	    setDateTime(year, month, day, hour, minute, second);
	}

	public void setDate(int year, int month, int day) {
	    _year = year;
	    _month = month % 13;
	    if (_month == 0) _month = 1;
	    _day = day % 32;
	    if (_day == 0) _day = 1;
	    int mld = getMonthLastDay(_year, _month);
	    if (_day > mld) _day = mld;
	    _dayOfWeek = getDayOfWeek(_year, _month, _day);
	}

	public void setTime(int hour, int minute, int second) {
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


	public int getDayOfWeek(int year, int month, int day) {
		int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
		int y = year;
	    if (month < 3) y --;
	    return (y + y/4 - y/100 + y/400 + t[month-1] + day) % 7;
	}
	
	public void setDateTime(int year, int month, int day, int hour, int minute, int second) {
	    setDate(year, month, day);
	    setTime(hour, minute, second);
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
        _checkAlarms();
	    if (_day <= getMonthLastDay(_year, _month)) return true;
	    	    
	    _day = 1;
	    _month ++;
	    if (_month <= 12) return true;
	    
	    _month = 1;
	    _year ++;
	    return true;
	}

	public int getMonthLastDay(int year, int month) {
		if ((month == JANUARY) || (month == MARCH) || (month == MAY) || (month == JULY) || (month == AUGUST) || 
				(month == OCTOBER) || (month == DECEMBER)) return 31;
		if ((month == APRIL) || (month == JUNE) || (month == SEPTEMBER) || (month == NOVEMBER)) return 30;
		if (((year % 100) == 0) && ((year / 100) != 4)) return 28;	// centuries are not leap years unless divided by 400
		if ((year % 4) != 0) return 28;
		return 29;
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
	    return getDayOfWeekAsString(_dayOfWeek);
	}

	public int[] getDayOfWeekAsString(int dow) {
		for (int i=0; i<3; i++) _buffer[i] = rtc_dayOfWeekString.charAt(dow*3+i);
		_buffer[3] = 0;
	    return _buffer;
	}

	public int[] getDateAsString(int year, int month, int day, boolean withDayOfWeek) {
	    for (int i=0; i<(withDayOfWeek ? 15 : 11); i++) _buffer[i] = ' ';
	    _buffer[withDayOfWeek ? 15 : 11] = 0;
		int i = 0;
		if (withDayOfWeek) {
			int dayOfWeek = getDayOfWeek(year, month, day);
	    	for (int j=0; j<3; j++) _buffer[j] = rtc_dayOfWeekString.charAt(dayOfWeek*3+j);
	        i = 4;
	    }
	    if (day > 10) _buffer[i++] = (day / 10) + '0';
	    else _buffer[i++] = ' ';
	    _buffer[i++] = (day % 10) + '0';
		i++;
    	for (int j=0; j<3; j++) _buffer[i+j] = rtc_monthString.charAt((month-1)*3+j);
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

	public int[] getDateAsString(boolean withDayOfWeek) {
		return getDateAsString(_year, _month, _day, withDayOfWeek);
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

	public int getYear() {
	    return _year;
	}

	public int getMonth() {
	    return _month;
	}

	public int getDay() {
	    return _day;
	}

	public int getDayOfWeek() {
	    return _dayOfWeek;
	}

	public int getHour() {
	    return _hour;
	}

	public int getMinute() {
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
