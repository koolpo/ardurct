package com.google.code.ardurct.libraries.graphics;

public class ArduRCT_GraphicsUIElement {
	int x;
	int y; 
	int width;
	int height;

	int _value;
	int _group;
	int _min;
	int _max;
	int _type;
	int _id;
	int _label[];
	boolean _editable;
	
	IUIActionCallback _actionHandler;
	IUIDrawCallback _drawHandler;
	
	ArduRCT_GraphicsUIElement _next;
	ArduRCT_GraphicsUIElement _previous;
	
	public ArduRCT_GraphicsUIElement(int type, int id) {
		this(type, id, null, null, null);
	}

	public ArduRCT_GraphicsUIElement(int type, int id, int[] label) {
		this(type, id, label, null, null);
	}

	public ArduRCT_GraphicsUIElement(int type, int id, int[] label, IUIActionCallback actionHandler) {
		this(type, id, label, actionHandler, null);
	}

	public ArduRCT_GraphicsUIElement(int type, int id, int[] label, IUIActionCallback actionHandler, IUIDrawCallback drawHandler) {
		_type = type;
		_id = id;
		_label = label;
		_value = 0;
		_group = 0;
		_editable = true;
		_actionHandler = actionHandler;
		_drawHandler = drawHandler;
	}
	
	public void setGroup(int group) {
		_group = group;
	}

	public void setValue(int value, int min, int max) {
		_value = value;
		_min = min;
		_max = max;
	}

	public void setEditable(boolean editable) {
		_editable = editable;
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

	public boolean run() {
		if (_actionHandler != null) return _actionHandler.run(_id, _value);
		return false;
	}
	
	public void draw(ArduRCT_Graphics graphics, int state) {
		if (_drawHandler != null) _drawHandler.draw(graphics, state);
		else {
			// draw the item
		}
	}
}
