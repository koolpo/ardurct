## Objectives ##
While using an Atmegaxx4 processor (Atmega164P, Atmega324P, Atmega644P, Atmega1284P):
  * Preserve as much as possible Arduino functionalities on the same pins as the Uno (up to [R3](https://code.google.com/p/ardurct/source/detail?r=3)) board.
  * Create an 8 bit bus with 3 control lines to manage external parallel devices such as memories and graphic screens, without impacting Arduino pins.

## Overview ##
  * Port A is the analogic input port and port D is the Serial port and PWM port, so can not be used for the 8 bit bus.
  * If we take port C as the bus and 3 pins for the control lines from port B, we only have 2 pins for D4, D7 and D8, as we have to take D11 to D13 from port B. On top of that, we can't use the bus during a Wire transfer, which can slow things down.
  * If we take port B as the bus and 3 pins for the control lines from port C, we have enough pins left on port C for D4, D7 and D8. The benefit of this mapping is that we can use Wire at slow speed. The only constrain is we have to use SPI in master mode and have to wait for SPI to complete a transfer before using the bus, which is quite easy to do.

## Solution ##
  * Arduino Uno mapping has been followed as much as possible, with the exception of:
    * I2C pins which have been mapped to D19=SCL and D20=SDA, as they can not be mapped to A4 and A5.
    * Only 4 PWM pins are mapped : D5, D6, D9 and D10. The 2 last PWM pins are on the 8 bits bus and are mapped to D17 and D18, so they can only be used if the 8 bits port is not used. But in most cases  4 PWM pins are enough.
    * SS pin has been mapped to D18 to allow the use of SPI without having D10 as an output. With this mapping, D10 can also be a PWM.
  * PortB can be used as an 8 bits port without affecting Arduino pins (other than the SPI pins) when it changes. Some care has to be taken when SPI is used: saving and restoring the state of the pins is required if the 8 bis port is used.
  * PortC and PortD provides pins for the Arduino compatible footprint.
  * PortA provides the analog part.

## Schematics ##
![http://ardurct.googlecode.com/svn/images/pins.png](http://ardurct.googlecode.com/svn/images/pins.png)
<br>

<h2>Header file</h2>
Content of <a href='http://code.google.com/p/ardurct/source/browse/trunk/hardware/arduRCT/variants/standard/pins_arduino.h'><pins_arduino.h><a> for Arduino 100<br>
<pre><code>#ifndef Pins_Arduino_h<br>
#define Pins_Arduino_h<br>
<br>
#include &lt;avr/pgmspace.h&gt;<br>
<br>
#define NUM_DIGITAL_PINS            32<br>
#define NUM_ANALOG_INPUTS           8<br>
#define analogInputToDigitalPin(p)  ((p &lt; 8) ? (p) + 24 : -1)<br>
#define digitalPinHasPWM(p)         ((p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 17 || (p) == 18)<br>
<br>
const static uint8_t SS   = 18;<br>
const static uint8_t MOSI = 11;<br>
const static uint8_t MISO = 12;<br>
const static uint8_t SCK  = 13;<br>
<br>
const static uint8_t SDA = 20;<br>
const static uint8_t SCL = 19;<br>
const static uint8_t LED_BUILTIN = 13;<br>
<br>
const static uint8_t A0 = 24;<br>
const static uint8_t A1 = 25;<br>
const static uint8_t A2 = 26;<br>
const static uint8_t A3 = 27;<br>
const static uint8_t A4 = 28;<br>
const static uint8_t A5 = 29;<br>
const static uint8_t A6 = 30;<br>
const static uint8_t A7 = 31;<br>
<br>
// Only pins available for RECEIVE (TRANSMIT can be on any pin):<br>
// I've deliberately left out pin mapping to the Hardware USARTs<br>
// and also the analog pins: this already makes for 20 RECEIVE pins !<br>
<br>
#define digitalPinToPCICR(p)    (((p) &gt;= 4 &amp;&amp; (p) &lt;= 23) ? (&amp;PCICR) : ((uint8_t *)0))<br>
#define digitalPinToPCICRbit(p) ( ((p) == 5 || (p) == 6 || (p) == 9 || (p) == 10) ? 3 : \<br>
				( ((p) &gt;= 11 &amp;&amp; (p) &lt;= 18) ? 1 : 2 ))<br>
#define digitalPinToPCMSK(p)    ( ((p) == 5 || (p) == 6 || (p) == 9 || (p) == 10) ? (&amp;PCMSK3) : \<br>
				( ((p) &gt;= 11 &amp;&amp; (p) &lt;= 18) ? (&amp;PCMSK1) : \<br>
				( ((p) &gt;= 4 &amp;&amp; (p) &lt;= 23) ? (&amp;PCMSK2) : ((uint8_t *)0) )))<br>
#define digitalPinToPCMSKbit(p) ( ((p) == 5 || (p) == 6) ? (p)-1 : \<br>
				( ((p) == 9 || (p) == 10) ? (p)-3 : \<br>
				( ((p) == 4) ? (p)-2 : \<br>
				( ((p) == 7 || (p) == 8) ? (p)-4 : \<br>
				( ((p) &gt;= 11 || (p) &lt;= 13) ? (p)-6 : \<br>
				( ((p) &gt;= 14 || (p) &lt;= 18) ? (p)-14 : \<br>
				( ((p) == 19 || (p) == 20) ? (p)-19 : \<br>
				( ((p) &gt;= 21 || (p) &lt;= 23) ? (p)-16 : 0) )))))))<br>
<br>
#ifdef ARDUINO_MAIN<br>
<br>
// these arrays map port names (e.g. port B) to the<br>
// appropriate addresses for various functions (e.g. reading<br>
// and writing)<br>
const uint16_t PROGMEM port_to_mode_PGM[] = {<br>
	NOT_A_PORT,<br>
	(uint16_t) &amp;DDRA,<br>
	(uint16_t) &amp;DDRB,<br>
	(uint16_t) &amp;DDRC,<br>
	(uint16_t) &amp;DDRD,<br>
};<br>
<br>
const uint16_t PROGMEM port_to_output_PGM[] = {<br>
	NOT_A_PORT,<br>
	(uint16_t) &amp;PORTA,<br>
	(uint16_t) &amp;PORTB,<br>
	(uint16_t) &amp;PORTC,<br>
	(uint16_t) &amp;PORTD,<br>
};<br>
<br>
const uint16_t PROGMEM port_to_input_PGM[] = {<br>
	NOT_A_PORT,<br>
	(uint16_t) &amp;PINA,<br>
	(uint16_t) &amp;PINB,<br>
	(uint16_t) &amp;PINC,<br>
	(uint16_t) &amp;PIND,<br>
};<br>
<br>
const uint8_t PROGMEM digital_pin_to_port_PGM[] = {<br>
	PD,		// PD0 -  D0 / RX<br>
	PD,		// PD1 -  D1 / TX<br>
	PD,		// PD2 -  D2 / RX2<br>
	PD,		// PD3 -  D3 / TX2<br>
	PC,		// PC2 -  D4<br>
	PD,		// PD4 -  D5 / PWM1<br>
	PD,		// PD5 -  D6 / PWM2<br>
	PC,		// PC3 -  D7<br>
	PC,		// PC4 -  D8<br>
	PD,		// PD6 -  D9 / PWM3<br>
	PD,		// PD7 - D10 / PWM4<br>
	PB,		// PB5 - D11 / MOSI<br>
	PB,		// PB6 - D12 / MISO<br>
	PB,		// PB7 - D13 / SCK / LED_BUILTIN<br>
	PB,		// PB0 - D14<br>
	PB,		// PB1 - D15<br>
	PB,		// PB2 - D16<br>
	PB,		// PB3 - D17 / PWM5<br>
	PB,		// PB4 - D18 / PWM6 / SS<br>
	PC,		// PC0 - D19 / SCL<br>
	PC,		// PC1 - D20 / SDA<br>
	PC,		// PC5 - D21<br>
	PC,		// PC6 - D22<br>
	PC,		// PC7 - D23<br>
	PA,		// PA0 - D24 / A0<br>
	PA,		// PA1 - D25 / A1<br>
	PA,		// PA2 - D26 / A2<br>
	PA,		// PA3 - D27 / A3<br>
	PA,		// PA4 - D28 / A4<br>
	PA,		// PA5 - D29 / A5<br>
	PA,		// PA6 - D30 / A6<br>
	PA,		// PA7 - D31 / A7<br>
};<br>
<br>
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {<br>
	_BV(0),		// PD0 -  D0 / RX<br>
	_BV(1),		// PD1 -  D1 / TX<br>
	_BV(2),		// PD2 -  D2 / RX2<br>
	_BV(3),		// PD3 -  D3 / TX2<br>
	_BV(2),		// PC2 -  D4<br>
	_BV(4),		// PD4 -  D5 / PWM1<br>
	_BV(5),		// PD5 -  D6 / PWM2<br>
	_BV(3),		// PC3 -  D7<br>
	_BV(4),		// PC4 -  D8<br>
	_BV(6),		// PD6 -  D9 / PWM3<br>
	_BV(7),		// PD7 - D10 / PWM4<br>
	_BV(5),		// PB5 - D11 / MOSI<br>
	_BV(6),		// PB6 - D12 / MISO<br>
	_BV(7),		// PB7 - D13 / SCK / LED_BUILTIN<br>
	_BV(0),		// PB0 - D14<br>
	_BV(1),		// PB1 - D15<br>
	_BV(2),		// PB2 - D16<br>
	_BV(3),		// PB3 - D17 / PWM5<br>
	_BV(4),		// PB4 - D18 / PWM6 / SS<br>
	_BV(0),		// PC0 - D19 / SCL<br>
	_BV(1),		// PC1 - D20 / SDA<br>
	_BV(5),		// PC5 - D21<br>
	_BV(6),		// PC6 - D22<br>
	_BV(7),		// PC7 - D23<br>
	_BV(0),		// PA0 - D24 / A0<br>
	_BV(1),		// PA1 - D25 / A1<br>
	_BV(2),		// PA2 - D26 / A2<br>
	_BV(3),		// PA3 - D27 / A3<br>
	_BV(4),		// PA4 - D28 / A4<br>
	_BV(5),		// PA5 - D29 / A5<br>
	_BV(6),		// PA6 - D30 / A6<br>
	_BV(7),		// PA7 - D31 / A7<br>
};<br>
<br>
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	TIMER1B,	// PD4 -  D5 / PWM1<br>
	TIMER1A,	// PD5 -  D6 / PWM2<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	TIMER2B,	// PD6 -  D9 / PWM3<br>
	TIMER2A,	// PD7 - D10 / PWM4<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	TIMER0A,	// PB3 - D17 / PWM5<br>
	TIMER0B,	// PB4 - D18 / PWM6 / SS<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
	NOT_ON_TIMER,<br>
};<br>
<br>
#endif<br>
<br>
#endif<br>
</code></pre>