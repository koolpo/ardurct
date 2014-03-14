package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUIValue extends ArduRCT_GraphicsUIElement {
	
	int _max;
	int _min;
	int _step;
	int _display[] = new int[6];
	int _radix;
	
	public ArduRCT_GraphicsUIValue() {
		init(-1, 0, 0, 1, null, 1, 10);
	}
	
	public ArduRCT_GraphicsUIValue(int id, int value, int min, int max, IUIActionCallback actionHandler) {
		init(id, value, min, max, actionHandler, 1, 10);
	}

	public ArduRCT_GraphicsUIValue(int id, int value, int min, int max, IUIActionCallback actionHandler, int step) {
		init(id, value, min, max, actionHandler, step, 10);
	}

	public ArduRCT_GraphicsUIValue(int id, int value, int min, int max, IUIActionCallback actionHandler, int step, int radix) {
		init(id, value, min, max, actionHandler, step, radix);
	}
	
	public void init(int id, int value, int min, int max, IUIActionCallback actionHandler, int step, int radix) {
		_id = id;
		_value = value;
		_actionHandler = actionHandler;
		_min = min;
		_max = max;
		_step = step;
		_radix = radix;
		repeatable = true;
		if (_step == 0) _step = 1;
	}
	
	public void draw(ArduRCT_Graphics graphics, int uiX, int uiY, int uiWidth) {
		int bgColor = graphics.getBackgroundColor();
		int color = (_state == GRAPHICS_UI_PRESSED ? GRAPHICS_UI_COLOR_SELECTED : GRAPHICS_UI_COLOR_RELEASED);
		graphics.setBackgroundColor(color);
		graphics.fillRectangle(x+uiX, y+uiY, width, height, color);
		int hColor = (_state == GRAPHICS_UI_RELEASED || _state == GRAPHICS_UI_SELECTED || _state == GRAPHICS_UI_PRESSED ? 
				GRAPHICS_UI_COLOR_HIGHLIGHTED : BLACK);
		graphics.drawRectangle(x+uiX, y+uiY, width, height, hColor, 1);
		if (_value < _max) graphics.fillCorner(uiX+x+width-2, uiY+y+1, 5, GRAPHICS_POSITION_SW, BLACK);
		if (_value > _min) graphics.fillCorner(uiX+x+1, uiY+y+height-2, 5, GRAPHICS_POSITION_NE, BLACK);
		int len = _intToString(_value);
		int textX = x+uiX+(width - len*graphics.getFontCharWidth(GRAPHICS_UI_ELEMENT_FONT))/2;
		int textY = y+uiY+(height-graphics.getFontHeight(GRAPHICS_UI_ELEMENT_FONT))/2;
		graphics.drawString(_display, textX, textY, BLACK, GRAPHICS_UI_ELEMENT_FONT, GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, false);
		graphics.setBackgroundColor(bgColor);
	}
	
	public void autoSize(ArduRCT_Graphics graphics) {
		int len2 = _intToString(_max);
		int len = _intToString(_min);
		if (len2 > len) len = len2;
		height = graphics.getFontHeight(GRAPHICS_UI_ELEMENT_FONT) + GRAPHICS_UI_ELEMENT_TOP_MARGIN * 2;
		width =  len*graphics.getFontCharWidth(GRAPHICS_UI_ELEMENT_FONT) + GRAPHICS_UI_ELEMENT_LEFT_MARGIN * 2;
	}
	
	protected int _intToString(int value) {
		if ((value >= 0) && (value < _radix)) {
			_display[0] = '0' + value;
			if (_display[0] > '9') _display[0] += 'A'-'0'-10;
			_display[1] = 0;
			return 1;
		}
		int i = 0;
		int temp[] = new int[6];
		boolean neg = false;		
		if (value < 0) {
			neg = true;
			value = -value;
		}
		while (value > 0) {
			temp[i++] = (value % _radix) + '0';
			if (temp[i-1] > '9') temp[i-1] += 'A'-'0'-10;
			value = value/_radix;
		}
		i--;
		int j = 0;
		if (neg) _display[j++] = '-';
		while (i >= 0) _display[j++] = temp[i--]; 
		_display[j] = 0;
		return j;
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
		if (touchY < height/2) {
			if (_value == _max) return null;
			if (touchX < width/2) _value += _step;
			else _value += (_max-_min)/10;
			if (_value > _max) _value = _max;
		} else {
			if (_value == _min) return null;
			if (touchX < width/2) _value -= _step;
			else _value -= (_max-_min)/10;
			if (_value < _min) _value = _min;			
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
