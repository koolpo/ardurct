/*
 * GraphicsUILabel - a simple label
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

ArduRCT_GraphicsUILabel::ArduRCT_GraphicsUILabel(char *label, uint8_t fontSize) {
    _fontSize = fontSize;
    _id = -1;
    _text = label;
    editable = false;
}

ArduRCT_GraphicsUILabel::ArduRCT_GraphicsUILabel(uint8_t id, char *label, uint8_t fontSize) {
    _fontSize = fontSize;
    _id = id;
    _text = label;
    editable = false;
}

void ArduRCT_GraphicsUILabel::setText(char *text) {
    _text = text;
}

char *ArduRCT_GraphicsUILabel::getText() {
    return _text;
}

void ArduRCT_GraphicsUILabel::draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth) {
    graphics->fillRectangle(x+uiX, y+uiY, width-1, height-1, graphics->getBackgroundColor());
    graphics->drawString(_text, uiX+x+_textX, uiY+y+_textY, BLACK, _fontSize, _fontSize == FONT_SMALL ? false : GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, false);
}

void ArduRCT_GraphicsUILabel::autoSize(ArduRCT_Graphics *graphics) {
    if (_text == 0) return;
    uint16_t *tW;
    uint16_t *tH;
    graphics->getStringBoundingBox(_text, _fontSize, _fontSize == FONT_SMALL ? false : GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, GRAPHICS_UI_ELEMENT_LEFT_MARGIN, &width, &height);
    height += GRAPHICS_UI_ELEMENT_TOP_MARGIN * 2;
}

void ArduRCT_GraphicsUILabel::placeLabel(ArduRCT_Graphics *graphics) {
    if (_text == 0) return; 
    uint16_t tW;
    uint16_t tH;
    graphics->getStringBoundingBox(_text, _fontSize, _fontSize == FONT_SMALL ? false : GRAPHICS_UI_ELEMENT_FONT_IS_BOLD, GRAPHICS_UI_ELEMENT_LEFT_MARGIN/2, &tW, &tH);
    _textX = GRAPHICS_UI_ELEMENT_LEFT_MARGIN/2;
    _textY = (height-tH)/2;
}

#endif