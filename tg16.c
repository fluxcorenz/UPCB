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
#include "tg16.h"
#include "upcb.h"

#define TG16_D1_L _asm bcf LATB, 0, ACCESS _endasm
#define TG16_D1_H _asm bsf LATB, 0, ACCESS _endasm
#define TG16_D2_L _asm bcf LATC, 6, ACCESS _endasm
#define TG16_D2_H _asm bsf LATC, 6, ACCESS _endasm
#define TG16_D3_L _asm bcf LATC, 7, ACCESS _endasm
#define TG16_D3_H _asm bsf LATC, 7, ACCESS _endasm
#define TG16_D4_L _asm bcf LATB, 1, ACCESS _endasm
#define TG16_D4_H _asm bsf LATB, 1, ACCESS _endasm

#define TG16_OUT_0 TG16_D1_L TG16_D2_L TG16_D3_L TG16_D4_L
#define TG16_OUT_1 TG16_D1_L TG16_D2_L TG16_D3_L TG16_D4_H
#define TG16_OUT_2 TG16_D1_L TG16_D2_L TG16_D3_H TG16_D4_L
#define TG16_OUT_3 TG16_D1_L TG16_D2_L TG16_D3_H TG16_D4_H
#define TG16_OUT_4 TG16_D1_L TG16_D2_H TG16_D3_L TG16_D4_L
#define TG16_OUT_5 TG16_D1_L TG16_D2_H TG16_D3_L TG16_D4_H
#define TG16_OUT_6 TG16_D1_L TG16_D2_H TG16_D3_H TG16_D4_L
#define TG16_OUT_7 TG16_D1_L TG16_D2_H TG16_D3_H TG16_D4_H
#define TG16_OUT_8 TG16_D1_H TG16_D2_L TG16_D3_L TG16_D4_L
#define TG16_OUT_9 TG16_D1_H TG16_D2_L TG16_D3_L TG16_D4_H
#define TG16_OUT_A TG16_D1_H TG16_D2_L TG16_D3_H TG16_D4_L
#define TG16_OUT_B TG16_D1_H TG16_D2_L TG16_D3_H TG16_D4_H
#define TG16_OUT_C TG16_D1_H TG16_D2_H TG16_D3_L TG16_D4_L
#define TG16_OUT_D TG16_D1_H TG16_D2_H TG16_D3_L TG16_D4_H
#define TG16_OUT_E TG16_D1_H TG16_D2_H TG16_D3_H TG16_D4_L
#define TG16_OUT_F TG16_D1_H TG16_D2_H TG16_D3_H TG16_D4_H

#define TG16_WAIT_FOR_DROP(nibble) while(TG16_BANKSEL); TG16_OUT_##nibble 
#define TG16_SEND(nibble) TG16_OUT_##nibble

//four bit nested ifs
#define TG16_FBNI(COMMAND, FIRST_ARG, SECOND_ARG, THIRD_ARG, FOURTH_ARG) \
if (FIRST_ARG) {  \
	if (SECOND_ARG) {  \
		if (THIRD_ARG) { \
			if (FOURTH_ARG) { \
				COMMAND(F); \
			} else { \
				COMMAND(E); \
			} \
		} else { \
			if (FOURTH_ARG) { \
				COMMAND(D); \
			} else { \
				COMMAND(C); \
			} \
		} \
	} else { \
		if (THIRD_ARG) { \
			if (FOURTH_ARG) { \
				COMMAND(B); \
			} else { \
				COMMAND(A); \
			} \
		} else { \
			if (FOURTH_ARG) { \
				COMMAND(9); \
			} else { \
				COMMAND(8); \
			} \
		} \
	} \
} else { \
	if (SECOND_ARG) { \
		if (THIRD_ARG) { \
			if (FOURTH_ARG) { \
				COMMAND(7); \
			} else { \
				COMMAND(6); \
			} \
		} else { \
			if (FOURTH_ARG) { \
				COMMAND(5); \
			} else { \
				COMMAND(4); \
			} \
		} \
	} else { \
		if (THIRD_ARG) { \
			if (FOURTH_ARG) { \
				COMMAND(3); \
			} else { \
				COMMAND(2); \
			} \
		} else { \
			if (FOURTH_ARG) { \
				COMMAND(1); \
			} else { \
				COMMAND(0); \
			} \
		} \
	} \
}


void TG16_Init_Pins(void) {
	TRISEbits.TRISE0=INPUT;	//#define TG16_ENABLE		PORTEbits.RE0
	TRISAbits.TRISA5=INPUT;	//#define TG16_BANKSEL		PORTAbits.RA5
	TRISBbits.TRISB0=OUTPUT; //#define TG16_D1		LATBbits.LATB0
	TRISCbits.TRISC6=OUTPUT; //#define TG16_D2		LATCbits.LATC6
	TRISCbits.TRISC7=OUTPUT; //#define TG16_D3		LATCbits.LATC7
	TRISBbits.TRISB1=OUTPUT; //#define TG16_D4		LATBbits.LATB1
}

void TG16_main(void) {
	unsigned char sixbutton = !Real_Stick_Select;
	unsigned char turbo_counter1 = 1;
	unsigned char turbo_counter2 = 1;
	
	TG16_Init_Pins();

	while (1) {
		TG16_OUT_0; //initial state is all 0s.
		while (TG16_ENABLE); //wait while we're not active (active low) 
		
		//now that we're active, we know the bank sel is initially high
		//so send directions immediately for speed
		TG16_FBNI(TG16_SEND, Real_Stick_Up, Real_Stick_Right, Real_Stick_Down, Real_Stick_Left);
		//now wait until banksel drops, then send buttons
		TG16_FBNI(TG16_WAIT_FOR_DROP, Real_Stick_Forward && (sixbutton || Real_Stick_Strong || turbo_counter1 == 1), Real_Stick_Short && (sixbutton || Real_Stick_Jab || turbo_counter1 == 1), Real_Stick_Select, Real_Stick_Start);
		
		if (sixbutton) {
			while (!TG16_ENABLE) { //while we're still active in 2 button phase
				if (TG16_BANKSEL) { //if banksel ever goes high again
					TG16_OUT_F; //we shouldn't output data anymore
				}
			}
			TG16_OUT_0; //now we're not active, output all 0s
			while (TG16_ENABLE); //wait until we're selected

			//the initial output is all 0 for 6-button pad detection
			//we're already doing that, so just wait until select drops
			//and then send the extra buttons
			TG16_FBNI(TG16_WAIT_FOR_DROP, Real_Stick_Roundhouse, Real_Stick_Jab, Real_Stick_Strong, Real_Stick_Fierce);
		} else {

			if (turbo_counter1 == 0 && turbo_counter2 == 0) {
				turbo_counter1 = 1;
				turbo_counter2 = 1;
			} else {
				if (turbo_counter2 == 0) {
					turbo_counter1 = 0;
					turbo_counter2 = 1;
				} else {
					turbo_counter2 = 0;
				}
			}
		}
		while (!TG16_ENABLE) { //while we're active
			if (TG16_BANKSEL) { //if banksel ever goes high again
				TG16_OUT_F; //we shouldn't output data anymore
			}
		}
	}
}
