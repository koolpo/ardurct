package com.google.code.ardurct.libraries.eventManager;

import com.google.code.ardurct.hardware.TFTTouchPanel;

public class ArduRCT_TouchPanel 
implements IEventDefines {

	private int _state = EVENT_STATE_IDLE;
	private int _touchX;
	private int _touchY;
	private int _touchZ;
	
	public int getPenXYZ(int[] x, int[] y, int[] z) {
		x[0] = _touchX;
		y[0] = _touchY;
		z[0] = _touchZ;
		return z[0];
	}
	
	public int update() {
		if (TFTTouchPanel.getTouchZ() == TOUCHPANEL_NO_TOUCH) {
			if (_state >  EVENT_STATE_IDLE) _state = EVENT_STATE_RELEASED;
			else _state = EVENT_STATE_IDLE;
			return _state;
		} 
		if ((_state == EVENT_STATE_RELEASED) || (_state == EVENT_STATE_IDLE)) _state = EVENT_STATE_PRESSED;
		else if ((Math.abs(_touchX-TFTTouchPanel.getTouchX()) > TOUCHPANEL_TOLERANCE) || 
				(Math.abs(_touchY-TFTTouchPanel.getTouchY()) > TOUCHPANEL_TOLERANCE)) _state = EVENT_STATE_DRAGGED;
		else _state = EVENT_STATE_PRESSED_MOTIONLESS;
		if (_state != EVENT_STATE_PRESSED_MOTIONLESS) {
			_touchX = TFTTouchPanel.getTouchX();
			_touchY = TFTTouchPanel.getTouchY();
			_touchZ = TFTTouchPanel.getTouchZ();
		}
		return _state;
	}
	
	public boolean isPenPressed() {
		return (_state >= EVENT_STATE_PRESSED);
	}

	public boolean isPenPressedMotionless() {
		return (_state == EVENT_STATE_PRESSED_MOTIONLESS);
	}

	public boolean isPenReleased() {
		return (_state == EVENT_STATE_RELEASED);
	}

	public boolean isPenDragged() {
		return (_state == EVENT_STATE_DRAGGED);
	}

	public int getPenX() {
		return _touchX;
	}

	public int getPenY() {
		return _touchY;
	}

	public int getPenZ() {
		return _touchZ;
	}
}
