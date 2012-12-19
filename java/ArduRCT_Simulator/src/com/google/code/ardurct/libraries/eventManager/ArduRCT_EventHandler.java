package com.google.code.ardurct.libraries.eventManager;


public class ArduRCT_EventHandler
implements IEventDefines {
	
	int _type;
	int _value;
	IEventCallback _handlerS;
	IEventCallback _handlerM;
	IEventCallback _handlerL;
	ArduRCT_EventHandler _next = null;
	
	public ArduRCT_EventHandler(int type, IEventCallback handler) {
		_type = type;
		_value = EVENT_ANY_VALUE;
		_handlerS = handler;
	}

	public ArduRCT_EventHandler(int type, int value, IEventCallback handler) {
		_type = type;
		_value = value;
		_handlerM = handler;
	}

	public ArduRCT_EventHandler(int type, int value, int x, int y, IEventCallback handler) {
		_type = type;
		_value = value;
		_handlerL = handler;
	}

	public boolean handle(int type) {
		if (_handlerS == null) return false;
		// check if the _type matches the type, or if the _type is a class and matches the type class
		if ((_type != type) && (_type != (type & 0xF0))) return false;
		return _handlerS.handle(type);
	}

	public boolean handle(int type, int value) {
		if (_handlerM == null) return false;
		// check if the _type matches the type, or if the _type is a class and matches the type class
		if ((_type != type) && (_type != (type & 0xF0))) return false;
		// check if we have a handler for the TICK event that needs to run
		if ((_type == EVENT_SYSTEM_TICK) && ((value % _value) == 0)) return _handlerM.handle(type, value);
		// check if we run in all matches of _type or only on some values
		if ((_value == EVENT_ANY_VALUE) || (_value == value)) return _handlerM.handle(type, value);
		return false;
	}
	
	public boolean handle(int type, int value, int x, int y) {
		if (_handlerL == null) return false;
		// check if the _type matches the type, or if the _type is a class and matches the type class
		if ((_type != type) && (_type != (type & 0xF0))) return false;
		// check if we run in all matches of _type or only on some values
		if ((_value == EVENT_ANY_VALUE) || (_value == value)) return _handlerL.handle(type, value, x, y);
		return false;
	}

	public ArduRCT_EventHandler getNext() {
		return _next;
	}
	
	public void setNext(ArduRCT_EventHandler next) {
		_next = next;
	}
}
