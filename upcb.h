/* Worldwide defines for Universal PCB */
#ifndef UPCB_H
#define UPCB_H

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

#include "config.h"
#include "program.h"

#define INPUT 				1
#define OUTPUT				0
//#define TRUE				1
//#define FALSE				0


#ifdef STATUS_LEDS
	#define mInitAllLEDs()      TRISAbits.TRISA0=OUTPUT; TRISAbits.TRISA1=OUTPUT;
	#define mLED_1              LATAbits.LATA0
	#define mLED_2              LATAbits.LATA1

	#define mLED_1_On()         mLED_1 = 1;
	#define mLED_2_On()         mLED_2 = 1;

	#define mLED_1_Off()        mLED_1 = 0;
	#define mLED_2_Off()        mLED_2 = 0;

	#define mLED_1_Toggle()     mLED_1 = !mLED_1;
	#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#else
	
	#define mInitAllLEDs()      {}
	#define mLED_1              {}
	#define mLED_2              {}

	#define mLED_1_On()         {}
	#define mLED_2_On()         {}

	#define mLED_1_Off()        {}
	#define mLED_2_Off()        {}

	#define mLED_1_Toggle()     {}
	#define mLED_2_Toggle()     {}
#endif

/* set all 8 input buttons (all of port D) as input, and enable 
	internal pull up resistor. If we have extra buttons, get those too */
#ifdef EXTRA_BUTTONS
	#define mInitButtons()		TRISD = 0xFF;PORTEbits.RDPU=1;TRISAbits.TRISA0=1;TRISAbits.TRISA1=1;
#else
	#define mInitButtons()		TRISD = 0xFF;PORTEbits.RDPU=1;
#endif

/* set all four directions as input. No pull up resistor, so pull up 
	needed on the board.										*/
#define mInitStick()		TRISCbits.TRISC0=1;TRISCbits.TRISC1=1;\
							TRISCbits.TRISC2=1;TRISAbits.TRISA4=1;
							
/* Sets all of the pins going to the console as input. Once we 
	identify the console connected to, then we'll set the pins as 
	needed. */
#define mInitConsolePins()	TRISAbits.TRISA2=1;TRISAbits.TRISA3=1; \
							TRISEbits.TRISE0=1;TRISEbits.TRISE1=1; \
							TRISEbits.TRISE2=1;TRISBbits.TRISB2=1; \
							TRISBbits.TRISB3=1;TRISBbits.TRISB4=1; \
							TRISCbits.TRISC6=1;TRISCbits.TRISC7=1; \
							TRISAbits.TRISA5=1;TRISBbits.TRISB1=1; \
							TRISBbits.TRISB0=1;
							
						//	TRISCbits.TRISC4=1; \
						//	TRISCbits.TRISC5=1;

							
#define DISABLE_INTERRUPTS() {while(INTCONbits.GIE) INTCONbits.GIE=0;}  
#define ENABLE_INTERRUPTS() {INTCONbits.GIE=1;}  
		
unsigned char EEpromGet(unsigned char addr); 
void EEpromPut(unsigned char addr, unsigned char data);


#endif