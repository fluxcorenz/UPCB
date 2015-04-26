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
#include "nes.h"
#include "upcb.h"
#include "delays.h"
void NES_Init_Pins(void)
{
	TRISBbits.TRISB0=INPUT; // #define NES_CLOCK 		PORTBbits.RB0
	TRISCbits.TRISC6=INPUT; // #define NES_LATCH 		PORTCbits.RC6
	TRISCbits.TRISC7=OUTPUT; // #define NES_DATA		LATCbits.LATC7		
	NES_DATA=0;
}

unsigned char NES_Send_Bit(unsigned char data)
/* dat should be sent as soona s the clock is High. If we start High, immediately set data
	high. If we're here on Low, wait for High first. 
	After setting the data, sit and spin until the clock drops.  */

{
		while (!NES_CLOCK) if (NES_LATCH) return 1;//wait until rising edge, or go straight ahead if already high
		NES_DATA=data;	
		while (NES_CLOCK) if (NES_LATCH) return 1;//wait for falling edge
		return 0;
		//viola, it is now low.continue on
}


void NES_main(void)
{
	unsigned char bU, bD, bL, bR, bSt, bSe, bB, bA;
	bU=bD=bL=bR=bSt=bSe=bB=bA=1;
	NES_Init_Pins();
	NES_DATA=0;

	while(1) {
		if (NES_LATCH) { //we've seen the latch, store the state & perform output procedure

			bA=Stick_Forward;		
			bB=Stick_Short;	
			bSe=Stick_Select;
			bSt=Stick_Start;
			bU=Stick_Up;
			bD=Stick_Down;
			bL=Stick_Left;
			bR=Stick_Right;
	
			//send the first output (A) until the first clock.
			NES_DATA=bA;
			//now wait for the nes clocks, and send the remaining 
			//outputs in order (B,Sel,St,U,D,L,R)
			while(!NES_CLOCK);
			NES_DATA=bB;
			while(!NES_CLOCK);
			NES_DATA=bSe;
			while(!NES_CLOCK);
			NES_DATA=bSt;
			while(!NES_CLOCK);
			NES_DATA=bU;
			while(!NES_CLOCK);
			NES_DATA=bD;
			while(!NES_CLOCK);
			NES_DATA=bL;
			while(!NES_CLOCK);
			NES_DATA=bR;
			while(!NES_CLOCK); //the final clock signals the end of this data.
			NES_DATA=0;
			FrameUpdate();
		}
	}	
}

