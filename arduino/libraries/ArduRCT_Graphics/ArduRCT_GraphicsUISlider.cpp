/*
 * GraphicsUISlider - an adjustable slider
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

ArduRCT_GraphicsUISlider::ArduRCT_GraphicsUISlider(uint8_t id, int16_t value, int16_t min, int16_t max, bool (*actionHandler)(uint8_t elementId, uint8_t state, int16_t value), int16_t step) {
    _id = id;
    _value = value;
    _actionHandler = actionHandler;
    _min = min;
    _max = max;
    _step = step;
    editable = true;
    if (_step == 0) _step = 1;
}

void ArduRCT_GraphicsUISlider::draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth) {
    graphics->fillRectangle(x+uiX, y+uiY, width, height, GRAPHICS_UI_COLOR_BACKGROUND);
    uint16_t color = (_state == GRAPHICS_UI_PRESSED ? GRAPHICS_UI_COLOR_SELECTED : GRAPHICS_UI_COLOR_RELEASED);
    uint16_t hColor = (_state == GRAPHICS_UI_RELEASED || _state == GRAPHICS_UI_SELECTED  || _state == GRAPHICS_UI_PRESSED ? 
            GRAPHICS_UI_COLOR_HIGHLIGHTED : BLACK);
    if (width > height) {
        // horizontal slider
        graphics->fillRectangle(uiX+x, uiY+y+height/2-height/8, width, 2*height/8, BLACK);
        int32_t knobX = _value;
        knobX = x+uiX + knobX*(width-GRAPHICS_UI_SLIDER_KNOB_SIZE)/(_max-_min);
        graphics->fillRectangle((int16_t)knobX, y+uiY, GRAPHICS_UI_SLIDER_KNOB_SIZE, height, color);
        graphics->drawRectangle((int16_t)knobX, y+uiY, GRAPHICS_UI_SLIDER_KNOB_SIZE, height, hColor, 1);
    } else {
        // vertical slider
        graphics->fillRectangle(uiX+x+width/2-width/8, uiY+y, 2*width/8, height, BLACK);
        int32_t knobY = (_max-_value);
        knobY = y+uiY + knobY*(height-GRAPHICS_UI_SLIDER_KNOB_SIZE)/(_max-_min);
        graphics->fillRectangle(x+uiX, (int16_t)knobY, width, GRAPHICS_UI_SLIDER_KNOB_SIZE, color);
        graphics->drawRectangle(x+uiX, (int16_t)knobY, width, GRAPHICS_UI_SLIDER_KNOB_SIZE, hColor, 1);
    }
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUISlider::setValue(int16_t value) {
    if (_step != 1) _value = (value / _step) * _step;
    else _value = value;
    if (_value > _max) _value = _max;
    if (_value < _min) _value = _min;
    return 0;
}


// called when the item is pressed
// return the element that was modified if any
// we toggle the state on press
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUISlider::press() {
    if (_state == GRAPHICS_UI_PRESSED) _state = GRAPHICS_UI_RELEASED;
    else _state = GRAPHICS_UI_PRESSED;
    return 0;
}

// called when the touch is released
// return true if the element needs to be escaped after release
bool ArduRCT_GraphicsUISlider::release() {
    return (_state == GRAPHICS_UI_RELEASED);
}


// called when the item is selected and Up is pressed
// return true if the value was increased
bool ArduRCT_GraphicsUISlider::increase() {
    if (_value == _max) return false;
    _value += _step;
    if (_value > _max) _value = _max;
    return true;
}

// called when the item is selected and Down is pressed
// return true if the value was decreased
bool ArduRCT_GraphicsUISlider::decrease() {
    if (_value == _min) return false;
    _value -= _step;
    if (_value < _min) _value = _min;
    return true;
}

// called when the item is touched with a pen
// returns another element changed if any
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUISlider::touch(int16_t touchX, int16_t touchY) {
    _state = GRAPHICS_UI_SELECTED;
    if (width > height) {
        int32_t value = touchX;
        value = value * (_max-_min) / (width-GRAPHICS_UI_SLIDER_KNOB_SIZE) + _min;
        _value = value; 
    } else {
        int32_t value = height - touchY;
        value = value * (_max-_min) / (height-GRAPHICS_UI_SLIDER_KNOB_SIZE) + _min;
        _value = value; 
    }
    if (_value < _min) _value = _min;
    else if (_value > _max) _value = _max;
    else if (_step != 1) {
        if (_value >= _max-_step/2) _value = _max;
        else _value = _value - (_value % _step);
    }
    return 0;
}

#endif