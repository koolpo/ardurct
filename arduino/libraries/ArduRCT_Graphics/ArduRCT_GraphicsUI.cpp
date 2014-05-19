/*
 * GraphicsUI - An UI system
 *	Part of Graphics class
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
 
/**
 *    The functions in this file are in fact part of the ArduRCT_Graphics class
 *    They are kept separate as they are only included if GRAPHICS_HAS_UI is defined
 **/
 
#include "ArduRCT_Graphics.h"
#include "ArduRCT_GraphicsUI.hpp"

#if defined(GRAPHICS_HAS_UI)

void ArduRCT_Graphics::addScreen(ArduRCT_GraphicsUIScreen *screen) {
    screen->setGraphics(this);
    if (!_homeScreen) _homeScreen = screen;
}

void ArduRCT_Graphics::setGraphicsUIHome(ArduRCT_GraphicsUIScreen *screen, ArduRCT_GraphicsUIElement *element) {
    _homeScreen = screen;
    _homeElement = element;
}

void ArduRCT_Graphics::setGraphicsUIScreen(ArduRCT_GraphicsUIScreen *screen, ArduRCT_GraphicsUIScreen *returnToScreen) {
    if (!screen) screen = _homeScreen;
    if (_screen == screen) return;
    if (_screen) _screen->deactivate();
    _screen = screen;
    _screen->setPreviousScreen(returnToScreen ? returnToScreen : _homeScreen);
    _screen->activate();
}

bool ArduRCT_Graphics::handleGraphicsUI(uint8_t actionId, int16_t x, int16_t y) {
    if (_screen == 0) return false;
    if (_screen->process(actionId, x, y)) return true;
    if (actionId == GRAPHICS_UI_ACTION_MENU) {
        if (_homeElement) {
            _homeElement->select();
            _homeElement->press();
            _homeElement->release();
            _homeElement->unselect();
        }
        setGraphicsUIScreen(_homeScreen);
        return true;
    }
    return false;
}

#endif