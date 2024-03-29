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
 
#ifndef GRAPHICS_UI_TAB_HPP
#define GRAPHICS_UI_TAB_HPP 1

#include <inttypes.h>

class ArduRCT_GraphicsUITab : public ArduRCT_GraphicsUIOption {
    
    public:
        ArduRCT_GraphicsUITab(uint8_t id, char *label, bool (*actionHandler)(uint8_t elementId, uint8_t state, int16_t value), uint8_t group) ;

        ArduRCT_GraphicsUITab(uint8_t id, void (*drawHandler)(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height), 
                bool (*actionHandler)(uint8_t elementId, uint8_t state, int16_t value), uint8_t group);

        virtual void autoSize(ArduRCT_Graphics *graphics);

        virtual void draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth);
/*        
    protected:
        uint16_t _drawBorder(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t color);
*/
}; 

#endif