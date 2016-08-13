#ifndef TG16_H
#define TG16_H
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

/* UPCB module for TurboGrafx-16/PC Engine support
   
     DIN-8 SOCKET  (TG)            Mini-DIN-8 PLUG  (PCE)

      -------------                     ----##----
     /      2      \                   /    ##    \
    /  5         4  \                 /  6   7   8 \
   /                 \               |              |
  |                   |              | 3   4     5  |
  |  3      8      1  |               \            /
  |                   |                \## 1  2 ##/
   \                 /                   #------#
    \  7         6  /
     \     ###     /
      -----###-----
   	
   	
   TG-16 pinout
   1	VCC
   2	Data 1
   3	Data 2
   4	Data 3
   5	Data 4
   6	Bank Select
   7	Enable
   8	GND
   
   The TG-16 (and all members of the family. I'll write TG-16 for short, 
   but know I mean the PC Engine and Duo as well.) uses a single bank 
   select pin, a single Enable pin, and 4 data pins. 

   If Enable is High, all outputs are Low, no matter what the Select is
   If Enable is Low and select is Low the output pins match the A bank
   	A bank consists of 'I', 'II', Select and Start
   If Enable is Low and Select is High, the output matches the B bank
   	B bank consists of Up, Right, Down, Left.
   
   The Enable pin is pulled low to check the status at once per frame,
   and also increments a counter used for the turbo function.

   After the enable line is pulled low, the select pin is already high,
   to poll for the stick directions.
   Then the select line is pulled low, to poll for the buttons.

   The select line is toggled high->low, then low->high, 5 times each
   time enable is pulled low. This is to support up to 5 controllers
   (via a multitap), or possibly games with more complex controllers.

   A multitap steps through to the next controller port after a Select
   cycle (H->L, L->H), and thus enables each controller in turn, while
   disabling the others.

   On two-button controllers with turbo switches, the turbo modes are:
   	Off: button presses are always obeyed
	I: on a 4-frame on, 4-frame off cycle (slow)
	II: on a 2-frame on, 2-frame off cycle (fast)

   On the six button Avenue6 controller, the enable pin is also used to
   check an additional two banks by alternating between the banks each
   time enable is dropped.  Rather than the usual 5 cycles of the select
   line, SF2 (and presumably other 6 button games) only send 3 cycles, 
   and then reset the enable pin, so it doesn't lose resolution by 
   having to poll twice.

   Secondary bank A consists of 'III', 'IV', 'V', and 'VI'
   Secondary bank B should be all low; Since it isnt physically possible
   to press all four directions on a pad, this was used to identify that
   a 6 button pad was connected. 
   
   The current way of playing the UPCB on TG-16 is as follows:

	2 button mode:
   		The default mode when connected to a TG-16. 
   		Short is button 'II'
   		Forward is button 'I'
   		Jab is button 'II' at turbo II (hold it down) 
   		Strong is button 'I' at turbo II (hold it down)
   	6 button mode:
		Hold Select when connecting to a TG-16.
   		No autofire/turbo available in 6 button mode. 
   		Roundhouse will be button 'III'
   		Jab Strong Fierce will be buttons 'IV', 'V', and 'VI'
	      	 	respectively.

	No button remapping (via programming button on UPCB) is
	currently supported, due to protocol speed reasons.
   		
   To make a UPCB cable for TG-16 and family, wire it up as so:
   	D-Sub 15				TG-16 Pin
   	1						8 (GND)
   	2						High
   	3						High
   	4						7 (Enable)
   	5						6 (Bank Select)
   	6						Low
   	7						Low
   	8						1 (VCC)
   	9						NC - Not connected to anything
   	10						Low
   	11						High
   	12						2 (Data 1)
   	13						3 (Data 2)
   	14						4 (Data 3)
   	15						5 (Data 4)
*/

#define TG16_ENABLE		PORTEbits.RE0
#define TG16_BANKSEL		PORTAbits.RA5
#define TG16_D1			LATBbits.LATB0
#define TG16_D2			LATCbits.LATC6
#define TG16_D3			LATCbits.LATC7
#define TG16_D4			LATBbits.LATB1

/* Prototypes */
void TG16_main(void);
void TG16_Init_Pins(void);

#endif //TG16_H
