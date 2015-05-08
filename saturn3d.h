#ifndef SATURN3D_H
#define SATURN3D_H
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
/* UPCB Sega Saturn 3D module
   	 
   	 1       9
	 _________
	/         \
	|=========| (at the Sega Saturn)
	+---------+

	Pin	Name 	Function
	1 	VCC 	+5v (Out)
	2 	D1	Data 1 (aka D)
	3 	D0 	Data 0 (aka U, least significant bit)
	4 	REQ 	Request (Select 1, aka TR)
	5 	SEL 	Select (Select 0, aka TH)
	6 	ACK 	Acknowledge (aka TL)
	7 	D3 	Data 3 (aka R, most significant bit)
	8 	D2 	Data 2 (aka L)
	9 	GND 	Ground
	
	
	To make a UPCB cable for this system:
	D-Sub Pin       Saturn Pin
	1				9 (GND)
	2				High
	3				6 (ACK/TL)
	4				2 (D1)
	5				3 (D0)
	6				Low
	7				High
	8				1 (+5v)
	9				NC - Not connected to anything
	10				Low
	11				Low
	12				5 (REQ/TR)
	13				7 (D3)
	14				8 (D2)
	15				4 (SEL/TH)

	The Saturn sends out data a nibble at a time (4 bits across the 4 data lines).
	The 3D controller also uses pin 6 to signal to the Saturn that it has data
	which should be read.

	
*/

#define SATURN3D_ACK LATEbits.LATE1
#define SATURN3D_D1 LATEbits.LATE0
#define SATURN3D_D0 LATAbits.LATA5
#define SATURN3D_SEL PORTBbits.RB1 //pin 15, read port (select/TH)
#define SATURN3D_REQ PORTBbits.RB0 //pin 12, read port (request/TR)
#define SATURN3D_D3 LATCbits.LATC6
#define SATURN3D_D2 LATCbits.LATC7

// Prototypes
void SATURN3D_Init_Pins(void);
void SATURN3D_main (void);

#endif //SATURN3D_H
