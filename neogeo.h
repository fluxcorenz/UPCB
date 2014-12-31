#ifndef NEOGEO_H
#define NEOGEO_H
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
/* UPCB module for Neogeo support
From GameSX.com:
Pin	Description	-	Pin	Description
1	Common (GND)-	9	D Button
2	NC			-	10	NC
3	Select		-	11	Start
4	D Button	-	12	C Button
5	B Button	-	13	A Button
6	Right		-	14	Left
7	Down		-	15	Up
8	+5v	-		

This is the commonly accepted pinout for the Neogeo, but Razoola has 
made it pretty clear that this isn't truly the case. The Neo-Geo uses
a banking system similar to the Saturn and Genesis. The format is 
actually like this:
Pin	Description		-	Pin	Description
1	Common (GND)	-	9	Bank Select 2
2	Bank Select 1	-	10	Bank Select 3
3	Select			-	11	Start
4	D Button		-	12	C Button
5	B Button		-	13	A Button
6	Right			-	14	Left
7	Down			-	15	Up
8	+5v	-			-	-

The game in the Neogeo controls which bank is checked. Each one of the
pins listed as an A-D button or direction are actually the 8 data bit 
lines. Since the far majority of the games don't require anything more 
than the 4 directions, 4 buttons, Start and Select, they are rarely 
used but exist when more exotic controls are needed, such as Mahjong 
controllers, Irritating Maze trackball, dual sticks for Kizuna Encounter, 
etc. UPCB has no plans for supporting any of these inputs, so there is 
no need to detail them here. 

The banking is not used on the UPCB. The format used is a modified 
Neogeo pinout, that allows full use of the Neogeo system when directly
connected, and also supports the two additional buttons so the UPCB can
be used with project box setups and superguns. The pinout is like this:
Pin	Description				-	Pin	Description
1	Common (GND)			-	9	GND (For Neogeo system detection)
2	F Button (Roundhouse)	-	10	E Button (Fierce)
3	Select					-	11	Start
4	D Button (Forward)		-	12	C Button (Strong)
5	B Button (Short)		-	13	A Button (Jab)
6	Right					-	14	Left
7	Down					-	15	Up
8	+5v	-					-	-

For connecting to a NeoGeo system, hacking a cable is not required. A 
direct 15 pin extension cable will work perfectly. 

Because of the simplicity of the communication, many older systems are 
immediately supported by the UPCB using the NeoGeo module. 

Sega Mastersystem uses the same D-Sub 9 like almost every system in the 
80's used, so just wire it up to the matching NeoGeo pins, make sure 
pin 9 on the UPCB side is connected to GND, and you're done. Like So:
(Sega SG-1000/Mk3/Mastersystem pinout from GameSX.com)
1 	Up
2 	Down
3 	Left
4 	Right
5 	+5v
6 	Button I
7 	NC
8 	Common
9 	Button II

So making a UPCB cable for the Mastersystem is easy:
D-Sub 15 Pin			Mastersystem Pin
1 (GND)					8 (GND)
4 (Forward)				9 (Button 2)
5 (Short)				6 (Button 1)
6 (Right)				4 (Right)
7 (Down)				2 (Down)
8 (VCC)					5 (VCC)
9 Low (tied to ground)
14 (Left)				3 (Left)
15 (Up)					1 (Up)

Short and Forward as used as Button 1 and Button 2 because they are 
the most comfortable to use in a two button situation. You can of course
swap them with any button of you choice. 

The MSX and X68000 (two button) both use the same pinout, which is 
slightly different from the Mastersystem above. IF YOU INTEND ON 
MAKING A 9 PIN CABLE FOR ALL OF YOUR ANCIENT SYSTEMS, USE THE PINOUT
ABOVE. ONLY USE THIS PINOUT IF YOU'RE SO HARD CORE YOU OWN A 
MSX, X68000 or FM-Towns! 
(data from GameSX.com)
Pin	Function
1	Up
2	Down
3	Left
4	Right
5	+5v
6	Button I
7	Button II
8	Out (?)  //I'll ignore this until I get clarification WTF it is
9	Ground


D-Sub 15 Pin			MSX/X68000 Pin
1 (GND)					9 (GND)
4 (Forward)				7 (Button 2)
5 (Short)				6 (Button 1)
6 (Right)				4 (Right)
7 (Down)				2 (Down)
8 (VCC)					5 (VCC)
9 Low (tied to ground)
14 (Left)				3 (Left)
15 (Up)					1 (Up)

The additional 2 buttons on the FM-Towns 
controller require a pair of diodes each, like this:

D-Sub 15 Pin			FM-Towns Pin
1 (GND)					9 (GND)
4 (Forward)				7 (Button 2)
5 (Short)				6 (Button 1)
6 (Right)				4 (Right)
7 (Down)				2 (Down)
8 (VCC)					5 (VCC)
9 Low (tied to ground)
14 (Left)				3 (Left)
15 (Up)					1 (Up)
11 (Start) ---|<|---    2 (Down)
11 (Start) ---|<|---    1 (Up)
3 (Select) ---|<|---    4 (Right)
3 (Select) ---|<|---    3 (Left)

The information and pinouts available here are from
usually reliable internet sources like GameSX.com, 
however they are UNTESTED. Please use with caution and
always check with a multimeter. The only one tested by
the auther is the NeoGeo, tested on a consolized MVS.

*/
#define Con_Neo_A		LATCbits.LATC6
#define Con_Neo_B		LATAbits.LATA5
#define Con_Neo_C		LATBbits.LATB0
#define Con_Neo_D		LATEbits.LATE0
#define Con_Neo_E		LATBbits.LATB3
#define Con_Neo_F		LATEbits.LATE2
#define Con_Neo_Start	LATBbits.LATB2
#define Con_Neo_Select	LATEbits.LATE1
#define Con_Neo_Up		LATBbits.LATB1
#define Con_Neo_Down	LATAbits.LATA2
#define Con_Neo_Left	LATCbits.LATC7
#define Con_Neo_Right	LATAbits.LATA3

/* Prototypes */
void NEOGEO_main(void);

#endif //NEOGEO_H
