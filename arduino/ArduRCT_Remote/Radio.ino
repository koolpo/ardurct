#include <Servo.h>

#include "ArduRCT_Remote.h"
#include "Radio.h"

#define radio Serial1

uint8_t radioCounter;
uint8_t radioState;


void radioSetup() {
    radioCounter = 0;
    radioState = RADIO_IDLE;
    radio.begin(XBEE_BAUDRATE);
}


void radioProcessReceive() {
    if (!radio.available()) return;
    
    if (radioState == RADIO_IDLE) {
        while (radio.available() && (radioCounter < RADIO_REPEAT_FRAME_START)) {
            char val = radio.read();
Serial.write(val);
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
        if (!radio.available()) return;
        char val = radio.read();
 Serial.write(val);
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
            if (radio.available() < NB_CHANNELS) return;
            // process the channels
            Serial.print(" ");
            for (int i=0; i<NB_CHANNELS; i++) {
                uint8_t val = radio.read();
                servo[i].write(val);

                Serial.print(i, DEC);
                Serial.print(":");
                Serial.print(val, DEC);
                Serial.print(" ");
            }
            Serial.println();
            radioState = RADIO_IDLE;
            radio.print("#");
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
    while (Serial.available()) radio.print((char)Serial.read());
}

inline void radioDebugPulse(uint8_t pin, uint8_t count) {
    volatile uint8_t *pport = portOutputRegister(digitalPinToPort(pin));
    uint8_t val = *pport;

    while (count--) {
        *pport = val | digitalPinToBitMask(pin);
        *pport = val;
    }
}
