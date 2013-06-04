/*
 * GraphicsUIListItem - a button like list member
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

ArduRCT_GraphicsUIListItem::ArduRCT_GraphicsUIListItem(uint8_t id, char *label, bool (*actionHandler)(uint8_t elementId, int16_t value)) {
    _init(id, GRAPHICS_UI_NO_GROUP, label, 0, actionHandler);
}

ArduRCT_GraphicsUIListItem::ArduRCT_GraphicsUIListItem(uint8_t id, void (*drawHandler)(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height), 
        bool (*actionHandler)(uint8_t elementId, int16_t value)) {
    _init(id, GRAPHICS_UI_NO_GROUP, 0, drawHandler, actionHandler);
}

void ArduRCT_GraphicsUIListItem::placeLabel(ArduRCT_Graphics *graphics) {
    if (_text == 0) return; 
    int fontSize = getFontSize(_text);
    _textX = GRAPHICS_UI_ELEMENT_LEFT_MARGIN;
    _textY = (height-graphics->getFontHeight(fontSize))/2;
}	

uint16_t ArduRCT_GraphicsUIListItem::_drawBorder(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t color) {
    if (_state == GRAPHICS_UI_HIGHLIGHTED) color = LIGHT_GREY;
    graphics->fillRectangle(uiX+x, uiY+y, width, height, color);
    graphics->drawRectangle(uiX+x, uiY+y, width, height, _state == GRAPHICS_UI_HIGHLIGHTED ? GRAPHICS_UI_COLOR_HIGHLIGHTED : BLACK, 1);
    return color;
}

#endif