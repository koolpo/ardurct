#ifndef LOGO_HPP
#define LOGO_HPP 1

/**
 * Approximation of sparkfun logo, 57 x 88
 *    w 0                    write to eeprom slot 0
 *    pc F800                set color to red
 *    tf 0 42 44 42 0 88     fill bottom left triangle
 *    afe 27 35 30           fill right half circle
 *    pts 7                  set line thickness to 7, scalable
 *    annw 25 79 17          draw thick arc to join triangle and half circle
 *    afnw 17 42 17          fill middle left quarter circle
 *    rf 17 35 11 7          fill middle left rectangle
 *    cf 30 13 13            fill top circle
 *    pc FFFF                set color to white, to erase
 *    cf 20 32 9             fill circle at middle left
 *    cf 43 11 8             fill circle at top right
 *    pc F800                set color to red
 *    afnne 32 14 14         fill NNE arc at top right
 *    tf 49 18 49 14 51 17   fill triangle at top right
 *	  0						 end the macro
 **/
 
uint8_t logo[] = "pc F800 "\
    "tf 0 42 44 42 0 88 "\
    "afe 27 35 30 "\
    "pts 7 "\
    "annw 25 79 17 "\
    "afnw 17 42 17 "\
    "rf 17 35 11 7 "\
    "cf 30 13 13 "\
    "pc FFFF "\
    "cf 20 32 9 "\
    "cf 43 11 8 "\
    "pc F800 "\
    "afnne 32 14 14 "\
    "tf 49 18 49 14 51 17"\
	"\0";

#define LOGO_WIDTH 57
#define LOGO_HEIGHT 88

#endif