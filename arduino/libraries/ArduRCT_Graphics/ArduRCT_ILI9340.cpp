/*
 * ILI9340 - Implementation of the ScreenHAL abstract functions for the ILI9340
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

#include "ArduRCT_ILI9340.h"

#define ILI9340_NOP     0x00
#define ILI9340_SWRESET 0x01
#define ILI9340_RDDID   0x04
#define ILI9340_RDDST   0x09

#define ILI9340_SLPIN   0x10
#define ILI9340_SLPOUT  0x11
#define ILI9340_PTLON   0x12
#define ILI9340_NORON   0x13

#define ILI9340_RDMODE  0x0A
#define ILI9340_RDMADCTL  0x0B
#define ILI9340_RDPIXFMT  0x0C
#define ILI9340_RDIMGFMT  0x0A
#define ILI9340_RDSELFDIAG  0x0F

#define ILI9340_INVOFF  0x20
#define ILI9340_INVON   0x21
#define ILI9340_GAMMASET 0x26
#define ILI9340_DISPOFF 0x28
#define ILI9340_DISPON  0x29

#define ILI9340_CASET   0x2A
#define ILI9340_PASET   0x2B
#define ILI9340_RAMWR   0x2C
#define ILI9340_RAMRD   0x2E

#define ILI9340_PTLAR   0x30
#define ILI9340_MADCTL  0x36

#define ILI9340_MADCTL_MY  0x80
#define ILI9340_MADCTL_MX  0x40
#define ILI9340_MADCTL_MV  0x20
#define ILI9340_MADCTL_ML  0x10
#define ILI9340_MADCTL_RGB 0x00
#define ILI9340_MADCTL_BGR 0x08
#define ILI9340_MADCTL_MH  0x04

#define ILI9340_PIXFMT  0x3A

#define ILI9340_FRMCTR1 0xB1
#define ILI9340_FRMCTR2 0xB2
#define ILI9340_FRMCTR3 0xB3
#define ILI9340_INVCTR  0xB4
#define ILI9340_DFUNCTR 0xB6

#define ILI9340_PWCTR1  0xC0
#define ILI9340_PWCTR2  0xC1
#define ILI9340_PWCTR3  0xC2
#define ILI9340_PWCTR4  0xC3
#define ILI9340_PWCTR5  0xC4
#define ILI9340_VMCTR1  0xC5
#define ILI9340_VMCTR2  0xC7

#define ILI9340_RDID1   0xDA
#define ILI9340_RDID2   0xDB
#define ILI9340_RDID3   0xDC
#define ILI9340_RDID4   0xDD

#define ILI9340_GMCTRP1 0xE0
#define ILI9340_GMCTRN1 0xE1

#define ILI9340_PWCTR6  0xFC

#define ILI9340_R0   0
#define ILI9340_R90  1
#define ILI9340_R180 2
#define ILI9340_R270 3

// define all the macros starting with _psu_
#include "ArduRCT_ParallelScreenUtils.h"

ArduRCT_ILI9340::ArduRCT_ILI9340() {    
    _psu_setup();
}
 
/* ---------------- Protected functions ------------------------ */

void ArduRCT_ILI9340::initScreenImpl(void) {
    _psu_prepareWR();
    _psu_setDataBusDirectionAsOutput();
    
    _psu_write8bCommand(0xEF);
    _psu_write8bData(0x03);
    _psu_write8bData(0x80);
    _psu_write8bData(0x02);

    _psu_write8bCommand(0xCF);  
    _psu_write8bData(0x00); 
    _psu_write8bData(0XC1); 
    _psu_write8bData(0X30); 

    _psu_write8bCommand(0xED);  
    _psu_write8bData(0x64); 
    _psu_write8bData(0x03); 
    _psu_write8bData(0X12); 
    _psu_write8bData(0X81); 

    _psu_write8bCommand(0xE8);  
    _psu_write8bData(0x85); 
    _psu_write8bData(0x00); 
    _psu_write8bData(0x78); 

    _psu_write8bCommand(0xCB);  
    _psu_write8bData(0x39); 
    _psu_write8bData(0x2C); 
    _psu_write8bData(0x00); 
    _psu_write8bData(0x34); 
    _psu_write8bData(0x02); 

    _psu_write8bCommand(0xF7);  
    _psu_write8bData(0x20); 

    _psu_write8bCommand(0xEA);  
    _psu_write8bData(0x00); 
    _psu_write8bData(0x00); 

    _psu_write8bCommand(ILI9340_PWCTR1);    //Power control 
    _psu_write8bData(0x23);   //VRH[5:0] 

    _psu_write8bCommand(ILI9340_PWCTR2);    //Power control 
    _psu_write8bData(0x10);   //SAP[2:0];BT[3:0] 

    _psu_write8bCommand(ILI9340_VMCTR1);    //VCM control 
    _psu_write8bData(0x3e); //???????
    _psu_write8bData(0x28); 

    _psu_write8bCommand(ILI9340_VMCTR2);    //VCM control2 
    _psu_write8bData(0x86);  //--

    _psu_write8bCommand(ILI9340_MADCTL);    // Memory Access Control 
    _psu_write8bData(ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);

    _psu_write8bCommand(ILI9340_PIXFMT);    
    _psu_write8bData(0x55); 

    _psu_write8bCommand(ILI9340_FRMCTR1);    
    _psu_write8bData(0x00);  
    _psu_write8bData(0x18); 

    _psu_write8bCommand(ILI9340_DFUNCTR);    // Display Function Control 
    _psu_write8bData(0x08); 
    _psu_write8bData(0x82);
    _psu_write8bData(0x27);  

    _psu_write8bCommand(0xF2);    // 3Gamma Function Disable 
    _psu_write8bData(0x00); 

    _psu_write8bCommand(ILI9340_GAMMASET);    //Gamma curve selected 
    _psu_write8bData(0x01); 

    _psu_write8bCommand(ILI9340_GMCTRP1);    //Set Gamma 
    _psu_write8bData(0x0F); 
    _psu_write8bData(0x31); 
    _psu_write8bData(0x2B); 
    _psu_write8bData(0x0C); 
    _psu_write8bData(0x0E); 
    _psu_write8bData(0x08); 
    _psu_write8bData(0x4E); 
    _psu_write8bData(0xF1); 
    _psu_write8bData(0x37); 
    _psu_write8bData(0x07); 
    _psu_write8bData(0x10); 
    _psu_write8bData(0x03); 
    _psu_write8bData(0x0E); 
    _psu_write8bData(0x09); 
    _psu_write8bData(0x00); 

    _psu_write8bCommand(ILI9340_GMCTRN1);    //Set Gamma 
    _psu_write8bData(0x00); 
    _psu_write8bData(0x0E); 
    _psu_write8bData(0x14); 
    _psu_write8bData(0x03); 
    _psu_write8bData(0x11); 
    _psu_write8bData(0x07); 
    _psu_write8bData(0x31); 
    _psu_write8bData(0xC1); 
    _psu_write8bData(0x48); 
    _psu_write8bData(0x08); 
    _psu_write8bData(0x0F); 
    _psu_write8bData(0x0C); 
    _psu_write8bData(0x31); 
    _psu_write8bData(0x36); 
    _psu_write8bData(0x0F); 

    _psu_write8bCommand(ILI9340_SLPOUT);    //Exit Sleep 
    delay(120); 		
    _psu_write8bCommand(ILI9340_DISPON);    //Display on     
}

// draw a single pixel
void ArduRCT_ILI9340::drawPixelImpl(uint16_t x, uint16_t y, uint16_t color) {
    _psu_prepareWR();
    _setClippingRectangle(x, y, x+1, y+1);
    _psu_write8bCommand(ILI9340_RAMWR);
    _psu_write16bData(color);
}


void ArduRCT_ILI9340::fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
    uint32_t nbPixels = hx - lx + 1;
    nbPixels *= (hy - ly + 1);
    _psu_prepareWR();
    _setClippingRectangle(lx, ly, hx, hy);
    _psu_write8bCommand(ILI9340_RAMWR); // write to RAM
    _psu_writeSingleColorBlock(nbPixels, color);
}


uint16_t *ArduRCT_ILI9340::retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
#ifdef __AVR__
    uint32_t nbPixels = width;
    nbPixels *= height;
    
    _psu_prepareWR();
    _setClippingRectangle(x, y, x+width-1, y+height-1); 
    _psu_write8bCommand(ILI9340_RAMRD);
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

void ArduRCT_ILI9340::pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    _psu_prepareWR();
    _setClippingRectangle(x, y, x+width-1, y+height-1); 
    _psu_write8bCommand(ILI9340_RAMWR); // write to RAM
    _psu_write16bBitmapBuffer(bitmap, width, height);
}


void ArduRCT_ILI9340::setRotationImpl(uint8_t rotation) {
    _psu_prepareWR();
    _psu_write8bCommand(ILI9340_MADCTL);
    if (rotation == ILI9340_R0) { 
        _psu_write8bData(ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
    } else if (rotation == ILI9340_R90) {
        _psu_write8bData(ILI9340_MADCTL_MV | ILI9340_MADCTL_BGR);
    } else if (rotation == ILI9340_R180) {
        _psu_write8bData(ILI9340_MADCTL_MY | ILI9340_MADCTL_BGR);
    } else {
        _psu_write8bData(ILI9340_MADCTL_MV | ILI9340_MADCTL_MY | ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
    }
}


void ArduRCT_ILI9340::selectScreenImpl() {
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


void ArduRCT_ILI9340::unselectScreenImpl() {
    // unselect the screen
    if (_cs != 0xFF) digitalWrite(_cs, HIGH);
#ifdef __AVR__
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

// define the zone we are going to write to
void ArduRCT_ILI9340::_setClippingRectangle(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy) {
    _psu_write8bCommand(ILI9340_CASET); // Column addr set
    _psu_write16bData(lx);  // XSTART 
    _psu_write16bData(hx);  // XEND
    _psu_write8bCommand(ILI9340_PASET); // Row addr set
    _psu_write16bData(ly);  // YSTART
    _psu_write16bData(hy);  // YEND
}

