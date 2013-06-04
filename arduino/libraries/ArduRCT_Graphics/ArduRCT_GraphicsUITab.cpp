/*
 * GraphicsUITab - a tab like Option
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

ArduRCT_GraphicsUITab::ArduRCT_GraphicsUITab(uint8_t id, char *label, bool (*actionHandler)(uint8_t elementId, int16_t value), uint8_t group) {
    _init(id, group, label, 0, actionHandler);
}

ArduRCT_GraphicsUITab::ArduRCT_GraphicsUITab(uint8_t id, void (*drawHandler)(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height), 
        bool (*actionHandler)(uint8_t elementId, int16_t value), uint8_t group) {
    _init(id, group, 0, drawHandler, actionHandler);
}

void ArduRCT_GraphicsUITab::autoSize(ArduRCT_Graphics *graphics) {
    if (_text == 0) return; 
    uint8_t fontSize = getFontSize(_text);
    height = graphics->getFontHeight(fontSize) + GRAPHICS_UI_TAB_TOP_MARGIN * 2;
    width =  graphics->getStringWidth(_text, fontSize) + GRAPHICS_UI_TAB_LEFT_MARGIN * 2;
    if ((getPrevious() == 0) || (getPrevious()->getGroup() != _group)) width += GRAPHICS_UI_TAB_LEFT_MARGIN;
}

void ArduRCT_GraphicsUITab::draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth) {
    uint16_t bgColor = graphics->getBackgroundColor();
    uint16_t color = GRAPHICS_UI_COLOR_RELEASED;
    if (_value == GRAPHICS_UI_SELECTED) color = GRAPHICS_UI_COLOR_BACKGROUND;
    if (_state == GRAPHICS_UI_SELECTED) color = GRAPHICS_UI_COLOR_PRESSED;
    graphics->setBackgroundColor(color);
    int16_t xStart = 0;
    if ((getPrevious() == 0) || (getPrevious()->getGroup() != _group)) xStart = GRAPHICS_UI_TAB_LEFT_MARGIN;
    graphics->fillRectangle(xStart+uiX+x, uiY+y+1, width-xStart, height-1, color);
    graphics->drawRectangle(xStart+uiX+x, uiY+y+1, width-xStart, height-1, 
            _state == GRAPHICS_UI_HIGHLIGHTED ? GRAPHICS_UI_COLOR_HIGHLIGHTED : BLACK, 1);	
    int16_t lineY = uiY+y+height-1;
    if (xStart != 0) graphics->drawLine(uiX+x, lineY, uiX+x+xStart, lineY, BLACK, 1);
    // for the last tab, go to the end of the ui
    if ((getNext() == 0) || (getNext()->getGroup() != _group)) 
        graphics->drawLine(uiX+x+width, lineY, uiX+x+width+uiWidth, lineY, BLACK, 1);
    if (_value == GRAPHICS_UI_SELECTED) 
        graphics->drawLine(uiX+x+xStart+1, lineY, uiX+x+width-2, lineY, WHITE, 1);
    if (_text) {
        uint8_t fontSize = getFontSize(_text);
        graphics->drawString(_text, uiX+x+_textX+xStart/2, uiY+y+_textY+1, BLACK, fontSize, GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, false);
    } else if (_drawHandler) {
        (*_drawHandler)(_id, _state, _value, x+uiX, y+uiY, width, height);
    }
    graphics->setBackgroundColor(bgColor);
}

#endif