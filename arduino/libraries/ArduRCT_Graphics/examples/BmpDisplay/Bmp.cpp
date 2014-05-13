/*
 * Bmp - Class to draw a *.bmp Bitmap
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
 
#include <sdFat.h>
#include "Bmp.h"

// we share the buffer between all the instances since we can only draw one image at a time
// we could malloc the buffer, but this helps defining the ram footprint
uint16_t Bmp::_imgBuffer[IMG_BUFFER_SIZE];

Bmp::Bmp() {
    _fileName = 0;
}

uint8_t Bmp::setFileName(char *fileName) {
    uint32_t tmp;
    
    _error = 1;
    _fileName = fileName;
    if (!_file.open(_fileName, O_READ))  goto setFileNameEnd;
    
    tmp = _readLE16b();
    _error = 2;
    if (tmp != 0x4D42) goto setFileNameEnd;

    // read and ignore file size
    _readLE32b();  
    // read and ignore creator bytes
    _readLE32b();
    // read offset
    _imageOffset = _readLE32b();  
    // read and ignore DIB header
    _readLE32b();
    _width = _readLE32b();
    _height = _readLE32b();
    
    _error = 5;
    if (_readLE16b() != 1) goto setFileNameEnd;
    
    // read bit depth
    _bitsPerPixel = _readLE16b();
    _error = 3;
    if (_bitsPerPixel != 24 && _bitsPerPixel != 16) goto setFileNameEnd;
    
    // read compression
    _compression = _readLE32b();
    _error = 4;
    if (_compression != 0 && !(_compression == 3 && _bitsPerPixel==16)) goto setFileNameEnd;  
    
    _error = 0;

setFileNameEnd:
    _file.close();
    return _error;
}

char *Bmp::getErrorMessage() {
    if (_error == 0) return 0;
    if (_error == 1) return "No such file";
    if (_error == 2) return "File type not supported";
    if (_error == 3) return "Bits per pixel not supported";
    if (_error == 4) return "Compression not supported";
    if (_error == 5) return "Unknown error";
    return 0;
}


uint32_t Bmp::draw(ArduRCT_Graphics *graphics, int16_t x, int16_t y) {
    if (!_fileName) return -1;
    if (_error) return -1;
    
    _file.open(_fileName);
    _file.seekSet(_imageOffset);

    uint32_t time = micros();
      
    int16_t lx = x;
    int16_t ly = y;
    
    if ((lx < -_width) || (lx >= graphics->getWidth())) lx = (graphics->getWidth()-_width)/2;
    if ((ly < -_height) || (ly >= graphics->getHeight())) ly = (graphics->getHeight()-_height)/2;

    int16_t imgXOffset = 0;
    if (lx < 0) imgXOffset = -lx;
    int16_t imgWidth = _width+lx;
    if (imgWidth > graphics->getWidth()) imgWidth -= (imgWidth-graphics->getWidth());

    uint32_t pixelsNotRead = _width;
    pixelsNotRead *= _height;
    uint16_t pixelsReadNotDrawn = 0;
    uint16_t *imgBufferPtr;
    for (uint16_t line=0; line<_height; line++) {
        if (pixelsReadNotDrawn == 0) {
            pixelsReadNotDrawn = _readImageData(_width * LINES_TO_READ, &pixelsNotRead);
            imgBufferPtr = _imgBuffer + imgXOffset;
        }
        // bitmaps are stored with the BOTTOM line first so we have to start from the bottom
        int16_t imgY = ly + _height-1 - line;
        // since we start from the bottom, if we have to paint beyond the top, we have finished
        if (imgY < 0) break;
        // only paint if we are on the screen
        if (imgY < graphics->getHeight()) graphics->pasteBitmap(imgBufferPtr, lx + imgXOffset, imgY, imgWidth, 1);
        imgBufferPtr += _width;
        pixelsReadNotDrawn -= _width;
    }    
    _file.close();
    return micros() - time;
}

uint16_t Bmp::_readImageData(uint16_t pixelsToRead, uint32_t *pixelsNotRead) {
    // define the number of pixels to read on SD
    uint16_t readPixels = min(pixelsToRead, *pixelsNotRead);
    *pixelsNotRead -= pixelsToRead;
     // read 2 or 3 bytes per pixel, according to the _bitsPerPixel
    _file.read(_imgBuffer, _bitsPerPixel == 16 ? readPixels * 2 : readPixels * 3);    
    // convert pixel from B8G8R8 to R5G6B5
    if (_bitsPerPixel != 16) {
        uint16_t *pixelsPtr = _imgBuffer;
        uint8_t *bytesPtr = (uint8_t *)_imgBuffer;
        uint16_t pixelsToConvert = readPixels;
        while (pixelsToConvert--) {
            uint8_t b = (*bytesPtr) >> 3;          // top 5 bits of blue
            uint8_t g = (*(bytesPtr+1)) >> 2;      // top 6 bits of green
            uint16_t color = (*(bytesPtr+2) >> 3); // top 5 bits of red
            color <<= 6;
            color |= g;
            color <<= 5;
            color |= b;
            *pixelsPtr = color;
            bytesPtr += 3;
            pixelsPtr++;
        } 
    } 
    return readPixels;
}

// Read 16b as Little Endian
uint16_t Bmp::_readLE16b() {
    uint8_t l = _file.read();
    uint16_t h = _file.read();
    h <<= 8;
    h |= l;
    return h;
}

// Read 32b as Little Endian
uint32_t Bmp::_readLE32b() {
    uint16_t l = _readLE16b();
    uint32_t h = _readLE16b();
    h <<= 16;
    h |= l;
    return h;
}