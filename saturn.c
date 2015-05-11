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
#include "saturn.h"
#include "upcb.h" 
#include <pic18f4550.h>
#include "inttest.h"

void SATURN_Init_Pins(void) {
	PORTAbits.RA5=1;
	PORTEbits.RE0=1;
	TRISEbits.TRISE0=OUTPUT; //#define SATURN_D1 LATEbits.LATE0
	TRISAbits.TRISA5=OUTPUT; //#define SATURN_D0 LATAbits.LATA5
	TRISBbits.TRISB1=INPUT; //#define SATURN_S0 PORTBbits.RB1
	TRISBbits.TRISB0=INPUT; //#define SATURN_S1 PORTBbits.RB0
	TRISCbits.TRISC6=OUTPUT; //#define SATURN_D3 LATCbits.LATC6
	TRISCbits.TRISC7=OUTPUT; //#define SATURN_D2 LATCbits.LATC7
	SATURN_D0_L; //U
	SATURN_D1_L; //D
	SATURN_D2_H; //L
	SATURN_D3_H; //R
}

void SATURN_main (void) {
	SATURN_Init_Pins();
	mLED_1_On();
	mLED_2_Off();
	
	//send megadrive peripheral id for saturn pad
	//wait for initial TH high
    while (!SATURN_S0) {}
	/*_asm
		ensure_th_high:
		btfss PORTB, 1, ACCESS; //if S0/TH is high, skip next instruction
		goto ensure_th_high

	//we're already sending 1100
	
		ensure_th_low:
		btfsc PORTB, 1, ACCESS; //if S0/TH is low, skip next instruction
		goto ensure_th_low
    _endasm;*/
    while (SATURN_S0) {};
                
	SATURN_D0_H; //U
	SATURN_D1_H; //D
	SATURN_D2_H; //L
	SATURN_D3_H; //R

	//should give MD peripheral id of 
	//ID3 = R||L TH1 = 1 || 1 = 1
	//ID2 = U||D TH1 = 0 || 0 = 0
	//ID1 = R||L TH0 = 1 || 1 = 1
	//ID0 = U||D TH0 = 1 || 1 = 1
	//= Bh, for saturn pad
	
	//now just try to go on to normal data loop
	while (1) {
		if (SATURN_S1) {
			if (SATURN_S0) { //HH, LT data
				SATURN_D0_L;
				SATURN_D1_L;
				SATURN_D2_H;
#ifdef EXTRA_BUTTONS
				if (!Stick_Extra0) {
					SATURN_D3_L;
				} else {
					SATURN_D3_H;
				}
#else
				SATURN_D3_H;
#endif
			} else { //LH, stick data
				if (!Stick_Up) { //pressing up
					SATURN_D0_L; //U, active low
					SATURN_D1_H;
				} else if (!Stick_Down) { //pressing down
					SATURN_D0_H;
					SATURN_D1_L;
				} else {
					SATURN_D0_H;
					SATURN_D1_H;
				}
				if (!Stick_Left) { //pressing left
					SATURN_D2_L;
					SATURN_D3_H;
				} else if (!Stick_Right) { //pressing right
					SATURN_D2_H;
					SATURN_D3_L;
				} else {
					SATURN_D2_H;
					SATURN_D3_H;
				}	
			}
		} else {
			if (SATURN_S0) {//HL, ABC data
				if (!Stick_Forward) {
					SATURN_D0_L;
				} else {
					SATURN_D0_H;
				}
				if (!Stick_Roundhouse) {
					SATURN_D1_L;
				} else {
					SATURN_D1_H;
				}
				if (!Stick_Short) {
					SATURN_D2_L;
				} else {
					SATURN_D2_H;
				}
				if (!Stick_Start) {
					SATURN_D3_L;
				} else {
					SATURN_D3_H;
				}	
			} else { //LL, XYZ data
				if (!Stick_Fierce) {
					SATURN_D0_L;
				} else {
					SATURN_D0_H;
				}
				if (!Stick_Strong) {
					SATURN_D1_L;
				} else {
					SATURN_D1_H;
				}
				if (!Stick_Jab) {
					SATURN_D2_L;
				} else {
					SATURN_D2_H;
				}
#ifdef EXTRA_BUTTONS
				if (!Stick_Extra1) {
					SATURN_D3_L;
				} else {
					SATURN_D3_H;
				}
#else
				SATURN_D3_H;
#endif
			}
		}
		mLED_2_Toggle();
	}
}
