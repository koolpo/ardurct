#include "pitches.h"

#define MELODY_LENGTH 39
#define MELODY_SECONDS_BETWEEN_PLAYS 3
#define MELODY_SILENCE_LENGTH 1
#define MELODY_NOTE_LENGTH 4

int melodyNotes[] = {   NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4, 0, 
                        NOTE_C5, NOTE_E5, NOTE_A5, NOTE_B5, 0, NOTE_E5, NOTE_GS5, NOTE_B5, NOTE_C6, 0, 
                        NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4, 0, 
                        NOTE_C5, NOTE_E5, NOTE_A5, NOTE_B5, 0, NOTE_E5, NOTE_C6, NOTE_B5, NOTE_A5};

int melodyNoteDurations[] = {   200, 200, 200, 200, 200, 200, 200, 200, 800, 200, 
                                200, 200, 200, 400, 200, 200, 200, 200, 800, 200, 
                                200, 200, 200, 200, 200, 200, 200, 200, 800, 200, 
                                200, 200, 200, 400, 200, 200, 200, 200, 800 };
                    
