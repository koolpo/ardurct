/*
 * GraphicsUIButton - a clickable button
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

ArduRCT_GraphicsUIButton::ArduRCT_GraphicsUIButton() {
}

ArduRCT_GraphicsUIButton::ArduRCT_GraphicsUIButton(uint8_t id, char *label, bool (*actionHandler)(uint8_t elementId, uint8_t state, int16_t value)) {
    _init(id, -1, label, 0, actionHandler);
}

ArduRCT_GraphicsUIButton::ArduRCT_GraphicsUIButton(uint8_t id, void (*drawHandler)(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height), 
        bool (*actionHandler)(uint8_t elementId, uint8_t state, int16_t value)) {
    _init(id, -1, 0, drawHandler, actionHandler);
}

void ArduRCT_GraphicsUIButton::autoSize(ArduRCT_Graphics *graphics) {
    if (_text == 0) return; 
    int fontSize = getFontSize(_text);
    height = graphics->getFontHeight(fontSize) + GRAPHICS_UI_ELEMENT_TOP_MARGIN * 2;
    width =  graphics->getStringWidth(_text, fontSize) + GRAPHICS_UI_ELEMENT_LEFT_MARGIN * 2;
}

void ArduRCT_GraphicsUIButton::placeLabel(ArduRCT_Graphics *graphics) {
    if (_text == 0) return; 
    int fontSize = getFontSize(_text);
    _textX = (width - graphics->getStringWidth(_text, fontSize))/2;
    _textY = (height-graphics->getFontHeight(fontSize))/2;
}

void ArduRCT_GraphicsUIButton::draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth) {
    int bgColor = graphics->getBackgroundColor();
    int color = GRAPHICS_UI_COLOR_RELEASED;
    if (_value == GRAPHICS_UI_PRESSED) color = GRAPHICS_UI_COLOR_PRESSED;
    color = _drawBorder(graphics, uiX, uiY, color);
    graphics->setBackgroundColor(color);
    if (_text != 0) {
        int fontSize = getFontSize(_text);
        graphics->drawString(_text, uiX+x+_textX, uiY+y+_textY, editable ? BLACK : GRAPHICS_UI_COLOR_PRESSED, fontSize, GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, false);
    } else if (_drawHandler != 0) {
        (*_drawHandler)(_id, _state, _value, x+uiX, y+uiY, width, height);
    }
    graphics->setBackgroundColor(bgColor);
}

    
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIButton::press() {
    _value = GRAPHICS_UI_PRESSED;
    if (_state < GRAPHICS_UI_PRESSED) _state = GRAPHICS_UI_PRESSED;
    if (!repeatable) return 0;
    _state ++;
    if (_state > GRAPHICS_UI_REPEATED) _state = GRAPHICS_UI_REPEATED;
    return 0;
}

bool ArduRCT_GraphicsUIButton::release() {
    _value = GRAPHICS_UI_RELEASED;
    _state = GRAPHICS_UI_RELEASED;
    return true;
}

void ArduRCT_GraphicsUIButton::_init(uint8_t id, uint8_t group, char *label, void (*drawHandler)(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height), 
        bool (*actionHandler)(uint8_t elementId, uint8_t state, int16_t value)) {
    _id = id;
    _group = group;
    _text = label;
    _state = GRAPHICS_UI_UNSELECTED;
    _value = GRAPHICS_UI_RELEASED;
    editable = true;
    repeatable = false;
    _actionHandler = actionHandler;
    _drawHandler = drawHandler;
}

uint16_t ArduRCT_GraphicsUIButton::_drawBorder(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t color) {
    graphics->fillRoundedRectangle(uiX+x, uiY+y, width, height, GRAPHICS_UI_ELEMENT_TOP_MARGIN, color);
    uint16_t bColor = BLACK;
    if (_state == GRAPHICS_UI_SELECTED || _state == GRAPHICS_UI_RELEASED) bColor = GRAPHICS_UI_COLOR_HIGHLIGHTED;
    graphics->drawRoundedRectangle(uiX+x, uiY+y, width, height, GRAPHICS_UI_ELEMENT_TOP_MARGIN, bColor, 1);
    return color;
}


#endif