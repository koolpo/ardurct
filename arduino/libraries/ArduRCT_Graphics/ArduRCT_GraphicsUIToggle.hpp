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
 
#ifndef GRAPHICS_UI_TOGGLE_HPP
#define GRAPHICS_UI_TOGGLE_HPP 1

#include <inttypes.h>

class ArduRCT_GraphicsUIToggle : public ArduRCT_GraphicsUIElement {
    
    public:
        ArduRCT_GraphicsUIToggle(uint8_t id, int16_t value, bool (*actionHandler)(uint8_t elementId, uint8_t state, int16_t value) = 0);

        virtual void draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth);

        virtual void autoSize(ArduRCT_Graphics *graphics);

        virtual ArduRCT_GraphicsUIElement *press();
        
        virtual ArduRCT_GraphicsUIElement *touch(int16_t touchX, int16_t touchY);
}; 

#endif