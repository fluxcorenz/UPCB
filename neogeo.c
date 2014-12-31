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
#include "neogeo.h"
#include "upcb.h"
#include "timers.h"

void NEOGEO_main(void)
{
	unsigned char button_status;
	unsigned char timertemp;	
	/* If we're here, we've determined we're using the neo output. */
	/* set the appropriate pins to outputs by clearing the TRIS bit */
	TRISA &= 0b11010011; /* pins A2, A3, A5 */
	TRISB &= 0b11010000; /* B0-B3, B5 */
	TRISC &= 0b00111111; /* C6 and C7 */
	TRISE &= 0b11111000; /* E0-E2 */
	
	OpenTimer0(TIMER_INT_OFF
				& T0_16BIT
				& T0_SOURCE_INT 
				& T0_PS_1_8  );
	//with a 1:8 prescale, 1/60th of a second is 0x61A8	ticks of the clock.
	// 0xFFFF-0x61A8 ==	9E57
	//so if we start the timer at 0x9E57, it will turn over after 1 frame. 
	TMR0H=0x9E;TMR0L=0x57;
	
   	
	while(1)
	{
		Con_Neo_A = Stick_Jab; 
		Con_Neo_B = Stick_Short;
		Con_Neo_C = Stick_Strong;
		Con_Neo_D = Stick_Forward;
		Con_Neo_E = Stick_Fierce;
		Con_Neo_F = Stick_Roundhouse;
		Con_Neo_Select = Stick_Select;
		Con_Neo_Start = Stick_Start;
		Con_Neo_Up = Stick_Up;
		Con_Neo_Down = Stick_Down;
		Con_Neo_Left = Stick_Left;
		Con_Neo_Right = Stick_Right;
		
			
		timertemp=TMR0L;  //reading L so high will be refreshed.
		timertemp=TMR0H; // reading H for real
		if (timertemp < 0x9D)
		{
			TMR0H=0x9E;
			TMR0L=0x57; //reset the timer
			FrameUpdate();
		}
		
	} //while(1)
}
