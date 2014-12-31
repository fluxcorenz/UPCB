#ifndef SNES_H
#define SNES_H
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
/* UPCB module for the Super Nintendo Entertainment System
	Pinout and explanation from GameSX.com
	For US SNES
	 ________
	|1234|567)
	---------
	
	Pin		Description
	1		VCC
	2		Clock
	3		Latch
	4		Data
	5		N/C
	6		N/C
	7		GND
	
Every 16.67ms (or about 60Hz), the SNES CPU sends out a 
12us wide, positive going data latch pulse on pin 3. This 
instructs the ICs in the controller to latch the state of 
all buttons internally. Six microsenconds after the fall of 
the data latch pulse, the CPU sends out 16 data clock pulses 
on pin 2. These are 50% duty cycle with 12us per full cycle. 
The controllers serially shift the latched button states out 
pin 4 on every rising edge of the clock, and the CPU samples 
the data on every falling edge.

Each button on the controller is assigned a specific ID which 
corresponds to the clock cycle during which that button's 
state will be reported. The table in section 4.0 lists the ids 
for all buttons. Note that multiple buttons may be depressed 
at any given moment. Also note that a logic "high" on the 
serial data line means the button is NOT depressed.

At the end of the 16 cycle sequence, the serial data line is 
driven low until the next data latch pulse. The only slight 
deviation from this protocol is apparent in the first clock 
cycle. Because the clock is normally high, the first 
transition it makes after latch signal is a high-to-low 
transition. Since data for the first button (B in this case) 
will be latched on this transition, it's data must actually 
be driven earlier. The SNES controllers drive data for the 
first button at the falling edge of latch. Data for all other 
buttons is driven at the rising edge of clock. 

SNES Controller Button-to-Clock Pulse Assignment
Clock Cycle 	Button Reported
1 				B
2 				Y
3 				Select
4 				Start
5 				Up on joypad
6 				Down on joypad
7 				Left on joypad
8 				Right on joypad
9 				A
10 				X
11 				L
12 				R
13-16 			none (always high)

To make a UPCB cable for SNES, follow the mapping below
D-Sub 15 Pin		SNES Pin
1					7 (GND)
2					Low
3					Low
4					Low
5					Low
6					Low
7					Low
8					1 (VCC)
9					NC - Not connected to anything
10					High
11					High
12					2 (Clock)	
13					3 (Latch)
14					4 (Data)
15					High

Since the first bit has to be out the instant the latch is going, when the clock is already active,
I'll be using bitbanging to control the input and output instead of the SPI 
module. I expect this will be the case for all 4021 based devices (NES, SNES, 3D0)


*/

#define SNES_CLOCK 		PORTBbits.RB0
#define SNES_LATCH 		PORTCbits.RC6
#define SNES_DATA		LATCbits.LATC7

/* Prototypes */
void SNES_Init_Pins(void);
unsigned char SNES_Send_Bit(unsigned char data);
void SNES_main(void);

#endif //SNES_H