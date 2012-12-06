/*
 * HMC6352 - Interface to I2C magnetometer
 *
 * Copyright (c) 2010 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include "HMC6352.h"

#define HMC6352_ADDRESS		0x42
#define HMC6352_GET_HEADING	'A'

HMC6352Class::HMC6352Class() {
}


void HMC6352Class::begin() {
	Wire.begin();
}


uint16_t HMC6352Class::getHeading() {
	uint16_t heading;

	// Send the get data command
	Wire.beginTransmission(HMC6352_ADDRESS >> 1);
	Wire.write(HMC6352_GET_HEADING); 
	Wire.endTransmission();

	// Time delays required by HMC6352 upon receipt of the command
	// Get Data. Compensate and Calculate New Heading : 6ms
	delay(6);

	//get the two data bytes, MSB and LSB
	Wire.requestFrom(HMC6352_ADDRESS >> 1, 2); 

	// The heading output data will be the value in tenths of degrees
	// from zero to 3599 and provided in binary format over the two bytes.
	heading = Wire.read();
	heading = heading << 8;
	heading += Wire.read();
	
	return heading / 10;
}


// Preinstantiate Objects //////////////////////////////////////////////////////
HMC6352Class HMC6352 = HMC6352Class();
