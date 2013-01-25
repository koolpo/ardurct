package com.google.code.ardurct.libraries.eventManager;

import com.google.code.ardurct.hardware.Digital;
import com.google.code.ardurct.libraries.IArduinoDefines;

public class ArduRCT_Switch implements IEventDefines, IArduinoDefines {

	public ArduRCT_Switch _next = null;
	public int _pin;
	public int _state;
	
	public ArduRCT_Switch(int pin) {
		_pin = pin;
		_state = EVENT_STATE_IDLE;
		// set as input
		pinMode(pin, INPUT);
		// add a pull up
		digitalWrite(pin, HIGH);
	}
	
	public int getPin() {
		return _pin;
	}
	
	public ArduRCT_Switch getNext() {
		return _next;
	}
	
	public void setNext(ArduRCT_Switch next) {
		_next = next;
	}

	/*
	 * from idle
	 * 	if pin=0 (down)
	 * 		set to idle+1=debouncingDown
	 * 		if still down, reach deboucingDown+3=pressed
	 * 			if still down, reach pressed+20=repeating
	 * 			if still down, reach repeating+80=fastRepeating
	 * 			if still down and reach FF, reset to fastRepeating
	 * 	if pin=1 (up)
	 * 		if idle, do nothing
	 * 		if was down ( >= pressed) set to debounceUp
	 * 			if still down, reach debounceUp+3=released
	 * 			if still down, reach released+1=idle
	 * 		if was debouncingDown set to idle
	 */
	public int updateState() {
		if (digitalRead(_pin) == 1) {
			// if already idle, return
			if (_state == EVENT_STATE_IDLE) return _state;
			// if was pressed, start to debounce up
			if (_state >= EVENT_STATE_PRESSED) _state = EVENT_STATE_DEBOUNCING_UP;
			// else if was starting to be pressed, return to idle
			else if (_state >= EVENT_STATE_DEBOUNCING_DOWN) _state = EVENT_STATE_IDLE-1;
		} 
		if (_state == 0xFF) _state = EVENT_STATE_FAST_REPEATING - 1;
		_state ++;
		return _state;
	}
	
	public boolean isPressed() {
		return (_state == EVENT_STATE_PRESSED);
	}

	public boolean isReleased() {
		return (_state == EVENT_STATE_RELEASED);
	}

	public boolean hasBeenPressed() {
		return (_state >= EVENT_STATE_PRESSED);
	}

	public boolean isRepeating() {
		if (_state < EVENT_STATE_REPEATING) return false;
		// after SWITCH_STATE_FAST_REPEATING * EVENT_MANAGER_CYCLE = 4 seconds
		if (_state >= EVENT_STATE_FAST_REPEATING) {
			// indicate a repeat every 5 * EVENT_MANAGER_CYCLE = 250ms
			if ((_state % 5) == 0) return true;
		// after SWITCH_STATE_REPEATING * EVENT_MANAGER_CYCLE = 1 seconds
		// indicate a repeat every 10 * EVENT_MANAGER_CYCLE = 500ms
		} else if ((_state % 10) == 0) return true;
		return false;
	}

	private int digitalRead(int pin) {
		return Digital.read(pin);
	}
	
	public void pinMode(int pin, int value) {
	}
	
	public void digitalWrite(int pin, int value) {
	}

}
