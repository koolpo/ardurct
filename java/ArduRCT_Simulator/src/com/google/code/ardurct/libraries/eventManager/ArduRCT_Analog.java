package com.google.code.ardurct.libraries.eventManager;

import com.google.code.ardurct.hardware.Analog;
import com.google.code.ardurct.libraries.IArduinoDefines;

public class ArduRCT_Analog implements IEventDefines, IArduinoDefines {

	
	ArduRCT_Analog _next = null;
	int _pin;
	int _resolution; 
	int _previousValue;
	int _value;
	int _history[] = new int[ANALOG_MAX_AVERAGING];
	int _historyIndex;
	int _averaging;
	
	public ArduRCT_Analog(int pin) {
		this(pin, ANALOG_AVERAGING, ANALOG_RESOLUTION_12B);
	}


	public ArduRCT_Analog(int pin, int averaging) {
		this(pin, averaging, ANALOG_RESOLUTION_12B);
	}

	public ArduRCT_Analog(int pin, int averaging, int resolution) {
		_pin = pin;
		_value = 0;
		_historyIndex = -1;
		_previousValue = ANALOG_NO_VALUE;
		_averaging = averaging;
		if (_averaging > ANALOG_MAX_AVERAGING) _averaging = ANALOG_MAX_AVERAGING;
		else if (_averaging < 1) _averaging = 1; 
		_resolution = resolution;
		pinMode(pin, INPUT);
	}

	public int getPin() {
		return _pin;
	}
	
	public ArduRCT_Analog getNext() {
		return _next;
	}
	
	public void setNext(ArduRCT_Analog next) {
		_next = next;
	}

	public int updateValue() {
		if (_historyIndex == -1) {
			_history[0] = analogRead(_pin);
			for (int i=1; i<_averaging; i++) _history[i] = _history[0];
		} else _history[_historyIndex] = analogRead(_pin);
		_historyIndex ++;
		if (_historyIndex == _averaging) _historyIndex = 0;
		_value = 0;
		for (int i=0; i<_averaging; i++) _value += _history[i];
		_value = _value/_averaging;
		if (_resolution != ANALOG_HARDWARE_RESOLUTION) {
			int value32 = _value;
			value32 = value32 * _resolution / ANALOG_HARDWARE_RESOLUTION;
			_value = value32;
		}
		return _value;
	}
	
	public int getChange() {
		int change;
		if (_previousValue == ANALOG_NO_VALUE) change = 0;
		else change = _value - _previousValue;
		_previousValue = _value;
		return change;
	}

	public int getValue() {
		return _value;
	}

	private int analogRead(int pin) {
		return Analog.read(pin);
	}
	
	public void pinMode(int pin, int value) {
	}

}
