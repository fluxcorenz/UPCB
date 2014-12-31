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
#include "snes.h"
#include "upcb.h"

void SNES_Init_Pins(void)
{
	TRISBbits.TRISB0=INPUT; // #define SNES_CLOCK 		PORTBbits.RB0
	TRISCbits.TRISC6=INPUT; // #define SNES_LATCH 		PORTCbits.RC6
	TRISCbits.TRISC7=OUTPUT; // #define SNES_DATA		LATCbits.LATC7		
}

unsigned char SNES_Send_Bit(unsigned char data)
/* dat should be sent as soona s the clock is High. If we start High, immediately set data
	high. If we're here on Low, wait for High first. 
	After setting the data, sit and spin until the clock drops.  */
{

		while (!SNES_CLOCK) if (SNES_LATCH) return 1;//wait until rising edge, or go straight ahead if already high
		SNES_DATA=data;	
		while (SNES_CLOCK) if (SNES_LATCH) return 1;//wait for falling edge
		return 0;
		//viola, it is now low.continue on
}

void SNES_main(void)
{
	unsigned char bU, bD, bL, bR, bSt, bSe, bB, bA;
	unsigned char bX, bY, bLT, bRT;
	bU=bD=bL=bR=bSt=bSe=bB=bA=1;
	bX=bY=bLT=bRT=1;
	SNES_Init_Pins();
	SNES_DATA=0;
	while(!SNES_LATCH); //sit and spin until up.
	
	while(1)
	{
			/*
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
			13-16 			none (always high)*/
		//if we're here, latch is up
		//output first bit
		SNES_DATA=bB;
		//wait until clock drops
		while(SNES_CLOCK);    //first drop, send second bit
		
		if (SNES_Send_Bit(bY)) continue; 
			//point of second drop, send third bit
		if (SNES_Send_Bit(bSe)) continue; 
			//point of third drop, send fourth bit
		if (SNES_Send_Bit(bSt)) continue; 
			//point of fourth drop, send fifth bit
		if (SNES_Send_Bit(bU)) continue; 
			//point of fifth drop, send sixth bit
		if (SNES_Send_Bit(bD)) continue; 
			//point of sixth drop, send seventh bit
		if (SNES_Send_Bit(bL)) continue;
			//point of seventh drop, send eight bit
		if (SNES_Send_Bit(bR)) continue;
			//point of eigth drop, send ninth bit
		if (SNES_Send_Bit(bA)) continue;
			//point of ninth drop, send tenth bit
		if (SNES_Send_Bit(bX)) continue;
			//point of tenth drop, send eleventh bit
		if (SNES_Send_Bit(bLT)) continue;
			//point of eleventh drop, send twelveth (sp???) bit
		if (SNES_Send_Bit(bRT)) continue;
			//point of twelth drop, send thirteenth bit
		if (SNES_Send_Bit(1)) continue;
		//point of thirteenth drop, send fourteenth bit
		/* rest will remain high. No need to monitor the clock, just start checking the button/stick statuses. */			
		FrameUpdate();
		while (1)
		{
			//end of data. We have time here, so camp out, watch for the Latch to go up, 
			//and fill up the buffer variables until it does
			bU=Stick_Up; if (SNES_LATCH) break;
			bD=Stick_Down; if (SNES_LATCH) break;
			bL=Stick_Left; if (SNES_LATCH) break;
			bR=Stick_Right; if (SNES_LATCH) break;
			bSt=Stick_Start; if (SNES_LATCH) break;
			bSe=Stick_Select; if (SNES_LATCH) break;
			bB=Stick_Short; if (SNES_LATCH) break;
			bA=Stick_Forward; if (SNES_LATCH) break;
			bY=Stick_Jab; if (SNES_LATCH) break;
			bX=Stick_Strong; if (SNES_LATCH) break;
			bLT=Stick_Fierce; if (SNES_LATCH) break;
			bRT=Stick_Roundhouse; if (SNES_LATCH) break;			
		}

	
	}
}