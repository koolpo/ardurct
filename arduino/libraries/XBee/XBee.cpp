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
#define XBEE_CMD_ID_WRITE 'I'
#define XBEE_CMD_ID_READ 'i'
#define XBEE_CMD_BAUDRATE_WRITE 'B'
#define XBEE_CMD_SH_READ 's'
#define XBEE_CMD_SL_READ 't'
#define XBEE_CMD_DHDL_WRITE 'D'
#define XBEE_CMD_DH_READ 'd'
#define XBEE_CMD_DL_READ 'e'
#define XBEE_CMD_GUARD_WRITE 'G'
#define XBEE_CMD_RADIO_DB_READ 'r'
#define XBEE_CMD_ENABLE_64B_ADRESSING 'A'
#define XBEE_CMD_ENTER_CMD_MODE '+'
#define XBEE_CMD_EXIT_CMD_MODE '-'
#define XBEE_CMD_COMMIT 'W'


// States
#define XBEE_IDLE 0
#define XBEE_GUARDING_BEFORE 1
#define XBEE_GUARDING_AFTER 2
#define XBEE_WAIT_FOR_OK 3
#define XBEE_SEND_COMMAND 4
#define XBEE_WRITING 5
#define XBEE_PROCESSING 6

// Validity
#define XBEE_VALID_ID 	(0x01 << 0)
#define XBEE_VALID_SHSL	(0x01 << 1)
#define XBEE_VALID_DHDL	(0x01 << 2)
#define XBEE_VALID_RADIO_DB	(0x01 << 3)

XBee::XBee() {
#ifdef XBEE_CONFIG_USE_SOFTWARE_SERIAL
	_softSerial = NULL;
#endif
	_serial = NULL;
    _state = XBEE_IDLE;
    _command = XBEE_CMD_NONE;
	_isInCommandMode = false;
	_valid = 0;
}

#ifdef XBEE_CONFIG_USE_SOFTWARE_SERIAL
void XBee::begin(SoftwareSerial *softSerial, uint32_t baudrate, uint16_t guardTime) {
	_softSerial = softSerial;
	_guardTime = guardTime;
	_baudrate = baudrate;
	_softSerial->begin(baudrate);
}
#endif

void XBee::begin(HardwareSerial *serial, uint32_t baudrate, uint16_t guardTime) {
	_serial = serial;
	_guardTime = guardTime;
	_baudrate = baudrate;
	_serial->begin(baudrate);
}

uint8_t XBee::available() {
    if (_serial) return _serial->available();
#ifdef XBEE_CONFIG_USE_SOFTWARE_SERIAL	
    if (_softSerial) return _softSerial->available();
#endif
	return 0;
}


char XBee::read() {
    if (_serial) return _serial->read();
#ifdef XBEE_CONFIG_USE_SOFTWARE_SERIAL	
	if (_softSerial) return _softSerial->read();
#endif
	return -1;
}


size_t XBee::write(uint8_t val) {
    if (_serial) _serial->print(val);
#ifdef XBEE_CONFIG_USE_SOFTWARE_SERIAL	
	else if (_softSerial) _softSerial->print(val);
#endif
	return 1;
}

bool XBee::setId(uint8_t *id) {
	if (!_isInCommandMode) return false;
	uint8_t i;
	// check if id is the same
	for (i=0; i<4; i++) {
		if (_id[i] != id[i]) break;
	}
	if (i == 4) return true;
	// if busy in another command, return
	if (isProcessingCommand()) return false;
	for (i=0; i<4; i++) _id[i] = id[i];
	_command = XBEE_CMD_ID_WRITE;
	return true;
}


uint8_t *XBee::getId() {
	if ((_valid & XBEE_VALID_ID) != 0) return _id;
	if (!_isInCommandMode) return NULL;
	if (isProcessingCommand()) return NULL;
	 _command = XBEE_CMD_ID_READ;
	return NULL;
}


bool XBee::setDHDL(uint8_t *dhdl) {
	if (!_isInCommandMode) return false;
	uint8_t i;
	// check if dhdl is the same
	for (i=0; i<16; i++) {
		if (_dhdl[i] != dhdl[i]) break;
	}
	if (i == 16) return true;
	// if busy in another command, return
	if (isProcessingCommand()) return false;
	for (i=0; i<16; i++) _dhdl[i] = dhdl[i];
	_command = XBEE_CMD_DHDL_WRITE;
	return true;
}

uint8_t *XBee::getDHDL() {
	if ((_valid & XBEE_VALID_DHDL) != 0) return _dhdl;
	if (!_isInCommandMode) return NULL;
	if (isProcessingCommand()) return NULL;
	 _command = XBEE_CMD_DH_READ;
	return NULL;
}

uint8_t *XBee::getSHSL() {
	if ((_valid & XBEE_VALID_SHSL) != 0) return _shsl;
	if (!_isInCommandMode) return NULL;
	if (isProcessingCommand()) return NULL;
	 _command = XBEE_CMD_SH_READ;
	return NULL;
}

uint8_t XBee::getRadioDb() {
	if ((_valid & XBEE_VALID_RADIO_DB) != 0) {
		// only read once
		_valid &= ~XBEE_VALID_RADIO_DB;
		return _radioDb;
	}
	if (!_isInCommandMode) return -1;
	if (isProcessingCommand()) return -1;
	 _command = XBEE_CMD_RADIO_DB_READ;
	return -1;
}


bool XBee::enterCommandMode() {
	if (_isInCommandMode) return true;
	if (isProcessingCommand()) return false;
	_command = XBEE_CMD_ENTER_CMD_MODE;
	return true;
}

bool XBee::exitCommandMode() {
	if (!_isInCommandMode) return true;
	if (isProcessingCommand()) return false;
	_command = XBEE_CMD_EXIT_CMD_MODE;
	return true;
}

bool XBee::commitToXBeeFlash() {
	if (isProcessingCommand()) return false;
	_command = XBEE_CMD_COMMIT;
	return true;
}

bool XBee::setBaudrate(uint32_t baudrate) {
	if (baudrate == _baudrate) return true;
	if (!_isInCommandMode) return false;
	if (isProcessingCommand()) return false;
	_baudrate = baudrate;
	_command = XBEE_CMD_BAUDRATE_WRITE;
	return true;
}


bool XBee::setGuardTime(uint16_t guardTime) {
	if (guardTime == _guardTime) return true;
	if (!_isInCommandMode) return false;
	if (isProcessingCommand()) return false;
	_temp = guardTime;
	_command = XBEE_CMD_GUARD_WRITE;
	return true;
}


bool XBee::enable64BitsAdressing() {
	if (!_isInCommandMode) return false;
	if (isProcessingCommand()) return false;
	_command = XBEE_CMD_ENABLE_64B_ADRESSING;
	return true;
}


bool XBee::isProcessingCommand() {
	return (_command != XBEE_CMD_NONE);
}

void XBee::processCommand() {
	// If nothing to do return
	if (_command == XBEE_CMD_NONE) return;
	
	switch (_state) {
		case XBEE_IDLE:
			if (!_isInCommandMode) {
				_waitUntilTime = millis() + _guardTime;
				_state = XBEE_GUARDING_BEFORE;
			} else _state = XBEE_SEND_COMMAND;
			break;
			
		case XBEE_GUARDING_BEFORE:
			// wait for the guard time to elapse
			if ((uint32_t)(millis()-_waitUntilTime) < 0) return;
			// enter command mode
			print(XBEE_COMMAND_SEQUENCE);
			// prepare to wait
			_waitUntilTime = millis() + _guardTime;
			_state = XBEE_GUARDING_AFTER;
			break;
			
		case XBEE_GUARDING_AFTER:
			// wait for the guard time to elapse
			if ((uint32_t)(millis()-_waitUntilTime) < 0) return;
			_state = XBEE_WAIT_FOR_OK;
			_counter = 0;

		case XBEE_WAIT_FOR_OK:
			while (available() > 0) {
				_buffer[_counter++] = read();
				if ((_buffer[_counter-1] == '\n') || (_counter > 9)) break;
			}
			// we have the end of line
			if ((_counter > 0) && (_buffer[_counter-1] == '\n'))  {
				// check for OK
				if ((_counter > 2) && (_buffer[_counter-3] == 'O') && (_buffer[_counter-2] == 'K')) _isInCommandMode = true;
			}	
			_command = XBEE_CMD_NONE;
			_state = XBEE_IDLE;
			break;

		case XBEE_SEND_COMMAND:
			switch (_command) {
				case XBEE_CMD_COMMIT:
					print("ATWR\n");
					break;
				case XBEE_CMD_EXIT_CMD_MODE:
					print("ATCN\n");
					break;
				case XBEE_CMD_ID_READ:
					print("ATID\n");
					break;
				case XBEE_CMD_ID_WRITE:
					print("ATID");
					for (int i=0; i<4; i++) print((char)_id[i]);
					print("\n");
					break;
				case XBEE_CMD_SH_READ:
					print("ATSH\n");
					break;
				case XBEE_CMD_SL_READ:
					print("ATSL\n");
					break;
				case XBEE_CMD_DH_READ:
					print("ATDH\n");
					break;
				case XBEE_CMD_DL_READ:
					print("ATDL\n");
					break;
				case XBEE_CMD_DHDL_WRITE:
					print("ATDH");
					for (int i=0; i<8; i++) print((char)_dhdl[i]);
					print(",DL");
					for (int i=8; i<16; i++) print((char)_dhdl[i]);
					print("\n");
					break;
				case XBEE_CMD_BAUDRATE_WRITE:
					print("ATBD");
					print(_baudrate, HEX);
					print("\n");
					break;
				case XBEE_CMD_GUARD_WRITE:
					print("ATGT");
					print(_temp, HEX);
					print("\n");
					break;
				case XBEE_CMD_RADIO_DB_READ:
					print("ATDB\n");
					break;
				case XBEE_CMD_ENABLE_64B_ADRESSING:
					print("ATMYFFFF\n");
					break;
				default:
					break;
			}
			_state = XBEE_PROCESSING;
			if (_command == XBEE_CMD_COMMIT) {
				_waitUntilTime = millis() + 100;
				_state = XBEE_WRITING;
			}
			_counter = 0;
			break;
			
		case XBEE_WRITING:
			// wait for the guard time to elapse
			if ((uint32_t)(millis()-_waitUntilTime) < 0) return;
			// indicate to the hardware to communicate at this baudrate from now on
			_state = XBEE_PROCESSING;
			
		case XBEE_PROCESSING:
			if (_command == XBEE_CMD_EXIT_CMD_MODE) _isInCommandMode = false;
			else if (_command == XBEE_CMD_ID_WRITE) _valid |= XBEE_VALID_ID;
			else if (_command == XBEE_CMD_DHDL_WRITE) _valid |= XBEE_VALID_DHDL;
			else if (_command == XBEE_CMD_GUARD_WRITE) _guardTime = _temp;
			while (available() > 0) {
				_buffer[_counter++] = read();
				if ((_buffer[_counter-1] == '\n') || (_counter > 9)) break;
			}
			// we have the end of line
			if ((_counter > 0) && (_buffer[_counter-1] == '\n'))  {
				_counter--; 	// remove the \n
				switch (_command) {
					case XBEE_CMD_ID_READ:
						for (int i=0; i<4; i++) _id[i] = '0';
						for (int i=0; i<_counter; i++) _id[3-i] = _buffer[_counter-1-i];
						_valid |= XBEE_VALID_ID;
						_command = XBEE_CMD_NONE;
						break;
					case XBEE_CMD_DH_READ:
						for (int i=0; i<8; i++) _dhdl[i] = '0';
						for (int i=0; i<_counter; i++) _dhdl[7-i] = _buffer[_counter-1-i];
						_command = XBEE_CMD_DL_READ;
						_state = XBEE_SEND_COMMAND;
						break;
					case XBEE_CMD_DL_READ:
						for (int i=0; i<8; i++) _dhdl[i+8] = '0';
						for (int i=0; i<_counter; i++) _dhdl[7-i+8] = _buffer[_counter-1-i];
						_valid |= XBEE_VALID_DHDL;
						_command = XBEE_CMD_NONE;
						break;
					case XBEE_CMD_SH_READ:
						for (int i=0; i<8; i++) _shsl[i] = '0';
						for (int i=0; i<_counter; i++) _shsl[7-i] = _buffer[_counter-1-i];
						_command = XBEE_CMD_SL_READ;
						_state = XBEE_SEND_COMMAND;
						break;
					case XBEE_CMD_SL_READ:
						for (int i=0; i<8; i++) _shsl[i+8] = '0';
						for (int i=0; i<_counter; i++) _shsl[7-i+8] = _buffer[_counter-1-i];
						_valid |= XBEE_VALID_SHSL;
						_command = XBEE_CMD_NONE;
						break;
					case XBEE_CMD_RADIO_DB_READ:
						if (_buffer[0] < 'A') _radioDb = (_buffer[0]-'0') << 4;
						else _radioDb = (_buffer[0]-'A'+10) << 4;
						if (_buffer[1] < 'A') _radioDb += _buffer[1]-'0';
						else _radioDb += _buffer[1]-'A'+10;
						_valid |= XBEE_VALID_RADIO_DB; 
						_command = XBEE_CMD_NONE;
						break;
					case XBEE_CMD_BAUDRATE_WRITE:
						if (_serial) _serial->begin(_baudrate);
#ifdef XBEE_CONFIG_USE_SOFTWARE_SERIAL						
						else if (_softSerial) _softSerial->begin(_baudrate);
#endif
					default:
						_command = XBEE_CMD_NONE;
						break;
				}
			}
			// there was an error, we return to no command
			if ((_state == XBEE_PROCESSING) && (_counter > 9)) _command = XBEE_CMD_NONE;
			// if no command, return to IDLE state
			if (_command == XBEE_CMD_NONE) _state = XBEE_IDLE;
			break;
	}
}
