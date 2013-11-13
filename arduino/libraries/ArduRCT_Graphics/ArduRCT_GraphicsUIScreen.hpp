/*
 * GraphicsUIScreen - The class managing UI elements
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
 
#ifndef GRAPHICS_UI_SCREEN_HPP
#define GRAPHICS_UI_SCREEN_HPP 1

#include <inttypes.h>

class ArduRCT_Graphics;
class ArduRCT_GraphicsUIElement;

class ArduRCT_GraphicsUIScreen {

    public:
    
        ArduRCT_GraphicsUIScreen(int16_t x = 0, int16_t y = 0, uint16_t width = 0, uint16_t height = 0);
        
        void addElement(ArduRCT_GraphicsUIElement *elt);
        
        void addElement(ArduRCT_GraphicsUIElement *elt, int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t section = GRAPHICS_UI_SECTION_MAIN);
        
        void setGraphics(ArduRCT_Graphics *graphics);
        
        void deactivate();
        
        void activate();
        
        void pack(uint8_t position);

        bool process(uint8_t actionId, int16_t x, int16_t y);
        
        ArduRCT_GraphicsUIElement *getElement(uint8_t id);
        
        void setElementValue(uint8_t id, int16_t value);
        
        int16_t getElementValue(uint8_t id);
        
        void setBackgroundColor(uint16_t color);
                
        void setPreviousScreen(ArduRCT_GraphicsUIScreen *previous);
        
        ArduRCT_GraphicsUIScreen *getPreviousScreen();
                
    private:
        int16_t _x;
        int16_t _y;
        uint16_t _width;
        uint16_t _height;
        int16_t _topX;
        int16_t _topY;
        ArduRCT_Graphics *_graphics;
        ArduRCT_GraphicsUIElement *_firstHeaderElement;
        ArduRCT_GraphicsUIElement *_lastHeaderElement;
        ArduRCT_GraphicsUIElement *_firstFooterElement;
        ArduRCT_GraphicsUIElement *_firstElement;
        ArduRCT_GraphicsUIElement *_lastElement;
        ArduRCT_GraphicsUIElement *_firstDisplayedElement;
        ArduRCT_GraphicsUIElement *_lastDisplayedElement;
        ArduRCT_GraphicsUIElement *_currentElement;
        ArduRCT_GraphicsUIElement *_selectedElement;
        int8_t _currentElementSection;
        bool _enabled;
        uint8_t _position;
        uint16_t _backgroundColor;
        uint8_t _headerHeight;
        uint8_t _footerHeight;
        ArduRCT_GraphicsUIScreen *_previousScreen;

        void _reset();        
        
        void _draw();
        
        void _drawElement(ArduRCT_GraphicsUIElement *elt);
        
        ArduRCT_GraphicsUIElement *_findNextEditableElementInSection(ArduRCT_GraphicsUIElement *current, bool upDirection);
        
        ArduRCT_GraphicsUIElement *_findNextElement(bool upDirection);
        
        bool _adjustTopToDisplayCurrentElement();

        ArduRCT_GraphicsUIElement *_findElementAt(int16_t x, int16_t y);
        
        bool _processOutOfScreenTouch(int16_t x, int16_t y);
}; 

#endif