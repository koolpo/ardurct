#include "ArduRCT_Remote.h"
#include "Radio.h"

uint8_t radioCounter;
uint8_t radioState;

uint8_t servoPosition[NB_CHANNELS];

uint8_t proposedRadioId[4];
uint8_t confirmedRadioId[4];

NewSoftSerial nss(XBEE_RX, XBEE_TX);
XBee xBee;

void radioSetup() {
    radioCounter = 0;
    radioState = RADIO_IDLE;
    xBee.begin(&nss, XBEE_BAUDRATE, XBEE_GUARD_TIME);
}


void radioProcessReceive() {
    // check if we have a command to finish
    if (radioState == RADIO_COMMAND) {
        if (xBee.isInCommandMode()) {
            xBee.processCommand();
            return;
        }
        // command is finished, return to the idle mode
        radioState = RADIO_IDLE;
    }

    // expect the start of frame if we are IDLE
    uint8_t val;
    if (radioState == RADIO_IDLE) {
        do {
            val = xBee.read();
            if (val == RADIO_FRAME_START) radioCounter ++;
            else {
                // release the trapped character as we are not in a frame
                Serial.write((char)val);
                radioCounter = 0;
            }
        } while (xBee.available() && (radioCounter < RADIO_REPEAT_FRAME_START));
        if (!xBee.available()) return;
        val = xBee.read();
        switch (val) {
            case RADIO_SERVO:
            case RADIO_ID_PROPOSE:
            case RADIO_ID_CHANGE:
            case RADIO_MESURE:
            case RADIO_GET_GPS:
            case RADIO_OUTPUT:
                radioState = val;
            default:
                radioCounter = 0;
                break;
        }
        if (!xBee.available()) return;
    }
    
    switch (radioState) {
        case RADIO_SERVO:
            do {
                val = xBee.read();
                if (radioCounter % 2 == 0) servoPosition[radioCounter/2] = val;
                else servoPosition[radioCounter/2] = servoPosition[radioCounter/2] * 16 + val;
                radioCounter ++;
            } while (xBee.available() && (radioCounter < NB_CHANNELS));
            if (radioCounter/2 == NB_CHANNELS) {
                for (int i=0; i<NB_CHANNELS; i++) ServoManager.mapSet(i, servoPosition[i], 0, 100);
                radioCounter = 0;
                radioState = RADIO_IDLE;
            }
            break;
            
        case RADIO_ID_PROPOSE:
            do {
                val = xBee.read();
                proposedRadioId[radioCounter++] = val;
            } while (xBee.available() && (radioCounter < 4));
            if (radioCounter == 4) {
                // return the proposed id to verify it
                for (uint8_t i=0; i<RADIO_REPEAT_FRAME_START; i++) xBee.print(RADIO_FRAME_START);
                xBee.print(RADIO_ID_VERIFY);
                for (uint8_t i=0; i<4; i++) xBee.print(proposedRadioId[i]);
                radioCounter = 0;
                radioState = RADIO_IDLE;
            }
            break;
        
        case RADIO_ID_CHANGE:
            do {
                val = xBee.read();
                confirmedRadioId[radioCounter++] = val;
            } while (xBee.available() && (radioCounter < 4));
            if (radioCounter == 4) {
                uint8_t confirmedBytes;
                for (confirmedBytes=0; confirmedBytes<4; confirmedBytes++) {
                    if (confirmedRadioId[confirmedBytes] != proposedRadioId[confirmedBytes]) break;
                }
                if (confirmedBytes == 4) {
                    radioState = RADIO_COMMAND;
                    xBee.setId(confirmedRadioId);
                } else radioState = RADIO_IDLE;
                radioCounter = 0;
            }
            break;

        case RADIO_MESURE:
        case RADIO_GET_GPS:
        case RADIO_OUTPUT:
            break;
            
        default:
            break;
    }
} 

void radioProcessTransmit() {
    // Send all the characters received by Serial to the radio
    while (Serial.available()) xBee.print((char)Serial.read());
}
