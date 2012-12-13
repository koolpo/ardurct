package com.google.code.ardurct.libraries.eventManager;


public class ArduRCT_EventHandler
implements IEventDefines {
	
	int _type;
	int _value;
	IEventCallback _handler;
	ArduRCT_EventHandler _next = null;
	
	public ArduRCT_EventHandler(int type, IEventCallback handler) {
		_type = type;
		_value = EVENT_NO_VALUE;
		_handler = handler;
	}

	public ArduRCT_EventHandler(int type, int value, IEventCallback handler) {
		_type = type;
		_value = value;
		_handler = handler;
	}

	public boolean handle(int type, int value, int x, int y) {
		// check if we have a handler for the TICK event that needs to run
		if ((type == EVENT_TIME_TICK) && ((value % _value) == 0)) return _handler.handle(type, value, 0, 0);
		// or a handler for the type or the type class
		if (((_type == type) || (_type == (type & 0xF0))) && ((_value == EVENT_NO_VALUE) || (_value == value))) 
			return _handler.handle(type, value, x, y);
		return false;
	}

	public boolean process(int type) {
		return process(type, EVENT_NO_VALUE, 0, 0);
	}

	public boolean process(int type, int value) {
		return process(type, value, 0, 0);
	}

	public boolean process(int type, int value, int x, int y) {
		return _handler.handle(type, value, x, y);
	}

	public ArduRCT_EventHandler getNext() {
		return _next;
	}
	
	public void setNext(ArduRCT_EventHandler next) {
		_next = next;
	}
}
