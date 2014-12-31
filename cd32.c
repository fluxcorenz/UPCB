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
#include <p18cxxx.h>
#include "cd32.h"
#include "upcb.h"

void CD32_main(void)
{
	CD32_Init_Pins();
	
	while(1)
	{
		//no matter which mode we're in, direction pins will always be direction
		//pins. Let's set them now. 
		CD32_UP=Stick_Up;
		CD32_DOWN=Stick_Down;
		CD32_LEFT=Stick_Left;
		CD32_RIGHT=Stick_Right;
		//check which mode we're in
		if (CD32_MODESEL)
			{ //line is high, so we're in  standard mode.
				// set the two button pins and we're done.
				CD32_Set_Button1_Write(); //make sure button1 is an output
				CD32_BUTTON1=Stick_Short;
				CD32_BUTTON2=Stick_Forward;			
			}	
		else
			{ //line is low, so we're in CD32 mode. 
				CD32_Set_Clock_Read(); // change the pin to an input so we can read the clock
				// Order of the serial data:
//					1 - Button Blue
//					2 - Button Red
//					3 - Button Yellow
//					4 - Button Green
//					5 - Button Right
//					6 - Button Left
//					7 - Button Pause
//					8 - 1 (bit tied high)
//					9+ - 0 (tied low)
				CD32_SERDATA=Stick_Forward; // map button Blue to Forward
				CD32_Wait_For_Rising_Edge();
				
				CD32_SERDATA=Stick_Short; // map button Red to Short
				CD32_Wait_For_Rising_Edge();
				
				CD32_SERDATA=Stick_Strong; // map button Yellow to Strong
				CD32_Wait_For_Rising_Edge();
				
				CD32_SERDATA=Stick_Jab; // map button Green to Jab
				CD32_Wait_For_Rising_Edge();
				
				CD32_SERDATA=Stick_Roundhouse; // map button Right to Roundhouse
				CD32_Wait_For_Rising_Edge();
				
				CD32_SERDATA=Stick_Fierce; // map button Left to Fierce
				CD32_Wait_For_Rising_Edge();
				
				CD32_SERDATA=Stick_Start; // map button Pause to Start
				CD32_Wait_For_Rising_Edge();
				
				CD32_SERDATA=1; // next to last bit is tied high
				CD32_Wait_For_Rising_Edge();
				
				CD32_SERDATA=0; // last bit is tied low, and remains there until the 
								//modeselect line goes back high.
				while(!CD32_MODESEL); //sit and sping until modesel goes high.
			}
	}
	
	
}
void CD32_Init_Pins(void)
{
	TRISCbits.TRISC6=INPUT; //#define CD32_MODESEL 	PORTCbits.RC6
	TRISEbits.TRISE1=INPUT;//#define CD32_UP			LATEbits.LATE1
	TRISEbits.TRISE0=INPUT;//#define CD32_DOWN		LATEbits.LATE0
	TRISAbits.TRISA5=INPUT;//#define CD32_LEFT		LATAbits.LATA5
	TRISBbits.TRISB0=INPUT;//#define CD32_RIGHT		LATBbits.LATB0
	//We'll leave the button1/clock line input for now and leave the TRIS
	//assignments to the macros in the main routine.
	TRISCbits.TRISC7=INPUT;//#define CD32_BUTTON1	LATCbits.LATC7
							//#define CD32_CLOCK		PORTCbits.RC7
	TRISBbits.TRISB1=INPUT;//#define CD32_BUTTON2    LATBbits.LATB1
							//#define CD32_SERDATA	LATBbits.LATB1
}