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

void NES_Init_Pins(void) {
	TRISBbits.TRISB0=INPUT; // #define NES_CLOCK 	PORTBbits.RB0
	TRISCbits.TRISC6=INPUT; // #define NES_LATCH 	PORTCbits.RC6
	TRISCbits.TRISC7=OUTPUT; // #define NES_DATA	LATCbits.LATC7		
}

void NES_main(void) {
	unsigned char bU, bD, bL, bR, bSt, bSe, bB, bA;
	bU=bD=bL=bR=bSt=bSe=bB=bA=1;
	NES_Init_Pins();
	NES_DATA=0;

	while (!NES_LATCH) {} //latch high=start of data

	while(1) {
		//Latch should be high at this point.
		//send the first output (A) until the first clock.
		NES_DATA=bA;
		while(NES_CLOCK) {} //wait until clock is low (value read by NES)
		
		//now wait for the nes clocks, and send the remaining 
		//outputs in order (B,Sel,St,U,D,L,R)
		while(!NES_CLOCK) {} //clock high = set data
		NES_DATA=bB; //start sending next bit of data
		while(NES_CLOCK) {} //clock low = NES reads data

		while(!NES_CLOCK) {}
		NES_DATA=bSe;
		while(NES_CLOCK) {}

		while(!NES_CLOCK) {}
		NES_DATA=bSt;
		while(NES_CLOCK) {}

		while(!NES_CLOCK) {}
		NES_DATA=bU;
		while(NES_CLOCK) {}

		while(!NES_CLOCK) {}
		NES_DATA=bD;
		while(NES_CLOCK) {}

		while(!NES_CLOCK) {}
		NES_DATA=bL;
		while(NES_CLOCK) {}

		while(!NES_CLOCK) {}
		NES_DATA=bR;
		while(NES_CLOCK) {} //the final clock signals the end of this data.

		NES_DATA=0;
		FrameUpdate();

		while (1) { //Now just buffer the inputs until we're latched again
	 		bA=Stick_Forward; if (NES_LATCH) break;	
			bB=Stick_Short; if (NES_LATCH) break;
			bSe=Stick_Select; if (NES_LATCH) break;
			bSt=Stick_Start; if (NES_LATCH) break;
			bU=Stick_Up; if (NES_LATCH) break;
			bD=Stick_Down; if (NES_LATCH) break;
			bL=Stick_Left; if (NES_LATCH) break;
			bR=Stick_Right; if (NES_LATCH) break;
		}
	}	
}

