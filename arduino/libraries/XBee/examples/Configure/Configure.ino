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

/**
 *    This xBee configuration application recognizes:
 *        ##I[new_id]    Get or set the ID of the xBee
 *        ##D[new_dhdl]  Get or set the DHDL of the xBee
 *        ##S            Get the SHSL of the module
 *        ##Bxxxxx       Set the baudrate to the decimal value xxxxx
 *        ##R            Get the radio strength
 *    The elements between brakets are optional. If ommited, the current value is returned
 */
 
#include <SoftwareSerial.h>
#include <XBee.h>

// hardware
#define XBEE_RX 3
#define XBEE_TX 2
#define XBEE_BAUDRATE 9600

#define SERIAL_BAUDRATE 19200

// software
#define XBEE_CMD_MARKER '#'
#define XBEE_CMD_MARKER_REPEAT 2

#define STATE_IDLE 0
#define STATE_IN_FRAME 1

#define XBEE_CMD_ID 'I'
#define XBEE_CMD_DHDL 'D'
#define XBEE_CMD_SHSL 'S'
#define XBEE_CMD_BAUDRATE 'B'
#define XBEE_CMD_RADIO 'R'


SoftwareSerial softSerial(XBEE_RX, XBEE_TX);
XBee xBee;

uint8_t state = STATE_IDLE;
uint8_t buffer[20];
uint8_t counter;
bool cmdComplete;

void setup() {
    // initialize the xBee on a NewSoftSerial port
    xBee.begin(&softSerial, XBEE_BAUDRATE);
    // initialize the serial
    Serial.begin(SERIAL_BAUDRATE);
    // set the adressing
    //xBee.enable64BitsAdressing();
}

void loop() {
    xBee.processCommand();
    if (xBee.isProcessingCommand()) return;
    
    if (state == STATE_IDLE) {
        while (Serial.available() && (counter != XBEE_CMD_MARKER_REPEAT)) {
            uint8_t val = Serial.read();
            counter ++;
            if (val != XBEE_CMD_MARKER) {
                // release the trapped markers
                for (uint8_t i=counter; i>1; i--) Serial.print(XBEE_CMD_MARKER);
                // write back the received character
                Serial.print(val);
                counter = 0;
            } 
        }
        if (counter != XBEE_CMD_MARKER_REPEAT) return;
        state = STATE_IN_FRAME;
    }
    
    if (state == STATE_IN_FRAME) {
        if (!Serial.available()) return;
        counter = 0;
        state = Serial.read();
        switch (state) {
            case XBEE_CMD_ID:
            case XBEE_CMD_DHDL:
            case XBEE_CMD_SHSL:
            case XBEE_CMD_BAUDRATE:
            case XBEE_CMD_RADIO:
                cmdComplete = false;
                break;
            default:
                state = STATE_IDLE;
                break;
        }
    }
        
    switch (state) {
        case XBEE_CMD_ID:
            if (cmdComplete) {
                uint8_t *ptr = xBee.getId();
                if (ptr != NULL) {
                    Serial.print("ID=");
                    for (uint8_t i=0; i<4; i++) Serial.print(ptr[i]);
                    Serial.println();
                    state = STATE_IDLE;
                }    
            } else {
                cmdComplete = fillBuffer();
                if (cmdComplete && (counter > 4)) {
                    xBee.setId(buffer);
                    state = STATE_IDLE;
                }
            }
            break;
        case XBEE_CMD_DHDL:
            if (cmdComplete) {
                uint8_t *ptr = xBee.getDHDL();
                if (ptr != NULL) {
                    Serial.print("DHDL=");
                    for (uint8_t i=0; i<16; i++) Serial.print(ptr[i]);
                    Serial.println();
                    state = STATE_IDLE;
                }    
            } else {
                cmdComplete = fillBuffer();
                if (cmdComplete && (counter > 16)) {
                    xBee.setDHDL(buffer);
                    state = STATE_IDLE;
                }
            }
            break;
        case XBEE_CMD_SHSL:
            if (cmdComplete) {
                uint8_t *ptr = xBee.getSHSL();
                if (ptr != NULL) {
                    Serial.print("SHSL=");
                    for (uint8_t i=0; i<16; i++) Serial.print(ptr[i]);
                    Serial.println();
                    state = STATE_IDLE;
                }    
            } else cmdComplete = fillBuffer();
            break;
        case XBEE_CMD_BAUDRATE:
            cmdComplete = fillBuffer();
            // have we finished to receive the command
            if (cmdComplete) {
                if (counter > 3) {
                    uint32_t baudrate = 0;
                    for (int i=0; i<counter-1; i++) {
                        // we expect a value as base 10
                        baudrate = baudrate * 10;
                        baudrate += buffer[i]-'0';
                    }
                    if (baudrate > 0) {
                        xBee.setBaudrate(baudrate);
                        Serial.print("BR=");
                        Serial.print(baudrate, DEC);
                        Serial.println();
                    }
                }
                state = STATE_IDLE;
            }
            break;
        case XBEE_CMD_RADIO:
            if (cmdComplete) {
                uint8_t radioDb = xBee.getRadioDb();
                if (radioDb != 0xFF) {
                    Serial.print("DB=");
                    Serial.print(radioDb, DEC);
                    Serial.println();
                    state = STATE_IDLE;
                }
            } else cmdComplete = fillBuffer();
            break;
    }
}

bool fillBuffer() {
    while (Serial.available()) {
        buffer[counter++] = Serial.read();
        if (buffer[counter-1] == '\n') return true;
    }
    return false;
}