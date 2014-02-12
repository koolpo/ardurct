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

	public int handle(int type) {
		if (_handlerS == null) return EVENT_HANDLING_VOID;
		// check if the _type matches the type, or if the _type is a class and matches the type class
		if ((_type != type) && (_type != (type & 0xF0))) return EVENT_HANDLING_VOID;
		return _handlerS.handle(type);
	}

	public int handle(int type, int value) {
		if (_handlerM == null) return EVENT_HANDLING_VOID;
		// check if the _type matches the type, or if the _type is a class and matches the type class
		if ((_type != type) && (_type != (type & 0xF0))) return EVENT_HANDLING_VOID;
		// check if we have a handler for the TICK event that needs to run
		if ((_type == EVENT_SYSTEM_TICK) && ((_value <= 1) || ((value % _value) == 0))) return _handlerM.handle(type, value);
		// check if we run in all matches of _type or only on some values
		if ((_value == EVENT_ANY_VALUE) || (_value == value)) return _handlerM.handle(type, value);
		return EVENT_HANDLING_VOID;
	}
	
	public int handle(int type, int value, int x, int y) {
		if (_handlerL == null) return EVENT_HANDLING_VOID;
		// check if the _type matches the type, or if the _type is a class and matches the type class
		if ((_type != type) && (_type != (type & 0xF0))) return EVENT_HANDLING_VOID;
		// check if we run in all matches of _type or only on some values
		if ((_value == EVENT_ANY_VALUE) || (_value == value)) return _handlerL.handle(type, value, x, y);
		return EVENT_HANDLING_VOID;
	}

	public ArduRCT_EventHandler getNext() {
		return _next;
	}
	
	public void setNext(ArduRCT_EventHandler next) {
		_next = next;
	}
}
