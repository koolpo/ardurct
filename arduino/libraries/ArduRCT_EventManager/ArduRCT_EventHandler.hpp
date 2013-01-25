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

#ifndef ARDURCT_EVENT_HANDLER
#define ARDURCT_EVENT_HANDLER 1

#include <inttypes.h>

#define EVENT_HANDLING_VOID 0
#define EVENT_HANDLING_EXIT -1
#define EVENT_HANDLING_DONE 1

class ArduRCT_EventHandler {

	public:	
		ArduRCT_EventHandler(uint8_t type, int8_t (*handler)(uint8_t type));
		
		ArduRCT_EventHandler(uint8_t type, uint8_t value, int8_t (*handler)(uint8_t type, uint8_t value));

        ArduRCT_EventHandler(uint8_t type, uint8_t value, int8_t (*handler)(uint8_t type, uint8_t value, int16_t x, int16_t y));
		        
		int8_t handle(uint8_t type);

		int8_t handle(uint8_t type, uint8_t value);

		int8_t handle(uint8_t type, uint8_t value, int16_t x, int16_t y);
				
		ArduRCT_EventHandler *getNext();
		
		void setNext(ArduRCT_EventHandler *next);
		
	private:
		uint8_t _type;
		uint8_t _value;
		int8_t (*_handlerL)(uint8_t type, uint8_t value, int16_t x, int16_t y);
		int8_t (*_handlerM)(uint8_t type, uint8_t value);
		int8_t (*_handlerS)(uint8_t type);
		ArduRCT_EventHandler *_next;

};

#endif
