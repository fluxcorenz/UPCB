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
#include "jaguar.h"
#include "upcb.h"

void JAGUAR_Init_Pins(void)
{
	TRISEbits.TRISE2=INPUT;	//#define JAGUAR_BANK1 		PORTEbits.RE2
	TRISEbits.TRISE1=INPUT;//#define JAGUAR_BANK2 		PORTEbits.RE1
	TRISEbits.TRISE0=INPUT;//#define JAGUAR_BANK3 		PORTEbits.RE0
	TRISAbits.TRISA5=INPUT;//#define JAGUAR_BANK4 		PORTAbits.RA5
	TRISBbits.TRISB3=OUTPUT;//#define JAGUAR_DATA1		LATBbits.LATB3
	TRISBbits.TRISB2=OUTPUT;//#define JAGUAR_DATA2		LATBbits.LATB2
	TRISBbits.TRISB0=OUTPUT;//#define JAGUAR_DATA3		LATBbits.LATB0
	TRISCbits.TRISC6=OUTPUT;//#define JAGUAR_DATA4		LATCbits.LATC4
	TRISCbits.TRISC7=OUTPUT;//#define JAGUAR_DATA5		LATCbits.LATC5
	TRISBbits.TRISB1=OUTPUT;//#define JAGUAR_DATA6		LATBbits.LATB1	
}


void JAGUAR_main(void)
{
	JAGUAR_Init_Pins();
	while(1)
	{
		if(!JAGUAR_BANK1)
		{ //bank 1.  Option (3)(6)(9)(#) N/A (High)	
			JAGUAR_DATA1=Stick_Select;  //map option to Select
			JAGUAR_DATA2=1;
			JAGUAR_DATA3=1;
			JAGUAR_DATA4=1;
			JAGUAR_DATA5=Stick_Fierce; // map # to fierce
			JAGUAR_DATA6=1;
		}
		if(!JAGUAR_BANK2)
		{ //bank 2.  (C)(2)(5)(8)(0) N/A (High)
			JAGUAR_DATA1=Stick_Roundhouse; //map C to Roundhouse
			JAGUAR_DATA2=1;
			JAGUAR_DATA3=1;
			JAGUAR_DATA4=1;
			JAGUAR_DATA5=Stick_Strong;  //map 0 to strong
			JAGUAR_DATA6=1;
		}
		if(!JAGUAR_BANK3)
		{ //bank 3.  (B)(1)(4)(7)(*) N/A (High)
			JAGUAR_DATA1=Stick_Forward; //map B to Forward
			JAGUAR_DATA2=1;
			JAGUAR_DATA3=1;
			JAGUAR_DATA4=1;
			JAGUAR_DATA5=Stick_Jab;// * to jab
			JAGUAR_DATA6=1;
		}
		if(!JAGUAR_BANK4)
		{ //bank 4 (A) Right Left Down Up Pause
			JAGUAR_DATA1=Stick_Short; //map A to short
			JAGUAR_DATA2=Stick_Right;
			JAGUAR_DATA3=Stick_Left;
			JAGUAR_DATA4=Stick_Down;
			JAGUAR_DATA5=Stick_Up;
			JAGUAR_DATA6=Stick_Start; //map pause to Start
		}		
	}	
}