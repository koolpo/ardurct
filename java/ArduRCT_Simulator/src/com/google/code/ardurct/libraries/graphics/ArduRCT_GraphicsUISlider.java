package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUISlider extends ArduRCT_GraphicsUIElement {
	
	int _max;
	int _min;
	int _step;
	
	public ArduRCT_GraphicsUISlider(int id, int value, int min, int max, IUIActionCallback actionHandler) {
		this(id, value, min, max, actionHandler, 1);
	}

	public ArduRCT_GraphicsUISlider(int id, int value, int min, int max, IUIActionCallback actionHandler, int step) {
		_id = id;
		_value = value;
		_actionHandler = actionHandler;
		_min = min;
		_max = max;
		_step = step;
		if (_step == 0) _step = 1;
	}
	
	public void draw(ArduRCT_Graphics graphics, int uiX, int uiY, int uiWidth) {
		graphics.fillRectangle(x+uiX, y+uiY, width, height, GRAPHICS_UI_COLOR_BACKGROUND);
		int color = (_state == GRAPHICS_UI_PRESSED ? GRAPHICS_UI_COLOR_SELECTED : GRAPHICS_UI_COLOR_RELEASED);
		int hColor = (_state == GRAPHICS_UI_RELEASED || _state == GRAPHICS_UI_SELECTED  || _state == GRAPHICS_UI_PRESSED ? 
				GRAPHICS_UI_COLOR_HIGHLIGHTED : BLACK);
		if (width > height) {
			// horizontal slider
			graphics.fillRectangle(uiX+x, uiY+y+height/2-height/8, width, 2*height/8, BLACK);
			int knobX = x+uiX + _value*(width-GRAPHICS_UI_SLIDER_KNOB_SIZE)/(_max-_min);
			graphics.fillRectangle(knobX, y+uiY, GRAPHICS_UI_SLIDER_KNOB_SIZE, height, color);
			graphics.drawRectangle(knobX, y+uiY, GRAPHICS_UI_SLIDER_KNOB_SIZE, height, hColor, 1);
		} else {
			// vertical slider
			graphics.fillRectangle(uiX+x+width/2-width/8, uiY+y, 2*width/8, height, BLACK);
			int knobY = y+uiY + (_max-_value)*(height-GRAPHICS_UI_SLIDER_KNOB_SIZE)/(_max-_min);
			graphics.fillRectangle(x+uiX, knobY, width, GRAPHICS_UI_SLIDER_KNOB_SIZE, color);
			graphics.drawRectangle(x+uiX, knobY, width, GRAPHICS_UI_SLIDER_KNOB_SIZE, hColor, 1);
		}
	}
	
	public ArduRCT_GraphicsUIElement setValue(int value) {
		_value = (value / _step) * _step;
		if (_value > _max) _value = _max;
		if (_value < _min) _value = _min;
		return null;	
	}
	
	// called when the item is selected and Up is pressed
	// return true if the value was increased
	public boolean increase() {
		if (_value == _max) return false;
		_value += _step;
		if (_value > _max) _value = _max;
		return true;
	}
	
	// called when the item is selected and Down is pressed
	// return true if the value was decreased
	public boolean decrease() {
		if (_value == _min) return false;
		_value -= _step;
		if (_value < _min) _value = _min;
		return true;
	}
	
	// called when the item is touched with a pen
	public ArduRCT_GraphicsUIElement touch(int touchX, int touchY) {
		_state = GRAPHICS_UI_SELECTED;
		if (width > height) {
			// horizontal
			_value = _min + touchX * (_max - _min) / (width - GRAPHICS_UI_SLIDER_KNOB_SIZE/2);
			if (_value > _max) _value = _max;
		} else {
			// vertical
			_value = _min + (height - touchY) * (_max - _min) / (height - GRAPHICS_UI_SLIDER_KNOB_SIZE/2);
			if (_value > _max) _value = _max;
		}
		return null;
	}
	
	// called when the item is pressed
	// return the element that was modified if any
	// we toggle the state on press
	public ArduRCT_GraphicsUIElement press() {
		if (_state == GRAPHICS_UI_PRESSED) _state = GRAPHICS_UI_RELEASED;
		else _state = GRAPHICS_UI_PRESSED;
		return null;
	}

	// called when the item is released
	// return true if the element can be released
	public boolean release() {
		return (_state == GRAPHICS_UI_RELEASED);
	}

}
