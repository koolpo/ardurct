#ifndef STREAM_PRINTF_H
#define STREAM_PRINTF_H 1

/**
 * http://www.utopiamechanicus.com/article/sprintf-arduino/ 
 * Posted on July 20, 2011 by David Pankhurst 
 **/
 
#include <avr/pgmspace.h>
#include <stdarg.h>
 
extern void Stream_printf_progmem(Print &out, PGM_P format, ...);

#endif
 