package com.google.code.ardurct.touchscreen;


public class TouchScreen_UserInterface extends ScreenHAL {

	public class uiTab_t {
		int[] text;
		int x;
		int y;
		int width;
		IUIDrawCallback drawer; 
		IUIActionCallback handler; 
	}
	
	public class uiElement_t {
		int type;
		int tab;
		public int id;
		int group;
		public int x;
		public int y;
		public int width;
		public int height;
		int[] text;
		public int value;
		int min;
		int max;
		boolean editable;
	}

	private static final long serialVersionUID = 1L;
	
	public static final int UI_MAX_ELEMENTS = 30;
	public static final int UI_MAX_TABS = 6;
	
	public static final int UI_BUTTON = 1;
	public static final int UI_PUSH_BUTTON = 2;
	public static final int UI_AREA = 3;
	public static final int UI_GAUGE = 4;
	public static final int UI_SLIDER = 5;
	public static final int UI_LABEL = 6;

	public static int UI_COLOR_BACKGROUND = WHITE;
	public static int UI_COLOR_TAB_UNSELECTED = LIGHT_GREY;
	public static int UI_COLOR_BUTTON_PUSHED = F_GREY;
	public static int UI_COLOR_BUTTON_RELEASED = LIGHT_GREY;
	public static int UI_COLOR_GAUGE = 0x7C7C;

	public static int UI_TAB_FONT = FONT_MEDIUM;
	public static boolean UI_TAB_FONT_IS_BOLD = true;
	public static int UI_TAB_TOP_MARGIN = 4;
	public static int UI_TAB_LEFT_MARGIN = 7;

	public static int UI_FONT = FONT_MEDIUM;
	public static boolean UI_FONT_IS_BOLD = true;

	public static int UI_ELEMENT_TOP_MARGIN = 4;
	public static int UI_ELEMENT_LEFT_MARGIN = 6;
	
	public static int UI_SLIDER_KNOB_SIZE = 12;
	
	public static final int UI_PUSHED = 1;
	public static final int UI_RELEASED = 0;
	
	uiElement_t _uiElement[];
	int _uiNbElements;
	int _uiActiveElement;
	uiTab_t _uiTab[];
	int _uiNbTabs;
	int _uiActiveTab;
	int _uiX;
	int _uiY;
	int _uiWidth;
	int _uiHeight;
	int _uiTabHeight;
	IUIDrawCallback _uiDrawer; 
	IUIActionCallback _uiHandler; 
	
	public void setupUI(int x, int y, int width, int height) {
		_uiX = x;
		_uiY = y;
		_uiWidth = width;
		_uiHeight = height;
		_uiTabHeight = 0;
		_uiElement = new uiElement_t[UI_MAX_ELEMENTS];
		_uiTab = new uiTab_t[UI_MAX_TABS];
		_uiNbElements = 0;
		_uiActiveElement = -1;
		_uiNbTabs = 0;
		_uiActiveTab = -1;
		_uiDrawer = null; 
		_uiHandler = null; 
	}
	
	public void setupUI(int x, int y, int width, int height, IUIActionCallback handler) {
		_uiX = x;
		_uiY = y;
		_uiWidth = width;
		_uiHeight = height;
		_uiTabHeight = 0;
		_uiElement = new uiElement_t[UI_MAX_ELEMENTS];
		_uiTab = new uiTab_t[UI_MAX_TABS];
		_uiNbElements = 0;
		_uiActiveElement = -1;
		_uiNbTabs = 0;
		_uiActiveTab = -1;
		_uiDrawer = null; 
		_uiHandler = handler; 
	}

	public void setupUI(int x, int y, int width, int height, IUIActionCallback handler, IUIDrawCallback drawer) {
		_uiX = x;
		_uiY = y;
		_uiWidth = width;
		_uiHeight = height;
		_uiTabHeight = 0;
		_uiElement = new uiElement_t[UI_MAX_ELEMENTS];
		_uiTab = new uiTab_t[UI_MAX_TABS];
		_uiNbElements = 0;
		_uiActiveElement = -1;
		_uiNbTabs = 0;
		_uiActiveTab = -1;
		_uiDrawer = drawer; 
		_uiHandler = handler; 
	}

	public int addUITab(int[] text) {
		return addUITab(text, null, null);
	}
	
	public int addUITab(int[] text, IUIActionCallback handler) {
		return addUITab(text, handler, null);
	}
	
	public int addUITab(int[] text, IUIActionCallback handler, IUIDrawCallback drawer) {
		if (_uiNbTabs == UI_MAX_TABS) return UI_ERROR;
		_uiTab[_uiNbTabs] = new uiTab_t();
		if (_uiNbTabs == 0) _uiTab[_uiNbTabs].x = UI_TAB_LEFT_MARGIN;
		else _uiTab[_uiNbTabs].x = _uiTab[_uiNbTabs-1].x+_uiTab[_uiNbTabs-1].width-1;
		if (_uiTab[_uiNbTabs].x > _uiWidth) return UI_ERROR;
		_uiTab[_uiNbTabs].y = 2;
		_uiTab[_uiNbTabs].text = text;
		if (_uiNbTabs > 0) _uiTabHeight = this.getFontHeight(UI_TAB_FONT) + 2 + UI_TAB_TOP_MARGIN*2;
		_uiTab[_uiNbTabs].width = getStringWidth(_uiTab[_uiNbTabs].text, UI_TAB_FONT) + UI_TAB_LEFT_MARGIN * 2;
		_uiTab[_uiNbTabs].drawer = drawer;
		_uiTab[_uiNbTabs].handler = handler;
		return _uiNbTabs++;
	}
	
	public int addUIButton(int tab, int id, int x, int y, int width, int height, int[] text) {
		return _addUIElement(UI_BUTTON,tab, id, 0, x, y, width, height, UI_RELEASED, text, UI_RELEASED, UI_PUSHED);
	}

	public int addUILabel(int tab, int id, int x, int y, int width, int height, int[] text) {
		return _addUIElement(UI_LABEL, tab, id, 0, x, y, width, height, UI_UNSELECTED, text, 0, 0);
	}

	public int addUIPushButton(int tab, int id, int group, int x, int y, int width, int height, int[] text, int state) {
		return _addUIElement(UI_PUSH_BUTTON, tab, id, group, x, y, width, height, state, text, UI_UNSELECTED, UI_SELECTED);
	}

	public int addUIArea(int tab, int id, int x, int y, int width, int height) {
		return _addUIElement(UI_AREA, tab, id, 0, x, y, width, height, 0, null, 0, 0xFFFF);
	}

	public int addUIGauge(int tab, int id, int x, int y, int width, int height, int value, int min, int max) {
		return _addUIElement(UI_GAUGE, tab, id, 0, x, y, width, height, value, null, min, max);
	}

	public int addUISlider(int tab, int id, int x, int y, int width, int height, int value, int min, int max) {
		return _addUIElement(UI_SLIDER, tab, id, 0, x, y, width, height, value, null, min, max);
	}

	private int _addUIElement(int type, int tab, int id, int group, int x, int y, int width, int height, int value, int[] text, int min, int max) {
		if (_uiNbElements == UI_MAX_ELEMENTS) return UI_ERROR;
		_uiElement[_uiNbElements] = new uiElement_t();
		_uiElement[_uiNbElements].type = type;		
		_uiElement[_uiNbElements].tab = tab;
		_uiElement[_uiNbElements].id = id;
		_uiElement[_uiNbElements].group = group;
		_uiElement[_uiNbElements].x = x;
		_uiElement[_uiNbElements].y = y;
		_uiElement[_uiNbElements].width = width;
		_uiElement[_uiNbElements].height = height;
		_uiElement[_uiNbElements].text = text;
		_uiElement[_uiNbElements].min = min;
		_uiElement[_uiNbElements].max = max;
		if (value > max) _uiElement[_uiNbElements].value = max;
		else if (value < min) _uiElement[_uiNbElements].value = min;
		else _uiElement[_uiNbElements].value = value;
		if ((type == UI_GAUGE) || (type == UI_LABEL)) _uiElement[_uiNbElements].editable = false;
		else _uiElement[_uiNbElements].editable = true;
		if (width == UI_AUTO_SIZE) {
			if (text != null) _uiElement[_uiNbElements].width = getStringWidth(text, UI_FONT) + (type == UI_LABEL ? 0 : UI_ELEMENT_LEFT_MARGIN * 2);
			else return UI_ERROR;
		} else if ((width & UI_SAME_AS) != 0) {
			int ref = _getUIElementIndex(x & ~UI_PLACE_MODIFIER_MASK);
			if (ref == -1) return UI_ERROR;
			_uiElement[_uiNbElements].width = _uiElement[ref].width;
		}
		if (height == UI_AUTO_SIZE) _uiElement[_uiNbElements].height = getFontHeight(UI_FONT) + UI_ELEMENT_TOP_MARGIN * 2;
		else if ((height & UI_SAME_AS) != 0) {
			int ref = _getUIElementIndex(x & ~UI_PLACE_MODIFIER_MASK);
			if (ref == -1) return UI_ERROR;
			_uiElement[_uiNbElements].height = _uiElement[ref].height;
		}
		if ((x & UI_PLACE_MODIFIER_MASK) != 0) {
			int ref = _getUIElementIndex(x & ~UI_PLACE_MODIFIER_MASK);
			if (ref == -1) return UI_ERROR;
			if ((x & UI_SAME_AS) != 0) _uiElement[_uiNbElements].x = _uiElement[ref].x;
			else if ((x & UI_RIGHT_OF) != 0) _uiElement[_uiNbElements].x = _uiElement[ref].x+ _uiElement[ref].width-1;
			else if ((x & UI_LEFT_OF) != 0) _uiElement[_uiNbElements].x = _uiElement[ref].x- _uiElement[_uiNbElements].width+1;
			if ((x & UI_WITH_MARGIN) != 0) _uiElement[_uiNbElements].x += UI_ELEMENT_LEFT_MARGIN;
		}
		if ((y & UI_PLACE_MODIFIER_MASK) != 0) {
			int ref = _getUIElementIndex(x & ~UI_PLACE_MODIFIER_MASK);
			if (ref == -1) return UI_ERROR;
			if ((y & UI_SAME_AS) != 0) _uiElement[_uiNbElements].y = _uiElement[ref].y;
			else if ((y & UI_BOTTOM_OF) != 0) _uiElement[_uiNbElements].y = _uiElement[ref].y+ _uiElement[ref].height-1;
			else if ((y & UI_TOP_OF) != 0) _uiElement[_uiNbElements].y = _uiElement[ref].y- _uiElement[_uiNbElements].height+1;
			if ((y & UI_WITH_MARGIN) != 0) _uiElement[_uiNbElements].y += UI_ELEMENT_TOP_MARGIN;
		}
		return _uiNbElements++;
	}

	public void setUITab(int tabId) {
		_uiActiveTab = tabId;
		_uiActiveElement = -1;
		if (_uiNbTabs > 1) {
			this.fillRectangle(_uiX, _uiY, _uiWidth, _uiTabHeight, UI_COLOR_BACKGROUND);
			// draw screen headers	
			int ax = 0;
			int aw = 0;
			for (int i=0; i<_uiNbTabs; i++) {
				if (i != _uiActiveTab) {
					fillRectangle(_uiX+_uiTab[i].x, _uiY+_uiTab[i].y, _uiTab[i].width, _uiTabHeight-2, UI_COLOR_TAB_UNSELECTED);
					setBackgroundColor(UI_COLOR_TAB_UNSELECTED);
				} else setBackgroundColor(UI_COLOR_BACKGROUND);
				drawRectangle(_uiX+_uiTab[i].x, _uiY+_uiTab[i].y, _uiTab[i].width, _uiTabHeight-2, BLACK, 1);
				drawString(_uiTab[i].text, _uiX+_uiTab[i].x+UI_TAB_LEFT_MARGIN, _uiY+3+UI_TAB_TOP_MARGIN, BLACK, UI_TAB_FONT, UI_TAB_FONT_IS_BOLD, false);
				if (i == _uiActiveTab) {
					ax = _uiX+_uiTab[i].x+1;
					aw = _uiTab[i].width-3;
				}
			}
			// draw a line accross the width of the screen
			this.drawLine(_uiX, _uiY+_uiTabHeight-1, _uiX+_uiWidth, _uiY+_uiTabHeight-1, BLACK, 1);
			// show the active tab
			this.drawLine(ax, _uiY+_uiTabHeight-1, ax+aw, _uiY+_uiTabHeight-1, UI_COLOR_BACKGROUND, 1);
		}
		// clear screen area
		this.fillRectangle(_uiX, _uiY+_uiTabHeight, _uiWidth, _uiHeight, UI_COLOR_BACKGROUND);
		// draw the ui element
		for (int i=0; i<_uiNbElements; i++) {
			if (_uiElement[i].tab == tabId) _drawUIElement(_uiElement[i]);
		}
	}

	private void _drawUIElement(uiElement_t elt) {
		int color = UI_COLOR_BUTTON_RELEASED;
		if (elt.value == UI_PUSHED) color = UI_COLOR_BUTTON_PUSHED;
		switch (elt.type) {
			case UI_LABEL:
				setBackgroundColor(UI_COLOR_BACKGROUND);
				drawString(elt.text, _uiX+elt.x, _uiY+_uiTabHeight+elt.y+1+(elt.height-getFontHeight(UI_FONT))/2, BLACK, UI_FONT, UI_FONT_IS_BOLD, false);
				break;
			case UI_BUTTON:
				fillRoundedRectangle(_uiX+elt.x, _uiY+_uiTabHeight+elt.y, elt.width, elt.height, 6, color);
				drawRoundedRectangle(_uiX+elt.x, _uiY+_uiTabHeight+elt.y, elt.width, elt.height, 6, BLACK, 1);
				setBackgroundColor(color);
				drawString(elt.text, _uiX+elt.x+(elt.width-getStringWidth(elt.text, UI_FONT))/2, 
						_uiY+_uiTabHeight+elt.y+1+(elt.height-getFontHeight(UI_FONT))/2, BLACK, UI_FONT, UI_FONT_IS_BOLD, false);
				break;
			case UI_PUSH_BUTTON:
				fillRectangle(_uiX+elt.x, _uiY+_uiTabHeight+elt.y, elt.width, elt.height, color);
				drawRectangle(_uiX+elt.x, _uiY+_uiTabHeight+elt.y, elt.width, elt.height, BLACK, 1);
				setBackgroundColor(color);
				drawString(elt.text, _uiX+elt.x+(elt.width-getStringWidth(elt.text, UI_FONT))/2, 
						_uiY+_uiTabHeight+elt.y+1+(elt.height-getFontHeight(UI_FONT))/2, BLACK, UI_FONT, UI_FONT_IS_BOLD, false);
				break;
			case UI_AREA:
				if (_uiTab[elt.tab].drawer != null) _uiTab[elt.tab].drawer.uiDrawCallback(elt.id, _uiX+elt.x, _uiY+_uiTabHeight+elt.y, elt.width, elt.height, elt.value);
				else if (_uiDrawer != null) _uiDrawer.uiDrawCallback(elt.id, _uiX+elt.x, _uiY+_uiTabHeight+elt.y, elt.width, elt.height, elt.value);
				break;
			case UI_GAUGE:
				fillRectangle(_uiX+elt.x, _uiY+_uiTabHeight+elt.y, elt.width, elt.height, UI_COLOR_BACKGROUND);
				if (elt.width > elt.height) 
					 fillRectangle(_uiX+elt.x, _uiY+_uiTabHeight+elt.y, 1+(elt.width-1)*elt.value/(elt.max-elt.min), elt.height, UI_COLOR_GAUGE);
				else fillRectangle(_uiX+elt.x, _uiY+_uiTabHeight+elt.y+1+(elt.height-1)*(elt.max-elt.value)/(elt.max-elt.min),
							elt.width, (elt.height-1)*elt.value/(elt.max-elt.min), UI_COLOR_GAUGE);
				drawRectangle(_uiX+elt.x, _uiY+_uiTabHeight+elt.y, elt.width, elt.height, BLACK, 2);
				break;
			case UI_SLIDER:
				fillRectangle(_uiX+elt.x, _uiY+_uiTabHeight+elt.y, elt.width, elt.height, UI_COLOR_BACKGROUND);
				int x;
				int y;
				int w;
				int h;
				if (elt.width > elt.height) {
					drawLine(_uiX+elt.x+2, _uiY+_uiTabHeight+elt.y+elt.height/2, _uiX+elt.x+elt.width-2, _uiY+_uiTabHeight+elt.y+elt.height/2, BLACK, 4);
					x = elt.x + ((elt.width-UI_SLIDER_KNOB_SIZE)*elt.value)/(elt.max-elt.min);
					y = elt.y + 2;
					w = UI_SLIDER_KNOB_SIZE;
					h = elt.height-2;
				} else {
					drawLine(_uiX+elt.x+elt.width/2, _uiY+_uiTabHeight+elt.y+2, _uiX+elt.x+elt.width/2, _uiY+_uiTabHeight+elt.y+elt.height-2, BLACK, 4);
					x = elt.x + 2;
					y = elt.y + ((elt.height-UI_SLIDER_KNOB_SIZE)*(elt.max-elt.value))/(elt.max-elt.min);
					w = elt.width-2;
					h = UI_SLIDER_KNOB_SIZE;
				}
				fillRectangle(_uiX+x, _uiY+_uiTabHeight+y, w, h, UI_COLOR_BUTTON_PUSHED);
				drawRectangle(_uiX+x, _uiY+_uiTabHeight+y, w, h, BLACK, 1);
				break;
		}	
	}
	
	public void stopUI() {
		_uiActiveTab = -1;
	}
		
	private boolean _isTouchInUITab(uiTab_t tab, int x, int y) {
		if (x == TOUCHSCREEN_NO_TOUCH) return false;
		return ((x >_uiX+tab.x) && (x < _uiX+tab.x+tab.width) && (y < _uiY+_uiTabHeight));
	}

	private boolean _isTouchInUIElement(uiElement_t elt, int x, int y) {
		if (x == TOUCHSCREEN_NO_TOUCH) return false;
		return ((x >_uiX+elt.x) && (y > _uiY+_uiTabHeight+elt.y) && 
				(x < _uiX+elt.x+elt.width) && (y < _uiY+_uiTabHeight+elt.y+elt.height));
	}
	
	public void handleUI() {
		if (_uiActiveTab == -1) return;
		int[] x = new int[1];
		int[] y = new int[1];
		
		// get the touched point
		getTouchXYZ(x, y, null);
		
		// check if we need to change tab
		for (int i=0; i<_uiNbTabs; i++) {
			if (_isTouchInUITab(_uiTab[i], x[0], y[0])) {
				if (i != _uiActiveTab) setUITab(i);
				return;
			}
		}

		// find out which item is active
		int activeElement = -1;
		// find a new active element 
		for (int i=0; i<_uiNbElements; i++) {
			uiElement_t elt = _uiElement[i];
			if ((elt.tab == _uiActiveTab) && (_isTouchInUIElement(elt, x[0], y[0]))) {
				activeElement = i;
				break;
			}
		}
		// update the visual state of previously active element
		if ((_uiActiveElement != -1) && (activeElement != _uiActiveElement)) {
			// draw the old active element as released
			if (_uiElement[_uiActiveElement].editable) {
				_updateUIElementValue(_uiElement[_uiActiveElement], UI_RELEASED, x[0], y[0]);
				_drawUIElement(_uiElement[_uiActiveElement]);
			}
		}
		if ((activeElement != -1) && ((activeElement != _uiActiveElement) || (_uiElement[activeElement].type == UI_SLIDER))) {
			if (_uiElement[activeElement].editable) {
				boolean hasValueChanged = _updateUIElementValue(_uiElement[activeElement], UI_PUSHED, x[0], y[0]);
				if (hasValueChanged) {
					_drawUIElement(_uiElement[activeElement]);
					if (_uiTab[_uiElement[activeElement].tab].handler != null) 
						_uiTab[_uiElement[activeElement].tab].handler.uiActionCallback(_uiElement[activeElement].id);
					else if (_uiHandler != null) _uiHandler.uiActionCallback(_uiElement[activeElement].id);
				}
			}
			
		}
		_uiActiveElement = activeElement;
	}
	
	private boolean _updateUIElementValue(uiElement_t elt, int state, int x, int y) {
		boolean hasValueChanged = true;
		switch (elt.type) {
			case UI_BUTTON:
			case UI_AREA:
				elt.value = state;
				break;
			case UI_PUSH_BUTTON:
				if (state == UI_PUSHED) {
					if (elt.group == UI_NO_GROUP) elt.value = (elt.value == UI_UNSELECTED ? UI_SELECTED : UI_UNSELECTED);
					else  {
						// find all the PUSH_BUTTON in the group
						for (int i=0; i<_uiNbElements; i++) {
							if ((_uiElement[i].group == elt.group) && (_uiElement[i].tab == elt.tab)) {
								// for the selected, push it
								if ((_uiElement[i].id == elt.id) && (_uiElement[i].value == UI_UNSELECTED)) _uiElement[i].value = UI_SELECTED;
								else if ((_uiElement[i].id != elt.id) && (_uiElement[i].value == UI_SELECTED)) {
									_uiElement[i].value = UI_UNSELECTED;
									_drawUIElement(_uiElement[i]);
								}
							}
						}
					}
				} else hasValueChanged = false;
				break;
			case UI_SLIDER:
				if (state == UI_PUSHED) {
					int value = 0;
					if (elt.width > elt.height) {
						int minX = _uiX + elt.x;
						int maxX = _uiX + elt.x + elt.width - 1; 
						value = (x - minX) * (elt.max - elt.min) / (maxX - minX);
					} else {
						int minY = _uiY + _uiTabHeight + elt.y;
						int maxY = _uiY + _uiTabHeight + elt.y + elt.height - 1; 
						value = elt.max - (y - minY) * (elt.max - elt.min) / (maxY - minY);					
					}
					if (value != elt.value) elt.value = value;
					else hasValueChanged = false;
				}
				break;
		}
		return hasValueChanged;
	}
	
	public int getUIElementWidth(int id) {
		int index = _getUIElementIndex(id);
		if (index == UI_ERROR) return UI_ERROR;
		return _uiElement[index].width;
	}
	
	private int _getUIElementIndex(int id) {
		for (int i=0; i<_uiNbElements; i++) {
			if (_uiElement[i].id == id) return i;
		}
		return UI_ERROR;
	}
	
	public void setUIElementValue(int id, int value) {
		int index = _getUIElementIndex(id);
		if (index == UI_ERROR) return;
		if (value > _uiElement[index].max) _uiElement[index].value = _uiElement[index].max;
		else if (value < _uiElement[index].min) _uiElement[index].value = _uiElement[index].min;
		else _uiElement[index].value = value;
		if (_uiElement[index].tab == _uiActiveTab) _drawUIElement(_uiElement[index]);
	}

	public int getUIElementValue(int id) {
		int index = _getUIElementIndex(id);
		if (index == UI_ERROR) return UI_ERROR;
		return _uiElement[index].value;		
	}

	public void setUIElementEditable(int id, boolean editable) {
		int index = _getUIElementIndex(id);
		if (index == UI_ERROR) return;
		_uiElement[index].editable = editable;	
	}

}
