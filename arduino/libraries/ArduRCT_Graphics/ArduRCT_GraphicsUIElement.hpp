/*
 * GraphicsUIElement - base class for all UI elements
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
 
#ifndef GRAPHICS_UI_ELEMENT_HPP
#define GRAPHICS_UI_ELEMENT_HPP 1

#include <inttypes.h>

class ArduRCT_Graphics;

class ArduRCT_GraphicsUIElement {

    public:
        int16_t x;
        int16_t y; 
        uint16_t width;
        uint16_t height;
        bool editable;
        uint8_t section;
        bool repeatable;
        
        ArduRCT_GraphicsUIElement();

        ArduRCT_GraphicsUIElement(uint8_t id, void (*drawHandler)(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height) = 0, 
                bool (*actionHandler)(uint8_t elementId, uint8_t state, int16_t value) = 0);
        
        void setNext(ArduRCT_GraphicsUIElement *next);
        
        void setPrevious(ArduRCT_GraphicsUIElement *previous);
        
        ArduRCT_GraphicsUIElement *getNext();
        
        ArduRCT_GraphicsUIElement *getPrevious();
        
        uint8_t getGroup();
        
        uint8_t getId();
               
        uint8_t getFontSize(char *text);
        
        virtual void draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth);
        
        virtual void autoSize(ArduRCT_Graphics *graphics);
        
        virtual void placeLabel(ArduRCT_Graphics *graphics);
        
        // returns another element which changed if any
        virtual ArduRCT_GraphicsUIElement *setValue(int16_t value);
        
        virtual void setState(uint8_t state);

        virtual int16_t getValue();

        // called when the item is selected
        virtual void select();

        // called when the item is unselected
        virtual void unselect();
        
        // called when the item is selected (enter is pressed)
        // returns another element which changed if any
        virtual ArduRCT_GraphicsUIElement *press();

        // called when the item is touched with a pen
        // returns another element which changed if any
        virtual ArduRCT_GraphicsUIElement *touch(int16_t x, int16_t y);

        // called when the touch is released
        // return true if the element needs to be escaped after release
        virtual bool release();
        
        // called when the item is selected and Up is pressed
        // return true if the value was increased
        virtual bool increase();

        // called when the item is selected and Down is pressed
        // return true if the value was decreased
        virtual bool decrease();

        // called when the element is pressed (repeated), increased, decreased, touched or released
        virtual bool run();
                
    protected:
        int16_t _value;
        uint8_t _state;
        uint8_t _id;
        uint8_t _group;
        ArduRCT_GraphicsUIElement *_next;
        ArduRCT_GraphicsUIElement *_previous;
        void (*_drawHandler)(uint8_t id, uint8_t state, int16_t value, int16_t x, int16_t y, uint16_t width, uint16_t height); 
        bool (*_actionHandler)(uint8_t elementId, uint8_t state, int16_t value);
}; 

#endif