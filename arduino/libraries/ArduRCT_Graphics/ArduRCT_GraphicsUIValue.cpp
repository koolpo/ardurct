/*
 * GraphicsUIValue - an adjustable value
 *
 * Copyright (c) 2013 Laurent Wibaux <lm.wibaux@gmail.com>
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

ArduRCT_GraphicsUIValue::ArduRCT_GraphicsUIValue(uint8_t id, int16_t value, int16_t min, int16_t max, bool (*actionHandler)(uint8_t elementId, int16_t value), int16_t step, uint8_t radix) {
    _id = id;
    _value = value;
    _actionHandler = actionHandler;
    _min = min;
    _max = max;
    _step = step;
    _radix = radix;
    if (_step == 0) _step = 1;
}

void ArduRCT_GraphicsUIValue::draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth) {
    uint16_t bgColor = graphics->getBackgroundColor();
    uint16_t color = (_state == GRAPHICS_UI_SELECTED ? GRAPHICS_UI_COLOR_SELECTED : GRAPHICS_UI_COLOR_RELEASED);
    graphics->setBackgroundColor(color);
    graphics->fillRectangle(x+uiX, y+uiY, width, height, color);
    uint16_t hColor = (_state == GRAPHICS_UI_RELEASED ? BLACK : GRAPHICS_UI_COLOR_HIGHLIGHTED);
    graphics->drawRectangle(x+uiX, y+uiY, width, height, hColor, 1);
    if (_value < _max) graphics->fillCorner(uiX+x+width-2, uiY+y+1, 5, GRAPHICS_POSITION_SW, BLACK);
    if (_value > _min) graphics->fillCorner(uiX+x+1, uiY+y+height-2, 5, GRAPHICS_POSITION_NE, BLACK);
    uint8_t len = _intToString(_value);
    int16_t textX = x+uiX+(width - len*graphics->getFontCharWidth(GRAPHICS_UI_ELEMENT_FONT))/2;
    int16_t textY = y+uiY+(height-graphics->getFontHeight(GRAPHICS_UI_ELEMENT_FONT))/2;
    graphics->drawString(_display, textX, textY, BLACK, GRAPHICS_UI_ELEMENT_FONT, GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, false);
    graphics->setBackgroundColor(bgColor);
}

void ArduRCT_GraphicsUIValue::autoSize(ArduRCT_Graphics *graphics) {
    uint8_t len2 = _intToString(_max);
    uint8_t len = _intToString(_min);
    if (len2 > len) len = len2;
    height = graphics->getFontHeight(GRAPHICS_UI_ELEMENT_FONT) + GRAPHICS_UI_ELEMENT_TOP_MARGIN * 2;
    width =  len*graphics->getFontCharWidth(GRAPHICS_UI_ELEMENT_FONT) + GRAPHICS_UI_ELEMENT_LEFT_MARGIN * 2;
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIValue::setValue(int16_t value) {
    _value = (value / _step) * _step;
    if (_value > _max) _value = _max;
    if (_value < _min) _value = _min;
    return 0;
}

// called when the item is selected (enter is pressed, or item is touched)
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIValue::enter() {
    _state = GRAPHICS_UI_SELECTED;
    return 0;
}

// called when the touch is released
// return true if the element needs to be escaped after release
bool ArduRCT_GraphicsUIValue::release() {
    return false;
}

// called when the item is escaped
// return true if the element needs to be run after escape
bool ArduRCT_GraphicsUIValue::escape() {
    _state = GRAPHICS_UI_RELEASED;
    return false;
}

// called when the item is selected and Up is pressed
// return true if the value was increased
bool ArduRCT_GraphicsUIValue::increase() {
    if (_value == _max) return false;
    _value += _step;
    if (_value > _max) _value = _max;
    return true;
}

// called when the item is selected and Down is pressed
// return true if the value was decreased
bool ArduRCT_GraphicsUIValue::decrease() {
    if (_value == _min) return false;
    _value -= _step;
    if (_value < _min) _value = _min;
    return true;
}

void ArduRCT_GraphicsUIValue::highlight() {
    _state = GRAPHICS_UI_HIGHLIGHTED;
}

// called when the item is touched with a pen
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIValue::touch(int16_t touchX, int16_t touchY) {
    _state = GRAPHICS_UI_SELECTED;
    if (touchY < height/2) {
        if (_value == _max) return 0;
        if (touchX < width/2) _value += _step;
        else _value += (_max-_min)/10;
        if (_value > _max) _value = _max;
    } else {
        if (_value == _min) return 0;
        if (touchX < width/2) _value -= _step;
        else _value -= (_max-_min)/10;
        if (_value < _min) _value = _min;
    }
    return 0;
}
    
uint8_t ArduRCT_GraphicsUIValue::_intToString(int16_t value) {
    if ((value >= 0) && (value < _radix)) {
        _display[0] = '0' + value;
        if (_display[0] > '9') _display[0] += 'A'-'0'-10;
        _display[1] = 0;
        return 1;
    }
    uint8_t i = 0;
    uint8_t temp[6];
    bool neg = false;		
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
    uint8_t j = 0;
    if (neg) _display[j++] = '-';
    while (i >= 0) _display[j++] = temp[i--]; 
    _display[j] = 0;
    return j;
}

#endif