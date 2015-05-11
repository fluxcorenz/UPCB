#ifndef SATURN_H
#define SATURN_H
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
/* UPCB Sega Saturn module
	data from GameSX.com
   	 1       9
	 _________
	/         \
	|=========| (at the Sega Saturn)
	+---------+

	Pin	Name 	Function
	1 	VCC 	+5v (Out)
	2 	D1 	Data 1 (aka D)
	3 	D0 	Data 0 (aka U, least significant bit)
	4 	S0 	Select 0 (aka SEL/TH)
	5 	S1 	Select 1 (aka REQ/TR)
	6 	5v 	+5v (Inp)
	7 	D3 	Data 3 (aka R)
	8 	D2 	Data 2 (aka L)
	9 	GND 	Ground
	

	To make a UPCB cable for this system:
	D-Sub Pin       Saturn Pin
	1				9 (GND)
	2				High (connected to Saturn pin 1 or 6)
	3				Low  (connected to Saturn pin 9)
	4				2 (D1)
	5				3 (D0)
	6				Low
	7				Low
	8				1 & 6 (VCC. You want to make sure you get power from both, in case a converter
							doesn't send power out both lines.)
	9				NC - Not connected to anything
	10				Low
	11				High
	12				5 (S1) (REQ/TR)
	13				7 (D3)
	14				8 (D2)
	15				4 (S0) (SEL/TH)


	Peripheral ID-1:
	= (R when TH=1 or L when TH=1) * 0x8	
	+ (D when TH=1 or U when TH=1) * 0x4
	+ (R when TH=0 or L when TH=0) * 0x2
	+ (D when TH=0 or U when TH=0) * 0x1

	The ID-1 of 0xB is a Saturn control pad.
	This peripheral ID will result in a TH/TR communication mode:

	S0/TH	S1/TR	ACK/TL	D3/R	D2/L	D1/D	D0/U
	On 	On 	1	LT	1	0	0
	Off 	On 	1	R	L	D	U
	On 	Off 	1	Start	A	C	B
	Off	Off 	1	RT	X	Y	Z

	
*/

#include <pic18fregs.h>

#define SATURN_D1 LATEbits.LATE0
#define SATURN_D0 LATAbits.LATA5
#define SATURN_S0 PORTBbits.RB1 //pin 15, sel
#define SATURN_S1 PORTBbits.RB0 //pin 12, req
#define SATURN_D3 LATCbits.LATC6
#define SATURN_D2 LATCbits.LATC7


#define SATURN_D0_L __asm__ ("bcf _LATA, 5");
#define SATURN_D0_H __asm__ ("bsf _LATA, 5");
#define SATURN_D1_L __asm__ ("bcf _LATE, 0");
#define SATURN_D1_H __asm__ ("bsf _LATE, 0");
#define SATURN_D2_L __asm__ ("bcf _LATC, 7");
#define SATURN_D2_H __asm__ ("bsf _LATC, 7");
#define SATURN_D3_L __asm__ ("bcf _LATC, 6");
#define SATURN_D3_H __asm__ ("bsf _LATC, 6");

#define SATURN_OUT_0 SATURN_D0_L SATURN_D1_L SATURN_D2_L SATURN_D3_L
#define SATURN_OUT_1 SATURN_D0_L SATURN_D1_L SATURN_D2_L SATURN_D3_H
#define SATURN_OUT_2 SATURN_D0_L SATURN_D1_L SATURN_D2_H SATURN_D3_L
#define SATURN_OUT_3 SATURN_D0_L SATURN_D1_L SATURN_D2_H SATURN_D3_H
#define SATURN_OUT_4 SATURN_D0_L SATURN_D1_H SATURN_D2_L SATURN_D3_L
#define SATURN_OUT_5 SATURN_D0_L SATURN_D1_H SATURN_D2_L SATURN_D3_H
#define SATURN_OUT_6 SATURN_D0_L SATURN_D1_H SATURN_D2_H SATURN_D3_L
#define SATURN_OUT_7 SATURN_D0_L SATURN_D1_H SATURN_D2_H SATURN_D3_H
#define SATURN_OUT_8 SATURN_D0_H SATURN_D1_L SATURN_D2_L SATURN_D3_L
#define SATURN_OUT_9 SATURN_D0_H SATURN_D1_L SATURN_D2_L SATURN_D3_H
#define SATURN_OUT_A SATURN_D0_H SATURN_D1_L SATURN_D2_H SATURN_D3_L
#define SATURN_OUT_B SATURN_D0_H SATURN_D1_L SATURN_D2_H SATURN_D3_H
#define SATURN_OUT_C SATURN_D0_H SATURN_D1_H SATURN_D2_L SATURN_D3_L
#define SATURN_OUT_D SATURN_D0_H SATURN_D1_H SATURN_D2_L SATURN_D3_H
#define SATURN_OUT_E SATURN_D0_H SATURN_D1_H SATURN_D2_H SATURN_D3_L
#define SATURN_OUT_F SATURN_D0_H SATURN_D1_H SATURN_D2_H SATURN_D3_H

// Prototypes

void SATURN_Init_Pins(void);
void SATURN_main (void);

#endif //SATURN_H
