/*
 * ArduRCT_Encoder - Handles rotary encoders
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


#include "ArduRCT_Encoder.hpp"
#include "ArduRCT_EventManager.h"

ArduRCT_Encoder::ArduRCT_Encoder(uint8_t pinA, uint8_t pinB, int16_t value, int16_t min, int16_t max) {
    _pinA = pinA;
    _pinB = pinB;
    _value = value;
    _min = min;
    _max = max;
    _previousValue = value;
    _previousPosition = 0;
    // set as input
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    // add a pull up
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
}

ArduRCT_Encoder *ArduRCT_Encoder::getNext() {
    return _next;
}

void ArduRCT_Encoder::setNext(ArduRCT_Encoder *next) {
    _next = next;
}

uint8_t ArduRCT_Encoder::getPinA() {
    return _pinA;
}

int16_t ArduRCT_Encoder::updateValue() {
    uint8_t position = digitalRead(_pinB)*2 + digitalRead(_pinA);
    if (position != _previousPosition) {
        bool isCCW = ((_previousPosition == 0) && (position == 1)) || ((_previousPosition == 1) && (position == 3)) ||
            ((_previousPosition == 2) && (position == 0)) || ((_previousPosition == 3) && (position == 2));
        if (isCCW) {
            _value--;
            if (_value < _min) {
                if (_min == 0) _value = _max;
                else _value = _min;
            }
        } else {
            _value++;
            if (_value > _max) {
                if (_min == 0) _value = 0;
                else _value = _max;
            }
        }
    }
    _previousPosition = position;
    return _value;
}

void ArduRCT_Encoder::setValue(int16_t value) {
    _value = value;
    if (_value < _min) _value = _min;
    if (_value > _max) _value = _max;
}

void ArduRCT_Encoder::setMinMax(int16_t min, int16_t max) {
    _min = min;
    _max = max;
}

int16_t ArduRCT_Encoder::getValue() {
    return _value;
}

int16_t ArduRCT_Encoder::getChange() {
    int16_t change = _value - _previousValue;
    if (change < -10) change += _max + 1;
    if (change >  10) change -= _max + 1;
    _previousValue = _value;
    return change;
}
