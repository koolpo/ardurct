package com.google.code.ardurct.libraries.touscruino;

import java.util.Date;


public class TouchScreen_UserInterface extends ArduRCT_Graphics {

	public class uiRectangle_t {
		public int x;
		public int y;
		public int width;
		public int height;		
	}
	
	public class uiTab_t extends uiRectangle_t {
		/* 
		 * RESTORE in Arduino
		public int x;
		public int y;
		public int width;
		public int height;
		*/
		int[] text;
		boolean enabled;
		IUIDrawCallback drawer; 
		IUIActionCallback handler; 
	}
	
	public class uiElement_t extends uiRectangle_t  {
		/* 
		 * RESTORE in Arduino
		public int x;
		public int y;
		public int width;
		public int height;
		*/
		int type;
		int tab;
		int id;
		int group;
		int[] text;
		int textX;
		int textY;
		public int value;
		int min;
		int max;
		boolean editable;
	}
	
	private static final long serialVersionUID = 1L;
	
	public static final int UI_BUTTON = 1;
	public static final int UI_PUSH_BUTTON = 2;
	public static final int UI_AREA = 3;
	public static final int UI_GAUGE = 4;
	public static final int UI_SLIDER = 5;
	public static final int UI_LABEL = 6;

	public static int UI_COLOR_BACKGROUND = WHITE;
	public static int UI_COLOR_POPUP_BACKGROUND = 0xF6F6;
	public static int UI_COLOR_TAB_UNSELECTED = LIGHT_GREY;
	public static int UI_COLOR_BUTTON_PUSHED = F_GREY;
	public static int UI_COLOR_BUTTON_RELEASED = LIGHT_GREY;
	public static int UI_COLOR_GAUGE = 0x7C7C;

	public static int UI_POPUP_MARGIN = 15;
	public static int UI_POPUP_PADDING = 10;
	public static int UI_POPUP_TITLE_FONT = FONT_MEDIUM;
	public static boolean UI_POPUP_TITLE_FONT_IS_BOLD = FONT_BOLD;
	public static int UI_POPUP_MESSAGE_FONT = FONT_SMALL;
	public static boolean UI_POPUP_MESSAGE_FONT_IS_BOLD = FONT_PLAIN;
		

	public static int UI_TAB_FONT = FONT_MEDIUM;
	public static boolean UI_TAB_FONT_IS_BOLD = true;
	public static int UI_TAB_TOP_MARGIN = 12;
	public static int UI_TAB_LEFT_MARGIN = 10;

	public static int UI_ELEMENT_FONT = FONT_MEDIUM;
	public static boolean UI_ELEMENT_FONT_IS_BOLD = true;

	public static int UI_ELEMENT_TOP_MARGIN = 13;
	public static int UI_ELEMENT_LEFT_MARGIN = 16;
	
	public static int UI_SLIDER_KNOB_SIZE = 16;
	
	public static final int UI_PUSHED = 1;
	public static final int UI_RELEASED = 0;
	
	static final int UI_POPUP_NONE = 0;
	static final int UI_POPUP_APPEARING = 1;
	static final int UI_POPUP_DISAPPEARING = 2;
	static final int UI_POPUP_SHOWN = 3;
	
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
	long _uiNextHandleTime;
	int _uiDebounceSteps;
	int _uiDebouncedActiveElement;
	boolean _uiIsShown;
	int _uiPopupState;
	
	public void setupUI(int x, int y, int width, int height) {
		setupUI(x, y, width, height, null, null);
	}
	
	public void setupUI(int x, int y, int width, int height, IUIActionCallback handler) {
		setupUI(x, y, width, height, handler, null);
	}

	public void setupUI(int x, int y, int width, int height, IUIActionCallback handler, IUIDrawCallback drawer) {
		_uiX = x;
		_uiY = y;
		_uiWidth = width;
		_uiHeight = height;
		_uiTabHeight = 0;
		_uiElement = new uiElement_t[CONFIGURATION_UI_MAX_ELEMENTS];
		_uiTab = new uiTab_t[CONFIGURATION_UI_MAX_TABS];
		_uiNbElements = 0;
		_uiActiveElement = -1;
		_uiNbTabs = 0;
		_uiActiveTab = -1;
		_uiDrawer = drawer; 
		_uiHandler = handler;
		_uiNextHandleTime = 0;
		_uiDebounceSteps = 0;
		_uiDebouncedActiveElement = -1;
		_uiIsShown = false;
		_uiPopupState = UI_POPUP_NONE;
	}

	public int addUITab(int[] text) {
		return addUITab(text, null, null);
	}
	
	public int addUITab(int[] text, IUIActionCallback handler) {
		return addUITab(text, handler, null);
	}
	
	public int addUITab(int[] text, IUIActionCallback handler, IUIDrawCallback drawer) {
		if (_uiNbTabs == CONFIGURATION_UI_MAX_TABS) return UI_ERROR;
		_uiTab[_uiNbTabs] = new uiTab_t();
		if (_uiNbTabs == 0) _uiTab[_uiNbTabs].x = UI_TAB_LEFT_MARGIN + _uiX;
		else _uiTab[_uiNbTabs].x = _uiTab[_uiNbTabs-1].x+_uiTab[_uiNbTabs-1].width-1;
		if (_uiTab[_uiNbTabs].x > _uiWidth) return UI_ERROR;
		_uiTab[_uiNbTabs].y = 2 + _uiY;
		_uiTab[_uiNbTabs].enabled = true;
		_uiTab[_uiNbTabs].text = text;
		_uiTab[_uiNbTabs].width = getStringWidth(_uiTab[_uiNbTabs].text, UI_TAB_FONT) + UI_TAB_LEFT_MARGIN * 2;
		_uiTab[_uiNbTabs].height = getFontHeight(UI_TAB_FONT) + UI_TAB_TOP_MARGIN*2;
		if (_uiNbTabs > 0) _uiTabHeight = _uiTab[_uiNbTabs].height + 2;
		_uiTab[_uiNbTabs].drawer = drawer;
		_uiTab[_uiNbTabs].handler = handler;
		return _uiNbTabs++;
	}
	
	public int addUIButton(int tab, int id, int x, int y, int width, int height, String text) {
		return _addUIElement(UI_BUTTON,tab, id, 0, x, y, width, height, UI_RELEASED, text, UI_RELEASED, UI_PUSHED);
	}

	public int addUILabel(int tab, int id, int x, int y, int width, int height, String text) {
		return _addUIElement(UI_LABEL, tab, id, 0, x, y, width, height, UI_UNSELECTED, text, 0, 0);
	}

	public int addUIPushButton(int tab, int id, int group, int x, int y, int width, int height, String text, int state) {
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

	public static int[] stringToArray(String s) {
		if (s == null) return null;
		int[] buffer = new int[s.length()+1];
		for (int i=0; i<s.length(); i++) buffer[i] = s.charAt(i);
		buffer[s.length()] = 0;
		return buffer;
	}

	private int _addUIElement(int type, int tab, int id, int group, int x, int y, int width, int height, int value, String text, int min, int max) {
		if (_uiNbElements == CONFIGURATION_UI_MAX_ELEMENTS) return UI_ERROR;
		_uiElement[_uiNbElements] = new uiElement_t();
		_uiElement[_uiNbElements].type = type;		
		_uiElement[_uiNbElements].tab = tab;
		_uiElement[_uiNbElements].id = id;
		_uiElement[_uiNbElements].group = group;
		_uiElement[_uiNbElements].x = x + _uiX;
		_uiElement[_uiNbElements].y = y + _uiY + _uiTabHeight;
		_uiElement[_uiNbElements].width = width;
		_uiElement[_uiNbElements].height = height;
		_uiElement[_uiNbElements].text = stringToArray(text);
		_uiElement[_uiNbElements].min = min;
		_uiElement[_uiNbElements].max = max;
		if (value > max) _uiElement[_uiNbElements].value = max;
		else if (value < min) _uiElement[_uiNbElements].value = min;
		else _uiElement[_uiNbElements].value = value;
		if ((type == UI_GAUGE) || (type == UI_LABEL)) _uiElement[_uiNbElements].editable = false;
		else _uiElement[_uiNbElements].editable = true;
		if (width == UI_AUTO_SIZE) {
			if (text != null) _uiElement[_uiNbElements].width = getStringWidth(_uiElement[_uiNbElements].text, UI_ELEMENT_FONT) + (type == UI_LABEL ? 0 : UI_ELEMENT_LEFT_MARGIN * 2);
			else return UI_ERROR;
		} else if ((width & UI_SAME_AS) != 0) {
			int ref = _getUIElementIndex(width & ~UI_PLACE_MODIFIER_MASK);
			if (ref == -1) return UI_ERROR;
			_uiElement[_uiNbElements].width = _uiElement[ref].width;
		}
		if (height == UI_AUTO_SIZE) _uiElement[_uiNbElements].height = getFontHeight(UI_ELEMENT_FONT) + UI_ELEMENT_TOP_MARGIN * 2;
		else if ((height & UI_SAME_AS) != 0) {
			int ref = _getUIElementIndex(height & ~UI_PLACE_MODIFIER_MASK);
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
			int ref = _getUIElementIndex(y & ~UI_PLACE_MODIFIER_MASK);
			if (ref == -1) return UI_ERROR;
			if ((y & UI_SAME_AS) != 0) _uiElement[_uiNbElements].y = _uiElement[ref].y;
			else if ((y & UI_BOTTOM_OF) != 0) _uiElement[_uiNbElements].y = _uiElement[ref].y+ _uiElement[ref].height-1;
			else if ((y & UI_TOP_OF) != 0) _uiElement[_uiNbElements].y = _uiElement[ref].y- _uiElement[_uiNbElements].height+1;
			if ((y & UI_WITH_MARGIN) != 0) _uiElement[_uiNbElements].y += UI_ELEMENT_TOP_MARGIN;
		}
		if (text != null) {
			int fontSize = UI_ELEMENT_FONT;
			if (((_uiElement[_uiNbElements].text[0] == '+') || (_uiElement[_uiNbElements].text[0] == '-')) && (_uiElement[_uiNbElements].text[1] == 0)) 
				fontSize = FONT_HUGE;
			_uiElement[_uiNbElements].textX = _uiElement[_uiNbElements].x + 
				(_uiElement[_uiNbElements].width-getStringWidth(_uiElement[_uiNbElements].text, fontSize))/2;
			_uiElement[_uiNbElements].textY = _uiElement[_uiNbElements].y + 1 + (_uiElement[_uiNbElements].height-getFontHeight(fontSize))/2;
		}
		return _uiNbElements++;
	}

	public void setUITab(int tabId) {
		if (tabId >= CONFIGURATION_UI_MAX_TABS) return; 
		_uiActiveTab = tabId;
		_uiActiveElement = -1;
		_uiDebouncedActiveElement = -1;
		_uiIsShown = false;
	}
	
	private void _drawActiveUITab() {
		if (_uiNbTabs > 1) {
			this.fillRectangle(_uiX, _uiY, _uiWidth, _uiTabHeight, UI_COLOR_BACKGROUND);
			// draw screen headers	
			int ax = 0;
			int aw = 0;
			for (int i=0; i<_uiNbTabs; i++) {
				if (i != _uiActiveTab) {
					fillRectangle(_uiX+_uiTab[i].x, _uiY+_uiTab[i].y, _uiTab[i].width, _uiTab[i].height, UI_COLOR_TAB_UNSELECTED);
					setBackgroundColor(UI_COLOR_TAB_UNSELECTED);
				} else setBackgroundColor(UI_COLOR_BACKGROUND);
				drawRectangle(_uiTab[i].x, _uiTab[i].y, _uiTab[i].width,  _uiTab[i].height, BLACK, 1);
				drawString(_uiTab[i].text, _uiTab[i].x+UI_TAB_LEFT_MARGIN, _uiY+3+UI_TAB_TOP_MARGIN, BLACK, UI_TAB_FONT, UI_TAB_FONT_IS_BOLD, false);
				if (_uiTab[i].drawer != null) _uiTab[i].drawer.uiDrawCallback(UI_DRAW_CALLBACK_TAB_ID, _uiTab[i].x, _uiTab[i].y, 
						_uiTab[i].width, _uiTab[i].height, i == _uiActiveTab ? UI_SELECTED : UI_UNSELECTED);
				if (i == _uiActiveTab) {
					ax = _uiTab[i].x+1;
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
			if (_uiElement[i].tab == _uiActiveTab) _drawUIElement(_uiElement[i]);
		}
		_uiIsShown = true;
	}

	private void _drawUIElement(uiElement_t elt) {
		int bgColor = getBackgroundColor();
		int color = UI_COLOR_BUTTON_RELEASED;
		if (elt.value == UI_PUSHED) color = UI_COLOR_BUTTON_PUSHED;
		int fontSize = UI_ELEMENT_FONT;
		if (elt.text != null) {
			if (((elt.text[0] == '+') || (elt.text[0] == '-')) && (elt.text[1] == 0)) fontSize = FONT_HUGE;			
		}
		switch (elt.type) {
			case UI_LABEL:
				setBackgroundColor(UI_COLOR_BACKGROUND);
				drawString(elt.text, elt.x, elt.textY, BLACK, fontSize, UI_ELEMENT_FONT_IS_BOLD, false);
				break;
			case UI_BUTTON:
				fillRoundedRectangle(elt.x, elt.y, elt.width, elt.height, UI_ELEMENT_TOP_MARGIN*80/100, color);
				drawRoundedRectangle(elt.x, elt.y, elt.width, elt.height, UI_ELEMENT_TOP_MARGIN*80/100, BLACK, 1);
				setBackgroundColor(color);
				drawString(elt.text, elt.textX, elt.textY, BLACK, fontSize, UI_ELEMENT_FONT_IS_BOLD, false);
				break;
			case UI_PUSH_BUTTON:
				fillRectangle(elt.x, elt.y, elt.width, elt.height, color);
				drawRectangle(elt.x, elt.y, elt.width, elt.height, BLACK, 1);
				setBackgroundColor(color);
				drawString(elt.text, elt.textX, elt.textY, BLACK, fontSize, UI_ELEMENT_FONT_IS_BOLD, false);
				break;
			case UI_AREA:
				if (_uiTab[elt.tab].drawer != null) _uiTab[elt.tab].drawer.uiDrawCallback(elt.id, elt.x, elt.y, elt.width, elt.height, elt.value);
				else if (_uiDrawer != null) _uiDrawer.uiDrawCallback(elt.id, elt.x, elt.y, elt.width, elt.height, elt.value);
				break;
			case UI_GAUGE:
				fillRectangle(elt.x, elt.y, elt.width, elt.height, UI_COLOR_BACKGROUND);
				if (elt.width > elt.height) fillRectangle(elt.x, elt.y, 1+(elt.width-1)*elt.value/(elt.max-elt.min), elt.height, UI_COLOR_GAUGE);
				else fillRectangle(elt.x, elt.y+1+(elt.height-1)*(elt.max-elt.value)/(elt.max-elt.min),
							elt.width, (elt.height-1)*elt.value/(elt.max-elt.min), UI_COLOR_GAUGE);
				drawRectangle(elt.x, elt.y, elt.width, elt.height, BLACK, 2);
				break;
			case UI_SLIDER:
				fillRectangle(elt.x, elt.y, elt.width, elt.height, UI_COLOR_BACKGROUND);
				int x;
				int y;
				int w;
				int h;
				if (elt.width > elt.height) {
					drawLine(elt.x+2, elt.y+elt.height/2, elt.x+elt.width-2, elt.y+elt.height/2, BLACK, 4);
					x = elt.x + ((elt.width-UI_SLIDER_KNOB_SIZE)*elt.value)/(elt.max-elt.min);
					y = elt.y + 2;
					w = UI_SLIDER_KNOB_SIZE;
					h = elt.height-2;
				} else {
					drawLine(elt.x+elt.width/2, +elt.y+2, elt.x+elt.width/2, elt.y+elt.height-2, BLACK, 4);
					x = elt.x + 2;
					y = elt.y + ((elt.height-UI_SLIDER_KNOB_SIZE)*(elt.max-elt.value))/(elt.max-elt.min);
					w = elt.width-2;
					h = UI_SLIDER_KNOB_SIZE;
				}
				fillRectangle(x, y, w, h, UI_COLOR_BUTTON_PUSHED);
				drawRectangle(x, y, w, h, BLACK, 1);
				break;
		}	
		setBackgroundColor(bgColor);
	}
	
	public void stopUI() {
		_uiActiveTab = -1;
	}
		
	private boolean _isTouchInUITab(uiTab_t tab, int x, int y) {
		if (x == TOUCHSCREEN_NO_TOUCH) return false;
		return ((x > tab.x) && (x < tab.x+tab.width) && (y < _uiY+_uiTabHeight));
	}

	private boolean _isTouchInUIElement(uiElement_t elt, int x, int y) {
		if (x == TOUCHSCREEN_NO_TOUCH) return false;
		return ((x > elt.x) && (y > elt.y) && (x < elt.x+elt.width) && (y < elt.y+elt.height));
	}
	
	private long millis() {
		return (new Date()).getTime();
	}

	public void handleUI() {
		if (_uiActiveTab == -1) return;
		int[] x = new int[1];
		int[] y = new int[1];
				
		// check if enough time has elapsed to run the loop
		if ((long)(millis() - _uiNextHandleTime) < 0) return;
		_uiNextHandleTime = millis() + CONFIGURATION_UI_LOOP_LENGTH;

		// manage popups
		if ((_uiPopupState == UI_POPUP_APPEARING) || (_uiPopupState == UI_POPUP_DISAPPEARING)) {
			// debounce the button which made the popup appear, or the touch which made the popup disappear
			if (getTouchXYZ(x, y, null) == TOUCHSCREEN_NO_TOUCH) _uiDebounceSteps ++;
			else _uiDebounceSteps = 0;
			if (_uiDebounceSteps >= CONFIGURATION_UI_DEBOUNCE_STEPS) {
				if (_uiPopupState == UI_POPUP_APPEARING) _uiPopupState = UI_POPUP_SHOWN;
				else _uiPopupState = UI_POPUP_NONE;
			}
			return;
		} else if (_uiPopupState == UI_POPUP_SHOWN) {
			if (getTouchXYZ(x, y, null) != TOUCHSCREEN_NO_TOUCH) _uiDebounceSteps ++;
			else _uiDebounceSteps = 0;
			if (_uiDebounceSteps < CONFIGURATION_UI_DEBOUNCE_STEPS) return;
			_uiPopupState = UI_POPUP_DISAPPEARING;
			_uiIsShown = false;
			return;
		}
		// draw the UI if required
		if (!_uiIsShown) _drawActiveUITab();
				
		// find out which item is active
		int activeElement = -1;
		if (getTouchXYZ(x, y, null) != TOUCHSCREEN_NO_TOUCH) {
			// check if we need to change tab, no need to debounce
			for (int i=0; i<_uiNbTabs; i++) {
				if (_isTouchInUITab(_uiTab[i], x[0], y[0])) {
					if (i != _uiActiveTab && _uiTab[i].enabled) setUITab(i);
					return;
				}
			}
			// find a new active element 
			for (int i=0; i<_uiNbElements; i++) {
				if ((_uiElement[i].tab == _uiActiveTab) && (_isTouchInUIElement(_uiElement[i], x[0], y[0]))) {
					activeElement = i;
					break;
				}
			}
		}

		// if nothing WAS touched, and nothing IS touched, return
		if ((activeElement == -1) && (_uiActiveElement == -1)) return;

		// draw the new element, no debouncing on entering to ensure fast visual response
		if ((activeElement != -1) && (activeElement != _uiActiveElement)) {
			_uiActiveElement = activeElement;
			_uiDebouncedActiveElement = activeElement;
			if (_uiElement[activeElement].editable) {
				boolean hasValueChanged = _updateUIElementValue(_uiElement[activeElement], UI_PUSHED, x[0], y[0]);
				if (hasValueChanged) {
					_drawUIElement(_uiElement[activeElement]);
					// try to run the active tab handler or if not defined, the default handler
					if (_uiTab[_uiActiveTab].handler != null) _uiTab[_uiActiveTab].handler.uiActionCallback(_uiElement[activeElement].id);
					else if (_uiHandler != null) _uiHandler.uiActionCallback(_uiElement[activeElement].id);
				}
			}	
		}
		
		// no debouncing for sliders
		if (_uiElement[_uiActiveElement].type == UI_SLIDER) {
			_uiActiveElement = activeElement;
			return;
		}
		
		// debounce on release only, the function exits if there are not enough debounced steps 
		if (_uiDebounceSteps < CONFIGURATION_UI_DEBOUNCE_STEPS) {
			if (activeElement != _uiDebouncedActiveElement) _uiDebounceSteps = 0;
			else _uiDebounceSteps ++;
			if (_uiDebounceSteps > CONFIGURATION_UI_DEBOUNCE_STEPS+1) _uiDebounceSteps = CONFIGURATION_UI_DEBOUNCE_STEPS+1;
			_uiDebouncedActiveElement = activeElement;
			return;
		}

		// update the visual state of previously active element
		if ((_uiActiveElement != -1) && (activeElement != _uiActiveElement)) {	
			// draw the previously active element as released
			if (_uiElement[_uiActiveElement].editable) {
				_updateUIElementValue(_uiElement[_uiActiveElement], UI_RELEASED, x[0], y[0]);
				_drawUIElement(_uiElement[_uiActiveElement]);
			}
			_uiActiveElement = activeElement;
		}

		_uiDebouncedActiveElement = -1;
		_uiDebounceSteps = 0;
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
						int minX = elt.x;
						int maxX = elt.x + elt.width - 1; 
						value = (x - minX) * (elt.max - elt.min) / (maxX - minX);
					} else {
						int minY = elt.y;
						int maxY = elt.y + elt.height - 1; 
						value = elt.max - (y - minY) * (elt.max - elt.min) / (maxY - minY);					
					}
					if (value != elt.value) elt.value = value;
					else hasValueChanged = false;
				}
				break;
		}
		return hasValueChanged;
	}
		
	public uiRectangle_t getUIElementBounds(int id) {
		int index = _getUIElementIndex(id);
		if (index == UI_ERROR) return null;
		return _uiElement[index];
	}

	public uiRectangle_t getUITabBounds(int tabId) {
		if (tabId >= _uiNbTabs) return null;
		return _uiTab[tabId];
	}

	public int getUITabHeight() {
		return _uiTabHeight - 2;
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
		if (_uiActiveTab != -1 && _uiElement[index].tab == _uiActiveTab) _drawUIElement(_uiElement[index]);
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

	public int[] getUIElementLabel(int id) {
		int index = _getUIElementIndex(id);
		if (index == UI_ERROR) return null;
		return _uiElement[index].text;
	}

	public void setUIElementLabel(int id, String text) {
		setUIElementLabel(id, stringToArray(text));
	}

	public void setUIElementLabel(int id, int[] text) {
		int index = _getUIElementIndex(id);
		if (index == UI_ERROR) return;
		_uiElement[index].text = text;
		if (_uiActiveTab != -1 && _uiElement[index].tab == _uiActiveTab) _drawUIElement(_uiElement[index]);
	}

	public void setUITabEnabled(int tabId, boolean enabled) {
		if (tabId >= _uiNbTabs) return; 
		_uiTab[tabId].enabled = enabled;
	}

	public int getUIGroupSelectedElement(int groupId) {
		for (int i=0; i<_uiNbElements; i++) {
			if (_uiElement[i].group == groupId && _uiElement[i].value == UI_SELECTED) return _uiElement[i].id;
		}
		return UI_ERROR;
	}
	
	public void setUIElementTab(int id, int tabId) {
		int index = _getUIElementIndex(id);
		if (index == UI_ERROR) return;
		if (_uiElement[index].tab == tabId) return;
		_uiElement[index].tab = tabId;	
		if (_uiActiveTab != -1) {
			if (_uiElement[index].tab == _uiActiveTab) _drawUIElement(_uiElement[index]);
			else fillRectangle(_uiX+_uiElement[index].x, _uiY+_uiTabHeight+_uiElement[index].y, _uiElement[index].width, _uiElement[index].height, UI_COLOR_BACKGROUND);
		}
	}

	public void showUIPopup(String title, String message, int type) {
		showUIPopup(stringToArray(title), stringToArray(message), type);
	}

	public void showUIPopup(int[] title, int[] message, int color) {
		// let all the buttons be shown as released
		for (int i=0; i<_uiNbElements; i++) {
			if ((_uiElement[i].tab == _uiActiveTab) && (_uiElement[i].type == UI_BUTTON)) {
				_uiElement[i].value = UI_RELEASED;
				_drawUIElement(_uiElement[i]);
			}
		}
		// calculate the window width and height
		int w[] = new int[1];
		int h[] = new int[1];
		int height = 0;
		int width = 0;
		int tHeight = 0;
		if (title != null) {
			getStringBoundingBox(title, FONT_MEDIUM, FONT_BOLD, UI_POPUP_MARGIN + UI_POPUP_PADDING, w, h);
			height = h[0];
			width = w[0];
			tHeight = h[0]+3;
		}
		if (message != null) {
			getStringBoundingBox(message, FONT_SMALL, FONT_PLAIN, UI_POPUP_MARGIN + UI_POPUP_PADDING, w, h);
			height += (h[0] != 0 ? 3 : 0) + h[0];
			if (w[0] > width) width = w[0];
		}
		// popup the window
		if (_uiPopupState != UI_POPUP_SHOWN) _uiPopupState = UI_POPUP_APPEARING;
		int popupX = (getWidth()-width-UI_POPUP_MARGIN-UI_POPUP_PADDING)/2;
		int popupY = (getHeight()-height-UI_POPUP_MARGIN-UI_POPUP_PADDING)/2;
		int popupWidth = width + 2*UI_POPUP_PADDING;
		int popupHeight = height + 2*UI_POPUP_PADDING;
		setBackgroundColor(UI_COLOR_POPUP_BACKGROUND);
		fillRoundedRectangle(popupX, popupY, popupWidth, popupHeight, 6, UI_COLOR_POPUP_BACKGROUND);
		drawRoundedRectangle(popupX, popupY, popupWidth, popupHeight, 6, BLACK, 2);
		if (title != null) drawString(title, popupX+UI_POPUP_PADDING, popupY+UI_POPUP_PADDING, color, 
				UI_POPUP_TITLE_FONT, UI_POPUP_TITLE_FONT_IS_BOLD, NO_OVERLAY, UI_POPUP_MARGIN+UI_POPUP_PADDING);
		if (message != null) drawString(message, popupX+UI_POPUP_PADDING, popupY+UI_POPUP_PADDING+tHeight, BLACK, 
				UI_POPUP_MESSAGE_FONT, UI_POPUP_MESSAGE_FONT_IS_BOLD, NO_OVERLAY, UI_POPUP_MARGIN+UI_POPUP_PADDING);
	}
	
	public void hideUIPopups() {
		_uiPopupState = UI_POPUP_NONE;
		_uiIsShown = false;
	}
	
	public int getUISelectedTab() {
		return _uiActiveTab;
	}
		
}
