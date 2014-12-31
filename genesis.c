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
#include "genesis.h"
#include "upcb.h"
#include "timers.h"

#define GENESIS_D1_L _asm bcf LATE, 1, ACCESS _endasm
#define GENESIS_D1_H _asm bsf LATE, 1, ACCESS _endasm
#define GENESIS_D2_L _asm bcf LATE, 0, ACCESS _endasm
#define GENESIS_D2_H _asm bsf LATE, 0, ACCESS _endasm
#define GENESIS_D3_L _asm bcf LATA, 5, ACCESS _endasm
#define GENESIS_D3_H _asm bsf LATA, 5, ACCESS _endasm
#define GENESIS_D4_L _asm bcf LATB, 0, ACCESS _endasm
#define GENESIS_D4_H _asm bsf LATB, 0, ACCESS _endasm

#define GENESIS_OUT_0 GENESIS_D1_L GENESIS_D2_L GENESIS_D3_L GENESIS_D4_L
#define GENESIS_OUT_1 GENESIS_D1_L GENESIS_D2_L GENESIS_D3_L GENESIS_D4_H
#define GENESIS_OUT_2 GENESIS_D1_L GENESIS_D2_L GENESIS_D3_H GENESIS_D4_L
#define GENESIS_OUT_3 GENESIS_D1_L GENESIS_D2_L GENESIS_D3_H GENESIS_D4_H
#define GENESIS_OUT_4 GENESIS_D1_L GENESIS_D2_H GENESIS_D3_L GENESIS_D4_L
#define GENESIS_OUT_5 GENESIS_D1_L GENESIS_D2_H GENESIS_D3_L GENESIS_D4_H
#define GENESIS_OUT_6 GENESIS_D1_L GENESIS_D2_H GENESIS_D3_H GENESIS_D4_L
#define GENESIS_OUT_7 GENESIS_D1_L GENESIS_D2_H GENESIS_D3_H GENESIS_D4_H
#define GENESIS_OUT_8 GENESIS_D1_H GENESIS_D2_L GENESIS_D3_L GENESIS_D4_L
#define GENESIS_OUT_9 GENESIS_D1_H GENESIS_D2_L GENESIS_D3_L GENESIS_D4_H
#define GENESIS_OUT_A GENESIS_D1_H GENESIS_D2_L GENESIS_D3_H GENESIS_D4_L
#define GENESIS_OUT_B GENESIS_D1_H GENESIS_D2_L GENESIS_D3_H GENESIS_D4_H
#define GENESIS_OUT_C GENESIS_D1_H GENESIS_D2_H GENESIS_D3_L GENESIS_D4_L
#define GENESIS_OUT_D GENESIS_D1_H GENESIS_D2_H GENESIS_D3_L GENESIS_D4_H
#define GENESIS_OUT_E GENESIS_D1_H GENESIS_D2_H GENESIS_D3_H GENESIS_D4_L
#define GENESIS_OUT_F GENESIS_D1_H GENESIS_D2_H GENESIS_D3_H GENESIS_D4_H

#define GENESIS_WAIT_FOR_DROP(nibble) while(GENESIS_BANKSEL); GENESIS_OUT_##nibble 
#define GENESIS_WAIT_FOR_RISE(nibble) while(!GENESIS_BANKSEL); GENESIS_OUT_##nibble 

//four bit nested ifs
#define GENESIS_FBNI(COMMAND, FIRST_ARG, SECOND_ARG, THIRD_ARG, FOURTH_ARG) \
if(FIRST_ARG)  \
{  \
	if(SECOND_ARG) \
	{  \
		if(THIRD_ARG) \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(F); \
			} \
			else \
			{ COMMAND(E); \
			} \
		} \
		else \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(D); \
			} \
			else \
			{COMMAND(C); \
			} \
		} \
	} \
	else \
	{ \
		if(THIRD_ARG) \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(B); \
			} \
			else \
			{ COMMAND(A); \
			} \
		} \
		else \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(9); \
			} \
			else \
			{ COMMAND(8); \
			} \
		} \
	} \
} \
else \
{ \
	if(SECOND_ARG) \
	{ \
		if(THIRD_ARG) \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(7); \
			} \
			else \
			{ COMMAND(6); \
			} \
		} \
		else \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(5); \
			} \
			else \
			{ COMMAND(4); \
			} \
		} \
	} \
	else \
	{ \
		if(THIRD_ARG) \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(3); \
			} \
			else \
			{ COMMAND(2); \
			} \
		} \
		else \
		{ \
			if(FOURTH_ARG) \
			{ COMMAND(1); \
			} \
			else \
			{ COMMAND(0); \
			} \
		} \
	} \
}

//with 1:8 prescaler, 250 us is 0x177 (375d) 
//0xffff - 0x0177 = 0xFE88
//going with FE00 for easier math
#define GENESIS_COUNTER 0xFE88
#define GENESIS_COUNTER_HB 0xFE
#define GENESIS_COUNTER_LB 0x00

#define GENESIS_WRITE_TIMER { TMR0H=GENESIS_COUNTER_HB;TMR0L=GENESIS_COUNTER_LB; }
#define GENESIS_CHECK_TIMER {lowbuffer=TMR0L; if(GENESIS_COUNTER_HB > TMR0H) goto GENESIS_FIRST_VALLEY;}

void GENESIS_Init_Pins(void)
{
	TRISCbits.TRISC6=INPUT;  //#define GENESIS_BANKSEL PORTCbits.RC6
	TRISEbits.TRISE1=OUTPUT; //#define GENESIS_DATA_1  LATEbits.LATE1
	TRISEbits.TRISE0=OUTPUT; //#define GENESIS_DATA_2	LATEbits.LATE0
	TRISAbits.TRISA5=OUTPUT; //#define GENESIS_DATA_3	LATAbits.LATA5
	TRISBbits.TRISB0=OUTPUT; //#define GENESIS_DATA_4	LATBbits.LATB0
	TRISCbits.TRISC7=OUTPUT; //#define GENESIS_UP		LATCbits.LATC7
	TRISBbits.TRISB1=OUTPUT; //#define GENESIS_DOWN	LATBbits.LATB1	
	#ifdef DEBUG
		TRISBbits.TRISB4=OUTPUT;
	#endif
}



void GENESIS_main(void)
{
	unsigned char lowbuffer, TBM; //TBM = THree Button Mode
	GENESIS_Init_Pins();
	OpenTimer0(TIMER_INT_OFF
			& T0_16BIT
			& T0_SOURCE_INT 
			& T0_PS_1_8  );
//unsigned int ReadTimer0(void)
//void WriteTimer0(unsigned int)
	if (!Real_Stick_Select) TBM=1; else TBM=0;
	while(!GENESIS_BANKSEL); //in the rare case we're low, wait for high
	GENESIS_OUT_F;
	while(1)
	{
/*
//Known working 3 button code
		//we're high. load up and get ready for it to go low.
		GENESIS_FBNI(GENESIS_WAIT_FOR_DROP, 0, 0, Real_Stick_Short, Real_Stick_Start);
		GENESIS_UP=Real_Stick_Up;
		GENESIS_DOWN=Real_Stick_Down;
		//we're low. load up and get ready for it to go high
		GENESIS_FBNI(GENESIS_WAIT_FOR_RISE, Real_Stick_Left, Real_Stick_Right, Real_Stick_Forward, Real_Stick_Roundhouse);
		GENESIS_UP=Real_Stick_Up;
		GENESIS_DOWN=Real_Stick_Down;
//end known working 3 button code
*/

		//we're high. load up and get ready for it to go low.
//THIS point should be the long HH period between frames
GENESIS_TOP:
//GENESIS_FIRST_VALLEY:
		GENESIS_FBNI(GENESIS_WAIT_FOR_DROP, 0, 0, Real_Stick_Short, Real_Stick_Start);
		GENESIS_UP=Real_Stick_Up;
		GENESIS_DOWN=Real_Stick_Down;
		//we dropped. No point in checking the timer; if the timer expired, this is where we'd
		//send 'em
GENESIS_FIRST_VALLEY:
	
//first up		//we're low. load up and get ready for it to go high
		GENESIS_FBNI(GENESIS_WAIT_FOR_RISE, Real_Stick_Left, Real_Stick_Right, Real_Stick_Forward, Real_Stick_Roundhouse);
		GENESIS_UP=Real_Stick_Up;
		GENESIS_DOWN=Real_Stick_Down;
		//start the timer
		GENESIS_WRITE_TIMER;
	//we're high. load up and get ready for it to go low.
		GENESIS_FBNI(GENESIS_WAIT_FOR_DROP, 0, 0, Real_Stick_Short, Real_Stick_Start);
		GENESIS_UP=Real_Stick_Up;
		GENESIS_DOWN=Real_Stick_Down;
		GENESIS_CHECK_TIMER;
		if(TBM) goto GENESIS_FIRST_VALLEY;
		//we're low. load up and get ready for it to go high
//second up
		GENESIS_FBNI(GENESIS_WAIT_FOR_RISE, Real_Stick_Left, Real_Stick_Right, Real_Stick_Forward, Real_Stick_Roundhouse);
		GENESIS_UP=Real_Stick_Up;
		GENESIS_DOWN=Real_Stick_Down;
		//start the timer
		GENESIS_WRITE_TIMER;
		//we're high. load up and get ready for it to go low.
		GENESIS_FBNI(GENESIS_WAIT_FOR_DROP, 0, 0, Real_Stick_Short, Real_Stick_Start);
		GENESIS_UP=0;
		GENESIS_DOWN=0;
		GENESIS_CHECK_TIMER;
		//we're low. load up and get ready for it to go high
//third pulse up
		GENESIS_FBNI(GENESIS_WAIT_FOR_RISE, Real_Stick_Jab, Real_Stick_Select, Real_Stick_Strong, Real_Stick_Forward);
		GENESIS_UP=Real_Stick_Fierce;
		GENESIS_DOWN=Real_Stick_Strong;
		GENESIS_WRITE_TIMER;
		GENESIS_FBNI(GENESIS_WAIT_FOR_DROP, 1, 1, Real_Stick_Short, Real_Stick_Start);
		GENESIS_UP=1;
		GENESIS_DOWN=1;
		GENESIS_CHECK_TIMER;
		//we're low. load up and get ready for it to go high
//second up
		GENESIS_FBNI(GENESIS_WAIT_FOR_RISE, Real_Stick_Left, Real_Stick_Right, Real_Stick_Forward, Real_Stick_Roundhouse);
		GENESIS_UP=Real_Stick_Up;
		GENESIS_DOWN=Real_Stick_Down;


	}
}