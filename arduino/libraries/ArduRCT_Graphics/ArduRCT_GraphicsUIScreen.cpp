/*
 * GraphicsUIScreen - The class managing UI elts
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

ArduRCT_GraphicsUIScreen::ArduRCT_GraphicsUIScreen(int16_t x, int16_t y, uint16_t width, uint16_t height) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    _topX = 0;
    _topY = 0;
    _enabled = false;
    _previousScreen = 0;
    _position = GRAPHICS_POSITION_NW;
    _backgroundColor = GRAPHICS_UI_COLOR_BACKGROUND;
    _headerHeight = 0;
    _footerHeight = 0;
}

void ArduRCT_GraphicsUIScreen::setGraphics(ArduRCT_Graphics *graphics) {
    _graphics = graphics;
}

void ArduRCT_GraphicsUIScreen::setBackgroundColor(uint16_t color) {
    _backgroundColor = color;
}

void ArduRCT_GraphicsUIScreen::setPreviousScreen(ArduRCT_GraphicsUIScreen *previous) {
    _previousScreen = previous;
}

ArduRCT_GraphicsUIScreen *ArduRCT_GraphicsUIScreen::getPreviousScreen() {
    return _previousScreen;
}

void ArduRCT_GraphicsUIScreen::addElement(ArduRCT_GraphicsUIElement *elt) {
    addElement(elt, elt->x, elt->y, elt->width, elt->height, elt->section);
}

void ArduRCT_GraphicsUIScreen::addElement(ArduRCT_GraphicsUIElement *elt, int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t section) {
    if (_width == 0) _width = _graphics->getWidth();
    if (_height == 0) _height = _graphics->getHeight();
    // add the element to the section's chain
    elt->section = section;
    if (section == GRAPHICS_UI_SECTION_HEADER) {
        if (_firstHeaderElement == 0) _firstHeaderElement = elt;
        else _lastHeaderElement->setNext(elt);
        _lastHeaderElement = elt;
    } else if (section == GRAPHICS_UI_SECTION_FOOTER) {
        if (_firstFooterElement == 0) _firstFooterElement = elt;
        else {
            ArduRCT_GraphicsUIElement *next = _firstFooterElement;
            while (next->getNext()) next = next->getNext();
            next->setNext(elt);
        }
    } else {
        if (_firstElement == 0) _firstElement = elt;
        else _lastElement->setNext(elt);
        _lastElement = elt;
    }
    // place the element on the screen
    ArduRCT_GraphicsUIElement *previous = elt->getPrevious();
    if ((width == GRAPHICS_UI_AUTO_SIZE) || (height == GRAPHICS_UI_AUTO_SIZE)) elt->autoSize(_graphics);
    if (width == GRAPHICS_UI_WIDTH) elt->width = _graphics->getWidth();
    else if (width != GRAPHICS_UI_AUTO_SIZE) elt->width = width;
    if (height == GRAPHICS_UI_HEIGHT) elt->height = _graphics->getHeight();
    else if (height != GRAPHICS_UI_AUTO_SIZE) elt->height = height;
    if (previous) {
        if (width == GRAPHICS_UI_SAME_AS_PREVIOUS) elt->width = previous->width;
        if (height == GRAPHICS_UI_SAME_AS_PREVIOUS) elt->height = previous->height;
    }
    elt->x = x;
    elt->y = y;
    if (previous) {
        if (x == GRAPHICS_UI_SAME_AS_PREVIOUS) elt->x = previous->x;
        else if (x == GRAPHICS_UI_LEFT_OF_PREVIOUS) elt->x = previous->x - elt->width;
        else if (x == GRAPHICS_UI_RIGHT_OF_PREVIOUS) elt->x = previous->x + previous->width - 1;	
        else if (x == GRAPHICS_UI_RIGHT_OF_PREVIOUS_WITH_MARGIN) elt->x = previous->x + previous->width + GRAPHICS_UI_ELEMENT_LEFT_MARGIN;
        if (y == GRAPHICS_UI_SAME_AS_PREVIOUS) elt->y = previous->y;
        else if (y == GRAPHICS_UI_TOP_OF_PREVIOUS) elt->y = previous->y - elt->height;
        else if (y == GRAPHICS_UI_BOTTOM_OF_PREVIOUS) elt->y = previous->y + previous->height - 1;	
        else if (y == GRAPHICS_UI_BOTTOM_OF_PREVIOUS_WITH_MARGIN) elt->y = previous->y + previous->height + GRAPHICS_UI_ELEMENT_TOP_MARGIN;
    }
    elt->placeLabel(_graphics);
}

void ArduRCT_GraphicsUIScreen::deactivate() {
    _enabled = false;
}

void ArduRCT_GraphicsUIScreen::activate() {
    _enabled = true;
    _reset();
    _draw();
}

void ArduRCT_GraphicsUIScreen::pack(uint8_t position) {
    uint16_t width = 0;
    uint16_t height = 0;
    _width = _graphics->getWidth();
    _height = _graphics->getHeight();
    ArduRCT_GraphicsUIElement *elt = _firstElement;
    if (elt == 0) return;
    bool hasVerticalScroll = false;
    while (elt) {
        if (elt->x + elt->width > width) width = elt->x+elt->width;
        if (elt->y + elt->height < _height) height = elt->y + elt->height;
        else hasVerticalScroll = true;
        elt = elt->getNext();
    }
    if (width+GRAPHICS_UI_ELEMENT_LEFT_MARGIN < _width) _width = width+GRAPHICS_UI_ELEMENT_LEFT_MARGIN;
    if (height < _height) _height = height;
    // take care of scrolling for tall screens
    if (hasVerticalScroll && (_width + 10 < _graphics->getWidth())) _width += 10;
    // adjust the position
    if (position == GRAPHICS_POSITION_NW) return;
    _position = position;
    if ((position == GRAPHICS_POSITION_NE) || (position == GRAPHICS_POSITION_SE)) 
        _x = _graphics->getWidth()-_width;
    if ((position == GRAPHICS_POSITION_SE) || (position == GRAPHICS_POSITION_SW) || (position == GRAPHICS_POSITION_S)) 
        _y = _graphics->getHeight()-_height;
    if ((position == GRAPHICS_POSITION_N) || (position == GRAPHICS_POSITION_S) || (position == GRAPHICS_POSITION_CENTER)) 
        _x = (_graphics->getWidth()-_width)/2;
    if (position == GRAPHICS_POSITION_CENTER) {
        _y = (_graphics->getHeight()-_height)/2;
        // when centered, leave some space at the bottom during packing
        if (_height < _graphics->getHeight()) _height += GRAPHICS_UI_ELEMENT_TOP_MARGIN;
    }
}

bool ArduRCT_GraphicsUIScreen::process(uint8_t actionId, int16_t x, int16_t y) {
    if (actionId == GRAPHICS_UI_ACTION_TOUCH) {
        if (_selectedElement != 0) {
            _selectedElement->release();
            _selectedElement->escape();
            _drawElement(_selectedElement);
        }
        // find an item at these coordinates
        _selectedElement = _findElementAt(x, y);
        if (_selectedElement == 0) return _processOutOfScreenTouch(x, y);
        int touchX = x-_selectedElement->x;
        int touchY = y-_selectedElement->y;
        if (_selectedElement->section == GRAPHICS_UI_SECTION_MAIN) {
            touchX -= _topX;
            touchY -= _topY;
        } else if (_selectedElement->section == GRAPHICS_UI_SECTION_FOOTER) {
            touchY -= _height - _firstFooterElement->height;
        }
        ArduRCT_GraphicsUIElement *modifiedElement = _selectedElement->touch(touchX, touchY);
        if (modifiedElement != 0) _drawElement(modifiedElement);
        _drawElement(_selectedElement);
        return true;
    } else if (actionId == GRAPHICS_UI_ACTION_UNTOUCH) {
        // release the item
        if (_selectedElement == 0) return false;
        _selectedElement->release();
        bool runOnEscape = _selectedElement->escape();
        _drawElement(_selectedElement);
        if (runOnEscape) _selectedElement->run();
        _selectedElement = 0;
        return true;
    } else if ((actionId == GRAPHICS_UI_ACTION_UP) || (actionId == GRAPHICS_UI_ACTION_DOWN)) {
        if (_currentElement == 0) {
            _currentElement = _findNextElement(false);
            if (_currentElement == 0) return false;
            _currentElement->highlight();
            if (_adjustTopToDisplayCurrentElement()) _draw();
            else _drawElement(_currentElement);
        } else if (_selectedElement != 0) {
            // we are inside a selected elt
            bool runOnIncreaseOrDecrease = false;
            int16_t value = _selectedElement->getValue();
            if (actionId == GRAPHICS_UI_ACTION_UP) runOnIncreaseOrDecrease = _selectedElement->increase();
            else runOnIncreaseOrDecrease = _selectedElement->decrease();
            if (_selectedElement->getValue() != value) _drawElement(_selectedElement);
            if (runOnIncreaseOrDecrease) _selectedElement->run();
        } else {
            ArduRCT_GraphicsUIElement *nextElement = _findNextElement(actionId == GRAPHICS_UI_ACTION_UP);
            if (nextElement != 0) {
                _currentElement->escape();
                _drawElement(_currentElement);
                _currentElement = nextElement;
                _currentElement->highlight();
                if (_adjustTopToDisplayCurrentElement()) _draw();
                else _drawElement(_currentElement);
            }
        }
        return true;
    } else if (actionId == GRAPHICS_UI_ACTION_ENTER) {
        if (_currentElement == 0) return true;
        _selectedElement = _currentElement;
        ArduRCT_GraphicsUIElement *modifiedElement = _selectedElement->enter();
        if (modifiedElement != 0) _drawElement(modifiedElement);
        _drawElement(_selectedElement);
        return true;
    } else if (actionId == GRAPHICS_UI_ACTION_MENU) {
        if (_selectedElement == 0) return false;
        bool runOnEscape = _selectedElement->escape();
        _selectedElement->highlight();
        _drawElement(_selectedElement);
        if (runOnEscape) _selectedElement->run();
        _selectedElement = 0;
        return true;
    } else if (actionId == GRAPHICS_UI_ACTION_RELEASE) {
        if (_selectedElement == 0) return true;
        bool escapeAfterRelease = _selectedElement->release();
        bool runOnEscape = false;
        if (escapeAfterRelease) {
            runOnEscape = _selectedElement->escape();
            _selectedElement->highlight();				
        }
        _drawElement(_selectedElement);
        if (escapeAfterRelease) {
            if (runOnEscape) _selectedElement->run();
            _selectedElement = 0;
        }
        return true;
    }
    return false;
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIScreen::getElement(uint8_t id) {
    ArduRCT_GraphicsUIElement *elt = _firstElement;
    while (elt != 0) {
        if (elt->getId() == id) return elt;
        elt = elt->getNext();
    }
    elt = _firstHeaderElement;
    while (elt != 0) {
        if (elt->getId() == id) return elt;
        elt = elt->getNext();
    }
    elt = _firstFooterElement;
    while (elt != 0) {
        if (elt->getId() == id) return elt;
        elt = elt->getNext();
    }
    return 0;
}

void ArduRCT_GraphicsUIScreen::setElementValue(uint8_t id, int16_t value) {
    ArduRCT_GraphicsUIElement *elt = getElement(id);
    if (elt == 0) return;
    ArduRCT_GraphicsUIElement *modifiedElement = elt->setValue(value);
    if ((modifiedElement != 0) && _enabled) _drawElement(modifiedElement);
    if (_enabled) _drawElement(elt);
}

int16_t ArduRCT_GraphicsUIScreen::getElementValue(uint8_t id) {
    ArduRCT_GraphicsUIElement *elt = getElement(id);
    if (elt == 0) return 0;
    return elt->getValue();
}

void ArduRCT_GraphicsUIScreen::_reset() {
    ArduRCT_GraphicsUIElement *elt = _firstElement;
    while (elt) {
        elt->setState(GRAPHICS_UI_RELEASED);
        elt = elt->getNext();
    }
    elt = _firstHeaderElement;
    while (elt) {
        elt->setState(GRAPHICS_UI_RELEASED);
        elt = elt->getNext();
    }
    elt = _firstFooterElement;
    while (elt) {
        elt->setState(GRAPHICS_UI_RELEASED);
        elt = elt->getNext();
    }
    if (_firstHeaderElement) {
        _headerHeight = _firstHeaderElement->height + GRAPHICS_UI_ELEMENT_TOP_MARGIN;
        _topY = _headerHeight;
    } else _topY = 0;
    if (_firstFooterElement) _footerHeight = _firstFooterElement->height + GRAPHICS_UI_ELEMENT_TOP_MARGIN;
    _currentElement = 0;
    _selectedElement = 0;
    _currentElementSection = 0;
}

void ArduRCT_GraphicsUIScreen::_draw() {
    _graphics->setBackgroundColor(_backgroundColor);
    if (_position == GRAPHICS_POSITION_CENTER) {
        _graphics->fillRoundedRectangle(_x, _y, _width, _height, GRAPHICS_UI_ELEMENT_TOP_MARGIN, _backgroundColor);
        _graphics->drawRoundedRectangle(_x, _y, _width, _height, GRAPHICS_UI_ELEMENT_TOP_MARGIN, BLACK, 1);
    } else {
        _graphics->fillRectangle(_x, _y, _width, _height, _backgroundColor);
        if ((_x != 0) || (_width != _graphics->getWidth()) || (_y != 0) || (_height != _graphics->getHeight())) _graphics->drawRectangle(_x, _y, _width, _height, BLACK, 1);
    }
    ArduRCT_GraphicsUIElement *elt = _firstElement;
    _firstDisplayedElement = 0;
    while (elt) {
        if (_topY+elt->y+elt->height > _height-_footerHeight) break;
        if (elt->y+_topY >= _headerHeight) {
            if (_firstDisplayedElement == 0) _firstDisplayedElement = elt;
            _lastDisplayedElement = elt;
            elt->draw(_graphics, _x + _topX, _y + _topY, _width);
        }
        elt = elt->getNext();
    }
    elt = _firstHeaderElement;
    while (elt) {
        elt->draw(_graphics,  _x, _y, _width);
        elt = elt->getNext();
    }
    elt = _firstFooterElement;
    while (elt) {
        elt->draw(_graphics, _x, _y + _height - _firstFooterElement->height, _width);
        elt = elt->getNext();
    }	
    if (_firstDisplayedElement) {
        if (_firstDisplayedElement != _firstElement) {
            int headerHeight = _headerHeight;
            if (headerHeight > 0) headerHeight -= GRAPHICS_UI_ELEMENT_TOP_MARGIN-2; 
            _graphics->fillCorner(_x+_width-2, _y+headerHeight, GRAPHICS_UI_SCROLLER_ARROW_SIZE, GRAPHICS_POSITION_SW, GRAPHICS_UI_COLOR_GAUGE);
        }
        if (_lastDisplayedElement != _lastElement) { 
            int footerHeight = _footerHeight;
            if (footerHeight > 0) footerHeight -= GRAPHICS_UI_ELEMENT_TOP_MARGIN-2; 
            _graphics->fillCorner(_x+_width-2, _y+_height-footerHeight, GRAPHICS_UI_SCROLLER_ARROW_SIZE, GRAPHICS_POSITION_NW, GRAPHICS_UI_COLOR_GAUGE);
        }
    }
    if (_currentElement == 0) return;
    _drawElement(_currentElement);
}

void ArduRCT_GraphicsUIScreen::_drawElement(ArduRCT_GraphicsUIElement *elt) {
    int topX = 0;
    int topY = 0;
    if (elt->section == GRAPHICS_UI_SECTION_MAIN) {
        if (elt->y+_topY < _headerHeight) return;
        if (_topY+elt->y+elt->height > _height-_footerHeight) return;
        topX = _topX;
        topY = _topY;
    } else if (elt->section == GRAPHICS_UI_SECTION_FOOTER) {
        topY = _height - _firstFooterElement->height;
    }
    elt->draw(_graphics, _x + topX, _y + topY, _width);
}

bool ArduRCT_GraphicsUIScreen::_processOutOfScreenTouch(int16_t x, int16_t y) {
    // check if we are outside the boundaries of the screen
    if ((y < _y) || (y > _y+_height) || (x < _x) || (x > _x+_width)) {
        // close it and return to previous screen
        if (_previousScreen) _graphics->setGraphicsUIScreen(_previousScreen, _previousScreen->getPreviousScreen());
        return true;
    }
    // check if we have to scroll
    if (x > _width-20) {
        // check if we have to scroll up
        int headerHeight = _headerHeight;
        if (headerHeight > 0) headerHeight -= GRAPHICS_UI_ELEMENT_TOP_MARGIN-2; 
        int footerHeight = _footerHeight;
        if (footerHeight > 0) footerHeight -= GRAPHICS_UI_ELEMENT_TOP_MARGIN-2; 
        if ((_firstDisplayedElement != _firstElement) && (y-_y > headerHeight) && (y-_y < headerHeight+20)) {
            _currentElementSection = 0;
            _currentElement = _firstDisplayedElement->getPrevious();
            _adjustTopToDisplayCurrentElement();
            _draw();
        } else if ((_lastDisplayedElement != _lastElement) && (y-_y > _height-footerHeight-20) && (y-_y < _height-footerHeight)) {
        // check if we have to scroll down
            _currentElementSection = 0;
            _currentElement = _lastDisplayedElement->getNext();
            _adjustTopToDisplayCurrentElement();
            _draw();
        }
    }
    return false;
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIScreen::_findNextEditableElementInSection(ArduRCT_GraphicsUIElement *current, bool upDirection) {
    if (current == 0) return 0;
    if (current->editable) return current;
    if (upDirection) {
        ArduRCT_GraphicsUIElement *elt = current->getPrevious();
        while (elt) {
            if (elt->editable) return elt;
            elt = elt->getPrevious();
        }
        return 0;
    } else {
        ArduRCT_GraphicsUIElement *elt = current->getNext();
        while (elt) {
            if (elt->editable) return elt;
            elt = elt->getNext();
        }
        return 0;
    }
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIScreen::_findNextElement(bool upDirection) {
    ArduRCT_GraphicsUIElement *elt = 0;
    if (_currentElement == 0) {
        _currentElementSection = 0;
        elt = _findNextEditableElementInSection(_firstElement, upDirection);
        if (elt) return elt;
        _currentElementSection = -1;
        elt = _findNextEditableElementInSection(_firstHeaderElement, upDirection);
        if (elt) return elt;
        _currentElementSection = 1;
        return _findNextEditableElementInSection(_firstFooterElement, upDirection);
    }
    if (upDirection) {
        elt = _findNextEditableElementInSection(_currentElement->getPrevious(), true);
        if (elt) return elt;
        if (_currentElementSection == -1) return 0;
        if (_currentElementSection == 0) {
            elt = _findNextEditableElementInSection(_lastHeaderElement, true);
            if (elt) _currentElementSection = -1;
            return elt;
        } else if (_currentElementSection == 1) {
            elt = _findNextEditableElementInSection(_lastElement, true);
            if (elt) _currentElementSection = 0;
            return elt;
        }
        return 0;
    }
    elt = _findNextEditableElementInSection(_currentElement->getNext(), false);
    if (elt) return elt;
    if (_currentElementSection == 1) return 0;
    if (_currentElementSection == -1) {
        elt = _findNextEditableElementInSection(_firstElement, false);
        if (elt) _currentElementSection = 0;
        return elt;
    } else if (_currentElementSection == 0) {
        elt = _findNextEditableElementInSection(_firstFooterElement, false);
        if (elt) _currentElementSection = 1;
        return elt;
    }
    return 0;
}

bool ArduRCT_GraphicsUIScreen::_adjustTopToDisplayCurrentElement() {
    if (_currentElementSection != 0) return false;
    int adjustedTopY = _topY;
    if (_currentElement->y+_currentElement->height+_topY > _height-_footerHeight) {
        adjustedTopY = -_currentElement->y-_currentElement->height+_height-_footerHeight;
    } else if (_currentElement->y+_topY < _headerHeight) {
        adjustedTopY =  _headerHeight-_currentElement->y;
    }
    if (adjustedTopY == _topY) return false;
    _topY = adjustedTopY;
    return true;
}

ArduRCT_GraphicsUIElement *ArduRCT_GraphicsUIScreen::_findElementAt(int16_t x, int16_t y) {
    ArduRCT_GraphicsUIElement *elt = _firstElement;
    while (elt != 0) {
        if (_topY+elt->y+elt->height > _height-_footerHeight) break;
        if (elt->editable && (elt->y+_topY >= _headerHeight) && (y-_y > elt->y+_topY) && (y-_y < elt->y+elt->height+_topY) &&
                (x-_x > elt->x+_topX) && (x-_x < elt->x+elt->width+_topX)) return elt;
        elt = elt->getNext();
    }
    elt = _firstHeaderElement;
    while (elt != 0) {
        if (y > _headerHeight) break;
        if (elt->editable && (y > elt->y) && (y < elt->y+elt->height) && (x > elt->x) && (x < elt->x+elt->width)) return elt;
        elt = elt->getNext();
    }
    elt = _firstFooterElement;
    while (elt != 0) {
        if (y < _height-_footerHeight) break;
        if (elt->editable && (y+_height-_footerHeight > elt->y) && (y+_height-_footerHeight < elt->y+elt->height) && 
                (x > elt->x) && (x < elt->x+elt->width)) return elt;
        elt = elt->getNext();
    }	
    return 0;
}


#endif