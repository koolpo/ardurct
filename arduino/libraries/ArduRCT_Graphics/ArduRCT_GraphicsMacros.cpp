/*
 * Macros - Macro language to draw on the screen
 *    Part of Graphics class
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
 *    They are kept separate as they are only included if GRAPHICS_HAS_MACROS is defined
 **/
 
#include "ArduRCT_Graphics.h"
#include "ArduRCT_GraphicsMacros.hpp"

#if defined(GRAPHICS_HAS_MACROS)

/* 
 *    A macro is a serie of drawing commands which can be chained together.
 *  Each command is a few letters followed by a space and some parameters separated by spaces
 *
 *    Case:        There is no case sentitiveness. 
 *    Chaining:     If the number of parameters is not sufficient to finish the command, 
 *                the last point reached by the previous command is used 
 *    Numbers:    Colors are in hexadecimal
 *                all other numbers are in decimal
 *
 *    Examples
 *    --------
 *        Draw a thick red line of 3 segments
 *            pt 3 pc F800 l 10 10 30 40 l 40 50 
 *        Draw a blue seven
 *            pt 1 pc 001F l 10 10 30 10 l 30 17 arnww 38
 *
 *    Command syntax
 *  --------------
 *     (items between brackets can be ommited)
 *
 *        p    Preset            can preset the thickness, color, background, scale, fontSize, fontBold, fontOverlay
 *            pe                erase the screen: fill the screen with background
 *            pt thickness    preset the thickness
 *            ps scale [div]    preset the scale, all further drawing will be scaled by scale/div
 *            pc color        preset the foreground color
 *            pb color        preset the background color
 *            pf[b][o] size    preset the font
 *                b            if present the font will be bold
 *                o            if present the font will be overlaid
 *        
 *        l    Line
 *            l[d] x1 y1 [x2 y2]
 *                d        delta: x1 and y1 (or x2 and y2 if they exist) are delta values relative to last point drawn
 *                x1 y1    start or next point in the line
 *                x2 y2    next point in the line
 *
 *        a    Arc            can be drawn clockwise or counter clockwise and filled
 *            a[r][f]DDD [x0 y0] radius
 *                r        if present, the arc will be drawn reversed (counter clockwise). 
 *                        i.e.: if DDD=NE
 *                            if r the East end will be connected to the last point drawn
 *                            else the North end will be attached
 *                f        if present, the arc will be filled        
 *                DDD     can be n, s, e, w, ne, nw, se, sw, nne, nee, see, sse, ssw, sww, nww, nnw
 *                radius    radius of the arc
 *                x0 y0    center of the arc. If ommited the top (or bottom) of the arc will be connected to the last point drawn
 *
 *        r    Rectangle    can be rounded and filled
 *            r[r][f] x1 y1 width height [radius]
 *                r        if present, the rectangle will be rounded and radius is required
 *                f        if present, the rectangle will be filled
 *                x1 y1    top left of the rectangle
 *                width
 *                height    width and height of the rectangle
 *
 *        t    Triangle    can be filled
 *            t[f] x1 y1 x2 y2 x3 y3
 *                x1 y1    first point
 *                x2 y2    second point
 *                x3 y3    third point
 *
 *        c    Circle        can be filled
 *            c[f] x0 y0 radius
 *                x0 y0    center of the circle
 *                radius    radius of the circle
 * 
 *        s    String
 *            s x1 y1 "text"
 *                x1 y1    top left of the first char drawn
 *                "text"    text to draw. Some characters can be escaped \n, \", \\
 *
 *        w    Write        Write a macro in EEPROM, all following commands are written to the eeprom
 *            w n
 *                n        the macro number to write
 *
 *        e    Execute        Execute a macro from EEPROM
 *            e[r] n
 *                r        reset the macro variables before executing
 *                n        the macro number to execute
 */

#define GRAPHICS_MACRO_FORMAT_ERROR -1

#define GRAPHICS_MACRO_CMD_GROUP_MASK 0xF0

#define GRAPHICS_MACRO_CMD_GROUP_NONE            0x00
#define GRAPHICS_MACRO_CMD_GROUP_PRESET        0x10
#define GRAPHICS_MACRO_CMD_GROUP_LINE         0x20
#define GRAPHICS_MACRO_CMD_GROUP_ARC             0x30
#define GRAPHICS_MACRO_CMD_GROUP_RECTANGLE    0x40
#define GRAPHICS_MACRO_CMD_GROUP_TRIANGLE        0x50
#define GRAPHICS_MACRO_CMD_GROUP_CIRCLE        0x60
#define GRAPHICS_MACRO_CMD_GROUP_STRING        0x70
#define GRAPHICS_MACRO_CMD_GROUP_EXECUTE        0xF0

#define GRAPHICS_MACRO_COS45_LSH16    46341

#define GRAPHICS_MACRO_CMD_LINKED_FLAG    0x08

#define GRAPHICS_MACRO_PARAM_X1 0
#define GRAPHICS_MACRO_PARAM_Y1 1
#define GRAPHICS_MACRO_PARAM_X2 2
#define GRAPHICS_MACRO_PARAM_Y2 3
#define GRAPHICS_MACRO_PARAM_X3 4
#define GRAPHICS_MACRO_PARAM_Y3 5
#define GRAPHICS_MACRO_PARAM_WIDTH 2
#define GRAPHICS_MACRO_PARAM_HEIGHT 3
#define GRAPHICS_MACRO_PARAM_ROUNDING 4
#define GRAPHICS_MACRO_PARAM_RADIUS 2
#define GRAPHICS_MACRO_PARAM_THICKNESS 0
#define GRAPHICS_MACRO_PARAM_THICKNESS_IS_SCALABLE 1
#define GRAPHICS_MACRO_PARAM_FONT_SIZE 0
#define GRAPHICS_MACRO_PARAM_FONT_IS_BOLD 1
#define GRAPHICS_MACRO_PARAM_FONT_IS_OVERLAY 2
#define GRAPHICS_MACRO_PARAM_SCALE_MUL 0
#define GRAPHICS_MACRO_PARAM_SCALE_DIV 1
#define GRAPHICS_MACRO_PARAM_ARC_1 0
#define GRAPHICS_MACRO_PARAM_ARC_3 2
#define GRAPHICS_MACRO_PARAM_ARC_OCTANT 3
#define GRAPHICS_MACRO_PARAM_MACRO_NUMBER 0


extern uint8_t eeprom_read_uint8_t(uint16_t address);
extern void eeprom_write_uint8_t(uint16_t address, uint8_t value);

uint8_t *ArduRCT_Graphics::executeMacro(uint8_t *s, int16_t x, int16_t y, uint16_t scaleMul, uint16_t scaleDiv, bool continueLastMacro, bool selectAndUnselectScreen) {
    ardurct_graphicsMacroCommand_t mc;
    bool drawMode = true;
    uint8_t wBuffer[256];
    uint16_t wBufferPtr = 0;
    uint8_t wBufferNb = 0;
    
    // initialize relative origin
    if (!continueLastMacro) _initializeMacros();
    uint16_t i = 0;
    // convert to upper case, transform all char below space as space
    _formatMacroSentence(s);
    while (s[i] != 0) {
        bool hasText = false;
        // get the command
        switch (s[i]) {
            case 'L':
                mc.cmd = GRAPHICS_MACRO_CMD_LINE;
                if (s[i+1] == 'D') {
                    mc.cmd = GRAPHICS_MACRO_CMD_LINE_DELTA;
                    i++;
                }
                if (s[i+1] != ' ') return 0;
                break;
            case 'A':
                mc.cmd = GRAPHICS_MACRO_CMD_ARC;
                if (s[i+1] == 'R') {
                    mc.cmd = GRAPHICS_MACRO_CMD_ARC_REVERSED;
                    i++;
                }
                if (s[i+1] == 'F') {
                    if (mc.cmd == GRAPHICS_MACRO_CMD_ARC) mc.cmd = GRAPHICS_MACRO_CMD_ARC_FILLED;
                    else mc.cmd = GRAPHICS_MACRO_CMD_ARC_FILLED_REVERSED;
                    i++;
                } 
                if (s[i+1] == 'N') {
                    if (s[i+2] == 'N') {
                        if (s[i+3] == 'E') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_NNE;
                        else if (s[i+3] == 'W') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_NNW;
                        else if (s[i+3] != ' ') return  0;
                    } else if (s[i+2] == 'E') {
                        if (s[i+3] == 'E') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_NEE;
                        else if (s[i+3] == ' ') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_NE;
                        else return 0;
                    } else if (s[i+2] == 'W') {
                        if (s[i+3] == 'W') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_NWW;
                        else if (s[i+3] == ' ') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_NW;
                        else return 0;
                    } else if (s[i+2] == ' ') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_N;
                    else return 0;
                } else if (s[i+1] == 'S') {
                    if (s[i+2] == 'S') {
                        if (s[i+3] == 'E') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_SSE;
                        else if (s[i+3] == 'W') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_SSW;
                        else if (s[i+3] != ' ') return 0;
                    } else if (s[i+2] == 'E') {
                        if (s[i+3] == 'E') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_SEE;
                        else if (s[i+3] == ' ') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_SE;
                        else return 0;
                    } else if (s[i+2] == 'W') {
                        if (s[i+3] == 'W') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_SWW;
                        else if (s[i+3] == ' ') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_SW;
                        else return 0;
                    } else if (s[i+2] == ' ') mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_S;
                    else return 0;
                } else if ((s[i+1] == 'E') && (s[i+2] == ' ')) mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_E;
                else if ((s[i+1] == 'W') && (s[i+2] == ' ')) mc.param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = GRAPHICS_ARC_W;
                else return 0;
                while (s[i+1] != ' ') i++;
                break;
            case 'R':
                if (s[i+1] == 'R') {
                    i++;
                    if (s[i+1] == 'F') {
                        i++;
                        mc.cmd = GRAPHICS_MACRO_CMD_RECTANGLE_ROUNDED_FILLED;
                    } else if (s[i+1] == ' ') mc.cmd = GRAPHICS_MACRO_CMD_RECTANGLE_ROUNDED;
                } else if (s[i+1] == 'F') {
                    mc.cmd = GRAPHICS_MACRO_CMD_RECTANGLE_FILLED;
                    i++;
                } else mc.cmd = GRAPHICS_MACRO_CMD_RECTANGLE;
                if (s[i+1] != ' ') return 0;
                break;
            case 'S':
                mc.cmd = GRAPHICS_MACRO_CMD_STRING;
                hasText = true;
                if (s[i+1] != ' ') return 0;
                break;
            case 'C':
                if (s[i+1] == 'F') {
                    i++;
                    mc.cmd = GRAPHICS_MACRO_CMD_CIRCLE_FILLED;
                } else mc.cmd = GRAPHICS_MACRO_CMD_CIRCLE;
                if (s[i+1] != ' ') return 0;
                break;
            case 'T':
                if (s[i+1] == 'F') {
                    i++;
                    mc.cmd = GRAPHICS_MACRO_CMD_TRIANGLE_FILLED;
                } else mc.cmd = GRAPHICS_MACRO_CMD_TRIANGLE;
                if (s[i+1] != ' ') return 0;
                break;
            case 'P':
                if (s[i+1] == 'T') {
                    mc.param[GRAPHICS_MACRO_PARAM_THICKNESS_IS_SCALABLE] = 0;
                    mc.cmd = GRAPHICS_MACRO_CMD_PRESET_THICKNESS;
                    i++;
                    if (s[i+1] == 'S') {
                        i++;
                        mc.param[GRAPHICS_MACRO_PARAM_THICKNESS_IS_SCALABLE] = 1;
                    }
                    if (s[i+1] != ' ') return 0;
                } else if (s[i+1] == 'S') {
                    mc.cmd = GRAPHICS_MACRO_CMD_PRESET_SCALE;
                    i++;
                    if (s[i+1] != ' ') return 0;
                } else if (s[i+1] == 'F') {
                    mc.cmd = GRAPHICS_MACRO_CMD_PRESET_FONT;
                    mc.param[GRAPHICS_MACRO_PARAM_FONT_IS_BOLD] = 0;
                    mc.param[GRAPHICS_MACRO_PARAM_FONT_IS_OVERLAY] = 0;
                    i++;
                    if (s[i+1] == 'B') {
                        mc.param[GRAPHICS_MACRO_PARAM_FONT_IS_BOLD] = 1;
                        i++;
                    } 
                    if (s[i+1] == 'O') {
                        mc.param[GRAPHICS_MACRO_PARAM_FONT_IS_OVERLAY] = 1;
                        i++;
                    } 
                    if (s[i+1] != ' ') return 0;
                } else if (s[i+1] == 'C') {
                    int8_t len = _parseMacroCommandHexColor(s, i+2, &mc);
                    if (len == GRAPHICS_MACRO_FORMAT_ERROR) return 0;
                    i += len+1;
                    mc.cmd = GRAPHICS_MACRO_CMD_PRESET_FOREGROUND;
                } else if (s[i+1] == 'B') {
                    int8_t len = _parseMacroCommandHexColor(s, i+2, &mc);
                    if (len == GRAPHICS_MACRO_FORMAT_ERROR) return 0;
                    i += len+1;
                    mc.cmd = GRAPHICS_MACRO_CMD_PRESET_BACKGROUND;
                } else if (s[i+1] == 'E') {
                    mc.cmd = GRAPHICS_MACRO_CMD_PRESET_ERASE;
                    i++;
                    if (s[i+1] != ' ') return 0;
                } else return 0;
                break;
            case 'E':
                if (s[i+1] == 'R') {
                    i++;
                    mc.cmd = GRAPHICS_MACRO_CMD_EXECUTE_WITH_RESET;
                } else mc.cmd = GRAPHICS_MACRO_CMD_EXECUTE;
                if (s[i+1] != ' ') return 0;
                break;
            case 'W':
                drawMode = false;
                mc.cmd = GRAPHICS_MACRO_CMD_WRITE;
                if (s[i+1] != ' ') return 0;
                break;
            case ' ':
                mc.cmd = GRAPHICS_MACRO_CMD_NONE;
                break;
            default:
                return 0;
        }
        i++;
        
        if (mc.cmd == GRAPHICS_MACRO_CMD_NONE) continue;
        
        // parse the command parameters
        mc.nbParams = 0;
        int len = 0;
        while (true) {
            len = _parseMacroCommandParameter(s, i, &mc, mc.nbParams);
            if (len == GRAPHICS_MACRO_FORMAT_ERROR) break;
            i += len;
            mc.nbParams ++;
        }
        if (hasText) {
            // search for start of string
            while ((s[i] != 0) && (s[i] != '"')) i++;
            if (s[i] != '"') return 0;
            i++;
            int txtStart = i;
            // search for end of string
            while ((s[i] != 0) && (s[i] != '"')) {
                // replace the FF by quotes
                if (s[i] == 0xFF) s[i] = '"';
                i++;
            }
            if (s[i] != '"') return 0;
            s[i] = 0;
            mc.text = &s[txtStart];
            mc.textLen = i - txtStart;
            i++;
        }
        
        // Execute the command
        if (drawMode) executeMacroCommand(&mc, x, y, scaleMul, scaleDiv, true, selectAndUnselectScreen);
        else if (mc.cmd == GRAPHICS_MACRO_CMD_WRITE) {
            wBufferNb = mc.param[GRAPHICS_MACRO_PARAM_MACRO_NUMBER];
            if (wBufferNb > GRAPHICS_MACRO_MAX_NUMBER) return 0;
        }

        // At this stage, we have a struct containing the command to execute
        // this can be stored in EEPROM for future execution
        if (mc.cmd != GRAPHICS_MACRO_CMD_WRITE) {
            int16_t endWrite = _compressMacroCommand(&mc, wBuffer, wBufferPtr);
            if (endWrite != GRAPHICS_MACRO_FORMAT_ERROR) wBufferPtr = endWrite;
        }
    }
    if (!drawMode) {
        // write to EEPROM
        if ((wBufferPtr > 0) && (wBufferPtr < GRAPHICS_MACRO_MAX_SIZE)) {
            eeprom_write_uint8_t(wBufferNb, wBufferPtr);
            for (int j=0; j<wBufferPtr; j++) eeprom_write_uint8_t(GRAPHICS_MACRO_MAX_NUMBER + wBufferNb*GRAPHICS_MACRO_MAX_SIZE+j, wBuffer[j]);
        }
    }
    wBuffer[wBufferPtr] = 0;    
    return wBuffer;
}


void ArduRCT_Graphics::executeMacroCommand(ardurct_graphicsMacroCommand_t *mc, int16_t x, int16_t y, uint16_t scaleMul, uint16_t scaleDiv, bool continueLastMacro, bool selectAndUnselectScreen) {
    // initialize relative origin
    if (!continueLastMacro) _initializeMacros();
    if (selectAndUnselectScreen) selectScreenImpl();
    _executeMacroCommand(mc, x, y, scaleMul, scaleDiv);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::executeEepromMacro(uint8_t macroNb, int16_t x, int16_t y, uint16_t scaleMul, uint16_t scaleDiv, bool continueLastMacro, bool selectAndUnselectScreen) {
    ardurct_graphicsMacroCommand_t mc;
    // initialize relative origin
    if (!continueLastMacro) _initializeMacros();
    mc.cmd = GRAPHICS_MACRO_CMD_EXECUTE;
    mc.param[GRAPHICS_MACRO_PARAM_MACRO_NUMBER] = macroNb;
    if (selectAndUnselectScreen) selectScreenImpl();
    _executeMacroCommand(&mc, x, y, scaleMul, scaleDiv);
    if (selectAndUnselectScreen) unselectScreenImpl();
}


void ArduRCT_Graphics::_initializeMacros() {
    _mX = 0;
    _mY = 0;
    _mThickness = _thickness;
    _mScaleMul = 1;
    _mScaleDiv = 1;
    _mBackgroundColor = _backgroundColor;
    _mForegroundColor = _foregroundColor;
    _mFontSize = _fontSize;
    _mIsFontOverlay = _isFontOverlay;
    _mIsFontBold = _isFontBold;
}


void ArduRCT_Graphics::_formatMacroSentence(uint8_t *s) {
    uint16_t i=0;
    while (s[i] != 0) {
        // before quotes, convert lower into higher
        while ((s[i] != 0) && (s[i] != '"')) {
            if ((s[i] >= 'a') && (s[i] <= 'z')) s[i] = s[i] + 'A' - 'a';
            // replace characters not allowed by spaces
            if ((s[i] < '0') || (s[i] > 'Z') || ((s[i] > '9') && (s[i] < 'A'))) s[i] = ' ';
            i++;
        }
        if (s[i] == 0) break;
        i++;
        // inside the quotes, replace escapes
        while ((s[i] != 0) && (s[i] != '"')) {
            char c = (char)s[i];
            if (s[i] == '\\') {
                if (s[i+1] == 'n') s[i+1] = '\n';
                else if (s[i+1] == 't') s[i+1] = ' ';
                // replace the inside quotes by FF, this will be undone when the string command is parsed
                else if (s[i+1] == '"') s[i+1] = 0xFF;
                int j = i;
                while (s[j] != 0) s[j] = s[++j];
                i++;
            } else if (s[i] == 0xFF) s[i] = ' ';    // because quotes are now FF, replace FF by space
            i++;
        }
        if (s[i] == 0) break;
        i++;
    }
}


void ArduRCT_Graphics::_executeMacroCommand(ardurct_graphicsMacroCommand_t *mc, int16_t x, int16_t y, uint16_t scaleMul, uint16_t scaleDiv) {
    uint8_t group = mc->cmd & GRAPHICS_MACRO_CMD_GROUP_MASK;
    
    // presets
    if (group == GRAPHICS_MACRO_CMD_GROUP_PRESET) {
        if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_FOREGROUND) _mForegroundColor = mc->color;
        else if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_BACKGROUND) _mBackgroundColor = mc->color;
        else if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_THICKNESS) {
            _mThickness = mc->param[GRAPHICS_MACRO_PARAM_THICKNESS];
            _mIsThicknessScalable = mc->param[GRAPHICS_MACRO_PARAM_THICKNESS_IS_SCALABLE] != 0;
        } else if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_ERASE) fillScreen(_mBackgroundColor);
        else if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_FONT) {
            _mFontSize = mc->param[GRAPHICS_MACRO_PARAM_FONT_SIZE];
            _mIsFontBold = (mc->param[GRAPHICS_MACRO_PARAM_FONT_IS_BOLD] != 0);
            _mIsFontOverlay = (mc->param[GRAPHICS_MACRO_PARAM_FONT_IS_OVERLAY] != 0);
        } else if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_SCALE) {
            _mScaleMul = mc->param[GRAPHICS_MACRO_PARAM_SCALE_MUL];
            _mScaleDiv = 1;
            if ((mc->nbParams > 1) && (mc->param[GRAPHICS_MACRO_PARAM_SCALE_DIV] != 0)) _mScaleDiv = mc->param[GRAPHICS_MACRO_PARAM_SCALE_DIV];
        }
        return;
    }
    
    int32_t sMul = _mScaleMul;
    sMul = sMul * (scaleMul == 0 ? 1 : scaleMul);
    int32_t sDiv = _mScaleDiv;
    sDiv = sDiv * (scaleDiv == 0 ? 1 : scaleDiv);
    int32_t sX = mc->param[GRAPHICS_MACRO_PARAM_X1];
    sX = sX * sMul/sDiv + x;
    int32_t sY = mc->param[GRAPHICS_MACRO_PARAM_Y1];        
    sY = sY * sMul/sDiv + y;        
    int32_t sThickness = _mThickness;
    if (_mIsThicknessScalable) sThickness = sThickness * sMul/sDiv;    
    
    // lines
    if (group == GRAPHICS_MACRO_CMD_GROUP_LINE) {
        int sX2, sY2;
        if (mc->nbParams < 2) return;
        if (mc->cmd == GRAPHICS_MACRO_CMD_LINE) {
            if (mc->nbParams > 2) {
                sX2 = mc->param[GRAPHICS_MACRO_PARAM_X2];
                sX2 = x + sX2 * sMul/sDiv;
                sY2 = mc->param[GRAPHICS_MACRO_PARAM_Y2];
                sY2 = y + sY2 * sMul/sDiv;
                // p2 becomes the end point
                _mX = mc->param[GRAPHICS_MACRO_PARAM_X2];
                _mY = mc->param[GRAPHICS_MACRO_PARAM_Y2];
            } else {
                sX2 = sX;
                sY2 = sY;
                sX = _mX;
                sX = x + sX * sMul/sDiv;
                sY = _mY;    
                sY = y + sY * sMul/sDiv;    
                // p1 becomes the end point
                _mX = mc->param[GRAPHICS_MACRO_PARAM_X1];
                _mY = mc->param[GRAPHICS_MACRO_PARAM_Y1];
            }
        } else {
            // delta values
            if (mc->nbParams > 2) {
                // p2 = p1+delta
                sX2 = mc->param[GRAPHICS_MACRO_PARAM_X1] + mc->param[GRAPHICS_MACRO_PARAM_X2];
                sX2 = x + sX2 * sMul/sDiv;
                sY2 = mc->param[GRAPHICS_MACRO_PARAM_Y1] + mc->param[GRAPHICS_MACRO_PARAM_Y2];
                sY2 = y + sY2 * sMul/sDiv;
                // p1+delta becomes the end point
                _mX = mc->param[GRAPHICS_MACRO_PARAM_X1] + mc->param[GRAPHICS_MACRO_PARAM_X2];
                _mY = mc->param[GRAPHICS_MACRO_PARAM_Y1] + mc->param[GRAPHICS_MACRO_PARAM_Y2];
            } else {
                // p2 = old_point+delta
                sX2 = _mX + mc->param[GRAPHICS_MACRO_PARAM_X1];
                sX2 = x + sX2 * sMul/sDiv;
                sY2 = _mY + mc->param[GRAPHICS_MACRO_PARAM_Y1];
                sY2 = y + sY2 * sMul/sDiv;
                sX = _mX;
                sX = x + sX * sMul/sDiv;
                sY = _mY;    
                sY = y + sY * sMul/sDiv;    
                // old_point+delta  becomes the end point
                _mX += mc->param[GRAPHICS_MACRO_PARAM_X1];
                _mY += mc->param[GRAPHICS_MACRO_PARAM_Y1];
            }
            
        }
        drawLine(sX, sY, sX2, sY2, _mForegroundColor, sThickness, false);
        return;
    }    
    
    // arcs
    if (group == GRAPHICS_MACRO_CMD_GROUP_ARC) {
        if (mc->nbParams < 1) return;
        boolean reversed = (mc->cmd == GRAPHICS_MACRO_CMD_ARC_REVERSED) || (mc->cmd == GRAPHICS_MACRO_CMD_ARC_FILLED_REVERSED);
        int32_t sRadius = 0;
        if (mc->nbParams == 1) {
            sRadius = mc->param[GRAPHICS_MACRO_PARAM_ARC_1];
            sRadius = sRadius * sMul/sDiv;
            int32_t sArcStartX = _getArcEnd(sRadius, mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT], !reversed, true);
            int32_t sArcStartY = _getArcEnd(sRadius, mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT], !reversed, false);
            int32_t sArcEndX = _getArcEnd(sRadius, mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT], reversed, true);
            int32_t sArcEndY = _getArcEnd(sRadius, mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT], reversed, false);
            sX = _mX;
            sX = x + sX * sMul/sDiv - sArcStartX;
            sY = _mY;
            sY = y + sY * sMul/sDiv - sArcStartY;
            _mX += sArcEndX - sArcStartX;
            _mY += sArcEndY - sArcStartY;

        } else if (mc->nbParams == 3) {
            sRadius = mc->param[GRAPHICS_MACRO_PARAM_ARC_3];
            sRadius = sRadius * sMul/sDiv;
            _mX = mc->param[GRAPHICS_MACRO_PARAM_X1] + _getArcEnd(mc->param[GRAPHICS_MACRO_PARAM_ARC_3], mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT], reversed, true);
            _mY = mc->param[GRAPHICS_MACRO_PARAM_Y1] + _getArcEnd(mc->param[GRAPHICS_MACRO_PARAM_ARC_3], mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT], reversed, false);
        } else return;
        if ((mc->cmd == GRAPHICS_MACRO_CMD_ARC_REVERSED) || (mc->cmd == GRAPHICS_MACRO_CMD_ARC)) 
            drawArc(sX, sY, sRadius, mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT], _mForegroundColor, sThickness, false);
        else fillArc(sX, sY, sRadius, mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT], _mForegroundColor, false);            
        return;
    }
    
    // circles
    if (group == GRAPHICS_MACRO_CMD_GROUP_CIRCLE) {
        // we need at least 3 parameters
        if (mc->nbParams < 3) return;
        int32_t sRadius = mc->param[GRAPHICS_MACRO_PARAM_RADIUS];
        sRadius = sRadius * sMul/sDiv;
        if (mc->cmd == GRAPHICS_MACRO_CMD_CIRCLE) drawCircle(sX, sY, sRadius, _mForegroundColor, sThickness, false);
        else fillCircle(sX, sY, sRadius, _mForegroundColor, false);
        return;
    }
    
    // rectangles
    if (group == GRAPHICS_MACRO_CMD_GROUP_RECTANGLE) {
        // we need at least 4 parameters
        if (mc->nbParams < 4) return;
        int32_t sWidth = mc->param[GRAPHICS_MACRO_PARAM_WIDTH];
        sWidth = sWidth * sMul/sDiv;
        int32_t sHeight = mc->param[GRAPHICS_MACRO_PARAM_HEIGHT];
        sHeight = sHeight * sMul/sDiv;
        if (mc->cmd == GRAPHICS_MACRO_CMD_RECTANGLE) drawRectangle(sX, sY, sWidth, sHeight, _mForegroundColor, sThickness, false);
        else if (mc->cmd == GRAPHICS_MACRO_CMD_RECTANGLE_FILLED) fillRectangle(sX, sY, sWidth, sHeight, _mForegroundColor, false);
        // we need at least 1 more parameter to display the rounded rectangle
        if (mc->nbParams < 5) return;
        int32_t sRadius = mc->param[GRAPHICS_MACRO_PARAM_ROUNDING];
        sRadius = sRadius * sMul/sDiv;
        if (mc->cmd == GRAPHICS_MACRO_CMD_RECTANGLE_ROUNDED) drawRoundedRectangle(sX, sY, sWidth, sHeight, sRadius, _mForegroundColor, sThickness, false);
        else if (mc->cmd == GRAPHICS_MACRO_CMD_RECTANGLE_ROUNDED_FILLED) fillRoundedRectangle(sX, sY, sWidth, sHeight, sRadius, _mForegroundColor, false);
        return;
    }
    
    // triangles
    if (group == GRAPHICS_MACRO_CMD_GROUP_TRIANGLE) {
        // we need at least 6 parameters
        if (mc->nbParams < 6) return;
        int32_t sX2 = mc->param[GRAPHICS_MACRO_PARAM_X2];
        sX2 = x + sX2 * sMul/sDiv;
        int32_t sY2 = mc->param[GRAPHICS_MACRO_PARAM_Y2];
        sY2 = y + sY2 * sMul/sDiv;
        int32_t sX3 = mc->param[GRAPHICS_MACRO_PARAM_X3];
        sX3 = x + sX3 * sMul/sDiv;
        int32_t sY3 = mc->param[GRAPHICS_MACRO_PARAM_Y3];
        sY3 = y + sY3 * sMul/sDiv;
        if (mc->cmd == GRAPHICS_MACRO_CMD_TRIANGLE) drawTriangle(sX, sY, sX2, sY2, sX3, sY3, _mForegroundColor, sThickness, false);
        else if (mc->cmd == GRAPHICS_MACRO_CMD_TRIANGLE_FILLED) fillTriangle(sX, sY, sX2, sY2, sX3, sY3, _mForegroundColor, false);
        return;
    }
        
    // strings
    if (mc->cmd == GRAPHICS_MACRO_CMD_STRING) {
        if (mc->nbParams < 2) return;
        int bc = _backgroundColor;
        if (!_mIsFontOverlay) setBackgroundColor(_mBackgroundColor);
        drawString((char *)mc->text, sX, sY, _mForegroundColor, _mFontSize, _mIsFontBold, _mIsFontOverlay, false);
        if (!_mIsFontOverlay) setBackgroundColor(bc);
        return;
    }
    
    // executes
    if (group == GRAPHICS_MACRO_CMD_GROUP_EXECUTE) {
        if (mc->param[GRAPHICS_MACRO_PARAM_MACRO_NUMBER] >= GRAPHICS_MACRO_MAX_NUMBER) return;
        // read the length in the EEPROM allocation table
        int length = eeprom_read_uint8_t(mc->param[GRAPHICS_MACRO_PARAM_MACRO_NUMBER]);
        if (length == 0xFF) return;
        // read the EEPROM pointers table to get the start
        int start = GRAPHICS_MACRO_MAX_NUMBER + mc->param[GRAPHICS_MACRO_PARAM_MACRO_NUMBER] * GRAPHICS_MACRO_MAX_SIZE;
        // get the compressed macro
        uint8_t buffer[GRAPHICS_MACRO_MAX_SIZE];
        uint8_t i=0;
        while (i < length) {
            buffer[i] = eeprom_read_uint8_t(start+i);
            i++;
        }
        buffer[i] = 0;
        ardurct_graphicsMacroCommand_t emc;
        // uncompress the macro commands and execute them
        if (mc->cmd == GRAPHICS_MACRO_CMD_EXECUTE_WITH_RESET) _initializeMacros();
        i = 0;
        while (i < length) {
            int8_t len = _uncompressMacroCommand(buffer, i, &emc);
            if (len == GRAPHICS_MACRO_FORMAT_ERROR) return;
            _executeMacroCommand(&emc, x, y, scaleMul, scaleDiv);
            i = len;
        }
    }
}



int8_t ArduRCT_Graphics::_parseMacroCommandHexColor(uint8_t *s, int16_t n, ardurct_graphicsMacroCommand_t *mc) {
    int16_t i = n;
    mc->color = 0;
    // remove front spaces
    while ((s[i] != 0) && (s[i] <= ' ')) i++;
    if (s[i] == 0) return GRAPHICS_MACRO_FORMAT_ERROR;
    // get the digits    
    while ((s[i] != 0) && (s[i] > ' ')) {
        // check for unknown digits
        if ((s[i] < '0') || (s[i] > 'F') || ((s[i] > '9') && (s[i] < 'A'))) return GRAPHICS_MACRO_FORMAT_ERROR;
        mc->color = (mc->color << 4) + s[i] - (s[i] < 'A' ? '0' :  'A' - 10);
        i++;
    }
    return i-n;
}

int8_t ArduRCT_Graphics::_parseMacroCommandParameter(uint8_t *s, int16_t n, ardurct_graphicsMacroCommand_t *mc, uint8_t paramId) {
    int16_t i = n;
    boolean negative = false;
    // remove front spaces
    while ((s[i] != 0) && (s[i] <= ' ')) i++;
    // if no value, return error
    if (s[i] == 0) return GRAPHICS_MACRO_FORMAT_ERROR;
    // get the digits    
    if (s[i] == '-') {
        negative = true;
        i++;
    }
    int16_t value = 0;
    while ((s[i] != 0) && (s[i] > ' ')) {
        // check for unknown digits
        if ((s[i] < '0') || (s[i] > '9')) return GRAPHICS_MACRO_FORMAT_ERROR;
        value = value * 10 + s[i] - '0';
        i++;
    }
    if (negative) mc->param[paramId] = -value;
    else mc->param[paramId] = value;
    return i-n;
}

int32_t ArduRCT_Graphics::_getArcEnd(uint32_t radius, uint8_t octant, bool isReversed, bool isX) {
    int32_t value = radius;
    value = (value * GRAPHICS_MACRO_COS45_LSH16) >> 16;
    switch (octant) {
        case GRAPHICS_ARC_SSE:
            if (isReversed && isX) return value;
            if (isReversed) return value;
            if (isX) return 0;
            return radius;
        case GRAPHICS_ARC_SEE:
            if (isReversed && isX) return radius;
            if (isReversed) return 0;
            if (isX) return value;
            return value;
        case GRAPHICS_ARC_NEE:
            if (isReversed && isX) return value;
            if (isReversed) return -value;
            if (isX) return radius;
            return 0;
        case GRAPHICS_ARC_NNE:
            if (isReversed && isX) return 0;
            if (isReversed) return -radius;
            if (isX) return value;
            return -value;
        case GRAPHICS_ARC_SSW:
            if (isReversed && isX) return 0;
            if (isReversed) return radius;
            if (isX) return -value;
            return value;
        case GRAPHICS_ARC_SWW:
            if (isReversed && isX) return -value;
            if (isReversed) return value;
            if (isX) return -radius;
            return 0;
        case GRAPHICS_ARC_NWW:
            if (isReversed && isX) return -radius;
            if (isReversed) return 0;
            if (isX) return -value;
            return -value;
        case GRAPHICS_ARC_NNW:
            if (isReversed && isX) return -value;
            if (isReversed) return -value;
            if (isX) return 0;
            return -radius;
        case GRAPHICS_ARC_NE:
            if (isReversed && isX) return 0;
            if (isReversed) return -radius;
            if (isX) return radius;
            return 0;
        case GRAPHICS_ARC_SE:
            if (isReversed && isX) return radius;
            if (isReversed) return 0;
            if (isX) return 0;
            return radius;
        case GRAPHICS_ARC_SW:
            if (isReversed && isX) return 0;
            if (isReversed) return radius;
            if (isX) return radius;
            return 0;
        case GRAPHICS_ARC_NW:
            if (isReversed && isX) return -radius;
            if (isReversed) return 0;
            if (isX) return 0;
            return -radius;
        case GRAPHICS_ARC_N:
            if (isReversed && isX) return -radius;
            if (isReversed) return 0;
            if (isX) return radius;
            return 0;
        case GRAPHICS_ARC_S:
            if (isReversed && isX) return radius;
            if (isReversed) return 0;
            if (isX) return -radius;
            return 0;
        case GRAPHICS_ARC_E:
            if (isReversed && isX) return 0;
            if (isReversed) return -radius;
            if (isX) return 0;
            return radius;
        case GRAPHICS_ARC_W:
            if (isReversed && isX) return 0;
            if (isReversed) return radius;
            if (isX) return 0;
            return -radius;
    }
    return 0;
}

/*
 * Compression format for commands
 *    dependant on command
 *        cmd param1 .. paramN
 *
 *     Uncompressed:     L 10 10 30 10 L 30 17 ARNWW 38
 *    Compressed:     20 0A 0A 1E 0A 28 1E 11 3A 40 26 
 *    Ratio:            11 / 30
 *
 */
int16_t ArduRCT_Graphics::_compressMacroCommand(ardurct_graphicsMacroCommand_t *mc, uint8_t *buffer, uint16_t bufferPtr) {
    
    uint16_t i = bufferPtr;
    buffer[i++] = mc->cmd;
    uint8_t group = mc->cmd & GRAPHICS_MACRO_CMD_GROUP_MASK;
    // presets
    if (group == GRAPHICS_MACRO_CMD_GROUP_PRESET) {
        if ((mc->cmd == GRAPHICS_MACRO_CMD_PRESET_FOREGROUND) || (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_BACKGROUND)) { 
            buffer[i++] = mc->color >> 8;
            buffer[i++] = mc->color & 0x0FF;
        } else if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_THICKNESS) { 
            buffer[i++] = (mc->param[GRAPHICS_MACRO_PARAM_THICKNESS] & 0x07F) +
                (mc->param[GRAPHICS_MACRO_PARAM_THICKNESS_IS_SCALABLE] != 0 ? 0x80 : 0);
        } else if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_FONT) {
            buffer[i++] = (mc->param[GRAPHICS_MACRO_PARAM_FONT_SIZE] & 0x0F) + 
                (mc->param[GRAPHICS_MACRO_PARAM_FONT_IS_BOLD] != 0 ? 0x40 : 0) + 
                (mc->param[GRAPHICS_MACRO_PARAM_FONT_IS_OVERLAY] != 0 ? 0x80 : 0);
        }     
        if ((mc->cmd == GRAPHICS_MACRO_CMD_PRESET_SCALE) && (mc->nbParams == 1)) {
            mc->param[GRAPHICS_MACRO_PARAM_SCALE_DIV] = 1;
            mc->nbParams = 2;
        }
        // for the other presets, we can return, nothing more to do
        // GRAPHICS_MACRO_CMD_PRESET_ERASE
        else return i;
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_ARC) {
        // if only radius, indicate that the command is linked with previous one
        if (mc->nbParams == 1) buffer[i-1] |= GRAPHICS_MACRO_CMD_LINKED_FLAG;
        buffer[i++] = mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT];
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_LINE) {
        // if only 1 point, indicate that the command is linked with previous one
        if (mc->nbParams == 2) buffer[i-1] |= GRAPHICS_MACRO_CMD_LINKED_FLAG;
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_EXECUTE) {
        if (mc->nbParams >= GRAPHICS_MACRO_MAX_NUMBER) return GRAPHICS_MACRO_FORMAT_ERROR;
        buffer[i++] = mc->param[GRAPHICS_MACRO_PARAM_MACRO_NUMBER];
        return i;
    }

    // compress the parameters
    for (uint8_t j=0; j<mc->nbParams; j++) {
        int8_t len = _compressNumber(mc->param[j], buffer, i);
        if (len == GRAPHICS_MACRO_FORMAT_ERROR) return GRAPHICS_MACRO_FORMAT_ERROR;
        i += len;
    }
    if (mc->cmd == GRAPHICS_MACRO_CMD_STRING) {
        buffer[i++] = mc->textLen;
        for (uint8_t j=0; j<mc->textLen; j++) buffer[i++] = mc->text[j];
    } 
    return i;
}

int16_t ArduRCT_Graphics::_uncompressMacroCommand(uint8_t *buffer, uint16_t n, ardurct_graphicsMacroCommand_t *mc) {
    uint16_t i = n;
    mc->cmd = buffer[i++];
    
    uint8_t group = mc->cmd & GRAPHICS_MACRO_CMD_GROUP_MASK;
    bool linked = (mc->cmd & GRAPHICS_MACRO_CMD_LINKED_FLAG) != 0;
    mc->cmd = mc->cmd & ~GRAPHICS_MACRO_CMD_LINKED_FLAG;
    // presets
    if (group == GRAPHICS_MACRO_CMD_GROUP_PRESET) {
        if ((mc->cmd == GRAPHICS_MACRO_CMD_PRESET_FOREGROUND) || (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_BACKGROUND)) { 
            mc->color = (buffer[i] << 8) + buffer[i+1];
            return i+2;
        } 
        if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_ERASE) return i; 
        if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_THICKNESS) { 
            mc->param[GRAPHICS_MACRO_PARAM_THICKNESS] = buffer[i++];
            if ((mc->param[GRAPHICS_MACRO_PARAM_THICKNESS] & 0x80) != 0) mc->param[GRAPHICS_MACRO_PARAM_THICKNESS_IS_SCALABLE] = 1;
            mc->param[GRAPHICS_MACRO_PARAM_THICKNESS] = mc->param[GRAPHICS_MACRO_PARAM_THICKNESS] & 0x7F;
            return i;
        }
        if (mc->cmd == GRAPHICS_MACRO_CMD_PRESET_FONT) {
            mc->param[GRAPHICS_MACRO_PARAM_FONT_SIZE] = buffer[i++];
            mc->param[GRAPHICS_MACRO_PARAM_FONT_IS_BOLD] = ((mc->param[GRAPHICS_MACRO_PARAM_FONT_SIZE] & 0x40) != 0 ? 1 : 0);
            mc->param[GRAPHICS_MACRO_PARAM_FONT_IS_OVERLAY] = ((mc->param[GRAPHICS_MACRO_PARAM_FONT_SIZE] & 0x80) != 0 ? 1 : 0);
            mc->param[GRAPHICS_MACRO_PARAM_FONT_SIZE] = mc->param[GRAPHICS_MACRO_PARAM_FONT_SIZE] & 0x0F;
            return i;
        }     
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_EXECUTE) {
        mc->param[GRAPHICS_MACRO_PARAM_MACRO_NUMBER] = buffer[i++];
        if (mc->nbParams >= GRAPHICS_MACRO_MAX_NUMBER) return GRAPHICS_MACRO_FORMAT_ERROR;
        return i;
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_ARC) {
        mc->param[GRAPHICS_MACRO_PARAM_ARC_OCTANT] = buffer[i++];
        if (linked) mc->nbParams = 1;
        else mc->nbParams = 3;
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_LINE) {
        if (linked) mc->nbParams = 2;
        else mc->nbParams = 4;
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_RECTANGLE) {
        if (mc->cmd > GRAPHICS_MACRO_CMD_RECTANGLE_FILLED) mc->nbParams = 5;
        else mc->nbParams = 4;
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_CIRCLE) {
        mc->nbParams = 3;
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_TRIANGLE) {
        mc->nbParams = 6;
    } else if (group == GRAPHICS_MACRO_CMD_GROUP_STRING) {
        mc->nbParams = 2;
    }

    for (uint8_t j=0; j<mc->nbParams; j++) {
        int8_t len = _uncompressNumber(buffer, i, mc, j);
        if (len == GRAPHICS_MACRO_FORMAT_ERROR) return GRAPHICS_MACRO_FORMAT_ERROR;
        i += len;
    }
    if (group == GRAPHICS_MACRO_CMD_GROUP_STRING) {
        mc->textLen = buffer[i++];
        mc->text = &(buffer[i]);
        mc->text[mc->textLen] = 0;
        for (uint8_t j=0; j<mc->textLen; j++) mc->text[j] = buffer[i++];
    }
    return i;
}

/*
 * Compression format for numbers
 *        NSxxxxxx
 *            N        if N is null, only one byte of data, else 2 bytes
 *            S        if S is null, number is positive
 *            x..x    positive number
 *        1Snnnnnn nnnnnnnn
 *            From -16383 [11111111 11111111] to 16383 [10111111 11111111]
 *        0Snnnnnn
 *            From -63 [01111111] to 63 [00111111]
 */
int8_t ArduRCT_Graphics::_compressNumber(int16_t in, uint8_t *out, uint16_t n) {
    bool negative = false;
    if (in < 0) {
        negative = true;
        in = -in;
    }
    if (in > (0x01 << 14)-1) return GRAPHICS_MACRO_FORMAT_ERROR;
    if (in > (0x01 << 6)-1) {
        // 2 bytes
        out[n] = (in >> 8) | (negative ? (0x01 << 6) : 0x00) | (0x01 << 7);
        out[n+1] = (in & 0x0FF);
        return 2;
    }
    // 1 byte
    out[n] = in | (negative ? (0x01 << 6) : 0x00);
    return 1;
}


int8_t ArduRCT_Graphics::_uncompressNumber(uint8_t *in, uint16_t n,  ardurct_graphicsMacroCommand_t *mc, uint8_t paramId) {
    boolean negative = false;
    if ((in[n] & (0x01 << 6)) != 0) negative = true;
    mc->param[paramId] = in[n] & 0x3F;
    if ((in[n] & (0x01 << 7)) != 0) {
        mc->param[paramId] = (mc->param[paramId] << 8) | in[n+1];
        if (negative) mc->param[paramId] = -mc->param[paramId];
        return 2;
    }
    if (negative) mc->param[paramId] = -mc->param[paramId];
    return 1;    
}

#endif