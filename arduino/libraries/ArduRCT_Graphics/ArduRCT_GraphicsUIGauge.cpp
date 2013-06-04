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

ArduRCT_GraphicsUIGauge::ArduRCT_GraphicsUIGauge(uint8_t id, int16_t value, int16_t min, int16_t max) {
    _id = id;
    _value = value;
    _min = min;
    _max = max;
    editable = false;
}

void ArduRCT_GraphicsUIGauge::draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth) {
    if (width > height) {
        // horizontal gauge
        int fill = _value*width/(_max-_min);
        if (fill > 0) graphics->fillRectangle(uiX+x, uiY+y, fill, height, GRAPHICS_UI_COLOR_GAUGE);
        graphics->fillRectangle(uiX+x+fill, uiY+y, width-fill, height, WHITE);
    } else {
        // vertical gauge
        int fill = _value*height/(_max-_min);
        if (fill > 0) graphics->fillRectangle(uiX+x, uiY+y+height-fill, width, fill, GRAPHICS_UI_COLOR_GAUGE);
        graphics->fillRectangle(uiX+x, uiY+y, width, height-fill, WHITE);
    }
    graphics->drawRectangle(uiX+x, y+uiY, width, height, BLACK, 1);
}
    
ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIGauge::setValue(int16_t value) {
    _value = value;
    if (_value > _max) _value = _max;
    if (_value < _min) _value = _min;
    return 0;
}

#endif