#include "pitches.h"

#define MELODY_LENGTH (sizeof(melodyNotes)/sizeof(uint16_t))
#define MELODY_SECONDS_BETWEEN_PLAYS 3
#define MELODY_SILENCE 20
#define MELODY_SPEED_MUL 8
#define MELODY_SPEED_DIV 6

uint16_t melodyNotes[] = {  NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4, 0, 
                            NOTE_C5, NOTE_E5, NOTE_A5, NOTE_B5, 0, NOTE_E5, NOTE_GS5, NOTE_B5, NOTE_C6, 0, 
                            NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4, 0, 
                            NOTE_C5, NOTE_E5, NOTE_A5, NOTE_B5, 0, NOTE_E5, NOTE_C6, NOTE_B5, NOTE_A5};

uint8_t melodyNoteDurations[] = {   2, 2, 2, 2, 2, 2, 2, 2, 8, 2, 
                                    2, 2, 2, 4, 2, 2, 2, 2, 8, 2, 
                                    2, 2, 2, 2, 2, 2, 2, 2, 8, 2, 
                                    2, 2, 2, 4, 2, 2, 2, 2, 8 };
                    
