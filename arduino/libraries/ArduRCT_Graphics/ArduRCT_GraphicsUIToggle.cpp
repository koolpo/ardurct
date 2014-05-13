/*
 * GraphicsUIToggle - a toggle switch
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

ArduRCT_GraphicsUIToggle::ArduRCT_GraphicsUIToggle(uint8_t id, int16_t value, bool (*actionHandler)(uint8_t elementId, uint8_t state, int16_t value)) {
    _id = id;
    _value = (value ? 1 : 0);
    _actionHandler = actionHandler;
    editable = true;
}

void ArduRCT_GraphicsUIToggle::draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth) {
    uint16_t color = (_state == GRAPHICS_UI_PRESSED ? GRAPHICS_UI_COLOR_SELECTED : GRAPHICS_UI_COLOR_RELEASED);
    uint16_t hColor = (_state == GRAPHICS_UI_RELEASED || _state == GRAPHICS_UI_SELECTED || _state == GRAPHICS_UI_PRESSED ? 
            GRAPHICS_UI_COLOR_HIGHLIGHTED : BLACK);
    graphics->fillRoundedRectangle(x+uiX, y+uiY, width, height, GRAPHICS_UI_ELEMENT_TOP_MARGIN*75/100,
            _value == 1 ? GRAPHICS_UI_COLOR_GAUGE : GRAPHICS_UI_COLOR_RELEASED);
    graphics->fillRoundedRectangle(x+uiX+(_value == 1 ? width-height+2 : 2), y+uiY+2, height-5, height-5, GRAPHICS_UI_ELEMENT_TOP_MARGIN*75/100, color);
    graphics->drawRoundedRectangle(x+uiX, y+uiY, width, height, GRAPHICS_UI_ELEMENT_TOP_MARGIN*75/100, hColor, 1);
    graphics->drawRoundedRectangle(x+uiX+(_value == 1 ? width-height+2 : 2), y+uiY+2, height-5, height-5, GRAPHICS_UI_ELEMENT_TOP_MARGIN*75/100, BLACK, 2);
}

void ArduRCT_GraphicsUIToggle::autoSize(ArduRCT_Graphics *graphics) {
    height = graphics->getFontHeight(GRAPHICS_UI_ELEMENT_FONT) + GRAPHICS_UI_ELEMENT_TOP_MARGIN * 2;
    width =  2 * height - 6;

}

// called when the item is pressed
// return the element that was modified if any
// we toggle the state on press
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIToggle::press() {
    _state = GRAPHICS_UI_SELECTED;
    if (_value == 1) _value = 0;
    else _value = 1;
    return 0;
}

// called when the item is touched with a pen
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIToggle::touch(int16_t touchX, int16_t touchY) {
    _state = GRAPHICS_UI_SELECTED;
    if (touchX < width/2) _value = 0;
    else _value = 1;
    return 0;
}

#endif