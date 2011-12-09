/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 24 : -1)
#define digitalPinHasPWM(p)         ((p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11 || (p) == 17 || (p) == 18)

const static uint8_t SS   = 18;
const static uint8_t MOSI = 11;
const static uint8_t MISO = 12;
const static uint8_t SCK  = 13;

const static uint8_t SDA = 20;
const static uint8_t SCL = 19;
const static uint8_t LED_BUILTIN = 13;

const static uint8_t A0 = 24;
const static uint8_t A1 = 25;
const static uint8_t A2 = 26;
const static uint8_t A3 = 27;
const static uint8_t A4 = 88;
const static uint8_t A5 = 29;
const static uint8_t A6 = 30;
const static uint8_t A7 = 31;

// Only pins available for RECEIVE (TRANSMIT can be on any pin):
// I've deliberately left out pin mapping to the Hardware USARTs
// and also the analog pins: that already makes for 20 RECEIVE pins !

#define digitalPinToPCICR(p)    (((p) >= 4 && (p) <= 23) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) ( ((p) == 5 || (p) == 6 || (p) == 9 || (p) == 10) ? 3 : \
								( ((p) >= 11 && (p) <= 18) ? 1 : 2 ))
#define digitalPinToPCMSK(p)    ( ((p) == 5 || (p) == 6 || (p) == 9 || (p) == 10) ? (&PCMSK3) : \
								( ((p) >= 11 && (p) <= 18) ? (&PCMSK1) : \
								( ((p) >= 4 && (p) <= 23) ?	(&PCMSK2) : ((uint8_t *)0) )))
#define digitalPinToPCMSKbit(p) ( ((p) == 5 || (p) == 6) ? (p)-1 : \
								( ((p) == 9 || (p) == 10) ? (p)-3 : \
								( ((p) == 4) ? (p)-2 : \
								( ((p) == 7 || (p) == 8) ? (p)-4 : \
								( ((p) >= 11 || (p) <= 13) ? (p)-6 : \
								( ((p) >= 14 || (p) <= 18) ? (p)-14 : \
								( ((p) == 19 || (p) == 20) ? (p)-19 : \
								( ((p) >= 21 || (p) <= 23) ? (p)-16 : 0) )))))))

#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD,		// PD0 -  D0 / RX
	PD,		// PD1 -  D1 / TX
	PD,		// PD2 -  D2 / RX2
	PD,		// PD3 -  D3 / TX2
	PC,		// PC2 -  D4
	PD,		// PD4 -  D5 / PWM1
	PD,		// PD5 -  D6 / PWM2
	PC,		// PC3 -  D7
	PC,		// PC4 -  D8
	PD,		// PD6 -  D9 / PWM3
	PD,		// PD7 - D10 / PWM4
	PB,		// PB5 - D11 / MOSI
	PB,		// PB6 - D12 / MISO
	PB,		// PB7 - D13 / SCK / LED_BUILTIN
	PB,		// PB0 - D14
	PB,		// PB1 - D15
	PB,		// PB2 - D16
	PB,		// PB3 - D17 / PWM5
	PB,		// PB4 - D18 / PWM6 / SS
	PC,		// PC0 - D19 / SCL
	PC,		// PC1 - D20 / SDA
	PC,		// PC5 - D21
	PC,		// PC6 - D22
	PC,		// PC7 - D23
	PA,		// PA0 - D24 / A0
	PA,		// PA1 - D25 / A1
	PA,		// PA2 - D26 / A2
	PA,		// PA3 - D27 / A3
	PA,		// PA4 - D28 / A4
	PA,		// PA5 - D29 / A5
	PA,		// PA6 - D30 / A6
	PA,		// PA7 - D31 / A7
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0),		// PD0 -  D0 / RX
	_BV(1),		// PD1 -  D1 / TX
	_BV(2),		// PD2 -  D2 / RX2
	_BV(3),		// PD3 -  D3 / TX2
	_BV(2),		// PC2 -  D4
	_BV(4),		// PD4 -  D5 / PWM1
	_BV(5),		// PD5 -  D6 / PWM2
	_BV(3),		// PC3 -  D7
	_BV(4),		// PC4 -  D8
	_BV(6),		// PD6 -  D9 / PWM3
	_BV(7),		// PD7 - D10 / PWM4
	_BV(5),		// PB5 - D11 / MOSI
	_BV(6),		// PB6 - D12 / MISO
	_BV(7),		// PB7 - D13 / SCK / LED_BUILTIN
	_BV(0),		// PB0 - D14
	_BV(1),		// PB1 - D15
	_BV(2),		// PB2 - D16
	_BV(3),		// PB3 - D17 / PWM5
	_BV(4),		// PB4 - D18 / PWM6 / SS
	_BV(0),		// PC0 - D19 / SCL
	_BV(1),		// PC1 - D20 / SDA
	_BV(5),		// PC5 - D21
	_BV(6),		// PC6 - D22
	_BV(7),		// PC7 - D23
	_BV(0),		// PA0 - D24 / A0
	_BV(1),		// PA1 - D25 / A1
	_BV(2),		// PA2 - D26 / A2
	_BV(3),		// PA3 - D27 / A3
	_BV(4),		// PA4 - D28 / A4
	_BV(5),		// PA5 - D29 / A5
	_BV(6),		// PA6 - D30 / A6
	_BV(7),		// PA7 - D31 / A7
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER1B,		// PD4 -  D5 / PWM1
	TIMER1A,		// PD5 -  D6 / PWM2
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER2B,		// PD6 -  D9 / PWM3
	TIMER2A,		// PD7 - D10 / PWM4
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER0A,		// PB3 - D17 / PWM5
	TIMER0B,		// PB4 - D18 / PWM6 / SS
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#endif

#endif