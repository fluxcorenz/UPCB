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

/* int test code
	no real value except testing out how interrupts work on the 18f4550
	*/

#include <p18cxxx.h>
#include "inttest.h"
#include "upcb.h"


/* The HIGH_INT and LOW_INT variables are the core of the interrupt handling. 
	any console puts the pointer to their interrupt routine in these variables 
	as needed. The appropriate high and low ISR's call these functions. */

#pragma idata

void (*LOW_INT_HOOK)(void) = 0;
 void (*HIGH_INT_HOOK)(void) = 0;
 
#pragma code


#pragma code high_isr_vector=0x08
void high_interrupt(void)
{
	_asm GOTO high_int_handler _endasm
}

#pragma code low_isr_vector=0x18
void low_interrupt(void)
{
  	_asm GOTO low_int_handler _endasm
}

#pragma code
//#pragma interrupt high_int_handler

void high_int_handler(void)
{
	HIGH_INT_HOOK();
	_asm RETFIE 0x1 _endasm
}

#pragma code
#pragma interruptlow low_int_handler

void low_int_handler(void)
{
	LOW_INT_HOOK();
	_asm RETFIE 0x1 _endasm
}
#pragma code
void low_int_test(void)
{
	PORTEbits.RE0=(!PORTEbits.RE0);
	INTCON3bits.INT1IF=0;
}

void high_int_test(void)
{
	PORTEbits.RE0=(!PORTEbits.RE0);
	INTCONbits.INT0IF=0;
}




