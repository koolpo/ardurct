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
 
#include "XBee.h"

// Commands
#define XBEE_CMD_NONE 0
#define XBEE_CMD_ID 'I'
#define XBEE_CMD_ID_READ 'i'
#define XBEE_CMD_BAUDRATE 'B'

// States
#define XBEE_IDLE 0
#define XBEE_GUARDING_BEFORE 1
#define XBEE_GUARDING_AFTER 2
#define XBEE_WRITING 3
#define XBEE_PROCESSING 4


XBee::XBee() {
	_nss = NULL;
	_serial = NULL;
    _state = XBEE_IDLE;
    _command = XBEE_CMD_NONE;
}

void XBee::begin(NewSoftSerial *nss, uint32_t baudrate, uint16_t guardTime) {
	_nss = nss;
	_guardTime = guardTime;
	_baudrate = baudrate;
	_nss->begin(baudrate);
}

void XBee::begin(Serial *serial, uint32_t baudrate, uint16_t guardTime) {
	_serial = serial;
	_guardTime = guardTime;
	_baudrate = baudrate;
	_serial->begin(baudrate);
}


bool XBee::isInCommandMode() {
    return (_state != XBEE_IDLE);
}


uint8_t XBee::available() {
    if (_serial) return _serial->available();
    if (_nss) return _nss->available();
	return 0;
}


char XBee::read() {
    if (_serial) return _serial->read();
	if (_nss) return _nss->read();
	return -1;
}


void XBee::write(char val) {
    if (_serial) _serial->print(val);
	else if (_nss) _nss->print(val);
}


void XBee::setId(uint8_t *id) {
	uint8_t i;
	for (i=0; i<4; i++) {
		if (_id[i] != id[i]) break;
	}
	if (i < 4) return;
    for (i=0; i<4; i++) _id[i] = id[i];
    _command = XBEE_CMD_ID;
}


void XBee::retrieveId() {
    _command = XBEE_CMD_ID_REQUEST;
}


uint8_t *XBee::getId() {
	return _id;
}


void XBee::setBaudrate(uint32_t baudrate) {
	if (baudrate == _baudrate) return;
	_baudrate = baudrate;
	_command = XBEE_CMD_BAUDRATE;
}


void XBee::processCommand() {
    // If nothing to do return
    if (_command == XBEE_CMD_NONE) return;
    
    switch (_state) {
        case XBEE_IDLE:
            _waitUntilTime = millis() + _guardTime;
            _state = XBEE_GUARDING_BEFORE;
            break;
            
        case XBEE_GUARDING_BEFORE:
            // wait for the guard time to elapse
            if ((int32_t)(millis()-_waitUntilTime) < 0) return;
            // enter command mode
            xBee.print(XBEE_COMMAND_SEQUENCE);
            // prepare to wait
            _waitUntilTime = millis() + _guardTime;
            _state = XBEE_GUARDING_AFTER;
            break;
            
        case XBEE_GUARDING_AFTER:
            // wait for the guard time to elapse
            if ((int32_t)(millis()-_waitUntilTime) < 0) return;
            switch (_command) {
                case XBEE_CMD_ID:
                    print("ATID");
                    for (uint8_t i=0; i<4; i++) print(_id[i]);
                    print(",WR,CN\n");
                    break;
                case XBEE_CMD_ID_READ:
                    print("ATID,CN\n");
                    break;
                case XBEE_CMD_BAUDRATE:
                    print("ATBD");
                    print(_baudrate, HEX);
                    xBee.print(",CN,WR\n");
                    break;
                default:
                    break;
            }
            // prepare to wait
            _waitUntilTime = millis() + XBEE_WRITE_TIME;
            _state = XBEE_WRITING;
			_counter = 0;
            break;
            
        case XBEE_WRITING:
            // wait for the guard time to elapse
            if ((int32_t)(millis()-_waitUntilTime) < 0) return;
			// indicate to the hardware to communicate at this baudrate from now on
			if (_command == XBEE_CMD_BAUDRATE) {
				if (_serial) _serial->begin(_baudrate);
				else if (_nss) _nss->begin(_baudrate);
			}
			_state = XBEE_PROCESSING;
			
		case XBEE_PROCESSING:
			while (available()) {
				_buffer[_counter++] = read();
				if ((_buffer[_counter-1] == '\n') || (_counter > 9)) break;
			}
			// we have the end of line
			if (_buffer[_counter-1] == '\n')  {
				switch (_command) {
					case XBEE_CMD_ID_READ:
						for (uint8_t i=0; i<4; i++) _id[i] = '0';
						for (uint8_t i=0; i<_counter; i++) _id[3-i] = _buffer[_counter-1-i];
						break;
					default:
						break;
				}
				_counter = 10;
			}
			if (_counter == 10) _command = XBEE_CMD_NONE;
            break;
		}
    }
}
