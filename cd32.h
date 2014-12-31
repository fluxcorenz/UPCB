#ifndef CD32_H
#define CD32_H
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
/* UPCB module for Amiga CD32 Support
Main sources of information on the Amiga CD32 comes from:
http://www.gerdkautzmann.de/cd32gamepad/cd32gamepad.html
Much thanks to StreetSkiver for pointing it out to me. 

	The Amiga CD32 pad uses a standard 9 pin D-Sub connector like most
	consoles and computers of the 80's. To keep itself compatible with
	older 9 pin systems, it uses a single line (Pin 5) to detect whether 
	to go into 'standard' mode (high), or whether to go into CD32 mode 
	(low). When in 'standard' mode, there are only two buttons reported 
	(the 'red' and 'blue' ones) but when in CD32 mode, all 7 digital 
	buttons are reported via a serial connection using pin 6 as a clock
	and pin 9 as serial data. In both cases, the directions are sent 
	parallel over lines one through four. 
	
	The workings in standard mode are simple and work just like the other
	9 pin controllers like the MSX, all detailed in the neogeo.h file. 
	
	The workings in CD32 mode should be fleshed out. Pin 5 is actually 
	connected to the 'load/shift' input on a LS125 shift register. When
	Pin 5 is High, Pin 6 is used as an output for button 1. Because Pin
	9 is the output of the shift register, it will always output the 
	first bit of the shift register when the Pin 5 is high; the first
	bit in the sequence is button blue, so this works perfectly. When Pin 5
	goes low, the shift register goes into shift mode, and the pin 6 is
	disabled as an output. The output bit changes on the rising edge of the
	clock, in the following order:
	1 - Button Blue
	2 - Button Red
	3 - Button Yellow
	4 - Button Green
	5 - Button Right
	6 - Button Left
	7 - Button Pause
	8 - 1 (bit tied high)
	9+ - 0 (tied low)
	And will continue to output 0 until Pin 5 returns to high. 
	
	Based on the information on the site mentioned above, I am making the 
	assumption that the clock line will be high when Pin 5 drops to low. 
		
	
	Amiga CD32 pinout:
	Using standard D-Sub 9 numbering.
	Pin			Description
	1			Up
	2			Down
	3			Left
	4			Right
	5			Mode Select
	6			Button 1 'red' (Standard Mode)/ Clock (CD32 Mode)
	7			VCC (+5)
	8			Ground
	9			Button 2 'blue' (Standard Mode)/ Serial Data (CD32 Mode)
	
	To make a UPCB cable for Amiga CD32 support:
	Straight pin through - No addition parts needed.
	UPCB D-Sub 15 Pin			CD32 Pin
	1							8 (Ground)
	2							Low
	3							1 (Up)
	4							2 (Down)
	5							3 (Left)
	6							Low
	7							High
	8							7 (VCC)
	9							NC - Not Connected
	10							Low
	11							Low
	12							4 (Right)
	13							5 (Mode Select)
	14							6 (Button1/Clock)
	15							9 (Button2/SerialData)
	
*/
	
#define CD32_MODESEL 	PORTCbits.RC6
#define CD32_UP			LATEbits.LATE1
#define CD32_DOWN		LATEbits.LATE0
#define CD32_LEFT		LATAbits.LATA5
#define CD32_RIGHT		LATBbits.LATB0
#define CD32_BUTTON1	LATCbits.LATC7
#define CD32_CLOCK		PORTCbits.RC7
#define CD32_BUTTON2    LATBbits.LATB1
#define CD32_SERDATA	LATBbits.LATB1

#define CD32_Set_Clock_Read()	TRISCbits.TRISC7=INPUT;
#define CD32_Set_Button1_Write() TRISCbits.TRISC7=OUTPUT;
//Easy macro to sit and spin until either the clock falls and rises, or the mode select button raises.
// we should drop everything and exit the loop if the modeselect line raises for any reason
#define CD32_Wait_For_Rising_Edge() while(CD32_CLOCK && !CD32_MODESEL); while(!CD32_CLOCK && !CD32_MODESEL);
/* Prototypes */
void CD32_main(void);
void CD32_Init_Pins(void);


#endif // CD32_H