/*
 * ArduRCT_Graphics - Screen Hardware Abstraction Layer
 *
 * Copyright (c) 2010-2014 Laurent Wibaux <lm.wibaux@gmail.com>
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
 
/*
   fillTriangle is an adapted copy of Adafruit_GFX
 
    Copyright (c) 2013 Adafruit Industries.  All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
 */

#include "ArduRCT_Graphics.h"
#include "ArduRCT_FontBitmaps.hpp"
#include "ArduRCT_EepromUtils.hpp"

ArduRCT_Graphics::ArduRCT_Graphics(void) {
}

void ArduRCT_Graphics::setupScreen(uint8_t cd, uint8_t wr, uint8_t rd, uint8_t cs, uint8_t reset, bool spiOnBus) {    
#ifdef __AVR__
    _rdPortPtr = portOutputRegister(digitalPinToPort(rd));
    _rdHighBitMask = digitalPinToBitMask(rd);
    _rdLowBitMask = ~_rdHighBitMask;
    _wrPortPtr = portOutputRegister(digitalPinToPort(wr));
    _wrHighBitMask = digitalPinToBitMask(wr);
    _wrLowBitMask = ~_wrHighBitMask;
    _cdPortPtr = portOutputRegister(digitalPinToPort(cd));
    _cdBitMask = digitalPinToBitMask(cd);
    if (cs != 0xFF) {
        _csPortPtr = portOutputRegister(digitalPinToPort(cs));
        _csBitMask = digitalPinToBitMask(cs);
    }
#endif
    _rd = rd;
    _wr = wr;
    _cd = cd;
    _cs = cs;
    _reset = reset;
    _spiOnBus = spiOnBus;
    _spiUsed = false;
    _screenSelected = false;
    _backlightPin = 0xFF;
    _width = _widthImpl;
    _height = _heightImpl;
}

void ArduRCT_Graphics::setupScreen(uint8_t cd, uint8_t cs, uint8_t reset) {    
#ifdef __AVR__
    _cdPortPtr = portOutputRegister(digitalPinToPort(cd));
    _cdBitMask = digitalPinToBitMask(cd);
    if (cs != 0xFF) {
        _csPortPtr = portOutputRegister(digitalPinToPort(cs));
        _csBitMask = digitalPinToBitMask(cs);
    }
#endif
    _rd = 0xFF;
    _wr = 0xFF;
    _cd = cd;
    _cs = cs;
    _reset = reset;
    _spiOnBus = false;
    _spiUsed = true;
    _screenSelected = false;
    _backlightPin = 0xFF;
    _width = _widthImpl;
    _height = _heightImpl;
}

void ArduRCT_Graphics::setupBacklight(uint8_t backlightPin) {
    _backlightPin = backlightPin;
    if (_backlightPin == 0xFF) return;
    pinMode(_backlightPin, OUTPUT);
    _backlight = 128;
    analogWrite(_backlightPin, 128);
}

void ArduRCT_Graphics::begin(uint16_t foregroundColor, uint16_t backgroundColor, uint8_t fontSize, bool fontBold, bool fontOverlay) {
    pinMode(_cd, OUTPUT);
    pinMode(_rd, OUTPUT);
    pinMode(_wr, OUTPUT);
    digitalWrite(_cd, HIGH);
    digitalWrite(_rd, HIGH);
    digitalWrite(_wr, HIGH);
    if (_cs != 0xFF)  pinMode(_cs, OUTPUT);

    selectScreenImpl();
#ifdef __AVR__    
    // if we have a reset pin set to 0xFF, the RD and WR have been OR'ed to create the reset
    // they have to be on the same port for this to work
    if (_reset == 0xFF) {
        *_rdPortPtr &= ~(_rdHighBitMask | _wrHighBitMask);
        delay(20);
        *_rdPortPtr |= (_rdHighBitMask | _wrHighBitMask);
    } 
#endif
    if (_reset < 100) {
        if (_cs != 0xFF) digitalWrite(_cs, LOW);
        pinMode(_reset, OUTPUT);
        digitalWrite(_reset, HIGH);
        delay(200);
        digitalWrite(_reset, LOW);
        delay(200);
        digitalWrite(_reset, HIGH);
        delay(200);
        if (_cs != 0xFF) digitalWrite(_cs, HIGH);
    }    
    initScreenImpl();
    unselectScreenImpl();

    _rotation = 0;
    _margin = 0;
    setFont(1, FONT_PLAIN, NO_OVERLAY);
#if defined(CONFIGURATION_HAS_MACROS)    
    _initializeMacros();
#endif    
    if (_backlightPin != 0xFF) setupBacklight(_backlightPin);
    setFont(fontSize, fontBold, fontOverlay);
    setBackgroundColor(backgroundColor);
    setForegroundColor(foregroundColor);
    if (_backlightPin != 0xFF) setBacklight(0);
    if (backgroundColor != BLACK) fillRectangle(0, 0, _width, _height, backgroundColor);
    if (_backlightPin != 0xFF) setBacklight(128);
}

void ArduRCT_Graphics::setBacklight(uint8_t value) {
    if (_backlightPin == 0xFF) return;
    _backlight = value;
    analogWrite(_backlightPin, _backlight);
}


uint8_t ArduRCT_Graphics::getBacklight() {
    return _backlight;
}

void ArduRCT_Graphics::setRotation(uint8_t rotation, bool selectAndUnselectScreen) {
    _rotation = rotation;
    if (_rotation > GRAPHICS_ROTATION_270) _rotation = (_rotation % 4);
    if ((_rotation == GRAPHICS_ROTATION_0) || (_rotation == GRAPHICS_ROTATION_180)) {
        _width = _widthImpl;
        _height = _heightImpl;
    } else {
        _width = _heightImpl;
        _height = _widthImpl;
    }
    if (selectAndUnselectScreen) selectScreenImpl();
    setRotationImpl(_rotation);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


uint8_t ArduRCT_Graphics::getRotation() {
    return _rotation;
}


void ArduRCT_Graphics::getRotatedXY(int16_t *x, int16_t *y) {
    if (_rotation == GRAPHICS_ROTATION_0) return;

    int16_t X = *x;
    int16_t Y = *y;
    
    if (_rotation == GRAPHICS_ROTATION_90) {
        *x = _widthImpl - 1 - Y;
        *y = X;
    } else if (_rotation == GRAPHICS_ROTATION_180) {
        *x = _widthImpl - 1 - X;
        *y = _heightImpl - 1 - Y;
    } else if (_rotation == GRAPHICS_ROTATION_270) {
        *x = Y;
        *y = _heightImpl - 1 - X;
    } 
}

bool ArduRCT_Graphics::isVertical() {
    return ((_rotation == GRAPHICS_ROTATION_0) || (_rotation == GRAPHICS_ROTATION_180));
}

uint16_t ArduRCT_Graphics::getForegroundColor() {
    return _foregroundColor;
}

void ArduRCT_Graphics::setForegroundColor(uint16_t color) {
    _foregroundColor = color;
}


uint16_t ArduRCT_Graphics::getBackgroundColor() {
    return _backgroundColor;
}

void ArduRCT_Graphics::setBackgroundColor(uint16_t color) {
    _backgroundColor = color;
}


void ArduRCT_Graphics::setThickness(int8_t thickness) {
    _thickness = thickness;
}

int8_t ArduRCT_Graphics::getThickness() {
    return _thickness;
}

uint16_t ArduRCT_Graphics::getMargin() {
    return _margin;
}

void ArduRCT_Graphics::setMargin(uint16_t margin) {
    _margin = margin;
}


uint8_t ArduRCT_Graphics::getFontSize() {
    return _fontSize;
}

bool ArduRCT_Graphics::isFontBold() {
    return _isFontBold;
}

bool ArduRCT_Graphics::isFontOverlay() {
    return _isFontOverlay;
}

void ArduRCT_Graphics::setFont(uint8_t size, bool bold, bool overlay) {
    _fontSize = size;
    _isFontBold = bold;
    _isFontOverlay = overlay;
    if (_fontSize > FONT_LAST_DEF*2) _fontSize = FONT_LAST_DEF*2;
    if (_fontSize == 0) _fontSize = 1;
    _fontScale = (_fontSize <= FONT_LAST_DEF ? 1 : 2);
    _fontDef = &fontDefinition_small;
    if ((FONT_LAST_DEF >= 2) && ((_fontSize == 2) || (_fontSize == 2+FONT_LAST_DEF))) _fontDef = &fontDefinition_medium;
    else if ((FONT_LAST_DEF >= 3) && ((_fontSize == 3) || (_fontSize == 3+FONT_LAST_DEF))) _fontDef = &fontDefinition_big;
}


uint16_t ArduRCT_Graphics::getStringWidth(char *s, uint8_t fontSize) {
    uint8_t validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
    uint8_t fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
    fontDefinition_t *fontDef = &fontDefinition_small;
    if ((FONT_LAST_DEF >= 2) && ((validFontSize == 2) || (validFontSize == 2+FONT_LAST_DEF))) fontDef = &fontDefinition_medium;
    else if ((FONT_LAST_DEF >= 3) && ((validFontSize == 3) || (validFontSize == 3+FONT_LAST_DEF))) fontDef = &fontDefinition_big;
    uint16_t nbChr = 0;
    while (s[nbChr] != 0) nbChr ++;
    return nbChr * (fontDef->width + fontDef->charSpacing) * fontScale;
}

uint8_t ArduRCT_Graphics::getFontHeight(uint8_t fontSize) {
    uint8_t validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
    uint8_t fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
    fontDefinition_t *fontDef = &fontDefinition_small;
    if ((FONT_LAST_DEF >= 2) && ((validFontSize == 2) || (validFontSize == 2+FONT_LAST_DEF))) fontDef = &fontDefinition_medium;
    else if ((FONT_LAST_DEF >= 3) && ((validFontSize == 3) || (validFontSize == 3+FONT_LAST_DEF))) fontDef = &fontDefinition_big;
    return (fontDef->height + fontDef->lineSpacing) * fontScale;
}

uint8_t ArduRCT_Graphics::getFontCharWidth(uint8_t fontSize) {
    uint8_t validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
    uint8_t fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
    fontDefinition_t *fontDef = &fontDefinition_small;
    if ((FONT_LAST_DEF >= 2) && ((validFontSize == 2) || (validFontSize == 2+FONT_LAST_DEF))) fontDef = &fontDefinition_medium;
    else if ((FONT_LAST_DEF >= 3) && ((validFontSize == 3) || (validFontSize == 3+FONT_LAST_DEF))) fontDef = &fontDefinition_big;
    return (fontDef->width + fontDef->charSpacing) * fontScale;
}

void ArduRCT_Graphics::getStringBoundingBox(char *s, uint8_t fontSize, bool isBold, uint8_t marginX, uint16_t *width, uint16_t *height) {
    uint8_t validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
    uint8_t fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
    fontDefinition_t *fontDef = &fontDefinition_small;
    if ((FONT_LAST_DEF >= 2) && ((validFontSize == 2) || (validFontSize == 2+FONT_LAST_DEF))) fontDef = &fontDefinition_medium;
    else if ((FONT_LAST_DEF >= 3) && ((validFontSize == 3) || (validFontSize == 3+FONT_LAST_DEF))) fontDef = &fontDefinition_big;
    uint8_t lineHeight = (fontDef->height + fontDef->lineSpacing + (isBold ? 1 : 0)) * fontScale;
    uint8_t chrWidth = (fontDef->width + fontDef->charSpacing) * fontScale;
    *width = 0;
    *height = lineHeight;
    int16_t x = 0;
    uint8_t i = 0;
    while (s[i] != 0) {
        if (s[i] == '\n') {
            *height += lineHeight;
            x = 0;
        } else x += chrWidth;
        if (x > *width) *width = x;
        if (x > getWidth()-2*marginX-chrWidth) {
            x = 0;
            *height += lineHeight;
        }
        i++;
    }
}

void ArduRCT_Graphics::setCursor(int16_t x, int16_t y) {
    _x = x;
    _y = y;
}


uint16_t ArduRCT_Graphics::getWidth() {
    return _width;
}

uint16_t ArduRCT_Graphics::getHeight() {
    return _height;
}


void ArduRCT_Graphics::drawChar(uint8_t chr, int16_t x, int16_t y, uint16_t color, uint8_t fontSize, bool isBold, bool overlay, bool selectAndUnselectScreen) {
    if ((x < 0) || (y < 0)) return;
    uint8_t validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
    uint8_t fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
    fontDefinition_t *fontDef = &fontDefinition_small;
    if ((FONT_LAST_DEF >= 2) && ((validFontSize == 2) || (validFontSize == 2+FONT_LAST_DEF))) fontDef = &fontDefinition_medium;
    else if ((FONT_LAST_DEF >= 3) && ((validFontSize == 3) || (validFontSize == 3+FONT_LAST_DEF))) fontDef = &fontDefinition_big;
    if ((x + fontDef->width * fontScale >= _width) || (y + fontDef->height * fontScale >= _height)) return;
    if ((chr < fontDef->firstChar) || (chr > fontDef->lastChar)) return;
    if (selectAndUnselectScreen) selectScreenImpl();
    _drawValidChar(chr, x, y, color, validFontSize, fontDef, fontScale, isBold, overlay);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::drawString(char *s, int16_t x, int16_t y, uint16_t color, uint8_t fontSize, bool isBold, bool overlay, bool selectAndUnselectScreen) {
    if ((x < 0) || (y < 0)) return;
    uint8_t validFontSize = (fontSize > FONT_LAST_DEF*2 ? FONT_LAST_DEF*2 : (fontSize < 1 ? 1 : fontSize));
    uint8_t fontScale = (validFontSize <= FONT_LAST_DEF ? 1 : 2);
    fontDefinition_t *fontDef = &fontDefinition_small;
    if ((FONT_LAST_DEF >= 2) && ((validFontSize == 2) || (validFontSize == 2+FONT_LAST_DEF))) fontDef = &fontDefinition_medium;
    else if ((FONT_LAST_DEF >= 3) && ((validFontSize == 3) || (validFontSize == 3+FONT_LAST_DEF))) fontDef = &fontDefinition_big;
    if ((x + fontDef->width * fontScale >= _width) || (y + fontDef->height * fontScale >= _height)) return;
    
    if (selectAndUnselectScreen) selectScreenImpl();
    int16_t lx = x;
    while (s[0] != 0) {
        if (s[0] == '\n') {
            y += (fontDef->height + fontDef->lineSpacing + (isBold ? 1 : 0)) * fontScale;
            if (y >= _height) break;
            lx = x;
        } else if (s[0] == '\r') {
            // skip the character
        } else {
            uint8_t validChr = s[0];
            if ((validChr < fontDef->firstChar) || (validChr > fontDef->lastChar)) validChr = '?';
            _drawValidChar(validChr, lx, y, color, validFontSize, fontDef, fontScale, isBold, overlay);
            lx += (fontDef->width + fontDef->charSpacing) * fontScale;
            if (lx > _width) {
                lx = x;
                y += (fontDef->height + fontDef->lineSpacing + (isBold ? 1 : 0)) * fontScale;
                if (y >= _height) break;
            }
        }
        s++;
    }
    if (selectAndUnselectScreen) unselectScreenImpl();
}


#if ARDUINO >= 100
size_t ArduRCT_Graphics::write(uint8_t chr) {
#else
void ArduRCT_Graphics::write(uint8_t chr) {
#endif
    selectScreenImpl();
    if (chr == '\n') {
        _y += (_fontDef->height + _fontDef->lineSpacing + (_isFontBold ? 1 : 0)) * _fontScale;
        if (_y > _height - 2*_margin) _y = 0;
        _x = 0;
    } else if (chr == '\r') {
        // skip the character
    } else {
        uint8_t validChr = chr;
        if ((validChr < _fontDef->firstChar) || (validChr > _fontDef->lastChar)) validChr = '?';
        _drawValidChar(validChr, _x + _margin, _y + _margin, _foregroundColor, _fontSize, _fontDef, _fontScale, _isFontBold, _isFontOverlay);
        _x += (_fontDef->width + _fontDef->charSpacing) * _fontScale;
        if (_x > _width - 2*_margin) {
            _x = 0;
            _y += (_fontDef->height + _fontDef->lineSpacing + (_isFontBold ? 1 : 0)) * _fontScale;
            if (_y > _height-2*_margin) _y = 0;
        }
    }
    unselectScreenImpl();
#if ARDUINO >= 100
    return 1;
#endif
}

// Only 10 bytes, not worth putting in flash
const uint8_t DIGIT_SEGMENTS[] = { 
    0x3F,    // b00111111    0
    0x30,    // b00110000    1
    0x6D,    // b01101101    2
    0x79,    // b01111001    3
    0x72,    // b01110010    4
    0x5B,    // b01011011    5
    0x5F,    // b01011111    6
    0x31,    // b00110001    7
    0x7F,    // b01111111    8
    0x7B     // b01111011    9
};  

void ArduRCT_Graphics::drawBigDigit(uint8_t d, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, uint8_t thickness, uint8_t style, bool selectAndUnselectScreen) {
    // draws 7 segment style digits
    if (d > 9) return;
    uint16_t h = (height - 3*thickness)/2;
    uint16_t t2 = thickness/5;
    if (t2 == 0) t2 = 1;
    uint16_t x1 = x + width - thickness;
    uint16_t y1 = y,
            y2 = y + h + thickness,
            y3 = y + 2 * (h + thickness);
        
    if (selectAndUnselectScreen) selectScreenImpl();
    if ((DIGIT_SEGMENTS[d] & 0x01) != 0) {
        // horizontal top
        _fillBoundedArea(x+thickness, y1, x+width-thickness-1, y1+thickness-1, color);
        if (style >= GRAPHICS_STYLE_NORMAL) {
            fillCorner(x+thickness-1, y1+thickness-1, thickness, GRAPHICS_POSITION_NW, color, false);
            fillCorner(x+width-thickness, y1+thickness-1, thickness, GRAPHICS_POSITION_NE, color, false);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x02) != 0) {
        // vertical top left
        _fillBoundedArea(x, y1+thickness, x+thickness-1, y1+thickness+h-1, color);
        if (style >= GRAPHICS_STYLE_NORMAL) {
            fillCorner(x+thickness-1, y1+thickness-1, thickness, GRAPHICS_POSITION_NW, color, false);
            fillCorner(x+thickness-1, y1+h+thickness, thickness, GRAPHICS_POSITION_SW, color, false);
        }
        if (style >= GRAPHICS_STYLE_ADVANCED) {
            if ((DIGIT_SEGMENTS[d] & 0x01) != 0) fillCorner(x+thickness, y1+thickness, t2, GRAPHICS_POSITION_SE, color, false);
            if ((DIGIT_SEGMENTS[d] & 0x40) != 0) fillCorner(x+thickness, y1+h+thickness-1, t2, GRAPHICS_POSITION_NE, color, false);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x04) != 0) {
        // vertical bottom left
        _fillBoundedArea(x, y2+thickness, x+thickness-1, y2+thickness+h-1, color);
        if (style >= GRAPHICS_STYLE_NORMAL) {            
            fillCorner(x+thickness-1, y2+thickness-1, thickness, GRAPHICS_POSITION_NW, color, false);
            fillCorner(x+thickness-1, y2+h+thickness, thickness, GRAPHICS_POSITION_SW, color, false);
            if (DIGIT_SEGMENTS[d] == 0x3F || DIGIT_SEGMENTS[d] == 0x5F) fillRectangle(x, y2, thickness, thickness, color, false);
        }
        if (style >= GRAPHICS_STYLE_ADVANCED) {
            if ((DIGIT_SEGMENTS[d] & 0x40) != 0) fillCorner(x+thickness, y2+thickness, t2, GRAPHICS_POSITION_SE, color, false);
            if ((DIGIT_SEGMENTS[d] & 0x08) != 0) fillCorner(x+thickness, y2+h+thickness-1, t2, GRAPHICS_POSITION_NE, color, false);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x08) != 0) {
        // horizontal bottom
        _fillBoundedArea(x+thickness, y3, x+width-thickness-1, y3+thickness-1, color);
        if (style >= GRAPHICS_STYLE_NORMAL) {    
            fillCorner(x+thickness-1, y3, thickness, GRAPHICS_POSITION_SW, color, false);
            fillCorner(x+width-thickness, y3, thickness, GRAPHICS_POSITION_SE, color, false);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x10) != 0) {
        // vertical bottom right
        _fillBoundedArea(x1, y2+thickness, x1+thickness-1, y2+thickness+h-1, color);
        if (style >= GRAPHICS_STYLE_NORMAL) {
            fillCorner(x+width-thickness, y2+thickness-1, thickness, GRAPHICS_POSITION_NE, color, false);
            fillCorner(x+width-thickness, y2+h+thickness, thickness, GRAPHICS_POSITION_SE, color, false);
        }
        if (style >= GRAPHICS_STYLE_ADVANCED) {
            if ((DIGIT_SEGMENTS[d] & 0x40) != 0) fillCorner(x+width-thickness-1, y2+thickness, t2, GRAPHICS_POSITION_SW, color, false);
            if ((DIGIT_SEGMENTS[d] & 0x08) != 0) fillCorner(x+width-thickness-1, y2+h+thickness-1, t2, GRAPHICS_POSITION_NW, color, false);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x20) != 0) {
        // vertical top right
        _fillBoundedArea(x1, y1+thickness, x1+thickness-1, y1+thickness+h-1, color);
        if (style >= GRAPHICS_STYLE_NORMAL) {
            fillCorner(x+width-thickness, y1+thickness-1, thickness, GRAPHICS_POSITION_NE, color, false);
            fillCorner(x+width-thickness, y1+h+thickness, thickness, GRAPHICS_POSITION_SE, color, false);
            if (DIGIT_SEGMENTS[d] == 0x3F || DIGIT_SEGMENTS[d] == 0x30 || DIGIT_SEGMENTS[d] == 0x72 || DIGIT_SEGMENTS[d] == 0x31 || DIGIT_SEGMENTS[d] == 0x7B) 
                fillRectangle(x1, y2, thickness, thickness, color, false);
        }
        if (style >= GRAPHICS_STYLE_ADVANCED) {
            if ((DIGIT_SEGMENTS[d] & 0x01) != 0) fillCorner(x+width-thickness-1, y1+thickness, t2, GRAPHICS_POSITION_SW, color, false);
            if ((DIGIT_SEGMENTS[d] & 0x40) != 0) fillCorner(x+width-thickness-1, y1+h+thickness-1, t2, GRAPHICS_POSITION_NW, color, false);
        }
    }
    if ((DIGIT_SEGMENTS[d] & 0x40) != 0) {
        // horizontal middle
        _fillBoundedArea(x+thickness, y2, x+width-thickness-1, y2+thickness-1, color);
        if (style >= GRAPHICS_STYLE_NORMAL) {
            if (DIGIT_SEGMENTS[d] == 0x79) {
                for (int i=1; i<=thickness/2; i++) _fillBoundedArea(x+thickness-i, y2+i-1, x+thickness-i, y2+thickness-i-1, color);
            }
        }
    }
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::drawPixel(int16_t x, int16_t y, uint16_t color, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();
    _drawBoundedPixel(x, y, color);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


#define swap(a, b) { int16_t t = a; a = b; b = t; }

void ArduRCT_Graphics::drawHorizontalLine(int16_t x1, int16_t x2, int16_t y, uint16_t color, int8_t thickness, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();
    if (thickness <= 1) _fillBoundedArea(x1, y, x2, y, color);
    else {
        int8_t t1 = (thickness-1)/2;
        int8_t t2 = thickness/2;
        if (x2 < x1) swap(x2, x1);
        _fillBoundedArea(x1-t1, y-t1, x2+t2, y+t2, color);
    }
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::drawVerticalLine(int16_t x, int16_t y1, int16_t y2, uint16_t color, int8_t thickness, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();
    if (thickness <= 1) _fillBoundedArea(x, y1, x, y2, color);
    else {
        int8_t t1 = (thickness-1)/2;
        int8_t t2 = thickness/2;
        if (y2 < y1) swap(y2, y1);
        _fillBoundedArea(x-t1, y1-t1, x+t2, y2+t2, color);
    }
    if (selectAndUnselectScreen) unselectScreenImpl();
}

void ArduRCT_Graphics::fillCorner(int16_t x, int16_t y, uint16_t size, uint8_t corner, uint16_t color, bool selectAndUnselectScreen) {
    if (size == 0) return;
    if (selectAndUnselectScreen) selectScreenImpl();
    if (size == 1) _drawBoundedPixel(x, y, color);
    else {
        for (int i=0; i<size; i++) {
            if (corner & GRAPHICS_POSITION_NW) _fillBoundedArea(x-size+i+1, y-i, x, y-i, color);
            if (corner & GRAPHICS_POSITION_NE) _fillBoundedArea(x, y-i, x+size-i-1, y-i, color);
            if (corner & GRAPHICS_POSITION_SE) _fillBoundedArea(x, y+i, x+size-i-1, y+i, color);
            if (corner & GRAPHICS_POSITION_SW) _fillBoundedArea(x-size+i+1, y+i, x, y+i, color);
        }    
    }
    if (selectAndUnselectScreen) unselectScreenImpl();
}

/*
 * From Wikipedia - Bresenham's line algorithm
 */
void ArduRCT_Graphics::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int8_t thickness, bool selectAndUnselectScreen) {
    int16_t dx = abs(x2-x1);
    if (dx == 0) {
        drawVerticalLine(x1, y1, y2, color, thickness, selectAndUnselectScreen);
        return;
    }
    int16_t dy = abs(y2-y1);
    if (dy == 0) {
        drawHorizontalLine(x1, x2, y1, color, thickness, selectAndUnselectScreen);
        return;
    }
    
    int8_t sx = -1;
    if (x1 < x2) sx = 1;
    int8_t sy = -1;
    if (y1 < y2) sy = 1;
    int16_t err = dx - dy;
    int8_t d1 = (thickness-1) / 2;
    int8_t d2 = thickness / 2;

    if (selectAndUnselectScreen) selectScreenImpl();    
    while (1) {
        if (thickness == 1) _drawBoundedPixel(x1, y1, color);
        else _fillBoundedArea(x1-d1, y1-d1, x1+d2, y1+d2, color);
        if ((x1 == x2) && (y1 == y2)) break;
        int16_t e2 = 2 * err;
        if (e2 > -dy) {
            err = err - dy;
            x1 = x1 + sx;
        }
        if (e2 < dx) {
            err = err + dx;
            y1 = y1 + sy;
        }
    }
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int8_t thickness, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();    
    drawLine(x0, y0, x1, y1, color, thickness, false);
    drawLine(x1, y1, x2, y2, color, thickness, false);
    drawLine(x2, y2, x0, y0, color, thickness, false);
    if (selectAndUnselectScreen) unselectScreenImpl();    
}

/* 
    This fillTriangle is adapted from Adafruit_GFX
 */
void ArduRCT_Graphics::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool selectAndUnselectScreen) {
    int16_t a, b, y, last;

    if (y0 > y1) {
        swap(y0, y1); 
        swap(x0, x1);
    }
    if (y1 > y2) {
        swap(y2, y1); 
        swap(x2, x1);
    }
    if (y0 > y1) {
        swap(y0, y1); 
        swap(x0, x1);
    }

    if (selectAndUnselectScreen) selectScreenImpl();    
    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)      a = x1;
        else if (x1 > b) b = x1;
        if (x2 < a)      a = x2;
        else if (x2 > b) b = x2;
        _fillBoundedArea(a, y, b, y, color);
        if (selectAndUnselectScreen) unselectScreenImpl();
        return;
    }

    int16_t
        dx01 = x1 - x0,
        dy01 = y1 - y0,
        dx02 = x2 - x0,
        dy02 = y2 - y0,
        dx12 = x2 - x1,
        dy12 = y2 - y1,
        sa   = 0,
        sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2) last = y1;   // Include y1 scanline
    else          last = y1-1; // Skip it

    for (y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if (a > b) swap(a,b);
        _fillBoundedArea(a, y, b, y, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (a > b) swap(a,b);
        _fillBoundedArea(a, y, b, y, color);
    }
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::drawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color, int8_t thickness, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();
    drawHorizontalLine(x, x+width-1, y, color, thickness, false);
    drawHorizontalLine(x, x+width-1, y+height-1, color, thickness, false);
    drawVerticalLine(x, y, y+height-1, color, thickness, false);
    drawVerticalLine(x+width-1, y, y+height-1, color, thickness, false);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::fillRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();
    _fillBoundedArea(x, y, x+width-1, y+height-1, color);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::fillScreen(uint16_t color, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();
    fillAreaImpl(0, 0, _width-1, _height-1, color);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::drawArc(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, int8_t thickness, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();    
    _drawOrFillOctant(x0, y0, r, octant, color, thickness, false);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::fillArc(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();    
    _drawOrFillOctant(x0, y0, r, octant, color, 0, true);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::drawCircle(int16_t x0, int16_t y0, uint16_t r, uint16_t color, int8_t thickness, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();    
    _drawOrFillOctant(x0, y0, r, GRAPHICS_ARC_ALL, color, thickness, false);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::fillCircle(int16_t x0, int16_t y0, uint16_t r, uint16_t color, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();    
    _drawOrFillOctant(x0, y0, r, GRAPHICS_ARC_ALL, color, 0, true);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::drawRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t r, uint16_t color, int8_t thickness, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();    
    _drawOrFillOctant(x+r, y+r, r, GRAPHICS_ARC_NW, color, thickness, false);
    drawHorizontalLine(x+r, x+width-1-r, y, color, thickness, false);
    _drawOrFillOctant(x+width-1-r, y+r, r, GRAPHICS_ARC_NE, color, thickness, false);
    drawVerticalLine(x+width-1, y+r, y+height-1-r, color, thickness, false);
    _drawOrFillOctant(x+r, y+height-1-r, r, GRAPHICS_ARC_SW, color, thickness, false);
    drawHorizontalLine(x+r, x+width-1-r, y+height-1, color, thickness, false);
    _drawOrFillOctant(x+width-1-r, y+height-1-r, r, GRAPHICS_ARC_SE, color, thickness, false);
    drawVerticalLine(x, y+r, y+height-1-r, color, thickness, false);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::fillRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t r, uint16_t color, bool selectAndUnselectScreen) {
    if (selectAndUnselectScreen) selectScreenImpl();    
    _drawOrFillOctant(x+r, y+r, r, GRAPHICS_ARC_NW, color, 0, true);
    _drawOrFillOctant(x+width-1-r, y+r, r, GRAPHICS_ARC_NE, color, 0, true);
    _drawOrFillOctant(x+r, y+height-1-r, r, GRAPHICS_ARC_SW, color, 0, true);
    _drawOrFillOctant(x+width-1-r, y+height-1-r, r, GRAPHICS_ARC_SE, color, 0, true);
    _fillBoundedArea(x, y+r, x+width-1, y+height-1-r, color);
    _fillBoundedArea(x+r, y, x+width-1-r, y+r, color);
    _fillBoundedArea(x+r, y+height-1-r, x+width-1-r, y+height-1, color);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


uint16_t *ArduRCT_Graphics::retrieveBitmap(uint16_t *buffer, int16_t x, int16_t y, uint16_t width, uint16_t height, bool selectAndUnselectScreen) {
    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height) || (width*height > GRAPHICS_MAX_BITMAP_SPACE)) return 0;
    if (selectAndUnselectScreen) selectScreenImpl();
    retrieveBitmapImpl(buffer, x, y, width, height);
    if (selectAndUnselectScreen) unselectScreenImpl();
    return buffer;
}


void ArduRCT_Graphics::pasteBitmap(uint16_t *bitmap, int16_t x, int16_t y, uint16_t width, uint16_t height, bool hasTransparency, uint16_t transparentColor, bool selectAndUnselectScreen) {
    uint16_t buffer[GRAPHICS_MAX_BITMAP_SPACE];

    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
    // check for overflow
    if (hasTransparency && (width * height > GRAPHICS_MAX_BITMAP_SPACE)) return;
    if (selectAndUnselectScreen) selectScreenImpl();
    if (hasTransparency) {
        // get the background image
        retrieveBitmapImpl(buffer, x, y, width, height);
        // set all non transparent pixels of the bitmap in the buffer
        for (uint32_t i=0; i<((uint32_t)width)*height; i++) {
            if (bitmap[i] != transparentColor) buffer[i] = bitmap[i];
        }
        pasteBitmapImpl(buffer, x, y, width, height);
    } else pasteBitmapImpl(bitmap, x, y, width, height);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::drawPattern(uint8_t *pattern, uint8_t orientation, int16_t x, int16_t y, uint8_t width, uint8_t height, 
        uint16_t color, uint16_t backColor, uint8_t scale, bool overlay, bool selectAndUnselectScreen) {
    uint16_t buffer[GRAPHICS_MAX_BITMAP_SPACE];
    uint8_t unpacked[GRAPHICS_MAX_BITMAP_SPACE];
    uint16_t *retrieved = buffer;

    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
    if (width * height * scale * scale > GRAPHICS_MAX_BITMAP_SPACE) return;
    // unpack the pattern
    _unpackPattern(pattern, unpacked, width, height, 0, 0, orientation);
    if (selectAndUnselectScreen) selectScreenImpl();
    // if overlay, get the bitmap from the screen
    if (overlay) retrieved = retrieveBitmapImpl(buffer, x, y, width*scale, height*scale);
    // scale and colorize it, setting a background color if no need to overlay
    _scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, backColor, width, height, scale, 0, 0, !overlay || (overlay && !retrieved));
    // draw it
    if (!retrieved) _drawPatternPixelPerPixel(buffer, x, y, color, width*scale, height*scale);
    else pasteBitmapImpl(buffer, x, y, width*scale, height*scale);
    if (selectAndUnselectScreen) unselectScreenImpl();
}

void ArduRCT_Graphics::startDoubleBuffering(uint16_t *buffer, uint16_t bufferWidth, uint16_t bufferHeight) {
    _buffer = buffer;
    _bufferWidth = bufferWidth;
    _bufferHeight = bufferHeight;
}

void ArduRCT_Graphics::pasteDoubleBuffer(int16_t x, int16_t y, bool selectAndUnselectScreen) {
    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
    if (selectAndUnselectScreen) selectScreenImpl();
    pasteBitmapImpl(_buffer, x, y, _bufferWidth, _bufferHeight);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


/* ---------------- Protected functions ------------------------ */
// the following functions are overwritten by the implementing class

void ArduRCT_Graphics::initScreenImpl() {}
void ArduRCT_Graphics::fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {}
uint16_t *ArduRCT_Graphics::retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {}
void ArduRCT_Graphics::pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {}
void ArduRCT_Graphics::setRotationImpl(uint8_t rotation) {}
void ArduRCT_Graphics::drawPixelImpl(uint16_t x, uint16_t y, uint16_t color) {}
void ArduRCT_Graphics::selectScreenImpl() {}
void ArduRCT_Graphics::unselectScreenImpl() {}        

/* ---------------- Private functions ------------------------ */

void ArduRCT_Graphics::_drawValidChar(uint8_t chr, uint16_t x, uint16_t y, uint16_t color, uint8_t fontSize, fontDefinition_t *fontDef, uint8_t fontScale, bool isBold, bool overlay) {
    uint8_t pattern[FONT_MAX_PATTERN];
    uint8_t unpacked[FONT_MAX_SPACE];
    uint16_t buffer[FONT_MAX_SPACE*2*2];
    uint16_t *retrieved = buffer;

    if ((fontSize % 2) == 1) memcpy_P(pattern, &font_small[chr - fontDef->firstChar], fontDef->orientation == PATTERN_ORIENTATION_HORIZONTAL ? fontDef->height : fontDef->width);
    else if ((fontSize % 2) == 0) memcpy_P(pattern, &font_medium[chr - fontDef->firstChar], fontDef->orientation == PATTERN_ORIENTATION_HORIZONTAL ? fontDef->height : fontDef->width);
    // unpack the pattern
    _unpackPattern(pattern, unpacked, fontDef->width, fontDef->height, fontDef->charSpacing, fontDef->lineSpacing, fontDef->orientation);    
    // if overlay, get the bitmap from the screen
    if (overlay) retrieved = retrieveBitmapImpl(buffer, x, y, (fontDef->width+fontDef->charSpacing)*fontScale, (fontDef->height+fontDef->lineSpacing)*fontScale);
    // scale and colorize it
    _scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef->width+fontDef->charSpacing, fontDef->height+fontDef->lineSpacing, fontScale, 0, 0, !overlay || (overlay && !retrieved));
    if (isBold) {
        _scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef->width+fontDef->charSpacing, fontDef->height+fontDef->lineSpacing, fontScale, 1, 0, false);
        _scaleShiftAndColorizeUnpackedPattern(unpacked, buffer, color, _backgroundColor, fontDef->width+fontDef->charSpacing, fontDef->height+fontDef->lineSpacing, fontScale, 0, 1, false);
    }
    // draw it
    if (!retrieved) _drawPatternPixelPerPixel(buffer, x, y, color, (fontDef->width+fontDef->charSpacing)*fontScale, (fontDef->height+fontDef->lineSpacing)*fontScale);
    else pasteBitmapImpl(buffer, x, y, (fontDef->width+fontDef->charSpacing)*fontScale, (fontDef->height+fontDef->lineSpacing)*fontScale);
}

// Unpacks the bits contained in pattern into unpacked: each byte of unpacked is a bit of pattern
// the bits of the pattern can be stored horizontaly of verticaly
// we can add a border on the right and on the bottom
void ArduRCT_Graphics::_unpackPattern(uint8_t *pattern, uint8_t *unpacked, uint8_t width, uint8_t height, uint8_t borderRight, uint8_t borderBottom, uint8_t orientation) {
    for (uint16_t i=0; i<(width+borderRight)*(height+borderBottom); i++) unpacked[i] = 0;
    for (uint8_t l=0; l<height; l++) {
        for (uint8_t c=0; c<width; c++) {
            uint8_t patternByte;
            uint8_t mask;
            if (orientation == PATTERN_ORIENTATION_HORIZONTAL) {
                patternByte = pattern[l*(1+width/8)+c/8];
                mask = 0x01 << (c & 0x07);
                unpacked[l*(width+borderRight)+c] = patternByte & mask;
            } else {
                patternByte = pattern[c*(1+height/8)+l/8];
                mask = 0x01 << (l & 0x07);
                unpacked[l*(width+borderRight)+c] = patternByte & mask;
            }
        }
    }
}

// draws an unpacked pattern into the buffer, replacing all the result pixels by onColor if the corresponding byte is not null in unpacked
// can scale and shift the unpacked pattern before setting the pixels in the result
// can also fill the result with offColor before replacing
void ArduRCT_Graphics::_scaleShiftAndColorizeUnpackedPattern(uint8_t *unpacked, uint16_t *result, uint16_t onColor, uint16_t offColor, uint8_t width, uint8_t height, uint8_t scale,
    uint8_t xShift, uint8_t yShift, bool blankFirst) {
    if (blankFirst) {
        for (uint16_t i=0; i<(width*height)*scale*scale; i++) result[i] = offColor;
    }
    for (uint8_t l=0; l<height; l++) {
        for (uint8_t c=0; c<width; c++) {
            if (scale == 1) {
                if (unpacked[l*width+c]) result[(l+yShift)*width+(c+xShift)] = onColor;
            } else {
                for (uint8_t pl=0; pl<scale; pl++) {
                    for (uint8_t pc=0; pc<scale; pc++) {
                        if (unpacked[l*width+c]) result[(l*scale+pl+yShift)*width*scale+c*scale+pc+xShift] = onColor;
                    }
                }
            }
        }
    }
}

void ArduRCT_Graphics::_drawPatternPixelPerPixel(uint16_t *buffer, int16_t x, int16_t y, uint16_t onColor, uint8_t width, uint8_t height) {
    for (uint8_t l=0; l<height; l++) {
        for (uint8_t c=0; c<width; c++) {
            if ((buffer[l*width+c] == onColor) && (x+c < _width) && (y+l < _height)) drawPixelImpl(x+c, y+l, onColor);
        }
    } 
}

void ArduRCT_Graphics::_fillBoundedArea(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    if (x2 < x1) swap(x1, x2);
    if (y2 < y1) swap(y1, y2);
    if (_buffer) {
        if (x1 >= _bufferWidth) return;
        if (y1 >= _bufferHeight) return;
        if (x1 < 0) x1 = 0;
        if (x2 >= _bufferHeight) x2 = _bufferWidth-1;
        if (y1 < 0) y1 = 0;
        if (y2 >= _bufferHeight) y2 = _bufferHeight-1;
        for (int y=y1; y<=y2; y++) {
            for (int x=x1; x<=x2; x++) _buffer[y*_bufferWidth+x] = color;
        }
        return;
    }
    if (x1 >= _width) return;
    if (y1 >= _height) return;
    if (x1 < 0) x1 = 0;
    if (x2 >= _width) x2 = _width-1;
    if (y1 < 0) y1 = 0;
    if (y2 >= _height) y2 = _height-1;
    fillAreaImpl(x1, y1, x2, y2, color);
}


void ArduRCT_Graphics::_drawBoundedPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (y < 0)) return;
    if (_buffer) {
        if ((x >= _bufferWidth) || (y >= _bufferHeight)) return;
        _buffer[y*_bufferWidth+x] = color;
    } else if ((x < _width) && (y < _height)) drawPixelImpl(x, y, color);
}

/*
 * From Wikipedia: Andres arc drawing algorithm, extended to 
 *     - draw thick lines and 
 *     - fill with optimal use of horizontal lines
 */    
void ArduRCT_Graphics::_drawOrFillOctant(int16_t x0, int16_t y0, uint16_t r, uint8_t octant, uint16_t color, int8_t thickness, bool fill) {
    int16_t x = 0;
    int16_t y = r;
    int16_t d = r - 1;
    int8_t t1 = (thickness-1)/2;
    int8_t t2 = thickness/2;
    
    while (y >= x) {
        if (fill) {
            if (octant & GRAPHICS_ARC_SSW) _fillBoundedArea(x0, y0+y, x0-x, y0+y, color);
            if (octant & GRAPHICS_ARC_SSE) _fillBoundedArea(x0, y0+y, x0+x, y0+y, color);
            if (octant & GRAPHICS_ARC_SWW) _fillBoundedArea(x0, y0+x, x0-y, y0+x, color);
            if (octant & GRAPHICS_ARC_SEE) _fillBoundedArea(x0, y0+x, x0+y, y0+x, color);
            if (octant & GRAPHICS_ARC_NNW) _fillBoundedArea(x0, y0-y, x0-x, y0-y, color);
            if (octant & GRAPHICS_ARC_NNE) _fillBoundedArea(x0, y0-y, x0+x, y0-y, color);
            if (octant & GRAPHICS_ARC_NWW) _fillBoundedArea(x0, y0-x, x0-y, y0-x, color);        
            if (octant & GRAPHICS_ARC_NEE) _fillBoundedArea(x0, y0-x, x0+y, y0-x, color);
        } else if (thickness != 1) {
            if (octant & GRAPHICS_ARC_SSE) _fillBoundedArea(x0+x-t1, y0+y-t1, x0+x+t2, y0+y+t2, color);
            if (octant & GRAPHICS_ARC_SEE) _fillBoundedArea(x0+y-t1, y0+x-t1, x0+y+t2, y0+x+t2, color);
            if (octant & GRAPHICS_ARC_SSW) _fillBoundedArea(x0-x-t1, y0+y-t1, x0-x+t2, y0+y+t2, color);
            if (octant & GRAPHICS_ARC_SWW) _fillBoundedArea(x0-y-t1, y0+x-t1, x0-y+t2, y0+x+t2, color);
            if (octant & GRAPHICS_ARC_NNE) _fillBoundedArea(x0+x-t1, y0-y-t1, x0+x+t2, y0-y+t2, color);
            if (octant & GRAPHICS_ARC_NEE) _fillBoundedArea(x0+y-t1, y0-x-t1, x0+y+t2, y0-x+t2, color);
            if (octant & GRAPHICS_ARC_NNW) _fillBoundedArea(x0-x-t1, y0-y-t1, x0-x+t2, y0-y+t2, color);
            if (octant & GRAPHICS_ARC_NWW) _fillBoundedArea(x0-y-t1, y0-x-t1, x0-y+t2, y0-x+t2, color);        
        } else {
            if (octant & GRAPHICS_ARC_SSE) _drawBoundedPixel(x0+x, y0+y, color);
            if (octant & GRAPHICS_ARC_SEE) _drawBoundedPixel(x0+y, y0+x, color);
            if (octant & GRAPHICS_ARC_SSW) _drawBoundedPixel(x0-x, y0+y, color);
            if (octant & GRAPHICS_ARC_SWW) _drawBoundedPixel(x0-y, y0+x, color);
            if (octant & GRAPHICS_ARC_NNE) _drawBoundedPixel(x0+x, y0-y, color);
            if (octant & GRAPHICS_ARC_NEE) _drawBoundedPixel(x0+y, y0-x, color);
            if (octant & GRAPHICS_ARC_NNW) _drawBoundedPixel(x0-x, y0-y, color);
            if (octant & GRAPHICS_ARC_NWW) _drawBoundedPixel(x0-y, y0-x, color);
        }
        if (d >= 2*x) {
            d = d - 2*x - 1;
            x = x + 1;
        } else if (d <= 2*(r-y)) {
            d = d + 2*y -1;
            y = y - 1;
        } else {
            d = d + 2*(y-x-1);
            y = y - 1;
            x = x + 1;
        }
    }
}

