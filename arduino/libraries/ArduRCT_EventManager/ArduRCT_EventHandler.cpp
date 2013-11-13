/*
 * ArduRCT_EventHandler - Handles an event
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

#include "ArduRCT_EventManager.h"
#include "ArduRCT_EventHandler.hpp"

ArduRCT_EventHandler::ArduRCT_EventHandler(uint8_t type, int8_t (*handler)(uint8_t type)) {
	_type = type;
	_value = EVENT_ANY_VALUE;
	_handlerS = handler;
	_handlerM = 0;
	_handlerL = 0;
}

ArduRCT_EventHandler::ArduRCT_EventHandler(uint8_t type, uint8_t value, int8_t (*handler)(uint8_t type, uint8_t value)) {
	_type = type;
	_value = value;
	_handlerS = 0;
	_handlerM = handler;
	_handlerL = 0;
}

ArduRCT_EventHandler::ArduRCT_EventHandler(uint8_t type, uint8_t value, int16_t x, int16_t y, int8_t (*handler)(uint8_t type, uint8_t value, int16_t x, int16_t y)) {
	_type = type;
	_value = value;
	_handlerS = 0;
	_handlerM = 0;
	_handlerL = handler;
}

int8_t ArduRCT_EventHandler::handle(uint8_t type) {
	if (_handlerS == 0) return EVENT_HANDLING_VOID;
	// check if the _type matches the type, or if the _type is a class and matches the type class
	if ((_type != type) && (_type != (type & EVENT_CLASS_MASK))) return EVENT_HANDLING_VOID;
	return (*_handlerS)(type);
}

int8_t ArduRCT_EventHandler::handle(uint8_t type, uint8_t value) {
	if (_handlerM == 0) return EVENT_HANDLING_VOID;
	// check if the _type matches the type, or if the _type is a class and matches the type class
	if ((_type != type) && (_type != (type & EVENT_CLASS_MASK))) return EVENT_HANDLING_VOID;
	// check if we have a handler for the TICK event that needs to run
	if ((_type == EVENT_SYSTEM_TICK) && ((_value <= 1) || ((value % _value) == 0))) return (*_handlerM)(type, value);
	// check if we run in all matches of _type or only on some values
	if ((_value == EVENT_ANY_VALUE) || (_value == value)) return (*_handlerM)(type, value);
	return EVENT_HANDLING_VOID;
}

int8_t ArduRCT_EventHandler::handle(uint8_t type, uint8_t value, int16_t x, int16_t y) {
	if (_handlerL == 0) return EVENT_HANDLING_VOID;
	// check if the _type matches the type, or if the _type is a class and matches the type class
	if ((_type != type) && (_type != (type & EVENT_CLASS_MASK))) return EVENT_HANDLING_VOID;
	// check if we run in all matches of _type or only on some values
	if ((_value == EVENT_ANY_VALUE) || (_value == value)) return (*_handlerL)(type, value, x, y);
	return EVENT_HANDLING_VOID;
}


ArduRCT_EventHandler *ArduRCT_EventHandler::getNext() {
	return _next;
}

void ArduRCT_EventHandler::setNext(ArduRCT_EventHandler *next) {
	_next = next;
}
