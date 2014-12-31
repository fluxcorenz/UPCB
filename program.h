#ifndef PROGRAM_H
#define PROGRAM_H

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
#include "upcb.h"

#define Stick_Up			PORTCbits.RC2
#define Stick_Down			PORTAbits.RA4
#define Stick_Left			PORTCbits.RC1
#define Stick_Right			PORTCbits.RC0

/* Defines for the System Mapping Address, the starting EEPROM location
	of the 8 bytes of system mapping data. There's room for 32 systems;
	we'll figure out what to do whenever we break that barrier :)
	the order doesnt matter. The only thing that does matter is that they
	are the bytes at ADRESS through ADRESS+7 are unused by any other.
	Reserving the very last bytes for config information, so avoid using 0xF8 and above */

#define MAPPING_SAVED_FLAG 0b01010101 //single byte to stash in the last EEPROM byte
										//checked on boot; if not present, uses default config
#define MAPPING_SAVED_FLAG_LOCATION 0xFF 
									
#define SMA_PSX		0
#define SMA_USB		8
#define SMA_3DO		16
#define SMA_GC		24
#define SMA_JAGUAR	32
#define SMA_NEOGEO	40
#define SMA_NES		48
#define SMA_SATURN	56
#define SMA_TG16	64
#define SMA_X68		72
#define SMA_GENESIS	80
#define SMA_SNES	88
#define SMA_CD32	96
#define SMA_WII		104
#define SMA_XBOX	112

//so the system modes can make the bit 0 with currentstate &= STATE_JAB; 
//S_button commands can return ((currentstate & STATE_JAB)!=0xFFFF)
#define STATE_JAB			0b1111_1111_1111_1110
#define STATE_STRONG		0b1111_1111_1111_1101
#define STATE_FIERCE		0b1111_1111_1111_1011
#define STATE_SHORT			0b1111_1111_1111_0111
#define STATE_FORWARD		0b1111_1111_1110_1111
#define STATE_ROUNDHOUSE	0b1111_1111_1101_1111
#define STATE_EXTRA0		0b1111_1111_1011_1111
#define STATE_EXTRA1		0b1111_1111_0111_1111
#define STATE_START			0b1111_1110_1111_1111
#define STATE_SELECT		0b1111_1101_1111_1111
#define STATE_UP			0b1111_1011_1111_1111
#define STATE_DOWN			0b1111_0111_1111_1111
#define STATE_LEFT			0b1110_1111_1111_1111
#define STATE_RIGHT			0b1101_1111_1111_1111


//recordmode state flags to indicate if we're recording, playing, or what.
#define PROGRAMMODE_OFF			0
#define PROGRAMMODE_NORMAL		1
#define PROGRAMMODE_RECORDING	2
#define PROGRAMMODE_PLAYBACK	3
#define PROGRAMMODE_REMAPPING	4
#define PROGRAMMODE_TURBOSET	5


	//Real_Stick_* maps to the buttons directly that we'll be using
	//placing here so they'll be available whether program mode supported or not
	#define Real_Stick_Jab			PORTDbits.RD0
	#define Real_Stick_Strong		PORTDbits.RD1
	#define Real_Stick_Fierce		PORTDbits.RD2
	#define Real_Stick_Short		PORTDbits.RD3
	#define Real_Stick_Forward		PORTDbits.RD4
	#define Real_Stick_Roundhouse	PORTDbits.RD5
	#define Real_Stick_Select		PORTDbits.RD6
	#define Real_Stick_Start		PORTDbits.RD7
	#define Real_Stick_Up			PORTCbits.RC2
	#define Real_Stick_Down			PORTAbits.RA4
	#define Real_Stick_Left			PORTCbits.RC1
	#define Real_Stick_Right		PORTCbits.RC0
	#define Real_Stick_Program		PORTBbits.RB5
	
	#ifdef EXTRA_BUTTONS
		#define Real_Stick_Extra0		PORTAbits.RA0
		#define Real_Stick_Extra1		PORTAbits.RA1
	#endif // EXTRA_BUTTONS

#ifndef PROGRAM_BUTTON
	//then we don't need to sweat it. just make the defines straight to the ports
	//Program_load_mappings will still be called, but wont go anywhere if not using programming
	#define Program_Load_Mappings(x) {}
	#define FrameUpdate() {}		
	#define Stick_Jab			PORTDbits.RD0
	#define Stick_Strong		PORTDbits.RD1
	#define Stick_Fierce		PORTDbits.RD2
	#define Stick_Short			PORTDbits.RD3
	#define Stick_Forward		PORTDbits.RD4
	#define Stick_Roundhouse	PORTDbits.RD5
	#define Stick_Select		PORTDbits.RD6
	#define Stick_Start			S_Start()
	#define Real_Stick_Start		PORTDbits.RD7
	#ifdef EXTRA_BUTTONS
		#define Stick_Extra0		PORTAbits.RA0
		#define Stick_Extra1		PORTAbits.RA1
	#endif
	unsigned int S_Start(void);
#else
	// PROGRAM_BUTTON is defined, so here comes the work
	//now lets make an enum for the buttons
	#ifdef EXTRA_BUTTONS
		enum e_buttons {JAB=0, STR, FIE, SHO, FOR, ROU, EX0, EX1};
		// b defines are so we can access the bit out of a BYTE with bytevar.bJAB	
		#define bJAB b0
		#define bSTR b1
		#define bFIE b2
		#define bSHO b3
		#define bFOR b4
		#define bROU b5
		#define bEX0 b6
		#define bEX1 b7	
	#else
		enum e_buttons {JAB=0, STR, FIE, SHO, FOR, ROU, SEL, STA};
		#define bJAB b0
		#define bSTR b1
		#define bFIE b2
		#define bSHO b3
		#define bFOR b4
		#define bROU b5
		#define bSEL b6
		#define bSTA b7	
	#endif //EXTRA_BUTTONS
	

	
	void Program_Mapping_Check(void);
	void Program_Load_Mappings(unsigned char sma_address);
	void Program_Save_Mappings(void);
	void FrameUpdate(void);
	//S_* is the function that checks the mapping to see if 
	//any of the mapped buttons are pressed
	unsigned char S_Jab(void);
	unsigned char S_Strong(void);
	unsigned char S_Fierce(void);
	unsigned char S_Short(void);
	unsigned char S_Forward(void);
	unsigned char S_Roundhouse(void);
	unsigned char S_Select(void);
	unsigned char S_Start(void);
	#ifdef EXTRA_BUTTONS
		unsigned char S_Extra0(void);
		unsigned char S_Extra1(void);
	#endif // EXTRA_BUTTONS

	//Stick_* is the defines that make console code calling Stick_*
	//actually call the S_* function

	#define Stick_Jab			S_Jab()
	#define Stick_Strong		S_Strong()
	#define Stick_Fierce		S_Fierce()
	#define Stick_Short			S_Short()
	#define Stick_Forward		S_Forward()
	#define Stick_Roundhouse	S_Roundhouse()
	#define Stick_Select		S_Select()
	#define Stick_Start			S_Start()

	#ifdef EXTRA_BUTTONS
		#define Stick_Extra0		S_Extra0()
		#define Stick_Extra1		S_Extra1()
	#endif

#endif // PROGRAM_BUTTON
#endif // PROGRAM_H