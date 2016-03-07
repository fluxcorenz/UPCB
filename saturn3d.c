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
#include "saturn3d.h"
#include "upcb.h" 
#include <p18cxxx.h>
#include "inttest.h"

void SATURN3D_Init_Pins(void) {
	TRISEbits.TRISE1=OUTPUT; //#define SATURN3D_ACK LATEbits.LATE1
	TRISEbits.TRISE0=OUTPUT; //#define SATURN3D_D1 LATEbits.LATE0
	TRISAbits.TRISA5=OUTPUT; //#define SATURN3D_D0 LATAbits.LATA5
	TRISBbits.TRISB1=INPUT;  //#define SATURN3D_S0 PORTBbits.RB1
	TRISBbits.TRISB0=INPUT;  //#define SATURN3D_S1 PORTBbits.RB0
	TRISCbits.TRISC6=OUTPUT; //#define SATURN3D_D3 LATCbits.LATC6
	TRISCbits.TRISC7=OUTPUT; //#define SATURN3D_D2 LATCbits.LATC7
	SATURN3D_ACK = 1;
	SATURN3D_D0 = 1;
	SATURN3D_D1 = 0;
	SATURN3D_D2 = 0;
	SATURN3D_D3 = 0;
}

void SATURN3D_main (void) {
	SATURN3D_Init_Pins();
	mLED_1_Off();
	mLED_2_On();

	while(1) {
		while (!SATURN3D_SEL || !SATURN3D_REQ) { } //initial sync- both TR and TH are high 
		while (SATURN3D_SEL) { } //wait for this controller to be selected (active low)
		while (SATURN3D_REQ) { } //wait for data request

		mLED_1_Toggle();

		//first send the peripheral id
		//3D controller in digital mode id = 0000
		SATURN3D_D0 = SATURN3D_D1 = SATURN3D_D2 = SATURN3D_D3 = 0;
		SATURN3D_ACK = 0; //tell saturn we're clear to send

		mLED_2_Toggle();

		while (!SATURN3D_REQ && !SATURN3D_SEL) {} //wait for data request toggle

		//now send the data size
		//only 2 bytes of data for digital, so = 0010
		SATURN3D_D1 = 1;
		SATURN3D_ACK = 1; //tell saturn we're clear to send first nibble

		while (SATURN3D_REQ && !SATURN3D_SEL) {} //wait for data request toggle

		//now send directions, UDLR
		SATURN3D_D0 = Stick_Up;
		SATURN3D_D1 = Stick_Down;
		SATURN3D_D2 = Stick_Left;
		SATURN3D_D3 = Stick_Right;
		SATURN3D_ACK = 0;

		while (!SATURN3D_REQ && !SATURN3D_SEL) {} //wait for data request toggle

		//now send kick buttons, in order B/C/A/Start
		SATURN3D_D0 = Stick_Forward;
		SATURN3D_D1 = Stick_Roundhouse;
		SATURN3D_D2 = Stick_Short;
		SATURN3D_D3 = Stick_Start;
		SATURN3D_ACK = 1;
			
		while (SATURN3D_REQ && !SATURN3D_SEL) {} //wait for data request toggle

		//now send punch buttons, in order ZYXR
		SATURN3D_D0 = Stick_Fierce;
		SATURN3D_D1 = Stick_Strong;
		SATURN3D_D2 = Stick_Jab;
#ifdef EXTRA_BUTTONS
		SATURN3D_D3 = Stick_Extra1; //R
#else
		SATURN3D_D3 = 1;
#endif
		SATURN3D_ACK = 0;
		
		while (!SATURN3D_REQ && !SATURN3D_SEL) {} //wait for data request toggle

		//now send the final nibble of data
		SATURN3D_D0 = SATURN3D_D1 = SATURN3D_D2 = 1;
#ifdef EXTRA_BUTTONS
		SATURN3D_D3 = Stick_Extra0; //L
#else
		SATURN3D_D3 = 1;
#endif
		SATURN3D_ACK = 1;

		while (SATURN3D_REQ && !SATURN3D_SEL) {} //wait for data request toggle

		//we have an 'end byte' to send now
		//first nibble = 0000
		SATURN3D_D0 = SATURN3D_D1 = SATURN3D_D2 = SATURN3D_D3 = 0;
		SATURN3D_ACK = 0; //tell saturn we're clear to send

		while (!SATURN3D_REQ && !SATURN3D_SEL) {} //wait for data request toggle

		//second nibble = 0001
		SATURN3D_D0 = 1;
		SATURN3D_ACK = 1;

		//and we're done.
		FrameUpdate();
	}
}		
