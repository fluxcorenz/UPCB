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
#include "saturn.h"
#include "upcb.h" 
#include <p18cxxx.h>
#include "inttest.h"



void SATURN_Init_Pins(void)
{
	PORTAbits.RA5=1;
	PORTEbits.RE0=1;
	TRISEbits.TRISE0=OUTPUT; //	#define SATURN_D1 LATEbits.LATE0
	TRISAbits.TRISA5=OUTPUT; //	#define SATURN_D0 LATAbits.LATA5
	TRISBbits.TRISB1=INPUT; //	#define SATURN_S0 PORTBbits.RB1
	TRISBbits.TRISB0=INPUT; //	#define SATURN_S1 PORTBbits.RB0
	TRISCbits.TRISC6=OUTPUT; //	#define SATURN_D3 LATCbits.LATC6
	TRISCbits.TRISC7=OUTPUT; //	#define SATURN_D2 LATCbits.LATC7
}

void SATURN_main (void)
{
	SATURN_Init_Pins();
	while((!SATURN_D0) || (!SATURN_D1));
	SATURN_OUT_1;
	mLED_1_Off();
	mLED_2_Off(); 
	while(1) 
	{
	SATURN_LONLY:
	//Sanity check:the should be LL, waiting for the HH to follow
	if (!( (!SATURN_S0) && (!SATURN_S1) )) goto SATURN_RECHECK;
	SATURN_DO_HH:
#ifdef EXTRA_BUTTONS
		if (Stick_Extra0)
		{
			SATURN_LL(1);
		}
		else
		{
			SATURN_LL(0);
		}
#else
		SATURN_LL(1);
#endif



	
	SATURN_DIRS:
	//Sanity check:the should be HH, waiting for the LH to follow
	if (!( (SATURN_S0) && (SATURN_S1) )) goto SATURN_RECHECK;
	SATURN_DO_LH:
		SATURN_FBNI(SATURN_HH, Stick_Up, Stick_Down, Stick_Left, Stick_Right);			
		
	
	SATURN_LKICKS:		
	//Sanity check:the should be LH, waiting for the HL to follow
	if (!( (!SATURN_S0) && (SATURN_S1) )) goto SATURN_RECHECK;
	SATURN_DO_HL:
		SATURN_FBNI(SATURN_LH_1, Stick_Forward, Stick_Roundhouse, Stick_Short, Stick_Start);

	SATURN_PUNCHES:
	//Sanity check:the should be HL, waiting for the LL to follow
	if (!( (SATURN_S0) && (!SATURN_S1) )) goto SATURN_RECHECK;
	SATURN_DO_LL:
#ifdef EXTRA_BUTTONS
		SATURN_FBNI(SATURN_HL, Stick_Fierce, Stick_Strong, Stick_Jab, Stick_Extra1);
#else
		SATURN_FBNI(SATURN_HL, Stick_Fierce, Stick_Strong, Stick_Jab, 1);
#endif
		
	SATURN_RECHECK:
		if(SATURN_S0)
		{
			if(SATURN_S1) goto SATURN_DO_HH; 
			else goto SATURN_DO_HL;			
		}
		else //s0 is low
		{
			if(SATURN_S1) goto SATURN_DO_LH;
			else goto SATURN_DO_LL;
		}

	}
}		
		 
	
	