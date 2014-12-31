
#ifndef X68000_H
#define X68000_H
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

/*  UPCB module for Six Button X68000 support. 
	***** FOR TWO BUTTON X68000 SUPPORT, READ NEOGEO.H ******
	Information about how the six button X68000 controller
	works from:
	http://gamesx.com/wiki/doku.php?id=controls:x686button
	
	The X68000 uses a female DB-9 connector. Based on the 
	schematic linked above, it uses a single bank select
	with 6 data lines. 
					   	  Data1 Data2 Data3 Data4 Data5 Data6
	Bank 1 (Pin 8 low A)  Up    Down  Left  Right  L    Start
	Bank 2 (Pin 8 high B) R     Y     X     Select A    B
	
	So the X68000 six button pinout is like this:
	Pin				Description
	1				Data 1
	2 				Data 2
	3				Data 3
	4				Data 4
	5				VCC
	6				Data 5
	7				Data 6
	8				Bank Select
	9				GND
	
	To make a UPCB cable for the six button X68000:
	UPCB D-Sub 15 pin			X68000 D-Sub 9 pin
	1							9 (GND)
	2							Low
	3							1 (Data 1)
	4							2 (Data 2)
	5							3 (Data 3)
	6							Low
	7							Low
	8							5 (VCC)
	9							NC (Not connected to anything)
	10							Low
	11							Low
	12							4 (Data 4)
	13							6 (Data 5)
	14							7 (Data 6)
	15							8 (Bank Select)
	
	The layout of the pad is unknown to me, so this is a
	best guess. Feel free to contact me if the buttons
	should be arranged differently. GameSX also says that
	the pinout might be slightly of, but it should be 
	obvious how it should be when used. If any corrections
	are needed, please get a hold of me. Until I hear 
	otherwise, I'm assuming a six button layout like this:
	   X Y L
	   A B R
*/

#define X68000_DATA1 LATEbits.LATE1
#define X68000_DATA2 LATEbits.LATE0
#define X68000_DATA3 LATAbits.LATA5
#define X68000_DATA4 LATBbits.LATB0
#define X68000_DATA5 LATCbits.LATC6
#define X68000_DATA6 LATCbits.LATC7
#define X68000_BANKSEL  PORTBbits.RB1


/* Prototypes */
void X68000_Init_Pins(void);
void X68000_main(void);

#endif //X68000_H
