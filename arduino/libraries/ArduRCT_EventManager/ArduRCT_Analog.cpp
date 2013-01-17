/*
 * ArduRCT_Analog - Handles analog channel changes
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
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


#include "ArduRCT_Analog.hpp"
#include "ArduRCT_EventManager.h"

ArduRCT_Analog::ArduRCT_Analog(uint8_t pin, uint8_t averaging, uint16_t resolution) {
    _pin = pin;
    _value = 0;
    _averaging = averaging;
    if (_averaging == 0) averaging = 1;
    _resolution = resolution;
    _previousValue = ANALOG_NO_VALUE;
    pinMode(pin, INPUT);
}

uint8_t ArduRCT_Analog::getPin() {
    return _pin;
}

ArduRCT_Analog *ArduRCT_Analog::getNext() {
    return _next;
}

void ArduRCT_Analog::setNext(ArduRCT_Analog *next) {
    _next = next;
}

uint16_t ArduRCT_Analog::updateValue() {
    _value = 0;
    for (uint8_t i=0; i<_averaging; i++) _value += analogRead(_pin);
    _value = _value/_averaging;
    if (_resolution != ANALOG_HARDWARE_RESOLUTION) {
        uint32_t value32 = _value;
        value32 = value32 * _resolution / ANALOG_HARDWARE_RESOLUTION;
        _value = value32;
    }
    return _value;
}

int16_t ArduRCT_Analog::getChange() {
    int16_t change;
    if (_previousValue == ANALOG_NO_VALUE) change = 0;
    else change = _value - _previousValue;
    _previousValue = _value;
    return change;
}

uint16_t ArduRCT_Analog::getValue() {
    return _value;
}
