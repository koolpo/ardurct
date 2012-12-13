package com.google.code.ardurct.libraries.eventManager;

import com.google.code.ardurct.hardware.Switch;

public class ArduRCT_Switch {

	public final static int SWITCH_STATE_DEBOUNCING_UP = 0;
	public final static int SWITCH_STATE_RELEASED = 3;
	public final static int SWITCH_STATE_IDLE = 4;
	public final static int SWITCH_STATE_DEBOUNCING_DOWN = 5;
	public final static int SWITCH_STATE_PRESSED = 8;
	public final static int SWITCH_STATE_REPEATING = 20;
	public final static int SWITCH_STATE_FAST_REPEATING = 80;
	
	public ArduRCT_Switch _next = null;
	public int _pin;
	public int _state;
	
	public ArduRCT_Switch(int pin) {
		_pin = pin;
		_state = SWITCH_STATE_IDLE;
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
	 * 	if down 
	 * 		set to idle+1=debouncingDown
	 * 		if still down, reach deboucingDown+3=pressed
	 * 			if still down, reach pressed+20=repeating
	 * 			if still down, reach repeating+80=fastRepeating
	 * 			if still down and reach FF, reset to fastRepeating
	 * 	if up
	 * 		if idle, do nothing
	 * 		if was down ( >= debouncingDown) set to debounceUp
	 * 			if still down, reach debouncingDown+3=released
	 * 			if still down, reach released+1=idle
	 */
	public int updateState() {
		if (digitalRead(_pin) == 1) {
			// if already idle, return
			if (_state == SWITCH_STATE_IDLE) return _state;
			// if was pressed, start to debounce up
			if (_state >= SWITCH_STATE_PRESSED) _state = SWITCH_STATE_DEBOUNCING_UP;
			// else if was starting to be pressed, return to idle
			else if (_state >= SWITCH_STATE_DEBOUNCING_DOWN) _state = SWITCH_STATE_IDLE-1;
		} 
		if (_state == 0xFF) _state = SWITCH_STATE_FAST_REPEATING - 1;
		_state ++;
		
/*	
 		String s = _pin + ".";
		if (_state < SWITCH_STATE_RELEASED) s += "DU";
		else if (_state < SWITCH_STATE_IDLE) s += "RE";
		else if (_state < SWITCH_STATE_DEBOUNCING_DOWN) s += "ID";
		else if (_state < SWITCH_STATE_PRESSED) s += "DD";
		else if (_state < SWITCH_STATE_REPEATING) s += "PR";
		else if (_state < SWITCH_STATE_FAST_REPEATING) s += "SR";
		else s += "FR";
		System.out.print(s + (_state == SWITCH_STATE_IDLE ? "\r\n" : " "));
*/		
		return _state;
	}
	
	public boolean isPressed() {
		return (_state == SWITCH_STATE_PRESSED);
	}

	public boolean isReleased() {
		return (_state == SWITCH_STATE_RELEASED);
	}

	public boolean hasBeenPressed() {
		return (_state >= SWITCH_STATE_PRESSED);
	}

	public boolean isRepeating() {
		if (_state < SWITCH_STATE_REPEATING) return false;
		// after SWITCH_STATE_FAST_REPEATING * EVENT_MANAGER_CYCLE = 4 seconds
		if (_state >= SWITCH_STATE_FAST_REPEATING) {
			// indicate a repeat every 5 * EVENT_MANAGER_CYCLE = 250ms
			if ((_state % 5) == 0) return true;
		// after SWITCH_STATE_REPEATING * EVENT_MANAGER_CYCLE = 1 seconds
		// indicate a repeat every 10 * EVENT_MANAGER_CYCLE = 500ms
		} else if ((_state % 10) == 0) return true;
		return false;
	}

	private int digitalRead(int pin) {
		return Switch.isPressed(pin) ? 0 : 1;
	}
}
