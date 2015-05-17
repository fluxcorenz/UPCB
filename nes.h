#ifndef NES_H
#define NES_H
/*
UPCB - Universal Programmed Controller Board 
Copyright (C) 2007  Marcus Post marcus@marcuspost.com


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
/* UPCB module for Nintendo Entertainment System/Famicom support

	The pinout of the American NES controller is well documented, 
	however there is some discrepancy when talking about the 
	Japanese Famicom. The internal workings of the controllers is
	the same, so if you can figure out which lines are the clock,
	latch, and data, a cable for Famicom use can be easily made 
	the same way. 
	
	US NES Controller Pinout
	____	
	|   \	
	|1   \
	|2  7|
	|3  6|
	|4  5|
	------
	Pin 	Description
	1		GND
	2		Clock
	3		Latch
	4		Data
	5		N/C
	6		N/C
	7		VCC (+5v)
		
	To make a UPCB cable for the NES/Famicom, match up pins like this:
	D-Sub 15 Pin		NES Pin
	1			1 (GND)
	2			Low
	3			Low
	4			Low
	5			Low
	6			Low
	7			Low
	8			7 (VCC)
	9			NC - Not connected to anything
	10			High
	11			High
	12			3 (Latch)
	13			2 (Clock)	
	14			4 (Data)
	15			Low

	PAL NES have 'region locked' controllers - NTSC controllers 
	won't work on them without modification. Either modify the 
	PAL console (bridge the diodes on the boards on the controller
        ports - see http://retrogamester.shitfaced.eu/2010/05/how-to-
	be-able-to-use-any-controller-on.html), or you can use pull-up
       	resistors between any High/VCC pin and the Latch and Clock 
	pins (i.e. a resistor between pin 8 and 12, and another 
	between 8 and 13). The value for these resistors in an 
	official PAL NES pad is 3.6k ohm, however 4.7k or 5k will 
	probably work as well.
        Unfortunately I've had problems with using this code on a PAL
	NES with an XBOX360 pad piggybacked. Unplugging the piggyback
	fixes the issue.	
	
	Protocol:
	At rest, Latch is low, clock is high, and data is low. 
	Latch goes high, and data immediately shows the first bit. 
	After about 3-4us, latch drops, and a few (6ish?) us after,
       	the clock line drops for about 1us. Data line changes state 
	on the RISING edge of the clock, pushing the next bit out. 
	Clock stays high for about 9us before dropping again.
	On the 8th drop of the clock line, the data line goes back 
	to its at rest low state. 
	
	_____-__________________________________________________ Latch
	---------_----_----_-----_----_----_----_----_---------- Clock
	_____(b0 )(b1 )(b2 )(b3  )(b4 )(b5 )(b6 )(b7 )__________ Data
	(not to scale, natch. Testing/viewed on a Super Mario Bros. & 
	Duck Hunt cartridge)
	CD4021 chip shifts out the next bit on the rising clock edge; 
	NES reads on the falling edge.
*/

#define NES_CLOCK 		PORTCbits.RC6
#define NES_LATCH 		PORTBbits.RB0
#define NES_DATA		LATCbits.LATC7

/* Prototypes */
void NES_Init_Pins(void);
void NES_main(void);

#endif //NES_H
