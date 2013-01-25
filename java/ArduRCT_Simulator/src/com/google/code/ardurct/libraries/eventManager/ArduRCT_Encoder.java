package com.google.code.ardurct.libraries.eventManager;

import com.google.code.ardurct.hardware.Digital;
import com.google.code.ardurct.libraries.IArduinoDefines;

public class ArduRCT_Encoder implements IEventDefines, IArduinoDefines {

	public static final int ENCODER_STEPS = 24;
	
	public ArduRCT_Encoder _next = null;
	public int _pinA;
	public int _pinB;
	public int _value;
	public int _previousValue;
	public int _min;
	public int _max;
	public int _previousPosition;

	public ArduRCT_Encoder(int pinA, int pinB) {
		this(pinA, pinB, 0, 0, ENCODER_STEPS-1);
	}
	
	public ArduRCT_Encoder(int pinA, int pinB, int value, int min, int max) {
		_pinA = pinA;
		_pinB = pinB;
		_value = value;
		_min = min;
		_max = max;
		_previousValue = value;
		_previousPosition = 0;
		// set as input
		pinMode(pinA, INPUT);
		pinMode(pinB, INPUT);
		// add a pull up
		digitalWrite(pinA, HIGH);
		digitalWrite(pinB, HIGH);
	}
	
	public ArduRCT_Encoder getNext() {
		return _next;
	}
	
	public void setNext(ArduRCT_Encoder next) {
		_next = next;
	}

	public int getPinA() {
		return _pinA;
	}
	
	public int updateValue() {
		int position = digitalRead(_pinB)*2 + digitalRead(_pinA);
		if (position != _previousPosition) {
			boolean isCCW = ((_previousPosition == 0) && (position == 1)) || ((_previousPosition == 1) && (position == 3)) ||
				((_previousPosition == 2) && (position == 0)) || ((_previousPosition == 3) && (position == 2));
			if (isCCW) {
				_value--;
				if (_value < _min) {
					if (_min == 0) _value = _max;
					else _value = _min;
				}
			} else {
				_value++;
				if (_value > _max) {
					if (_min == 0) _value = 0;
					else _value = _max;
				}
			}
		}
		_previousPosition = position;
		return _value;
	}
	
	public void setValue(int value) {
		_value = value;
		if (_value < _min) _value = _min;
		if (_value > _max) _value = _max;
	}

	public void setMinMax(int min, int max) {
		_min = min;
		_max = max;
	}

	public int getValue() {
		return _value;
	}
	
	public int getChange() {
		int change = _value - _previousValue;
		if (change < -10) change += _max + 1;
		if (change > 10) change -= _max + 1;
		_previousValue = _value;
		return change;
	}

	private int digitalRead(int pin) {
		return Digital.read(pin);
	}
	
	public void pinMode(int pin, int value) {
	}
	
	public void digitalWrite(int pin, int value) {
	}

}
