/*
 * XBee - Configuration utilities for XBee through AT command set
 *
 * Copyright (c) 2011 Laurent Wibaux <lm.wibaux@gmail.com>
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
 
#ifndef XBEE_H
#define XBEE_H

#include <inttypes.h>
#include "Print.h"
#include "WProgram.h"
#include "../../NewSoftSerial/NewSoftSerial.h"

#include "XBee_Config.h"

class XBee : public Print {
	public:
		// constructor
		XBee();
		
		// begin by attaching a Serial or a NewSoftSerial
		void begin(HardwareSerial *serial, uint32_t baudrate = XBEE_DEFAULT_BAUD_RATE, uint16_t guardTime = XBEE_DEFAULT_GUARD_TIME);    
		void begin(NewSoftSerial *nss, uint32_t baudrate = XBEE_DEFAULT_BAUD_RATE, uint16_t guardTime = XBEE_DEFAULT_GUARD_TIME);    

		// Print superclass
		virtual void write(uint8_t byte);
		
		// Returns the number of bytes available in the queue
		uint8_t available();
		
		// Returns the next character received
		char read();
		
		// Sets the id of the module
		void setId(uint8_t *id);

		// RetrieveId retrieves the id from the XBee module. It will return immediately. 
		// The value can be read after isInCommandMode returns false by getId
		void retrieveId();
		
		// Gets the id from the module, this is the last known value
		// retrieveId() should be called before
		uint8_t *XBee::getId();
		
		// Sets the baudrate of the module
		void setBaudrate(uint16_t baudrate);

		void processCommand();
		
		bool isInCommandMode();
			
	private:
	
		HardwareSerial *_serial;
		NewSoftSerial *_nss;
		uint8_t _state;
		uint8_t _command;
		uint32_t _waitUntilTime;
		uint16t _guardTime;
		uint8_t _id[4];
		uint8_t _buffer[10];
		uint8_t _counter;
		uint32_t _baudrate;
};

#endif
