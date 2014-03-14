package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUIElement 
implements IGraphicsDefines {

	public int x;
	public int y; 
	public int width;
	public int height;
	public boolean editable;
	public int section;
	public boolean repeatable;

	int _value;
	int _state;
	int _id;
	int _group;

	IUIActionCallback _actionHandler;
	IUIDrawCallback _drawHandler;
	
	ArduRCT_GraphicsUIElement _next;
	ArduRCT_GraphicsUIElement _previous;
		
	public ArduRCT_GraphicsUIElement() {
		this(-1, null, null);
	}

	public ArduRCT_GraphicsUIElement(int id) {
		this(id, null, null);
	}

	public ArduRCT_GraphicsUIElement(int id, IUIDrawCallback drawHandler) {
		this(id, drawHandler, null);
	}

	public ArduRCT_GraphicsUIElement(int id, IUIDrawCallback drawHandler, IUIActionCallback actionHandler) {
		_id = id;
		_group = -1;
		_value = GRAPHICS_UI_RELEASED;
		_state = GRAPHICS_UI_UNSELECTED;
		editable = true;
		_actionHandler = actionHandler;
		_drawHandler = drawHandler;
	}

	public ArduRCT_GraphicsUIElement setValue(int value) {
		_value = value;
		return null;
	}
	
	public void setState(int state) {
		_state = state;
	}
	
	public void setNext(ArduRCT_GraphicsUIElement next) {
		_next = next;
		next.setPrevious(this);
	}
	
	public void setPrevious(ArduRCT_GraphicsUIElement previous) {
		_previous = previous;
	}
	
	public ArduRCT_GraphicsUIElement getNext() {
		return _next;
	}

	public ArduRCT_GraphicsUIElement getPrevious() {
		return _previous;
	}
	
	public int getGroup() {
		return _group;
	}
	
	protected int getFontSize(int text[]) {
		if (text != null) {
			if (((text[0] == '+') || (text[0] == '-')) && (text[1] == 0)) return GRAPHICS_UI_ELEMENT_FONT+1;			
		}
		return GRAPHICS_UI_ELEMENT_FONT;
	}
		
	public void draw(ArduRCT_Graphics graphics, int uiX, int uiY, int uiWidth) {
		if (_drawHandler != null) _drawHandler.draw(_id, _state, _value, x+uiX, y+uiY, width, height);
		else {
			graphics.fillRectangle(x+uiX, y+uiY, width, height, GRAPHICS_UI_COLOR_RELEASED);
			if (editable) {
				int color = GRAPHICS_UI_COLOR_RELEASED;
				if (_state == GRAPHICS_UI_SELECTED) color = GRAPHICS_UI_COLOR_HIGHLIGHTED;
				else if (_state == GRAPHICS_UI_PRESSED) color = GRAPHICS_UI_COLOR_PRESSED;
				graphics.drawRectangle(x+uiX, y+uiY, width, height, color, 1);
			}
		}
	}
	
	public void autoSize(ArduRCT_Graphics graphics) {
		// overwritten by subclasses
	}
	
	public void placeLabel(ArduRCT_Graphics graphics) {
		// overwritten by subclasses
	}

	public int getValue() {
		return _value;
	}
	
	public int getId() {
		return _id;
	}

	// called when the item is selected (enter is pressed, or item is touched)
	public void select() {
		_state = GRAPHICS_UI_SELECTED;
	}

	// called when the item is pressed
	// return the element that was modified if any
	public ArduRCT_GraphicsUIElement press() {
		if (_state < GRAPHICS_UI_PRESSED) _state = GRAPHICS_UI_PRESSED;
		else _state ++;
		if (_state > GRAPHICS_UI_REPEATED) _state = GRAPHICS_UI_REPEATED;
		return null;
	}
	
	// called when the item is released
	// return true if the element can be released
	public boolean release() {
		_state = GRAPHICS_UI_RELEASED;
		return true;
	}

	// called when the item is unselected
	public void unselect() {
		_state = GRAPHICS_UI_UNSELECTED;
	}
	
	// called when the item is selected and Up is pressed
	// return true if the value was increased
	public boolean increase() {
		return false;
	}
	
	// called when the item is selected and Down is pressed
	// return true if the value was decreased
	public boolean decrease() {
		return false;
	}
	
	// called when the item is touched with a pen
	// return the element that was modified if any
	public ArduRCT_GraphicsUIElement touch(int touchX, int touchY) {
		return press();
	}

	public void setRepeatable(boolean repeatable) {
		this.repeatable = repeatable;
	}

	// whether a motionless touch can repeat the touch
	public boolean isRepeatable() {
		return repeatable;
	}
	
	public boolean run() {
		if (_actionHandler != null) return _actionHandler.run(_id, _state <= GRAPHICS_UI_SELECTED ? GRAPHICS_UI_RELEASED : _state, _value);
		return false;
	}
			

}
