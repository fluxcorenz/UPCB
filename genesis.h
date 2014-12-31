#ifndef GENESIS_H
#define GENESIS_H
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
/* UPCB module for Sega Genesis Support *UNTESTED*
Genesis uses a standard D-Sub 9 type connection with the following pinout
(From GameSX.com)
Pin  	Description
1 	Up
2 	Down
3 	Chip 4
4 	Chip 7
5 	+5v
6 	Chip 9
7 	Chip 1
8 	Ground
9 	Chip 12

Here's the way I'm treating it:
Pin  	Description
1 	Up
2 	Down
3 	Data 1 (Tied Low | Left)
4 	Data 2 (Tied Low | Right)
5 	+5v
6 	Data 3 (A | B)
7 	Bank Select
8 	Ground
9 	Data 4 (Start | C)

The six button Genesis controllers works the same way, but adds on a third 
select state based on the number of times the select line is toggled. 
Good information on this is available from:
http://www.cs.cmu.edu/~chuck/infopg/segasix.txt
http://cgfm2.emuviews.com/txt/gen-hw.txt

Because the six button controller included a Mode button that could enable
or disable the extra button support, I'll be doing something similar. The 
Genesis module will default to the 3 button controller, and the Stick_Select
button will toggle it in and out of six button mode. For right now, six button
Genesis support is not included. It will require using the PIC's timer functions, 
which I am not familiar with yet.

To make a UPCB cable for the Genesis:
D-Sub 15 Pin		Genesis Pin
1					8 (GND)
2					High
3					3 (Data 1)
4					4 (Data 2)
5					6 (Data 3)
6					Low
7					Low
8					5 (+5v)
9					NC - Not connected to anything
10					Low
11					Low
12					9 (Data 4)
13					7 (Bank Select)
14					1 (Up)
15					2 (Down)



*/
#define GENESIS_BANKSEL PORTCbits.RC6
#define GENESIS_DATA_1  LATEbits.LATE1
#define GENESIS_DATA_2	LATEbits.LATE0
#define GENESIS_DATA_3	LATAbits.LATA5
#define GENESIS_DATA_4	LATBbits.LATB0
#define GENESIS_UP		LATCbits.LATC7
#define GENESIS_DOWN	LATBbits.LATB1

/* Prototypes */
void GENESIS_main(void);
void GENESIS_Init_Pins();




#endif //GENESIS_H
