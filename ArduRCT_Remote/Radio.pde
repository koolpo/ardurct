#include "ArduRCT_Remote.h"
#include "Radio.h"

uint8_t radioCounter;
uint8_t radioState;

uint8_t servoPosition[NB_CHANNELS];

NewSoftSerial nss(XBEE_TX, XBEE_RX);
XBee xBee;

void radioSetup() {
    radioCounter = 0;
    radioState = RADIO_IDLE;
    xBee.begin(&nss, XBEE_BAUDRATE, XBEE_GUARD_TIME);
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
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
            radioCounter ++;
    radioDebugPulse(12, radioCounter*10);
            if (val != RADIO_FRAME_START) {
                // release the trapped markers
                for (uint8_t i=radioCounter; i>1; i--) Serial.print(RADIO_FRAME_START);
                // write back the received character
                Serial.print(val);
                Serial.print(":");
                Serial.println(val, BIN);
                radioCounter = 0;
            } 
        }
        if (radioCounter < RADIO_REPEAT_FRAME_START) return;
        radioState = RADIO_IN_FRAME;
    }
    radioDebugPulse(12, 80);
    if (radioState == RADIO_IN_FRAME) {
        if (!xBee.available()) return;
        char val = xBee.read();
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
            radioDebugPulse(13, 10);
            while (xBee.available() && (radioCounter < NB_CHANNELS*2)) {
                char val = xBee.read();
                if (radioCounter % 2 == 0) servoPosition[radioCounter/2] = val;
                else servoPosition[radioCounter/2] = servoPosition[radioCounter/2] * 16 + val;
                radioCounter ++;
                radioDebugPulse(13, radioCounter*2);
            } 
            if (radioCounter == NB_CHANNELS*2) {
                radioDebugPulse(13, 5);
                for (int i=0; i<NB_CHANNELS; i++) ServoManager.mapSet(i, servoPosition[i], 0, 100);
                radioCounter = 0;
                radioState = RADIO_IDLE;
                xBee.print("#");
                radioDebugPulse(13, 5);
            }
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
