/*
 * ArduRCT_ParallelScreenUtils - Macros to drive parallel hardware screens
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
 
#ifndef ARDURCT_PARALLEL_SCREEN_UTILS_H
#define ARDURCT_PARALLEL_SCREEN_UTILS_H

#include "ArduRCT_Graphics_Configuration.h"

#define _psu_setup() { \
    _widthImpl = GRAPHICS_HARDWARE_WIDTH; \
    _heightImpl = GRAPHICS_HARDWARE_HEIGHT; \
    setupScreen(GRAPHICS_HARDWARE_CD_PIN, GRAPHICS_HARDWARE_WR_PIN, GRAPHICS_HARDWARE_RD_PIN, GRAPHICS_HARDWARE_CS_PIN, GRAPHICS_HARDWARE_RESET_PIN, GRAPHICS_HARDWARE_SPI_ON_BUS);\
    setupBacklight(GRAPHICS_HARDWARE_BACKLIGHT_PIN); \
}


#ifdef __AVR__

#define _psu_setDataBusDirectionAsOutput() { \
    GRAPHICS_HARDWARE_DATA_DDR_PORT = 0xFF; \
}

#define _psu_clearRD() { \
    *_rdPortPtr &= _rdLowBitMask; \
    *_rdPortPtr &= _rdLowBitMask; \
}

#define _psu_prepareWR() { \
    _wrLow = (*_wrPortPtr) & _wrLowBitMask; \
    _wrHigh = (*_wrPortPtr) | _wrHighBitMask; \
}

#define _psu_write8bCommand(cmd) { \
    *_cdPortPtr &= ~_cdBitMask; \
    GRAPHICS_HARDWARE_DATA_OUT_PORT = (cmd); \
    *_wrPortPtr &= _wrLowBitMask; \
    *_wrPortPtr |= _wrHighBitMask; \
    *_cdPortPtr |= _cdBitMask; \
}

#define _psu_write16bCommand(cmd) { \
    *_cdPortPtr &= ~_cdBitMask; \
    GRAPHICS_HARDWARE_DATA_OUT_PORT = (cmd >> 8); \
    *_wrPortPtr &= _wrLowBitMask; \
    *_wrPortPtr |= _wrHighBitMask; \
    GRAPHICS_HARDWARE_DATA_OUT_PORT = (cmd); \
    *_wrPortPtr &= _wrLowBitMask; \
    *_wrPortPtr |= _wrHighBitMask; \
    *_cdPortPtr |= _cdBitMask; \
}

#define _psu_write8bData(data) { \
    GRAPHICS_HARDWARE_DATA_OUT_PORT = (data); \
    *_wrPortPtr = _wrLow; \
    *_wrPortPtr = _wrHigh; \
}

#define _psu_write16bData(data) { \
    GRAPHICS_HARDWARE_DATA_OUT_PORT = ((data) >> 8); \
    *_wrPortPtr = _wrLow; \
    *_wrPortPtr = _wrHigh; \
    GRAPHICS_HARDWARE_DATA_OUT_PORT = (data); \
    *_wrPortPtr = _wrLow; \
    *_wrPortPtr = _wrHigh; \
}

/**
  _psu_writeSingleColorBlock uses assembly to go as fast as possible: 
  max nb of pixels takes 24bits (nbPixelsH:nbPixelsM:nbPixelsL) 240*320=76800 takes 17bits
    %0 is the write bit port
    %1 is the data bus port
    %2 is the value the write port needs to take to clear WR
    %3 is the value the write port needs to take to set WR
    %4:%5:%6 are nbPixelsH:nbPixelsM:nbPixelsL
    %7 is high byte of color
    %8 is low byte of color
**/
#define _psu_writeSingleColorBlock(nbPixels, color) { \
    uint8_t colH = color >> 8; \
    uint8_t colL = color; \
    uint8_t nbPixelsH = 1 + (nbPixels >> 16); \
    uint8_t nbPixelsM = 1 + (nbPixels >> 8); \
    uint8_t nbPixelsL = nbPixels; \
    asm ( \
      "cp %7,%8             ; compare colH and colL         \n\t" \
      "brne wscb_hnel       ; if notequal, go to wscb_hnel  \n\t" \
      "out %1,%7            ; PORTB = colH                  \n\t" \
    "wscb_heql:                                             \n\t" \
      "out %0,%2            ; WR = LOW                      \n\t" \
      "out %0,%3            ; WR = HIGH                     \n\t" \
      "out %0,%2            ; WR = LOW                      \n\t" \
      "out %0,%3            ; WR = HIGH                     \n\t" \
      "dec %6               ; pixelL --                     \n\t" \
      "brne wscb_heql       ; if pixelL > 0 goto wscb_heql  \n\t" \
      "dec %5               ; pixelM--                      \n\t" \
      "brne wscb_heql       ; if pixelM > 0 goto wscb_heql  \n\t" \
      "dec %4               ; pixelH--                      \n\t" \
      "brne wscb_heql       ; if pixelH > 0 goto wscb_heql  \n\t" \
      "rjmp wscb_end        ; go to end                     \n\t" \
    "wscb_hnel:                                             ²\n\t" \
      "out %1,%7            ; PORTB = colH                  \n\t" \
      "out %0,%2            ; WR = LOW                      \n\t" \
      "out %0,%3            ; WR = HIGH                     \n\t" \
      "out %1,%8            ; PORTB = colL                  \n\t" \
      "out %0,%2            ; WR = LOW                      \n\t" \
      "out %0,%3            ; WR = HIGH                     \n\t" \
      "dec %6               ; pixelL --                     \n\t" \
      "brne wscb_hnel       ; if pixelL > 0 goto wscb_hnel  \n\t" \
      "dec %5               ; pixelM--                      \n\t" \
      "brne wscb_hnel       ; if pixelM > 0 goto wscb_hnel  \n\t" \
      "dec %4               ; pixelH--                      \n\t" \
      "brne wscb_hnel       ; if pixelH > 0 goto wscb_hnel  \n\t" \
    "wscb_end:                                              \n\t" \
    :: \
    "I" (_SFR_IO_ADDR(GRAPHICS_HARDWARE_WR_PORT)), \
    "I" (_SFR_IO_ADDR(GRAPHICS_HARDWARE_DATA_OUT_PORT)), \
    "r" (_wrLow), \
    "r" (_wrHigh), \
    "r" (nbPixelsH), \
    "r" (nbPixelsM), \
    "r" (nbPixelsL), \
    "r" (colH), \
    "r" (colL) \
    ); \
}

#define _psu_write16bBitmapBuffer(bitmap, width, height) { \
    uint8_t *bitmap8 = (uint8_t *)bitmap; \
    uint8_t pixelL; \
    uint8_t pixelH; \
    uint16_t nbPixels = width; \
    nbPixels *= height; \
    while (nbPixels--) { \
        pixelL = *bitmap8; \
        bitmap8++; \
        pixelH = *bitmap8; \
        bitmap8++; \
        GRAPHICS_HARDWARE_DATA_OUT_PORT = pixelH; \
        *_wrPortPtr = _wrLow; \
        *_wrPortPtr = _wrHigh; \
        GRAPHICS_HARDWARE_DATA_OUT_PORT = pixelL; \
        *_wrPortPtr = _wrLow; \
        *_wrPortPtr = _wrHigh; \
    } \
}

#endif




#if defined(__arm__) && defined(CORE_TEENSY)

#define _psu_setDataBusDirectionAsOutput() { \
    pinMode(GRAPHICS_HARDWARE_D0, OUTPUT); \
    pinMode(GRAPHICS_HARDWARE_D1, OUTPUT); \
    pinMode(GRAPHICS_HARDWARE_D2, OUTPUT); \
    pinMode(GRAPHICS_HARDWARE_D3, OUTPUT); \
    pinMode(GRAPHICS_HARDWARE_D4, OUTPUT); \
    pinMode(GRAPHICS_HARDWARE_D5, OUTPUT); \
    pinMode(GRAPHICS_HARDWARE_D6, OUTPUT); \
    pinMode(GRAPHICS_HARDWARE_D7, OUTPUT); \
}

#define _psu_prepareWR() /* void */

#define _psu_write8bCommand(cmd) { \
    digitalWriteFast(GRAPHICS_HARDWARE_CD_PIN, 0);\
    *(volatile uint8_t *)(&GPIOD_PDOR) = (cmd); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
    asm("nop"); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
    digitalWriteFast(GRAPHICS_HARDWARE_CD_PIN, 1);\
}

#define _psu_write16bCommand(cmd) { \
    digitalWriteFast(GRAPHICS_HARDWARE_CD_PIN, 0);\
    *(volatile uint8_t *)(&GPIOD_PDOR) = (cmd >> 8); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
    asm("nop"); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
    *(volatile uint8_t *)(&GPIOD_PDOR) = (cmd); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
    asm("nop"); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
    digitalWriteFast(GRAPHICS_HARDWARE_CD_PIN, 1);\
}
    
#define _psu_write8bData(data) { \
    *(volatile uint8_t *)(&GPIOD_PDOR) = (data); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
    asm("nop"); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
}

#define _psu_write16bData(data) { \
    *(volatile uint8_t *)(&GPIOD_PDOR) = ((data) >> 8); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
    asm("nop"); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
    *(volatile uint8_t *)(&GPIOD_PDOR) = (data); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
    asm("nop"); \
    digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
}

#define _psu_writeSingleColorBlock(nbPixels, color) { \
    uint32_t colH = color >> 8; \
    uint32_t colL = color; \
    if (colH == colL) { \
        *(volatile uint32_t *)(&GPIOD_PDOR) = colH; \
        while (nbPixels--) { \
            digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
            asm("nop"); \
            digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
            asm("nop"); \
            digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
            asm("nop"); \
            digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
        } \
    } else { \
        while (nbPixels--) { \
            *(volatile uint32_t *)(&GPIOD_PDOR) = colH; \
            digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
            asm("nop"); \
            digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
            *(volatile uint32_t *)(&GPIOD_PDOR) = colL; \
            digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
            asm("nop"); \
            digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
        } \
    } \
}

#define _psu_write16bBitmapBuffer(bitmap, width, height) { \
    uint32_t nbPixels = width; \
    nbPixels *= height; \
    while (nbPixels--) { \
        *(volatile uint32_t *)(&GPIOD_PDOR) = (*bitmap) >> 8; \
        digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
        asm("nop"); \
        digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
        *(volatile uint32_t *)(&GPIOD_PDOR) = *bitmap; \
        digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 0);\
        asm("nop"); \
        digitalWriteFast(GRAPHICS_HARDWARE_WR_PIN, 1);\
        bitmap++;\
    } \
}
#endif

#endif