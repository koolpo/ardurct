#include "ArduRCT_Remote.h"
#include "Radio.h"

uint8_t radioCounter;
uint8_t radioState;


NewSoftSerial nss(XBEE_TX, XBEE_RX);
XBee xBee;

void radioSetup() {
    radioCounter = 0;
    radioState = RADIO_IDLE;
    xBee.begin(&nss, XBEE_BAUDRATE, XBEE_GUARD_TIME);
}


void radioProcessReceive() {
    // process the command
    if (radioState == RADIO_COMMAND) {
        xBee.processCommand();
        if (xBee.isInCommandMode()) return;
        // command is finished, return to the idle mode
        radioState = RADIO_IDLE;
    }

    if (!xBee.available()) return;
    

    if (radioState == RADIO_IDLE) {
        while (xBee.available() && (radioCounter < RADIO_REPEAT_FRAME_START)) {
            char val = xBee.read();
Serial.print(val, BYTE);
            radioCounter ++;
            if (val != RADIO_FRAME_START) {
                // release the trapped markers
                for (uint8_t i=radioCounter; i>1; i--) Serial.print(RADIO_FRAME_START);
                // write back the received character
                Serial.print(val);
                radioCounter = 0;
            } 
        }
        if (radioCounter < RADIO_REPEAT_FRAME_START) return;
        radioState = RADIO_IN_FRAME;
    }
    if (radioState == RADIO_IN_FRAME) {
        if (!xBee.available()) return;
        char val = xBee.read();
 Serial.print(val, BYTE);
        switch (val) {
            case RADIO_SERVO:
            case RADIO_MESURE:
            case RADIO_GET_GPS:
            case RADIO_OUTPUT:
            case RADIO_DB:
                radioState = val;
                radioCounter = 0;
                break;
            default:
                radioState = RADIO_IDLE;
                radioCounter = 0;
                break;
        }
    }    
    
    switch (radioState) {
        case RADIO_SERVO:
            // wait for the frame to arrive
            if (xBee.available() < NB_CHANNELS) return;
            // process the channels
            Serial.print(" ");
            for (int i=0; i<NB_CHANNELS; i++) {
                uint8_t val = xBee.read();
                ServoManager.mapSet(i, val, 0, 100, SERVO_MANAGER_SYNCHRONIZE);

                Serial.print(i, DEC);
                Serial.print(":");
                Serial.print(val, DEC);
                Serial.print(" ");
            }
            Serial.println();
            ServoManager.synchronizeChangeRequests();
            ServoManager.debug();
            radioState = RADIO_IDLE;
            xBee.print("#");
            break;
            
        case RADIO_MESURE:
        case RADIO_GET_GPS:
        case RADIO_OUTPUT:
        default:
            radioState = RADIO_IDLE;
            break;
    }
} 

void radioProcessTransmit() {
    // Send all the characters received by Serial to the radio
    while (!ServoManager.isPulsing() && Serial.available()) xBee.print((char)Serial.read());
}

inline void radioDebugPulse(uint8_t pin, uint8_t count) {
    volatile uint8_t *pport = portOutputRegister(digitalPinToPort(pin));
    uint8_t val = *pport;

    while (count--) {
        *pport = val | digitalPinToBitMask(pin);
        *pport = val;
    }
}
