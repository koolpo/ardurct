/*
 * UserInterface - User Interface elements
 *	Part of Touchscreen class
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include "configuration.hpp"

#if defined(CONFIGURATION_HAS_MACROS)

#include "UserInterface.hpp"

#define UI_BUTTON 1
#define UI_PUSH_BUTTON 2
#define UI_AREA 3
#define UI_GAUGE 4
#define UI_SLIDER 5
#define UI_LABEL 6

void setupUI(int16_t x, int16_t y, uint16_t width, uint16_t height) {
	if (_xm = 0xFF) setupTouchPanel();
	_uiX = x;
	_uiY = y;
	_uiWidth = width;
	_uiHeight = height;
	_uiNbTabs = 0;
	_uiActiveTab = -1;
	_uiTabHeight = 0;
	_uiNbElements = 0;
	_uiActiveElement = -1;
}

int16_t addUITab(uint8_t *text) {
	if (_uiNbTabs == UI_MAX_TABS) return UI_ERROR;
	if (_uiNbTabs == 0) _uiTab[_uiNbTabs].x = UI_TAB_LEFT_MARGIN;
	else _uiTab[_uiNbTabs].x = _uiTab[_uiNbTabs-1].x+_uiTab[_uiNbTabs-1].width-1;
	if (_uiTab[_uiNbTabs].x > _uiWidth) return UI_ERROR;
	_uiTab[_uiNbTabs].y = 2;
	_uiTab[_uiNbTabs].text = text;
	_uiTabHeight = getFontHeight(UI_TAB_FONT) + 2 + UI_TAB_TOP_MARGIN*2;
	_uiTab[_uiNbTabs].width = getStringWidth(_uiTab[_uiNbTabs].text, UI_TAB_FONT) + UI_TAB_LEFT_MARGIN * 2;
	return _uiNbTabs++;
}

int16_t addUIButton(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t *text, void (*handle)(uint8_t id)) {
	return _addUIElement(UI_BUTTON,tab, id, 0, x, y, width, height, UI_RELEASED, text, UI_RELEASED, UI_PUSHED, handle, 0);
}

int16_t addUILabel(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t *text) {
	return _addUIElement(UI_LABEL, tab, id, 0, x, y, width, height, UI_UNSELECTED, text, 0, 0, 0, 0);
}

int16_t addUIPushButton(int16_t tab, uint8_t id, int16_t group, int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t *text, uint16_t state, void (*handle)(uint8_t id)) {
	return _addUIElement(UI_PUSH_BUTTON, tab, id, group, x, y, width, height, state, text, UI_UNSELECTED, UI_SELECTED, handle, 0);
}

int16_t addUIArea(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, void (*handle)(uint8_t id), 
		void (*draw)(uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value)) {
	return _addUIElement(UI_AREA, tab, id, 0, x, y, width, height, 0, 0, 0, 0xFFFF, handle, draw);
}

int16_t addUIGauge(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value, int16_t min, int16_t max) {
	return _addUIElement(UI_GAUGE, tab, id, 0, x, y, width, height, value, 0, min, max, 0, 0);
}

int16_t addUISlider(int16_t tab, uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value, int16_t min, int16_t max, void (*handle)(uint8_t id)) {
	return _addUIElement(UI_SLIDER, tab, id, 0, x, y, width, height, value, 0, min, max, handle, 0);
}


void setUITab(uint8_t tabId) {
	_uiActiveTab = tabId;
	_uiActiveElement = -1;
	if (_uiNbTabs > 1) {
		fillRectangle(_uiX, _uiY, _uiWidth, _uiTabHeight, UI_COLOR_BACKGROUND);
		// draw screen headers	
		int16_t ax = 0;
		uint16_t aw = 0;
		for (uint8_t i=0; i<_uiNbTabs; i++) {
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
		drawLine(_uiX, _uiY+_uiTabHeight-1, _uiX+_uiWidth, _uiY+_uiTabHeight-1, BLACK, 1);
		// show the active tab
		drawLine(ax, _uiY+_uiTabHeight-1, ax+aw, _uiY+_uiTabHeight-1, UI_COLOR_BACKGROUND, 1);
	}
	// clear screen area
	fillRectangle(_uiX, _uiY+_uiTabHeight, _uiWidth, _uiHeight, UI_COLOR_BACKGROUND);
	// draw the ui element
	for (uint8_t i=0; i<_uiNbElements; i++) {
		if (_uiElement[i].tab == tabId) _drawUIElement(&(_uiElement[i]));
	}
}


void stopUI() {
	_uiActiveTab = -1;
}
	
void handleUI() {
	if (_uiActiveTab == -1) return;
	int16_t x;
	int16_t y;
	int16_t z;
	
	// get the touched point
	getTouchXYZ(&x, &y, &z);
	
	// check if we need to change tab
	for (uint8_t i=0; i<_uiNbTabs; i++) {
		if (_isTouchInUITab(&(_uiTab[i]), x, y)) {
			if (i != _uiActiveTab) setUITab(i);
			return;
		}
	}

	// find out which item is active
	int16_t activeElement = -1;
	// find a new active element 
	for (uint8_t i=0; i<_uiNbElements; i++) {
		if ((_uiElement[i].tab == _uiActiveTab) && (_isTouchInUIElement(&(_uiElement[i]), x, y))) {
			activeElement = i;
			break;
		}
	}
	// update the visual state of previously active element
	if ((_uiActiveElement != -1) && (activeElement != _uiActiveElement)) {
		// draw the old active element as released
		if (_uiElement[_uiActiveElement].editable) {
			_updateUIElementValue(_uiElement[_uiActiveElement], UI_RELEASED, x, y);
			_drawUIElement(_uiElement[_uiActiveElement]);
		}
	}
	if ((activeElement != -1) && ((activeElement != _uiActiveElement) || (_uiElement[activeElement].type == UI_SLIDER))) {
		if (_uiElement[activeElement].editable) {
			boolean hasValueChanged = _updateUIElementValue(&(_uiElement[activeElement]), UI_PUSHED, x, y);
			if (hasValueChanged) {
				_drawUIElement(&(_uiElement[activeElement]));
				if (_uiElement[activeElement].handle) 
					(*(_uiElement[activeElement].handle))(_uiElement[activeElement].id);
			}
		}
		
	}
	_uiActiveElement = activeElement;
}


uint16_t getUIElementWidth(uint8_t id) {
	int16_t index = _getUIElementIndex(id);
	if (index == UI_ERROR) return UI_ERROR;
	return _uiElement[index].width;
}

void setUIElementValue(uint8_t id, int16_t value) {
	int16_t index = _getUIElementIndex(id);
	if (index == UI_ERROR) return;
	if (value > _uiElement[index].max) _uiElement[index].value = _uiElement[index].max;
	else if (value < _uiElement[index].min) _uiElement[index].value = _uiElement[index].min;
	else _uiElement[index].value = value;
	if (_uiElement[index].tab == _uiActiveTab) _drawUIElement(_uiElement[index]);
}

int16_t getUIElementValue(uint8_t id) {
	int16_t index = _getUIElementIndex(id);
	if (index == UI_ERROR) return UI_ERROR;
	return _uiElement[index].value;		
}

void setUIElementEditable(uint8_t id, bool editable) {
	int16_t index = _getUIElementIndex(id);
	if (index == UI_ERROR) return;
	_uiElement[index].editable = editable;	
}

/* ---------------- Private functions ------------------------ */

int16_t _addUIElement(int16_t type, int16_t tab, uint8_t id, int16_t group, int16_t x, int16_t y, int16_t width, int16_t height, int16_t value, uint8_t *text, int16_t min, int16_t max,
		void (*handle)(uint8_t id), void (*draw)(uint8_t id, int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t value)) {
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
	_uiElement[_uiNbElements].draw = draw;
	_uiElement[_uiNbElements].handle = handle;
	if ((type == UI_GAUGE) || (type == UI_LABEL)) _uiElement[_uiNbElements].editable = false;
	else _uiElement[_uiNbElements].editable = true;
	if (width == UI_AUTO_SIZE) {
		if (text != null) _uiElement[_uiNbElements].width = getStringWidth(text, UI_FONT) + (type == UI_LABEL ? 0 : UI_ELEMENT_LEFT_MARGIN * 2);
		else return UI_ERROR;
	} else if ((width & UI_SAME_AS) != 0) {
		int16_t ref = _getUIElementIndex(x & ~UI_PLACE_MODIFIER_MASK);
		if (ref == -1) return UI_ERROR;
		_uiElement[_uiNbElements].width = _uiElement[ref].width;
	}
	if (height == UI_AUTO_SIZE) _uiElement[_uiNbElements].height = getFontHeight(UI_FONT) + UI_ELEMENT_TOP_MARGIN * 2;
	else if ((height & UI_SAME_AS) != 0) {
		int16_t ref = _getUIElementIndex(x & ~UI_PLACE_MODIFIER_MASK);
		if (ref == -1) return UI_ERROR;
		_uiElement[_uiNbElements].height = _uiElement[ref].height;
	}
	if ((x & UI_PLACE_MODIFIER_MASK) != 0) {
		int16_t ref = _getUIElementIndex(x & ~UI_PLACE_MODIFIER_MASK);
		if (ref == -1) return UI_ERROR;
		if ((x & UI_SAME_AS) != 0) _uiElement[_uiNbElements].x = _uiElement[ref].x;
		else if ((x & UI_RIGHT_OF) != 0) _uiElement[_uiNbElements].x = _uiElement[ref].x+ _uiElement[ref].width-1;
		else if ((x & UI_LEFT_OF) != 0) _uiElement[_uiNbElements].x = _uiElement[ref].x- _uiElement[_uiNbElements].width+1;
		if ((x & UI_WITH_MARGIN) != 0) _uiElement[_uiNbElements].x += UI_ELEMENT_LEFT_MARGIN;
	}
	if ((y & UI_PLACE_MODIFIER_MASK) != 0) {
		int16_t ref = _getUIElementIndex(x & ~UI_PLACE_MODIFIER_MASK);
		if (ref == -1) return UI_ERROR;
		if ((y & UI_SAME_AS) != 0) _uiElement[_uiNbElements].y = _uiElement[ref].y;
		else if ((y & UI_BOTTOM_OF) != 0) _uiElement[_uiNbElements].y = _uiElement[ref].y+ _uiElement[ref].height-1;
		else if ((y & UI_TOP_OF) != 0) _uiElement[_uiNbElements].y = _uiElement[ref].y- _uiElement[_uiNbElements].height+1;
		if ((y & UI_WITH_MARGIN) != 0) _uiElement[_uiNbElements].y += UI_ELEMENT_TOP_MARGIN;
	}
	return _uiNbElements++;
}

void _drawUIElement(uiElement_t *elt) {
	int16_t color = UI_COLOR_BUTTON_RELEASED;
	if (elt->value == UI_PUSHED) color = UI_COLOR_BUTTON_PUSHED;
	switch (elt->type) {
		case UI_LABEL:
			setBackgroundColor(UI_COLOR_BACKGROUND);
			drawString(elt->text, _uiX+elt->x, _uiY+_uiTabHeight+elt->y+1+(elt->height-getFontHeight(UI_FONT))/2, BLACK, UI_FONT, UI_FONT_IS_BOLD, false);
			break;
		case UI_BUTTON:
			fillRoundedRectangle(_uiX+elt->x, _uiY+_uiTabHeight+elt->y, elt->width, elt->height, 6, color);
			drawRoundedRectangle(_uiX+elt->x, _uiY+_uiTabHeight+elt->y, elt->width, elt->height, 6, BLACK, 1);
			setBackgroundColor(color);
			drawString(elt->text, _uiX+elt->x+(elt->width-getStringWidth(elt->text, UI_FONT))/2, 
					_uiY+_uiTabHeight+elt->y+1+(elt->height-getFontHeight(UI_FONT))/2, BLACK, UI_FONT, UI_FONT_IS_BOLD, false);
			break;
		case UI_PUSH_BUTTON:
			fillRectangle(_uiX+elt->x, _uiY+_uiTabHeight+elt->y, elt->width, elt->height, color);
			drawRectangle(_uiX+elt->x, _uiY+_uiTabHeight+elt->y, elt->width, elt->height, BLACK, 1);
			setBackgroundColor(color);
			drawString(elt->text, _uiX+elt->x+(elt->width-getStringWidth(elt->text, UI_FONT))/2, 
					_uiY+_uiTabHeight+elt->y+1+(elt->height-getFontHeight(UI_FONT))/2, BLACK, UI_FONT, UI_FONT_IS_BOLD, false);
			break;
		case UI_AREA:
			if (elt->draw) (*(elt->draw))(elt->id, _uiX+elt->x, _uiY+_uiTabHeight+elt->y, elt->width, elt->height, elt->value);
			break;
		case UI_GAUGE:
			fillRectangle(_uiX+elt->x, _uiY+_uiTabHeight+elt->y, elt->width, elt->height, UI_COLOR_BACKGROUND);
			if (elt->width > elt->height) 
				 fillRectangle(_uiX+elt->x, _uiY+_uiTabHeight+elt->y, 1+(elt->width-1)*elt->value/(elt->max-elt->min), elt->height, UI_COLOR_GAUGE);
			else fillRectangle(_uiX+elt->x, _uiY+_uiTabHeight+elt->y+1+(elt->height-1)*(elt->max-elt->value)/(elt->max-elt->min),
						elt->width, (elt->height-1)*elt->value/(elt->max-elt->min), UI_COLOR_GAUGE);
			drawRectangle(_uiX+elt->x, _uiY+_uiTabHeight+elt->y, elt->width, elt->height, BLACK, 2);
			break;
		case UI_SLIDER:
			fillRectangle(_uiX+elt->x, _uiY+_uiTabHeight+elt->y, elt->width, elt->height, UI_COLOR_BACKGROUND);
			int16_t x;
			int16_t y;
			int16_t w;
			int16_t h;
			if (elt->width > elt->height) {
				drawLine(_uiX+elt->x+2, _uiY+_uiTabHeight+elt->y+elt->height/2, _uiX+elt->x+elt->width-2, _uiY+_uiTabHeight+elt->y+elt->height/2, BLACK, 4);
				x = elt->x + ((elt->width-UI_SLIDER_KNOB_SIZE)*elt->value)/(elt->max-elt->min);
				y = elt->y + 2;
				w = UI_SLIDER_KNOB_SIZE;
				h = elt->height-2;
			} else {
				drawLine(_uiX+elt->x+elt->width/2, _uiY+_uiTabHeight+elt->y+2, _uiX+elt->x+elt->width/2, _uiY+_uiTabHeight+elt->y+elt->height-2, BLACK, 4);
				x = elt->x + 2;
				y = elt->y + ((elt->height-UI_SLIDER_KNOB_SIZE)*(elt->max-elt->value))/(elt->max-elt->min);
				w = elt->width-2;
				h = UI_SLIDER_KNOB_SIZE;
			}
			fillRectangle(_uiX+x, _uiY+_uiTabHeight+y, w, h, UI_COLOR_BUTTON_PUSHED);
			drawRectangle(_uiX+x, _uiY+_uiTabHeight+y, w, h, BLACK, 1);
			break;
	}	
}

boolean _isTouchInUITab(uiTab_t *tab, int16_t x, int16_t y) {
	if (x == TOUCHSCREEN_NO_TOUCH) return false;
	return ((x >_uiX+tab->x) && (x < _uiX+tab->x+tab->width) && (y > _uiY) && (y < _uiY+_uiTabHeight));
}

boolean _isTouchInUIElement(uiElement_t *elt, int16_t x, int16_t y) {
	if (x == TOUCHSCREEN_NO_TOUCH) return false;
	return ((x >_uiX+elt->x) && (y > _uiY+_uiTabHeight+elt->y) && (x < _uiX+elt->x+elt->width) && (y < _uiY+_uiTabHeight+elt->y+elt->height));
}

bool _updateUIElementValue(uiElement_t *elt, int16_t value, int16_t x, int16_t y) {
	boolean hasValueChanged = true;
	switch (elt->type) {
		case UI_BUTTON:
		case UI_AREA:
			elt->value = value;
			break;
		case UI_PUSH_BUTTON:
			if (state == UI_PUSHED) {
				if (elt->group == UI_NO_GROUP) elt->value = (elt->value == UI_UNSELECTED ? UI_SELECTED : UI_UNSELECTED);
				else  {
					// find all the PUSH_BUTTON in the group
					for (uint8_t i=0; i<_uiNbElements; i++) {
						if ((_uiElement[i].group == elt->group) && (_uiElement[i].tab == elt->tab)) {
							// for the selected, push it
							if ((_uiElement[i].id == elt->id) && (_uiElement[i].value == UI_UNSELECTED)) _uiElement[i].value = UI_SELECTED;
							else if ((_uiElement[i].id != elt->id) && (_uiElement[i].value == UI_SELECTED)) {
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
				int16_t value = 0;
				if (elt->width > elt->height) {
					int16_t minX = _uiX + elt->x;
					int16_t maxX = _uiX + elt->x + elt->width - 1; 
					value = (x - minX) * (elt->max - elt->min) / (maxX - minX);
				} else {
					int16_t minY = _uiY + _uiTabHeight + elt->y;
					int16_t maxY = _uiY + _uiTabHeight + elt->y + elt->height - 1; 
					value = elt->max - (y - minY) * (elt->max - elt->min) / (maxY - minY);					
				}
				if (value != elt->value) elt->value = value;
				else hasValueChanged = false;
			}
			break;
	}
	return hasValueChanged;
}

int16_t _getUIElementIndex(uint8_t id) {
	for (uint8_t i=0; i<_uiNbElements; i++) {
		if (_uiElement[i].id == id) return i;
	}
	return UI_ERROR;
}

#endif