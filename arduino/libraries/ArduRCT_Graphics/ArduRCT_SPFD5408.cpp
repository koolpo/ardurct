/*
 * SPFD5408 - Implementation of the ScreenHAL abstract functions for the SPFD5408
 *
 * Copyright (c) 2012 Laurent Wibaux <lm.wibaux@gmail.com>
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

#include "ArduRCT_SPFD5408.h"

#define SPFD5408_WIDTH  240
#define SPFD5408_HEIGHT 320

#define SPFD5408_ENTRY_MODE  0x03
#define SPFD5408_H_AD_SET    0x20
#define SPFD5408_V_AD_SET    0x21
#define SPFD5408_RAM         0x22
#define SPFD5408_H_AD_START  0x50
#define SPFD5408_H_AD_END    0x51
#define SPFD5408_V_AD_START  0x52
#define SPFD5408_V_AD_END    0x53

#define SPFD5408_R0       0x1030
#define SPFD5408_R90      0x1028
#define SPFD5408_R180     0x1000
#define SPFD5408_R270     0x1018

// the following functions are defined as macros to speed up the execution

#define _clearRD() { *_rdPort &= _rdLow; *_rdPort &= _rdLow; }

#define _prepareWR() { _wrPortLowWR = (*_wrPort) & _wrLow; _wrPortHighWR = (*_wrPort) | _wrHigh; }

#define _write16bCommand(cmd) { *_cdPort &= ~_cdBitMask; \
                                SPFD5408_DATA_OUT_PORT = cmd >> 8; *_wrPort &= _wrLow;  *_wrPort |= _wrHigh; \
                                SPFD5408_DATA_OUT_PORT = cmd; *_wrPort &= _wrLow;  *_wrPort |= _wrHigh; \
                                *_cdPort |= _cdBitMask; }

#define _pulseWR() { *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; }

#define _writeData(data8b) { SPFD5408_DATA_OUT_PORT = data8b; *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; }

#define _write16bData(data16b) { SPFD5408_DATA_OUT_PORT = data16b >> 8; *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; \
                                 SPFD5408_DATA_OUT_PORT = data16b; *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; }

ArduRCT_SPFD5408::ArduRCT_SPFD5408() {    
    _widthImpl = SPFD5408_WIDTH;
    _heightImpl = SPFD5408_HEIGHT;
    setupScreen(SPFD5408_CD_PIN, SPFD5408_WR_PIN, SPFD5408_RD_PIN, SPFD5408_CS_PIN, SPFD5408_RESET_PIN, SPFD5408_SPI_ON_BUS);
    setupBacklight(SPFD5408_BACKLIGHT_PIN);
    _clipped = true;
}
 
/* ---------------- Protected functions ------------------------ */

void ArduRCT_SPFD5408::initScreenImpl(void) {
    _prepareWR();
    delay(50);                        /* Delay 50 ms                        */
    _writeRegister(0xE5, 0x8000);     /* Set the internal vcore voltage     */
    _writeRegister(0x00, 0x0001);     /* Start internal OSC                 */
    _writeRegister(0x01, 0x0100);     /* Set SS and SM bit                  */
    _writeRegister(0x02, 0x0700);     /* Set 1 line inversion               */
    _writeRegister(0x03, 0x1030);     /* Set GRAM write direction and BGR=1 */
    _writeRegister(0x04, 0x0000);     /* Resize register                    */
    _writeRegister(0x08, 0x0202);     /* 2 lines each, back and front porch */
    _writeRegister(0x09, 0x0000);     /* Set non-disp area refresh cyc ISC  */
    _writeRegister(0x0A, 0x0000);     /* FMARK function                     */
    _writeRegister(0x0C, 0x0000);     /* RGB interface setting              */
    _writeRegister(0x0D, 0x0000);     /* Frame marker Position              */
    _writeRegister(0x0F, 0x0000);     /* RGB interface polarity             */
    /* Power On sequence ---------------------------------------------------*/
    _writeRegister(0x10, 0x0000);     /* Reset Power Control 1              */
    _writeRegister(0x11, 0x0000);     /* Reset Power Control 2              */
    _writeRegister(0x12, 0x0000);     /* Reset Power Control 3              */
    _writeRegister(0x13, 0x0000);     /* Reset Power Control 4              */
    delay(200);                       /* Discharge cap power voltage (200ms)*/
    _writeRegister(0x10, 0x17B0);     /* SAP, BT[3:0], AP, DSTB, SLP, STB   */
    _writeRegister(0x11, 0x0137);     /* DC1[2:0], DC0[2:0], VC[2:0]        */
    delay(50);                        /* Delay 50 ms                        */
    _writeRegister(0x12, 0x0139);     /* VREG1OUT voltage                   */
    delay(50);                        /* Delay 50 ms                        */
    _writeRegister(0x13, 0x1D00);     /* VDV[4:0] for VCOM amplitude        */
    _writeRegister(0x29, 0x0013);     /* VCM[4:0] for VCOMH                 */
    delay(50);                        /* Delay 50 ms                        */
    _writeRegister(0x20, 0x0000);     /* GRAM horizontal Address            */
    _writeRegister(0x21, 0x0000);     /* GRAM Vertical Address              */
    /* Adjust the Gamma Curve ----------------------------------------------*/
    _writeRegister(0x30, 0x0006);
    _writeRegister(0x31, 0x0101);
    _writeRegister(0x32, 0x0003);
    _writeRegister(0x35, 0x0106);
    _writeRegister(0x36, 0x0B02);
    _writeRegister(0x37, 0x0302);
    _writeRegister(0x38, 0x0707);
    _writeRegister(0x39, 0x0007);
    _writeRegister(0x3C, 0x0600);
    _writeRegister(0x3D, 0x020B);
    /* Set GRAM area -------------------------------------------------------*/
    _writeRegister(0x50, 0x0000);                 /* Horizontal GRAM Start Address      */
    _writeRegister(0x51, (SPFD5408_WIDTH-1));     /* Horizontal GRAM End   Address      */
    _writeRegister(0x52, 0x0000);                 /* Vertical   GRAM Start Address      */
    _writeRegister(0x53, (SPFD5408_HEIGHT-1));    /* Vertical   GRAM End   Address      */
    _writeRegister(0x60, 0x2700);                 /* Gate Scan Line                     */
    _writeRegister(0x61, 0x0001);                 /* NDL,VLE, REV                       */
    _writeRegister(0x6A, 0x0000);                 /* Set scrolling line                 */
    /* Partial Display Control ---------------------------------------------*/
    _writeRegister(0x80, 0x0000);
    _writeRegister(0x81, 0x0000);
    _writeRegister(0x82, 0x0000);
    _writeRegister(0x83, 0x0000);
    _writeRegister(0x84, 0x0000);
    _writeRegister(0x85, 0x0000);
    /* Panel Control -----------------------------------------------------------*/
    _writeRegister(0x90, 0x0010);
    _writeRegister(0x92, 0x0000);
    _writeRegister(0x93, 0x0003);
    _writeRegister(0x95, 0x0110);
    _writeRegister(0x97, 0x0000);
    _writeRegister(0x98, 0x0000);
    /* Mode entry --------------------------------------------------------------*/
    _writeRegister(0x03, 0x1030);
    _writeRegister(0x07, 0x0173);                 /* 262K color and display ON          */
}

// draw a single pixel
// we inline the function to go as fast as possible
void ArduRCT_SPFD5408::drawPixelImpl(uint16_t x, uint16_t y, uint16_t color) {
    _prepareWR();
    if (_clipped) {
        // restore the full window
        if (_rotation == GRAPHICS_ROTATION_0 || _rotation == GRAPHICS_ROTATION_180) _setClippingRectangle(0, 0, SPFD5408_WIDTH-1, SPFD5408_HEIGHT-1);
        else _setClippingRectangle(0, 0, SPFD5408_HEIGHT-1, SPFD5408_WIDTH-1);
    } else {
        _write16bCommand(SPFD5408_H_AD_SET);
        _write16bData(x);
        _write16bCommand(SPFD5408_V_AD_SET);
        _write16bData(y);
     }
    _write16bCommand(SPFD5408_RAM);
    _write16bData(color);
}


void ArduRCT_SPFD5408::fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
    uint32_t nbPixels = hx - lx + 1;
    nbPixels *= (hy - ly + 1);
    _prepareWR();
    _setClippingRectangle(lx, ly, hx, hy);
    _write16bCommand(SPFD5408_RAM);
    uint8_t colH = color >> 8;
    uint8_t colL = color;
    uint8_t lowWR = (*_wrPort) & _wrLow; 
    uint8_t highWR = (*_wrPort) | _wrHigh;
    uint8_t nbPixelsH = 1 + (nbPixels >> 16);
    uint8_t nbPixelsM = 1 + (nbPixels >> 8);
    uint8_t nbPixelsL = nbPixels;
    if (colH == colL) {
        asm (
          "out %1,%7\n\t"           // portB = colH
        "pixelL1:\n\t"
          "out %0,%2\n\t"           // WR low
          "out %0,%3\n\t"           // WR high
          "out %0,%2\n\t"           // WR low
          "out %0,%3\n\t"           // WR high
          "dec %6\n\t"              // nbPixelL --
          "brne pixelL1\n\t"        // if nbPixelL != 0 goto pixelL1
          "dec %5\n\t"              // nbPixelM --
          "brne pixelL1\n\t"        // if nbPixelM != 0 goto pixelL1
          "dec %4\n\t"              // nbPixelH --
          "brne pixelL1\n\t"        // if nbPixelH != 0 goto pixelL1
        ::
        "I" (_SFR_IO_ADDR(SPFD5408_WR_PORT)),  // %0
        "I" (_SFR_IO_ADDR(SPFD5408_DATA_OUT_PORT)),  // %1
        "r" (lowWR),                // %2
        "r" (highWR),               // %3
        "r" (nbPixelsH),            // %4
        "r" (nbPixelsM),            // %5
        "r" (nbPixelsL),            // %6
        "r" (colH)                  // %7
        );
    } else {
        asm (
        "pixelL2:\n\t"
          "out %1,%7\n\t"           // portB = colH
          "out %0,%2\n\t"           // WR low
          "out %0,%3\n\t"           // WR high
          "out %1,%8\n\t"           // portB = colL
          "out %0,%2\n\t"           // WR low
          "out %0,%3\n\t"           // WR high
          "dec %6\n\t"              // nbPixelL --
          "brne pixelL2\n\t"        // if nbPixelL != 0 goto pixelL2
          "dec %5\n\t"              // nbPixelM --
          "brne pixelL2\n\t"        // if nbPixelM != 0 goto pixelL2
          "dec %4\n\t"              // nbPixelH --
          "brne pixelL2\n\t"        // if nbPixelH != 0 goto pixelL2
        ::
        "I" (_SFR_IO_ADDR(SPFD5408_WR_PORT)),  // %0
        "I" (_SFR_IO_ADDR(SPFD5408_DATA_OUT_PORT)),  // %1
        "r" (lowWR),                // %2
        "r" (highWR),               // %3
        "r" (nbPixelsH),            // %4
        "r" (nbPixelsM),            // %5
        "r" (nbPixelsL),            // %6
        "r" (colH),                 // %7
        "r" (colL)                  // %8
        );
    }
}

uint16_t ArduRCT_SPFD5408::getChipId() {
    uint16_t id;
    selectScreenImpl();
    _write16bCommand(0);
    SPFD5408_DATA_DDR_PORT = 0x00;    
    _clearRD();
    id = SPFD5408_DATA_IN_PORT;
    *_rdPort |= _rdHigh; 
    id = id << 8;
    _clearRD();
    id += SPFD5408_DATA_IN_PORT;
    *_rdPort |= _rdHigh; 
    SPFD5408_DATA_DDR_PORT = 0xFF;
    unselectScreenImpl();
    return id;
}

uint16_t *ArduRCT_SPFD5408::retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    uint32_t nbPixels = width;
    nbPixels *= height;
    
    _prepareWR();
    _setClippingRectangle(x, y, x+width-1, y+height-1); 
    _write16bCommand(SPFD5408_RAM);
    SPFD5408_DATA_DDR_PORT = 0x00;    
    // one dummy read
    _clearRD();
    bitmap[0] = SPFD5408_DATA_IN_PORT;
    *_rdPort |= _rdHigh; 
    // the data is written in R5G6B5, changed to B6G6R6 by MADCTL
    // it therefore reads back in B6G6R6 format, each color byte is [cccccc00]
    for (uint32_t i=0; i<nbPixels; i++) {
        // read B, keep 5
        _clearRD();
        bitmap[i] = SPFD5408_DATA_IN_PORT >> 3;
        *_rdPort |= _rdHigh; 
        // read G, keep 6
        _clearRD();
        bitmap[i] |= (SPFD5408_DATA_IN_PORT >> 2) << 5;
        *_rdPort |= _rdHigh; 
        // read R, keep 5
        _clearRD();
        bitmap[i] |= (SPFD5408_DATA_IN_PORT >> 3) << 11;
        *_rdPort |= _rdHigh; 
    }
    SPFD5408_DATA_DDR_PORT = 0xFF;
    return bitmap;
}

void ArduRCT_SPFD5408::pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    // nbPixels is always < 8192 because of RAM constraint
    uint16_t nbPixels = width;
    nbPixels *= height;
    _prepareWR();
    _setClippingRectangle(x, y, x+width-1, y+height-1); 
    _write16bCommand(SPFD5408_RAM);
    
    uint8_t lowWR = (*_wrPort) & _wrLow; 
    uint8_t highWR = (*_wrPort) | _wrHigh;
    uint8_t *bitmap8 = (uint8_t *)bitmap;
    uint8_t pixelL;
    uint8_t pixelH;
    while (nbPixels-- > 0) {
        pixelL = *bitmap8;
        bitmap8++;
        pixelH = *bitmap8;
        bitmap8++;
        SPFD5408_DATA_OUT_PORT = pixelH;
        SPFD5408_WR_PORT = lowWR;
        SPFD5408_WR_PORT = highWR;
        SPFD5408_DATA_OUT_PORT = pixelL;
        SPFD5408_WR_PORT = lowWR;
        SPFD5408_WR_PORT = highWR;
    }

    /*
    uint8_t *bitmap8 = (uint8_t *)bitmap;
    uint8_t lowWR = (*_wrPort) & _wrLow; 
    uint8_t highWR = (*_wrPort) | _wrHigh;
    uint8_t nbPixelsM = 1 + (nbPixels >> 8);
    uint8_t nbPixelsL = nbPixels;
    asm (
    "pixelL3:\n\t"
      "ld r18, X+\n\t"          // r18 = (X), X++
      "ld r19, X+\n\t"          // r19 = (X), X++
      "out %1,r19\n\t"          // portB = r19
      "out %0,%2\n\t"           // WR low
      "out %0,%3\n\t"           // WR high
      "out %1,r18\n\t"          // portB = r18
      "out %0,%2\n\t"           // WR low
      "out %0,%3\n\t"           // WR high
      "dec %5\n\t"              // nbPixelL --
      "brne pixelL3\n\t"        // if nbPixelL != 0 goto pixelL3
      "dec %4\n\t"              // nbPixelM --
      "brne pixelL3\n\t"        // if nbPixelM != 0 goto pixelL3
    ::
    "I" (_SFR_IO_ADDR(PORTC)),  // %0
    "I" (_SFR_IO_ADDR(PORTB)),  // %1
    "r" (lowWR),                // %2
    "r" (highWR),               // %3
    "r" (nbPixelsM),            // %4
    "r" (nbPixelsL),            // %5
    "x" (bitmap8)               // %6
    : "r18", "r19"
    );
    */
}


void ArduRCT_SPFD5408::setRotationImpl(uint8_t rotation) {
    _prepareWR();
    _write16bCommand(SPFD5408_ENTRY_MODE);
    if (rotation == GRAPHICS_ROTATION_0) {
        _write16bData(SPFD5408_R0);
    } else if (rotation == GRAPHICS_ROTATION_90) {
        _write16bData(SPFD5408_R90);
    } else if (rotation == GRAPHICS_ROTATION_180) {
        _write16bData(SPFD5408_R180);    
    } else if (rotation == GRAPHICS_ROTATION_270) {
        _write16bData(SPFD5408_R270);
    }
}


void ArduRCT_SPFD5408::selectScreenImpl() {
    if (_screenSelected) return;
    if (_spiOnBus) {
        // if SPI is active, disable it but remember that we disabled it
        if (SPCR & _BV(SPE)) {
            SPCR &= ~_BV(SPE);
            _spiUsed = true;
        }
    }
    // set the direction to output
    SPFD5408_DATA_DDR_PORT = 0xFF;
    // select the screen
    if (_cs != 0xFF) digitalWrite(_cs, LOW);
    // put the screen in data mode
    *_cdPort |= _cdBitMask;
    _screenSelected = true;
}


void ArduRCT_SPFD5408::unselectScreenImpl() {
    // unselect the screen
    if (_cs != 0xFF) digitalWrite(_cs, HIGH);
    // release the SCK line, to switch off the led
    digitalWrite(SCK, LOW);
    // restore the SPI if it was active
    if (_spiOnBus & _spiUsed) {
        // we have to set SS as an output to enable SPI
        pinMode(SS, OUTPUT);
        // we always restore the master mode
        SPCR |= _BV(MSTR);
        SPCR |= _BV(SPE);
    }
    _screenSelected = false;
}


/* ---------------- Private functions -------------------------- */

void ArduRCT_SPFD5408::_writeRegister(uint8_t reg, uint16_t data) {
    _write16bCommand(reg);
    _write16bData(data);
}

// define the zone we are going to write to
void ArduRCT_SPFD5408::_setClippingRectangle(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy) {
    uint16_t x, y, t;

    _clipped = true;
    switch (_rotation) {
        default:
            x  = lx;
            y  = ly;
            break;
        case GRAPHICS_ROTATION_90:
            t  = ly;
            ly = lx;
            lx = SPFD5408_WIDTH-1-hy;
            hy = hx;
            hx = SPFD5408_WIDTH-1-t;
            x  = hx;
            y  = ly;
            break;
        case GRAPHICS_ROTATION_180:
            t  = lx;
            lx = SPFD5408_WIDTH-1-hx;
            hx = SPFD5408_WIDTH-1-t;
            t  = ly;
            ly = SPFD5408_HEIGHT-1-hy;
            hy = SPFD5408_HEIGHT-1-t;
            x  = hx;
            y  = hy;
            break;
        case GRAPHICS_ROTATION_270:
            t  = lx;
            lx = ly;
            ly = SPFD5408_HEIGHT - 1 - hx;
            hx = hy;
            hy = SPFD5408_HEIGHT - 1 - t;
            x  = lx;
            y  = hy;
        break;
    }
    _write16bCommand(SPFD5408_H_AD_START);
    _write16bData(lx);
    _write16bCommand(SPFD5408_V_AD_START);
    _write16bData(ly);
    _write16bCommand(SPFD5408_H_AD_END);
    _write16bData(hx);
    _write16bCommand(SPFD5408_V_AD_END);
    _write16bData(hy);
    _write16bCommand(SPFD5408_H_AD_SET);
    _write16bData(x);
    _write16bCommand(SPFD5408_V_AD_SET);
    _write16bData(y);
}

