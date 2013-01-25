package com.google.code.ardurct.libraries.eventManager;

import java.util.Date;

public class ArduRCT_EventManager 
implements IEventDefines {
			
	protected long millis() {
		return (new Date()).getTime();
	}

	long _nextUpdate = 0;
	int _tick;
	ArduRCT_Switch _switch = null;
	ArduRCT_Encoder _encoder = null;
	ArduRCT_EventHandler _handler = null;
	ArduRCT_RealTimeClock _rtc = null;
	ArduRCT_TouchPanel _touchPanel = null;
	ArduRCT_Analog _analog = null;
	
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

	public void registerEncoder(ArduRCT_Encoder anEncoder) {
		if (_encoder == null) _encoder = anEncoder;
		else {
			ArduRCT_Encoder nEncoder = _encoder;
			while (nEncoder.getNext() != null) nEncoder = nEncoder.getNext();
			nEncoder.setNext(anEncoder);
		}
	}

	public void registerAnalog(ArduRCT_Analog anAnalog) {
		if (_analog == null) _analog = anAnalog;
		else {
			ArduRCT_Analog nAnalog = _analog;
			while (nAnalog.getNext() != null) nAnalog = nAnalog.getNext();
			nAnalog.setNext(anAnalog);
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
	    	if (aSwitch.isPressed()) processEvent(EVENT_SWITCH_PRESSED, aSwitch.getPin());
    		else if (aSwitch.isRepeating()) processEvent(EVENT_SWITCH_REPEATING, aSwitch.getPin());
    		else if (aSwitch.isReleased()) processEvent(EVENT_SWITCH_RELEASED, aSwitch.getPin());
	    	aSwitch = aSwitch.getNext();
	    }
	    
	    // look after the touchPanel
	    if (_touchPanel != null) {
	    	_touchPanel.update();
	    	int x = _touchPanel.getPenX();
	    	int y = _touchPanel.getPenY();
	    	int z = _touchPanel.getPenZ();
	    	if (_touchPanel.isPenPressed()) {
	    		if (_touchPanel.isPenDragged()) processEvent(EVENT_TOUCHPANEL_DRAGGED, z, x, y);
	    		else if (!_touchPanel.isPenPressedMotionless())  processEvent(EVENT_TOUCHPANEL_PRESSED, z, x, y);
	    	} else if (_touchPanel.isPenReleased()) processEvent(EVENT_TOUCHPANEL_RELEASED, 0, 0, 0);
	    }
	    
	    // look after the analogs
    	ArduRCT_Analog anAnalog = _analog;
	    while (anAnalog != null) {
	    	int value = anAnalog.updateValue();
	    	// change can only been read once
	    	int change = anAnalog.getChange();
	    	// send an event if the value changed
	    	if (change != 0) processEvent(change > 0 ? EVENT_ANALOG_INCREASE : EVENT_ANALOG_DECREASE, anAnalog.getPin(), value, change);
	    	anAnalog = anAnalog.getNext();
	    }

	    // look after the encoder
    	ArduRCT_Encoder anEncoder = _encoder;
	    while (anEncoder != null) {
	    	int count = anEncoder.updateValue();
	    	// change can only been read once
	    	int change = anEncoder.getChange();
	    	// send an event if the value changed
	    	if (change != 0) processEvent(change > 0 ? EVENT_ENCODER_INCREASE : EVENT_ENCODER_DECREASE, anEncoder.getPinA(), count, change);
	    	anEncoder = anEncoder.getNext();
	    }
	}
	
	private void _updateTime() {
    	if (_rtc.updateTime()) {
    		processEvent(EVENT_TIME_SECOND, _rtc.getSecond());
	    	if (_rtc.isAlarmStartingToRing()) processEvent(EVENT_TIME_ALARM, _rtc.getDayOfWeek());
	    	if (_rtc.getSecond() == 0) {
	    		processEvent(EVENT_TIME_MINUTE, _rtc.getMinute());
	    		if (_rtc.getMinute() == 0) {
	    			processEvent(EVENT_TIME_MINUTE, _rtc.getHour());
	    			if (_rtc.getHour() == 0) {
	    				processEvent(EVENT_TIME_DAY, _rtc.getDay());
	    				if (_rtc.getDay() == 1) {
	    					processEvent(EVENT_TIME_MONTH, _rtc.getMonth());
		    				if (_rtc.getMonth() == 1) processEvent(EVENT_TIME_YEAR, _rtc.getYear());
	    				}
	    			}
	    		}
	    	}
    	}		
	}
	
	public int processEvent(int type, int value) {
		int processStatus = EVENT_HANDLING_VOID;
		ArduRCT_EventHandler handler = _handler;
	    while (handler != null) {
	    	processStatus = handler.handle(type, value);
	    	if (processStatus == EVENT_HANDLING_VOID) processStatus = handler.handle(type);
	    	if (processStatus == EVENT_HANDLING_EXIT) return EVENT_HANDLING_EXIT;
	    	handler = handler.getNext();
	    }
	    return processStatus;
	}


	public int processEvent(int type, int value, int x, int y) {
		int processStatus = EVENT_HANDLING_VOID;
		ArduRCT_EventHandler handler = _handler;
	    while (handler != null) {
	    	processStatus = handler.handle(type, value, x, y);
	    	if (processStatus == EVENT_HANDLING_VOID) processStatus = handler.handle(type, value);
	    	if (processStatus == EVENT_HANDLING_VOID) processStatus = handler.handle(type);
	    	if (processStatus == EVENT_HANDLING_EXIT) return EVENT_HANDLING_EXIT;
	    	handler = handler.getNext();
	    }
	    return processStatus;
	}

}
