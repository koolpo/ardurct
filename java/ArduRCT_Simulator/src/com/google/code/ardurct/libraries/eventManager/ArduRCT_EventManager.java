package com.google.code.ardurct.libraries.eventManager;

import java.util.Date;

public class ArduRCT_EventManager 
implements IEventDefines {
		
	public static int EVENT_MANAGER_CYCLE = 25;
	
	protected long millis() {
		return (new Date()).getTime();
	}

	long _nextUpdate = 0;
	int _tick;
	ArduRCT_Switch _switch = null;
	ArduRCT_EventHandler _handler = null;
	ArduRCT_RealTimeClock _rtc = null;
	ArduRCT_TouchPanel _touchPanel = null;
	
	public ArduRCT_EventManager() {
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
	
	public void registerTouchPanel(ArduRCT_TouchPanel touchPanel) {
		_touchPanel = touchPanel;
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
	    	handler.handle(EVENT_SYSTEM_TICK, _tick);
	    	handler = handler.getNext();
	    }

	    // update the time and generate corresponding events
	    if (_rtc != null) _updateTime();
	    
	    // look after the switches
    	ArduRCT_Switch aSwitch = _switch;
	    while (aSwitch != null) {
	    	aSwitch.updateState();
	    	if (aSwitch.isPressed()) addEvent(EVENT_SWITCH_PRESSED, aSwitch.getPin());
    		else if (aSwitch.isRepeating()) addEvent(EVENT_SWITCH_REPEATING, aSwitch.getPin());
    		else if (aSwitch.isReleased()) addEvent(EVENT_SWITCH_RELEASED, aSwitch.getPin());
	    	aSwitch = aSwitch.getNext();
	    }
	    
	    // look after the touchPanel
	    if (_touchPanel != null) {
	    	_touchPanel.update();
	    	int x = _touchPanel.getPenX();
	    	int y = _touchPanel.getPenY();
	    	int z = _touchPanel.getPenZ();
	    	if (_touchPanel.isPenPressed()) {
	    		if (_touchPanel.isPenDragged()) addEvent(EVENT_TOUCHPANEL_DRAGGED, z, x, y);
	    		else if (!_touchPanel.isPenPressedMotionless())  addEvent(EVENT_TOUCHPANEL_PRESSED, z, x, y);
	    	} else if (_touchPanel.isPenReleased()) addEvent(EVENT_TOUCHPANEL_RELEASED, 0, 0, 0);
	    }
	}
	
	private void _updateTime() {
    	if (_rtc.updateTime()) {
    		addEvent(EVENT_TIME_SECOND, _rtc.getSecond());
	    	if (_rtc.isAlarmStartingToRing()) addEvent(EVENT_TIME_ALARM, _rtc.getDayOfWeek());
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
	
	public void addEvent(int type, int value) {
		addEvent(type, value, 0, 0);
	}


	public boolean addEvent(int type, int value, int x, int y) {
		boolean atLeastOneHandlerHasRun = false;
		ArduRCT_EventHandler handler = _handler;
	    while (handler != null) {
	    	if (handler.handle(type, value, x, y)) atLeastOneHandlerHasRun = true;
	    	else if (handler.handle(type, value)) atLeastOneHandlerHasRun = true;
	    	else if (handler.handle(type)) atLeastOneHandlerHasRun = true;
	    	handler = handler.getNext();
	    }
	    return atLeastOneHandlerHasRun;
	}

}
