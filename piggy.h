#ifndef PIGGY_H
#define PIGGY_H

#include "config.h"
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
/* UPCB module for Piggyback connections. 
 The UPCB uses a single 4066N analog switch IC to allow two additional
 game controllers to be connected. When the output cable is checked, if
 the system select pins indicate one of the two piggybacked controllers
 is to be used, the analog switch is enabled to connect the two data lines
 from the connected controller to two of the output lines on the UPCB 
 console cable. The UPCB just acts as a pass-through at that point. 
 Because 'tournament mode' is an option that people may want with piggy-
 backed controllers, the RB0 line is used on the piggyback connection 
 instead of directly connecting Start like every other button and direction. 
 
 The two connections are labelled DC_AUX and X360_AUX because it is
 believed that the Dreamcast and Xbox360 will be the most common 
 controllers used in this fashion. However, it can be any controller
 that can accept common ground inputs (or modified to do so). Just to 
 make directions easier and lower confusion, we'll be calling them DC_AUX
 and X360_AUX, even though they are NOT limited to those options. 
 
 Enabling the Dreamcast requires line RB7 to be High, and connects the 
 line DC_1 to RC4&6, and DC_2 to RC5&7. Enabling the Xbox360 connector
 requires RB6 to be high, and connect X360_1 and X360_2 to RC4&6 and 
 RC5&7 respectively. ONE and only ONE of these two lines should be high;
 making both of them high will connect both pair of output lines together
 and cause no end of confusion pain and suffering. When not in Piggyback
 mode, both of these lines should be low. 
 
 DREAMCAST 
 
 Looking at controller end that plugs into the Dreamcast
 ___________
 | 5  3  1 |
 \  4   2  /
  ---------
  
  Pin 	Description		Standard Wire Color
  1		Serial A 		red
  2		+5v				blue
  3		GND				black
  4		sense			green
  5		Serial B		white
  
  Line 4, 'sense', is connected to ground in the controller. The Dreamcast
  uses this line to detect when there is a controller in place. Since we
  can connect it to ground inside the UPCB console cable hood, there is no
  need to connect this to the UPCB itself. 
  
 	To make a UPCB cable for the Dreamcast Piggyback, match up pins like this:
	D-Sub 15 Pin		GC Pin
	1					3 (GND) 
	2					Low
	3					Low
	4					High
	5					High
	6					Low
	7					Low
	8					2 (+5v)
	9					NC - Not connected to anything
	10					High
	11					High
	12					NC - Not connected to anything
	13					1 (Serial A)
	14					5 (Serial B)
	15					Low 
	You'll want to also solder in the 'sense' line to any one of the 'Low' lines.
	I soldered mine into 15 along with the little jumper wire that connected it
	to the rest of the 'Low' lines.
 
 XBox360
 
 Looking at the USB end that plugs into the Xbox360
 	___________
	| 4 3 2 1 |
	| _ _ _ _ |
	-----------    <- thicker half of cable where the contacts are.
	
	Pin		Description
	1		VCC
	2		D-	
	3		D+
	4		GND
	
	To make a UPCB cable for the Xbox360 Piggyback, match up pins like this:
	D-Sub 15 Pin		GC Pin
	1					4 (GND)
	2					Low
	3					Low
	4					High
	5					High
	6					Low
	7					Low
	8					1 (VCC)
	9					NC - Not connected to anything
	10					High
	11					High
	12					NC - Not connected to anything
	13					2 (D-)
	14					3 (D+)
	15					High

	NOTE: Xbox360 controllers almost never use a common ground setup for their
	controls. This means that setting up a piggyback xbox360 controller requires
	a good amount of work in order to convert it to a common ground style the 
	UPCB requires. 
*/

/* Macros */

/* We dont want to use any of these functions if we are in DEBUG mode, set in 
	config.h. */
	

/* Init_Piggyback() sets the piggyback control pins RB6 and RB7 to output, and
	sets them low so the piggyback switches are NOT on. */
#ifdef DEBUG
	#define Init_Piggyback() {}
#else	
	#define Init_Piggyback()	TRISBbits.TRISB7=OUTPUT;TRISBbits.TRISB6=OUTPUT; \
								LATBbits.LATB7=0;LATBbits.LATB6=0;
#endif

#define PB_START 		LATBbits.LATB0
#define PB_GUIDE 		LATBbits.LATB4
#define PB_X360 		1  //for passing argument to PB_main from main()
#define PB_DC			2 //for passing argument to PB_main from main()
/* Prototypes */
	void PB_Init();
	void PB_main(unsigned char select);

	
	
#endif //PIGGY_H