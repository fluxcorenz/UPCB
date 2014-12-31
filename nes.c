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
	while(!NES_LATCH); //sit and spin until up.
	while(1)
	{
		/*
			1		A
			2		B
			3		Select
			4		Start
			5		Up
			6		Down
			7		Left
			8		Right */
		//if we're here, latch is up
		//output first bit
		NES_DATA=bA;
		//wait until clock drops
		while(NES_CLOCK);    //first drop, send second bit
		if (NES_Send_Bit(bB)) continue; 
			//point of second drop, send third bit
		if (NES_Send_Bit(bSe)) continue; 
			//point of third drop, send fourth bit
		if (NES_Send_Bit(bSt)) continue; 
			//point of fourth drop, send fifth bit
		if (NES_Send_Bit(bU)) continue; 
			//point of fifth drop, send sixth bit
		if (NES_Send_Bit(bD)) continue; 
			//point of sixth drop, send seventh bit
		if (NES_Send_Bit(bL)) continue;
			//point of seventh drop, send eight bit
		if (NES_Send_Bit(bR)) continue;
			//point of eigth drop, send LOW
		NES_DATA=0;
		FrameUpdate();
		while (1)
		{
			//end of data. We have time here, so camp out, watch for the Latch to go up, 
			//and fill up the buffer variables until it does
			bU=Stick_Up; if (NES_LATCH) break;
			bD=Stick_Down; if (NES_LATCH) break;
			bL=Stick_Left; if (NES_LATCH) break;
			bR=Stick_Right; if (NES_LATCH) break;
			bSt=Stick_Start; if (NES_LATCH) break;
			bSe=Stick_Select; if (NES_LATCH) break;
			bB=Stick_Short; if (NES_LATCH) break;
			bA=Stick_Forward; if (NES_LATCH) break;
		}
		
	}
}