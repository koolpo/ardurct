package com.google.code.ardurct.libraries.eventManager;

import java.util.Date;

public class ArduRCT_EventManager 
implements IEventDefines {

	public class event_t {
		public int type;
		public int value;
		public int x;
		public int y;
	}
		
	public static int EVENT_MANAGER_CYCLE = 20;
	public static int EVENT_TYPE_MAX_EVENTS = 20;
	
	protected long millis() {
		return (new Date()).getTime();
	}

	long _nextUpdate = 0;
	int _tick;
	ArduRCT_Switch _switch = null;
	ArduRCT_EventHandler _handler = null;
	ArduRCT_RealTimeClock _rtc = null;
	
	event_t eventQueue[] = new event_t[EVENT_TYPE_MAX_EVENTS];
	int eventQueueIndex = 0;
	int eventQueueLastRead = 0;
	
	public ArduRCT_EventManager() {
		for (int i=0; i<EVENT_TYPE_MAX_EVENTS; i++) eventQueue[i] = new event_t();
	}
	
	public void setRTC(ArduRCT_RealTimeClock rtc) {
		_rtc = rtc;
	}
	
	public ArduRCT_RealTimeClock getRTC() {
		return _rtc;
	}
	
	public void registerSwitch(ArduRCT_Switch aSwitch) {
		if (_switch == null) _switch = aSwitch;
		else {
			ArduRCT_Switch nSwitch = _switch;
			while (nSwitch.getNext() != null) nSwitch = nSwitch.getNext();
			nSwitch.setNext(aSwitch);
		}
	}
	
	public void registerEventHandler(ArduRCT_EventHandler handler) {
		if (_handler == null) _handler = handler;
		else {
			ArduRCT_EventHandler nHandler = _handler;
			while (nHandler.getNext() != null) nHandler = nHandler.getNext();
			nHandler.setNext(handler);
		}
	}
	
	public void update() {
	    // check if we have to run or not
	    if ((millis() - _nextUpdate) < 0) return;
	    _nextUpdate = millis() + EVENT_MANAGER_CYCLE;

	    // update the tick counter
	    _tick ++;
	    // and the handlers attached to it
		ArduRCT_EventHandler handler = _handler;
	    while (handler != null) {
	    	handler.handle(EVENT_TIME_TICK, _tick, 0, 0);
	    	handler = handler.getNext();
	    }

	    // update the time and generate corresponding events
	    if (_rtc != null) _updateTime();
	    
	    // look after the switches
    	ArduRCT_Switch aSwitch = _switch;
	    while (aSwitch != null) {
	    	aSwitch.updateState();
	    	if (aSwitch.isPressed()) addEvent(EVENT_BUTTON_PRESSED, aSwitch.getPin());
    		else if (aSwitch.isRepeating()) addEvent(EVENT_BUTTON_REPEATING, aSwitch.getPin());
    		else if (aSwitch.isReleased()) addEvent(EVENT_BUTTON_RELEASED, aSwitch.getPin());
	    	aSwitch = aSwitch.getNext();
	    }
	}
	
	private void _updateTime() {
    	if (_rtc.updateTime()) {
    		addEvent(EVENT_TIME_SECOND, _rtc.getSecond());
	    	if (_rtc.isAlarmStartingToRing()) addEvent(EVENT_TIME_ALARM);
	    	if (_rtc.getSecond() == 0) {
	    		addEvent(EVENT_TIME_MINUTE, _rtc.getMinute());
	    		if (_rtc.getMinute() == 0) {
	    			addEvent(EVENT_TIME_MINUTE, _rtc.getHour());
	    			if (_rtc.getHour() == 0) {
	    				addEvent(EVENT_TIME_DAY, _rtc.getDay());
	    				if (_rtc.getDay() == 1) {
	    					addEvent(EVENT_TIME_MONTH, _rtc.getMonth());
		    				if (_rtc.getMonth() == 1) addEvent(EVENT_TIME_YEAR, _rtc.getYear());
	    				}
	    			}
	    		}
	    	}
    	}		
	}
	
	public event_t getEvent() {
		if (eventQueueIndex == eventQueueLastRead) return null;
		eventQueueLastRead ++;
		if (eventQueueLastRead == EVENT_TYPE_MAX_EVENTS) eventQueueLastRead = 0;
		return eventQueue[eventQueueLastRead == 0 ? EVENT_TYPE_MAX_EVENTS-1 : eventQueueLastRead-1];
	}

	public boolean hasEvent() {
		return (eventQueueIndex != eventQueueLastRead);
	}


	public void addEvent(int type) {
		addEvent(type, 0xFF, 0, 0);
	}

	public void addEvent(int type, int value) {
		addEvent(type, value, 0, 0);
	}

	public void addEvent(int type, int x, int y) {
		addEvent(type, 0xFF, x, y);
	}

	public void addEvent(int type, int value, int x, int y) {
		// check if one of the handler has run
		boolean hasRun = false;
		ArduRCT_EventHandler handler = _handler;
	    while (handler != null) {
	    	if (handler.handle(type, value, x, y)) hasRun = true;
	    	handler = handler.getNext();
	    }
	    if (hasRun) return;
		// if none have run, add the event to the queue
		if (eventQueueIndex + 1 == eventQueueLastRead) return;
		if ((eventQueueIndex == EVENT_TYPE_MAX_EVENTS-1) && (eventQueueLastRead == 0)) return;
		eventQueue[eventQueueIndex].type = type;
		eventQueue[eventQueueIndex].value = value;
		eventQueue[eventQueueIndex].x = x;
		eventQueue[eventQueueIndex].y = y;
		eventQueueIndex ++;
		if (eventQueueIndex == EVENT_TYPE_MAX_EVENTS) eventQueueIndex = 0;		
	}

}
