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
#include "x68000.h"
#include "upcb.h"

void X68000_Init_Pins(void)
{
	TRISEbits.TRISE1=OUTPUT; //#define X68000_DATA1 LATEbits.LATE1
	TRISEbits.TRISE0=OUTPUT; //#define X68000_DATA2 LATEbits.LATE0
	TRISAbits.TRISA5=OUTPUT; //#define X68000_DATA3 LATAbits.LATA5
	TRISBbits.TRISB0=OUTPUT; //#define X68000_DATA4 LATBbits.LATB0
	TRISCbits.TRISC6=OUTPUT; //#define X68000_DATA5 LATCbits.LATC6
	TRISCbits.TRISC7=OUTPUT; //#define X68000_DATA6 LATCbits.LATC7
	TRISBbits.TRISB1=INPUT; //#define X68000_BANKSEL  PORTBbits.RB1		
}


void X68000_main(void)
{
	X68000_Init_Pins();
	while(1)
	{
		if (!X68000_BANKSEL)
		{ // bank 1, Up    Down  Left  Right  L    Start
			X68000_DATA1=Stick_Up;
			X68000_DATA2=Stick_Down;
			X68000_DATA3=Stick_Left;
			X68000_DATA4=Stick_Right;
			X68000_DATA5=Stick_Fierce;
			X68000_DATA6=Stick_Start;			
		}		
		if (X68000_BANKSEL)
		{ // bank 2, R     Y     X     Select A    B
			X68000_DATA1=Stick_Roundhouse;
			X68000_DATA2=Stick_Strong;
			X68000_DATA3=Stick_Jab;
			X68000_DATA4=Stick_Select;
			X68000_DATA5=Stick_Short;
			X68000_DATA6=Stick_Forward;			
		}		
	}	
}
