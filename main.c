/*UPCB - Universal Programmed Controller Board 
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
#include <delays.h>
#include "upcb.h"						
#include "config.h"
#include "neogeo.h"
#include "psx.h"
#include "saturn.h"
#include "genesis.h"
#include "tg16.h"
#include "nes.h"
#include "snes.h"
#include "jaguar.h"
#include "x68000.h"
#include "3do.h"
#include "gc.h"
#include "piggy.h"
#include "cd32.h"
#include "wii.h"
#include "inttest.h"
//#include "pcusb.h"
#include "allusb.h"
#include "ps3usb.h"
#include "xboxusb.h"

/* This must be defined to do the needed remapping using a bootloader
	custom link script. 'rm18f4550.lkr' should be used if using a bootloader
	and normal link script '18f4550.lkr' in the MCC18 install should be used
	if no bootloader. */
	
#define USE_BOOTLOADER

#ifdef USE_BOOTLOADER
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _RESET_INTERRUPT_VECTOR = 0x000800
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code
#endif

#pragma code
/* main program entry point. Setup all pins as inputs, and check pin status
	to figure out which subsystem to run */
void main(void)
{
	/* Set all of the console pins to input, as well as the button & stick
	   pins, using the macros from upcb.h. The console pins used for output
	   will be set as needed in those system's main routine. For now, we want
	   everything nice and safe as inputs */
	ADCON1 |= 0x0F; //set all input as digital. 
	mInitAllLEDs();
	mLED_1_Off();
	mLED_2_Off();
	mInitConsolePins();
	mInitButtons();
	mInitStick();
	Init_Piggyback();

	
	/* System select pins have a specific order and priority and must be 
	   checked in that order. */
	/* first is the single Neogeo pin, D-Sub pin 9, MCU pin RB4. This is the
		only system specific pin; the next four pins afterwards will define 
		the controller 'family', with further pins specifying which specific
		protocol is used. */
	if (!PORTBbits.RB4) 
	{
		mLED_2_On();
		mLED_1_On();
		//yup, its neogeo. run the main neo routine, which never returns.
		Program_Load_Mappings(SMA_NEOGEO);
		NEOGEO_main();
	}
	mLED_1_On();
	/* Usually the next four pins would define the family, however there is one 
		system that uses 10 pins, so only 2 are available to test. If those two 
		are H H, that's the EJP/Jaguar system. */
	if (	(PORTAbits.RA3)  
		&&  (PORTAbits.RA2) )
	{  // yup, its a Jaguar
		Program_Load_Mappings(SMA_JAGUAR);
		JAGUAR_main();
	}
	
	
	
	mLED_2_On();
	/* Next four pins (D-Sub 6/RA3, 7/RA2, 10/RB3, and 11/RB2) define the family */
	/* Family LLLL is for controllers requiring 7 pins, namely the Genesis and 
		six button X68000 controllers. Since 7 pins are used to communicate, and only
		one pin is left to set it as Genesis or X68000, if any other 7 pin 
		controllers are found, a new family will have to be defined. */
	if (	(!PORTAbits.RA3)  
		&&  (!PORTAbits.RA2)
		&&  (!PORTBbits.RB3)
		&&  (!PORTBbits.RB2) )
	{ //then its a member of the 7 pin family. Pin 2/RE2 determines which member
		if (PORTEbits.RE2)
		{ //then its a genesis
			Program_Load_Mappings(SMA_GENESIS);
			GENESIS_main();
		}
		else
		{ // if not, its a six button x68000
			Program_Load_Mappings(SMA_X68);
			X68000_main();
		}
	}
	/* Family LHLL was needed for an addition 7 pin controller, the Amiga CD32,
		currently the only member of the LHLL family */
	if (	(!PORTAbits.RA3)  
		&&  (PORTAbits.RA2)
		&&  (!PORTBbits.RB3)
		&&  (!PORTBbits.RB2) )	
	{ //its a member of this family
		//only entry so far is the Amiga CD32, so let's run it. 
		Program_Load_Mappings(SMA_CD32);
		CD32_main();
	}
	
	
	/* Family LLLH = 6 wire controllers. */
	if (	(!PORTAbits.RA3)  
		&&  (!PORTAbits.RA2)
		&&  (!PORTBbits.RB3)
		&&  (PORTBbits.RB2) )
	{ // 6 pin family. Next two pins (2/RE2, and 3/RE1) define which system in the family
		if ( 	(PORTEbits.RE2)
			&&	(PORTEbits.RE1) )
		{ // HH is the TurboGrafx 16
			Program_Load_Mappings(SMA_TG16);
			TG16_main();
		}
		if ( 	(PORTEbits.RE2)
			&&	(!PORTEbits.RE1) )
		{ //HL is the Saturn	
			Program_Load_Mappings(SMA_SATURN);
			SATURN_main();
		}
	}
	
	
	/* Family LLHL = 5 wire controllers. Most of this is for Playstation work, 
		but the pins needed for I2C communication with the Wii-more are in this
		family, so the Wii is an honorary member. */
	
	if (	(!PORTAbits.RA3)  
		&&  (!PORTAbits.RA2)
		&&  (PORTBbits.RB3)
		&&  (!PORTBbits.RB2) )		
	{	// 5 pin family
		//HLH is the 3DO with additional controller connected.
		if (	(PORTEbits.RE2)
			&&	(!PORTEbits.RE1)
			&&	(PORTEbits.RE0) )
		{ //yep, its a 3do
			Program_Load_Mappings(SMA_3DO);
			THREEDO_main();
		}
	
		if (	(PORTEbits.RE2)
			&&	(!PORTEbits.RE1)
			&&	(!PORTEbits.RE0) )
		{ //its a Wii
			Program_Load_Mappings(SMA_WII);
			WII_main();
		}		
		else
		{ //default to digital PSX
			Program_Load_Mappings(SMA_PSX);
			PSX_DIG_main();
		}
	}	
		
	/* Family LLHH is the 3 pin select, including all USB. */
	if (	(!PORTAbits.RA3)  
		&&  (!PORTAbits.RA2)
		&&  (PORTBbits.RB3)
		&&  (PORTBbits.RB2) )		
	{	// 3 pin family. Pins 2/RE2 3/RE1 4/RE0 5/RA5 15/RB1 specify which member.
		// L L L L L specifies a NES cable
		if (	(!PORTEbits.RE2)
			&&	(!PORTEbits.RE1)
			&&	(!PORTEbits.RE0)
			&&	(!PORTAbits.RA5)
			&&	(!PORTBbits.RB1) )
		{  
			Program_Load_Mappings(SMA_NES);
			NES_main();
		}
		
		// L L L L H specifies a SNES cable
		if (	(!PORTEbits.RE2)
			&&	(!PORTEbits.RE1)
			&&	(!PORTEbits.RE0)
			&&	(!PORTAbits.RA5)
			&&	(PORTBbits.RB1) )
		{  
			Program_Load_Mappings(SMA_SNES);
			SNES_main();
		}
		
		// L L L H L specifies a 3do with no additional connector
		if (	(!PORTEbits.RE2)
			&&	(!PORTEbits.RE1)
			&&	(!PORTEbits.RE0)
			&&	(PORTAbits.RA5)
			&&	(!PORTBbits.RB1) )
		{  
			Program_Load_Mappings(SMA_3DO);
			THREEDO_main();
		}
		
		// L L L H H specifies a GameCube connector
		if (	(!PORTEbits.RE2)
			&&	(!PORTEbits.RE1)
			&&	(!PORTEbits.RE0)
			&&	(PORTAbits.RA5)
			&&	(PORTBbits.RB1) )
		{  
			Program_Load_Mappings(SMA_GC);
			GC_main();
		}
		
		// L L H H specifies a Piggyback connector
		if (	(!PORTEbits.RE2)
			&&	(!PORTEbits.RE1)
			&&	(PORTEbits.RE0)
			&&	(PORTAbits.RA5))
		{  //its a piggyback. Pin RB1 determines which one
			//&&	(PORTBbits.RB1) 
			if (PORTBbits.RB1)
			{  // High is Xbox360 piggyback
				PB_main(PB_X360);
			}
			else
			{  //else its low and the Dreamcast piggyback
				PB_main(PB_DC);
			}				
		}
		
		// H X X X X specifies the USB subfamily.
		
		if (PORTEbits.RE2) 
		{  
			if ( (!PORTEbits.RE1)
			&&	(!PORTEbits.RE0)
			&&	(!PORTAbits.RA5)
			&&	(!PORTBbits.RB1))  
			{
				//Button select cable
				//EDIT: PBXbox360 included in the USB subfamily now. If the button select USB cable 
				//is used, we want a button select option for 360 PB.
				if ( !Real_Stick_Roundhouse && !Real_Stick_Fierce )
				{ //xbox360 piggyback mode
					PB_main(PB_X360);
				}
				if ( !Real_Stick_Jab && !Real_Stick_Short )
				{
					Program_Load_Mappings(SMA_XBOX);
					XBOXUSB_main();	
				}

				{ //default
					mLED_2_On();
					Program_Load_Mappings(SMA_USB);
					PS3USB_main();
				}
			}
			if ( (!PORTEbits.RE1)
			&&	(!PORTEbits.RE0)
			&&	(PORTAbits.RA5)
			&&	(PORTBbits.RB1))  
			{ //LLHH, Xbox cable.
				Program_Load_Mappings(SMA_XBOX);
				XBOXUSB_main();	
			}
				//not a button select or Xbox cable. Use default USB for now.
				mLED_2_On();
				Program_Load_Mappings(SMA_USB);
				PS3USB_main();
		}

	}
	
	// if we get here to the end, we haven't matched any of the supported controllers
	// so sit and spin and wait for a replug
	mInitAllLEDs();
	mLED_1_On();
	mLED_2_Off();
	while(1)
	{
		Delay10KTCYx(250);
		Delay10KTCYx(250);
		Delay10KTCYx(250);
		Delay10KTCYx(250);
		Delay10KTCYx(200); // at 12 MIPS, 1.2mil cycles augh to = 1 sec
		mLED_1_Toggle();
		mLED_2_Toggle();			
	};
}//end main

//configulation‚ð’Ç‰Á‚µ‚Ü‚µ‚½***********************************************
/* This sets the PLL Postscaler to divide the primary osc input by 5
providing 4MHz to the 96MHz PLL. The 96MHz PLL input must always be 4MHz
as shown in the data sheet.*/
#pragma config PLLDIV = 5 

/* CPUDIV=OSC1_PLL2 ' This sets the PLL Postscaler to divide the 96MHz PLL
output by 2 providing a 48MHz clock to the CPU. */
#pragma config CPUDIV = OSC1_PLL2 

/* USBDIV=2 ' This divides the 96MHz PLL output by 2 providing 48MHz to the
USB core. */
#pragma config USBDIV = 2 

/* FOSC=HSPLL_HS ' This enables HSPLL for high-speed crystal/resonator. */
#pragma config FOSC = HSPLL_HS 
/* #pragma config FCMEM = OFF  */
#pragma config IESO = OFF 

#pragma config PWRT = OFF 
#pragma config BOR = ON 
/* #pragma config BORV = 28 */
#pragma config VREGEN = ON 

#pragma config WDT = OFF 
#pragma config WDTPS = 1 

#pragma config MCLRE = ON 
#pragma config LPT1OSC = OFF 
#pragma config PBADEN = OFF 
#pragma config CCP2MX = OFF

#pragma config STVREN = OFF 
#pragma config LVP = OFF 
#pragma config ICPRT = OFF 
#pragma config XINST = OFF 
//#pragma config DEBUG = OFF 

#pragma config CP0 = OFF 
#pragma config CPB = OFF 
#pragma config WRT0 = OFF 
#pragma config WRTB = OFF 
#pragma config EBTR0 = OFF 
#pragma config EBTRB = OFF

/** EOF main.c ***************************************************************/
