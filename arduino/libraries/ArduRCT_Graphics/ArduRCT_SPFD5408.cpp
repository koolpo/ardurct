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

// define all the macros starting with _psu_
#include "ArduRCT_ParallelScreenUtils.h"

ArduRCT_SPFD5408::ArduRCT_SPFD5408() {    
    _psu_setup();
}
 
/* ---------------- Protected functions ------------------------ */

void ArduRCT_SPFD5408::initScreenImpl(void) {
    _psu_prepareWR();
    _psu_setDataBusDirectionAsOutput();

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
    _writeRegister(0x51, (GRAPHICS_HARDWARE_WIDTH-1));     /* Horizontal GRAM End   Address      */
    _writeRegister(0x52, 0x0000);                 /* Vertical   GRAM Start Address      */
    _writeRegister(0x53, (GRAPHICS_HARDWARE_HEIGHT-1));    /* Vertical   GRAM End   Address      */
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
    _psu_prepareWR();
    if (_clipped) {
        // restore the full window to prepare for other pixels
        if ((_rotation == GRAPHICS_ROTATION_90) || (_rotation == GRAPHICS_ROTATION_270)) _setClippingRectangle(0, 0, GRAPHICS_HARDWARE_HEIGHT-1, GRAPHICS_HARDWARE_WIDTH-1);
        else _setClippingRectangle(0, 0, GRAPHICS_HARDWARE_WIDTH-1, GRAPHICS_HARDWARE_HEIGHT-1);
        _clipped = false;
    } 
    int16_t t;
    switch (_rotation) {
        case GRAPHICS_ROTATION_90:
            t = y;
            y = x;
            x = GRAPHICS_HARDWARE_WIDTH-1-t;
            break;
        case GRAPHICS_ROTATION_180:
            x = GRAPHICS_HARDWARE_WIDTH-1-x;
            y = GRAPHICS_HARDWARE_HEIGHT-1-y;
            break;
        case GRAPHICS_ROTATION_270:
            t = x;
            x = y;
            y = GRAPHICS_HARDWARE_HEIGHT-1-t;
            break;
        default:
            break;
    }
    _psu_write16bCommand(SPFD5408_H_AD_SET);
    _psu_write16bData(x);
    _psu_write16bCommand(SPFD5408_V_AD_SET);
    _psu_write16bData(y);
    _psu_write16bCommand(SPFD5408_RAM);
    _psu_write16bData(color);
}


void ArduRCT_SPFD5408::fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
    uint32_t nbPixels = hx - lx + 1;
    nbPixels *= (hy - ly + 1);
    _psu_prepareWR();
    _setClippingRectangle(lx, ly, hx, hy);
    _psu_write16bCommand(SPFD5408_RAM);
    _psu_writeSingleColorBlock(nbPixels, color);
}


uint16_t *ArduRCT_SPFD5408::retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
#ifdef __AVR__
    uint32_t nbPixels = width;
    nbPixels *= height;
    
    _psu_prepareWR();
    _setClippingRectangle(x, y, x+width-1, y+height-1); 
    _psu_write16bCommand(SPFD5408_RAM);
    GRAPHICS_HARDWARE_DATA_DDR_PORT = 0x00;    
    // one dummy read
    _psu_clearRD();
    bitmap[0] = GRAPHICS_HARDWARE_DATA_IN_PORT;
    *_rdPortPtr |= _rdHighBitMask; 
    // the data is written in R5G6B5, changed to B6G6R6 by MADCTL
    // it therefore reads back in B6G6R6 format, each color byte is [cccccc00]
    for (uint32_t i=0; i<nbPixels; i++) {
        // read B, keep 5
        _psu_clearRD();
        bitmap[i] = GRAPHICS_HARDWARE_DATA_IN_PORT >> 3;
        *_rdPortPtr |= _rdHighBitMask; 
        // read G, keep 6
        _psu_clearRD();
        bitmap[i] |= (GRAPHICS_HARDWARE_DATA_IN_PORT >> 2) << 5;
        *_rdPortPtr |= _rdHighBitMask; 
        // read R, keep 5
        _psu_clearRD();
        bitmap[i] |= (GRAPHICS_HARDWARE_DATA_IN_PORT >> 3) << 11;
        *_rdPortPtr |= _rdHighBitMask; 
    }
    _psu_setDataBusDirectionAsOutput();
#endif
    return bitmap;
}

void ArduRCT_SPFD5408::pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    // nbPixels is always < 8192 because of RAM constraint
    uint16_t nbPixels = width;
    nbPixels *= height;
    _psu_prepareWR();
    _setClippingRectangle(x, y, x+width-1, y+height-1); 
    _psu_write16bCommand(SPFD5408_RAM);
    uint8_t *bitmap8 = (uint8_t *)bitmap;
    uint8_t pixelL;
    uint8_t pixelH;
    while (nbPixels-- > 0) {
        pixelL = *bitmap8;
        bitmap8++;
        pixelH = *bitmap8;
        bitmap8++;
        _psu_write8bData(pixelH);
        _psu_write8bData(pixelL);
    }
}


void ArduRCT_SPFD5408::setRotationImpl(uint8_t rotation) {
    _psu_prepareWR();
    _psu_write16bCommand(SPFD5408_ENTRY_MODE);
    if (rotation == GRAPHICS_ROTATION_0) {
        _psu_write16bData(SPFD5408_R0);
    } else if (rotation == GRAPHICS_ROTATION_90) {
        _psu_write16bData(SPFD5408_R90);
    } else if (rotation == GRAPHICS_ROTATION_180) {
        _psu_write16bData(SPFD5408_R180);    
    } else if (rotation == GRAPHICS_ROTATION_270) {
        _psu_write16bData(SPFD5408_R270);
    }
}


void ArduRCT_SPFD5408::selectScreenImpl() {
    if (_screenSelected) return;
#ifdef __AVR__    
    if (_spiOnBus) {
        // if SPI is active, disable it but remember that we disabled it
        if (SPCR & _BV(SPE)) {
            SPCR &= ~_BV(SPE);
            _spiUsed = true;
        }
    }
    // put the screen in data mode
    *_cdPortPtr |= _cdBitMask;
#endif
    // select the screen
    if (_cs != 0xFF) digitalWrite(_cs, LOW);
    _screenSelected = true;
}


void ArduRCT_SPFD5408::unselectScreenImpl() {
    // unselect the screen
    if (_cs != 0xFF) digitalWrite(_cs, HIGH);
#ifdef __AVR__    
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
#endif
    _screenSelected = false;
}


/* ---------------- Private functions -------------------------- */

void ArduRCT_SPFD5408::_writeRegister(uint8_t reg, uint16_t data) {
    _psu_write16bCommand(reg);
    _psu_write16bData(data);
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
            lx = GRAPHICS_HARDWARE_WIDTH-1-hy;
            hy = hx;
            hx = GRAPHICS_HARDWARE_WIDTH-1-t;
            x  = hx;
            y  = ly;
            break;
        case GRAPHICS_ROTATION_180:
            t  = lx;
            lx = GRAPHICS_HARDWARE_WIDTH-1-hx;
            hx = GRAPHICS_HARDWARE_WIDTH-1-t;
            t  = ly;
            ly = GRAPHICS_HARDWARE_HEIGHT-1-hy;
            hy = GRAPHICS_HARDWARE_HEIGHT-1-t;
            x  = hx;
            y  = hy;
            break;
        case GRAPHICS_ROTATION_270:
            t  = lx;
            lx = ly;
            ly = GRAPHICS_HARDWARE_HEIGHT-1-hx;
            hx = hy;
            hy = GRAPHICS_HARDWARE_HEIGHT-1-t;
            x  = lx;
            y  = hy;
            break;
    }
    _psu_write16bCommand(SPFD5408_H_AD_START);
    _psu_write16bData(lx);
    _psu_write16bCommand(SPFD5408_V_AD_START);
    _psu_write16bData(ly);
    _psu_write16bCommand(SPFD5408_H_AD_END);
    _psu_write16bData(hx);
    _psu_write16bCommand(SPFD5408_V_AD_END);
    _psu_write16bData(hy);
    _psu_write16bCommand(SPFD5408_H_AD_SET);
    _psu_write16bData(x);
    _psu_write16bCommand(SPFD5408_V_AD_SET);
    _psu_write16bData(y);
}

