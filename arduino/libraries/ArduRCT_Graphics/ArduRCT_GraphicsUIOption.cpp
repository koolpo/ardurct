/*
 * GraphicsUIOption - a toggle button which can belong to a group
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

ArduRCT_GraphicsUIOption::ArduRCT_GraphicsUIOption() {
}

ArduRCT_GraphicsUIOption::ArduRCT_GraphicsUIOption(uint8_t id, char *label, bool (*actionHandler)(uint8_t elementId, int16_t value), uint8_t group) {
    _init(id, group, label, 0, actionHandler);
}

ArduRCT_GraphicsUIOption::ArduRCT_GraphicsUIOption(uint8_t id, void (*drawHandler)(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height), 
        bool (*actionHandler)(uint8_t elementId, int16_t value), uint8_t group) {
    _init(id, group, 0, drawHandler, actionHandler);
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIOption::setValue(int16_t value) {
    _value = value;
    if ((_group == GRAPHICS_UI_NO_GROUP) || (value != GRAPHICS_UI_SELECTED)) return 0;
    
    // search backward for an element in the same group
    ArduRCT_GraphicsUIElement *elt = getPrevious();
    while (elt) {
        // is there another selected in the same group
        if ((elt->getGroup() == _group) && (elt->getValue() == GRAPHICS_UI_SELECTED)) {
            // release it
            elt->setValue(GRAPHICS_UI_RELEASED);
            elt->setState(GRAPHICS_UI_RELEASED);
            // return it for redraw
            return elt;
        }
        elt = elt->getPrevious();
    }
    // search forward for an element in the same group
    elt = getNext();
    while (elt) {
        // is there another selected in the same group
        if ((elt->getGroup() == _group) && (elt->getValue() == GRAPHICS_UI_SELECTED)) {
            // release it
            elt->setValue(GRAPHICS_UI_RELEASED);
            elt->setState(GRAPHICS_UI_RELEASED);
            // return it for redraw
            return elt;
        }
        elt = elt->getNext();
    }
    return 0;
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIOption::enter() {
    if (_group != GRAPHICS_UI_NO_GROUP) return setValue(GRAPHICS_UI_SELECTED);
    if (_value == GRAPHICS_UI_SELECTED) setValue(GRAPHICS_UI_RELEASED);
    else setValue(GRAPHICS_UI_SELECTED);
    return 0;
}

bool ArduRCT_GraphicsUIOption::release() {
    return true;
}

uint16_t ArduRCT_GraphicsUIOption::_drawBorder(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t color) {
    // a pushbutton
    graphics->fillRectangle(uiX+x, uiY+y, width, height, color);
    graphics->drawRectangle(uiX+x, uiY+y, width, height, _state == GRAPHICS_UI_HIGHLIGHTED ? GRAPHICS_UI_COLOR_HIGHLIGHTED : BLACK, 1);
    return color;
}


#endif