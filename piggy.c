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
#include "upcb.h"
#include "piggy.h"
#include "config.h"
#include <delays.h>

void PB_Init(void)
{
	TRISBbits.TRISB0=OUTPUT; // #define PB_START 		LATBbits.LATB0		
	TRISBbits.TRISB4=OUTPUT;//  #define PB_GUIDE 		LATBbits.LATB4
}

void PB_main(unsigned char select)
{
	#ifdef DEBUG
		while(1); //sit and spin. We should never be debugging with a piggyback cable attached
	#else
		PB_Init();
		if ((PB_X360 != select) && (PB_DC != select))
		{//if the argument passed isnt one of the two we're expecting, sit and spin. There's a problem
				while(1)
				{
					Delay10KTCYx(250);
					Delay10KTCYx(250);
					Delay10KTCYx(250);
					Delay10KTCYx(250);
					Delay10KTCYx(200); // at 12 MIPS, 1.2mil cycles augh to = 1 sec
					mLED_1_Toggle();
					mLED_2_Toggle();			
				}
		} 
		//okay, we know the argument is legit. Figure out which one, set the PB switch, 
		//then go into the loop
		if (PB_X360 == select)
		{
			LATBbits.LATB7=0;LATBbits.LATB6=1;
		}
		if (PB_DC == select)
		{
			LATBbits.LATB7=1;LATBbits.LATB6=0;
		}
		while(1)
		{
			//not much to do here except keep an eye on the start button/RB0 connection.	
			PB_START = Stick_Start;
			//something else to watch for: version 2 UPCB include the RB4 line, the one 
			//pulled high and used for the Neogeo system select line. If we pull it low when 
			//start and select are pressed, we can simulate an extra button for the xbox360
			//Guide button
			//This is meant specifically for the xbox guide button, but if someone wants, there
			//is no reason this couldnt be used for other plans, like memory card switchs for 
			//four page DC VMU's, or whatever, so this is available for all PB systems.
			
			if (!Stick_Start && !Stick_Select && Stick_Short)
			{
				PB_GUIDE = 0;
			} 
			else
			{
				PB_GUIDE = 1;
			}	
		}
	#endif
}