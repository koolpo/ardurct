/*
 * GraphicsUIElement - base class for all UI elements
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
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
 
#include "ArduRCT_Graphics.h"

#if defined(GRAPHICS_HAS_UI)

#include "ArduRCT_GraphicsUI.hpp"

ArduRCT_GraphicsUIElement::ArduRCT_GraphicsUIElement() {
}

ArduRCT_GraphicsUIElement::ArduRCT_GraphicsUIElement(uint8_t id, void (*drawHandler)(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height), 
        bool (*actionHandler)(uint8_t elementId, int16_t value)) {
    _id = id;
    _group = -1;
    _value = GRAPHICS_UI_RELEASED;
    _state = GRAPHICS_UI_RELEASED;
    editable = true;
    _actionHandler = actionHandler;
    _drawHandler = drawHandler;
}

// returns the element modified when this value was set
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIElement::setValue(int16_t value) {  
    _value = value;
    return 0;
}

void ArduRCT_GraphicsUIElement::setState(uint8_t state) {
    _state = state;
}

void ArduRCT_GraphicsUIElement::setNext(ArduRCT_GraphicsUIElement *next) {
    _next = next;
    next->setPrevious(this);
}

void ArduRCT_GraphicsUIElement::setPrevious(ArduRCT_GraphicsUIElement *previous) {
    _previous = previous;
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIElement::getNext() {
    return _next;
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIElement::getPrevious() {
    return _previous;
}

uint8_t ArduRCT_GraphicsUIElement::getGroup() {
    return _group;
}

bool ArduRCT_GraphicsUIElement::run() {
    if (_actionHandler) return (*_actionHandler)(_id, _value);
    return false;
}
        
uint8_t ArduRCT_GraphicsUIElement::getFontSize(char *text) {
    if (text) {
        if (((text[0] == '+') || (text[0] == '-')) && (text[1] == 0)) return GRAPHICS_UI_ELEMENT_FONT+1;
    }
    return GRAPHICS_UI_ELEMENT_FONT;
}
    
void ArduRCT_GraphicsUIElement::draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth) {
    if (_drawHandler) (*_drawHandler)(_id, _state, _value, x+uiX, y+uiY, width, height);
    else {
        graphics->fillRectangle(x+uiX, y+uiY, width, height, GRAPHICS_UI_COLOR_RELEASED);
        if (editable) {
            int color = GRAPHICS_UI_COLOR_RELEASED;
            if (_state == GRAPHICS_UI_HIGHLIGHTED) color = GRAPHICS_UI_COLOR_HIGHLIGHTED;
            else if (_state == GRAPHICS_UI_SELECTED) color = GRAPHICS_UI_COLOR_PRESSED;
            graphics->drawRectangle(x+uiX, y+uiY, width, height, color, 1);
        }
    }
}

void ArduRCT_GraphicsUIElement::autoSize(ArduRCT_Graphics *graphics) {
    // overwritten by sub classes
}

void ArduRCT_GraphicsUIElement::placeLabel(ArduRCT_Graphics *graphics) {
    // overwritten by sub classes
}

int16_t ArduRCT_GraphicsUIElement::getValue() {
    return _value;
}

uint8_t ArduRCT_GraphicsUIElement::getId() {
    return _id;
}

// called when the item is selected (enter is pressed, or item is touched)
// returns another element changed if any
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIElement::enter() {
    _state = GRAPHICS_UI_SELECTED;
    return 0;
}

// called when the item is touched with a pen
// returns another element changed if any
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIElement::touch(int16_t touchX, int16_t touchY) {
    return enter();
}

// called when the touch is released
// return true if the element needs to be escaped after release
bool ArduRCT_GraphicsUIElement::release() {
    return true;
}

// called when the item is escaped
// return true if the element needs to be run after escape
bool ArduRCT_GraphicsUIElement::escape() {
    _state = GRAPHICS_UI_RELEASED;
    return true;
}

// called when the item is selected and Up is pressed
// return true if the value was increased
bool ArduRCT_GraphicsUIElement::increase() {
    return false;
}

// called when the item is selected and Down is pressed
// return true if the value was decreased
bool ArduRCT_GraphicsUIElement::decrease() {
    return false;
}

void ArduRCT_GraphicsUIElement::highlight() {
    _state = GRAPHICS_UI_HIGHLIGHTED;
}

#endif