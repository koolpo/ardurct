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
 
#include <NewSoftSerial.h>
#include <XBee.h>

#define XBEE_RX 3
#define XBEE_TX 2
#define BAUDRATE 57600

uint8_t newId[4] = { 'A', 'B', 'C', 'D' };

NewSoftSerial nss(XBEE_RX, XBEE_TX);
XBee xBee;

uint8_t state = 0;

void setup() {
	// initialize the xBee on a NewSoftSerial port with BAUDRATE speed
    xBee.begin(&nss, BAUDRATE);
	
	Serial.begin(BAUDRATE);
}

void loop() {
	uint8_t *buffer;
		
	if ((state < 10) && (state % 2 == 0) && !XBee.isInCommandMode()) state ++;
	if (state == 1) {
		// retrieves the id
		Serial.print("Old id:");
		xBee.requestId();
		state ++;
	} else if (state == 3) {
		// print it
		buffer = xBee.getId();
		for (uint_t i=0; i<4; i++) Serial.print((char)buffer[i]);
		Serial.println();
		state += 2;
	if (state == 5) {
		// set the new id
		xBee.setId(newId);
		state ++;
	} else if (state == 7) {
		// retrieves the id
		Serial.print("New id:");
		xBee.requestId();
		state ++;
	} else if (state == 9) {
		// print it
		buffer = xBee.getId();
		for (uint_t i=0; i<4; i++) Serial.print((char)buffer[i]);
		Serial.println();
		state += 2;
	}
	
	xBee.processCommand();
}