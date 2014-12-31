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

	Pin	 Name 	 Function
	1 	VCC 	+5v (Out)
	2 	D1 		Data 1
	3 	D0 		Data 0
	4 	S0 		Select 0
	5 	S1 		Select 1
	6 	5v 		+5v (Inp)
	7 	D3 		Data 3
	8 	D2 		Data 2
	9 	GND 	Ground
	
	Bit Assignments
	S0		S1		D0	d1	d2	d3
	Off 	Off 	Z 	Y 	X 	R
	On 		Off 	B 	C 	A 	St
	Off		On 		Up 	Dn 	Lt 	Rt
	On 		On 		- 	- 	- 	L
	Uses a bank select function, with two pins selecting one of four banks
	to report on the 4 data pins for a total of 16 digital signals.
	The three marked '-' dont correspond to a button, and are believed to be
	used to by the console detect the controller is connected. We'll report
	them as low for now until we get a reason to do otherwise. Pins are active 
	low.
	
	Oscope view of the Saturn hardware:
	S0:---___-___----
	S1:------____----
	The first LH period is about 55us, the HL period is about 15us, and the LL period is about 75us.
	No idea why the inconsistent period lengths. What is even weirder is that even with such long periods,
	the Saturn checks for a response about 1-2us after the level change; WAY faster than it needs to be. If 
	the response to a change wasn't out in about 11 assembly instructions, the outputs would be jittery. 
	To be able to respond that fast, a lot of the work had to be cached in the period before, and unwound 
	using macros;optimized for speed at the cost of code size. After each state change, the outputs are 
	immediately set to what they would be in the next state if connected to a Saturn (so we meet the time
	restriction) and then the lines are rechecked to see if they match what a Saturn would give; if the
	states are out of order, quickly jump to the matching state and output what is needed for that state.
	Both of the converters tried (MagicBox and Innovation) used different orders for checking the state, 
	but luckily they are far more lenient on when they check for the response. Innovation had each period 
	over 100us. 
	
	
	To make a UPCB cable for this system:
	D-Sub Pin       Saturn Pin
	1				9 (GND)
	2				High (connected to saturn pin 1 or 6)
	3				Low  (connected to saturn pin 9)
	4				2 (D1)
	5				3 (D0)
	6				Low
	7				Low
	8				1 & 6 (VCC. You want to make sure you get power from both, in case a converter
							doesnt send power out both lines.)
	9				NC - Not connected to anything
	10				Low
	11				High
	12				5 (S1)
	13				7 (D3)
	14				8 (D2)
	15				4 (S0)
	
	
	*/

#define SATURN_D1 LATEbits.LATE0
#define SATURN_D0 LATAbits.LATA5
#define SATURN_S0 PORTBbits.RB1
#define SATURN_S1 PORTBbits.RB0
#define SATURN_D3 LATCbits.LATC6
#define SATURN_D2 LATCbits.LATC7


#define SATURN_D0_L _asm bcf LATA, 5, ACCESS _endasm
#define SATURN_D0_H _asm bsf LATA, 5, ACCESS _endasm
#define SATURN_D1_L _asm bcf LATE, 0, ACCESS _endasm
#define SATURN_D1_H _asm bsf LATE, 0, ACCESS _endasm
#define SATURN_D2_L _asm bcf LATC, 7, ACCESS _endasm
#define SATURN_D2_H _asm bsf LATC, 7, ACCESS _endasm
#define SATURN_D3_L _asm bcf LATC, 6, ACCESS _endasm
#define SATURN_D3_H _asm bsf LATC, 6, ACCESS _endasm

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

#define SATURN_WAIT_FOR_DROP(label) _asm SATURN_WFD_##label: btfsc PORTB, 1, ACCESS _endasm _asm BRA SATURN_WFD_##label _endasm
#define SATURN_WAIT_FOR_RISE(label) _asm SATURN_WFR_##label: btfss PORTB, 1, ACCESS _endasm _asm BRA SATURN_WFR_##label _endasm

#define SATURN_HH(nibble) {while(SATURN_S0 && SATURN_S1);} SATURN_OUT_##nibble goto SATURN_LKICKS;
#define SATURN_HL(nibble) {while(SATURN_S0 && !SATURN_S1);} SATURN_OUT_##nibble goto SATURN_LONLY;
#define SATURN_LL(nibble) {while(!SATURN_S0 && !SATURN_S1);} SATURN_OUT_##nibble goto SATURN_DIRS;


//	SATURN_WAIT_FOR_RISE(LH_##nibble)  
#define SATURN_LH_0(nibble)  \
	{while(!SATURN_S0 && SATURN_S1);}  \
	_asm btfsc PORTB, 0, ACCESS _endasm \
	_asm BRA SATURN_LH0_HHfound_##nibble _endasm \
	SATURN_OUT_##nibble \
	goto SATURN_PUNCHES; \
	_asm SATURN_LH0_HHfound_##nibble: _endasm \
	SATURN_OUT_0; \
	goto SATURN_DIRS; 

#define SATURN_LH_1(nibble)  \
	{while(!SATURN_S0 && SATURN_S1);}  \
	_asm btfsc PORTB, 0, ACCESS _endasm \
	_asm BRA SATURN_LH0_HHfound_##nibble _endasm \
	SATURN_OUT_##nibble \
	goto SATURN_PUNCHES; \
	_asm SATURN_LH0_HHfound_##nibble: _endasm \
	SATURN_OUT_1; \
	goto SATURN_DIRS; 

//four bit nested ifs
#define SATURN_FBNI(COMMAND, FIRST_ARG, SECOND_ARG, THIRD_ARG, FOURTH_ARG) \
if(FIRST_ARG)  \
{  \
	if(SECOND_ARG) \
	{  \
		if(THIRD_ARG) \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(F); \
			} \
			else \
			{ COMMAND(E); \
			} \
		} \
		else \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(D); \
			} \
			else \
			{COMMAND(C); \
			} \
		} \
	} \
	else \
	{ \
		if(THIRD_ARG) \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(B); \
			} \
			else \
			{ COMMAND(A); \
			} \
		} \
		else \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(9); \
			} \
			else \
			{ COMMAND(8); \
			} \
		} \
	} \
} \
else \
{ \
	if(SECOND_ARG) \
	{ \
		if(THIRD_ARG) \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(7); \
			} \
			else \
			{ COMMAND(6); \
			} \
		} \
		else \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(5); \
			} \
			else \
			{ COMMAND(4); \
			} \
		} \
	} \
	else \
	{ \
		if(THIRD_ARG) \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(3); \
			} \
			else \
			{ COMMAND(2); \
			} \
		} \
		else \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(1); \
			} \
			else \
			{ COMMAND(0); \
			} \
		} \
	} \
}

// Prototypes

void SATURN_Init_Pins(void);
void SATURN_main (void);

#endif //SATURN_H