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
/* UPCB module for TurboGrafx-16/PCEngine support *UNTESTED*
   
   
   	And here it is with the pin numbering from GameSX.com:
   	  DIN-8 SOCKET  (TG)            Mini-DIN-8 PLUG  (Duo)

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
   	
   	
   The actual plugs may differ depending on whether you're using a 
   TG-16, a Duo, or a PC-Engine. However the numbered pins all have
   the same function, so when building a UPCB cable, identify which
   of the plugs you're using, and followed the numbers pinout from 
   above. 
   
   None of the pinouts available on the web indicate which pin is VCC 
   and which pin is GND. I did find close up pictures of the Avenue6 
   controller:
   http://wiki.pcengine.info/uploads/Hardware/ap6_pcb_1.jpg
   http://wiki.pcengine.info/uploads/Hardware/ap6_pcb_2.jpg
   The cable connection is labeled 1-9 (9 being the DIN hood) and the
   rest of the pins map out on the circuit as expected in the GameSX
   documents. According to it, pin 1 is VCC, and pin 8 (and the hood/
   shielding) are GND. 
   
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
   but know I mean the PCEngine and Duo as well.) uses a single bank 
   select pin, a single Enable pin, and 4 data pins. 
   If Enable is High, all outputs are Low, no matter what the Select is
   If Enable is Low and select is Low the output pins match the A bank
    A bank consists of 'I', 'II', Select and Start
   If Enable is Low and Select is High, the output matches the B bank
   	B bank consists of Up, Right, Down, Left.
   
   The Enable pin is pulled low to check the status at once per frame,
   and also increments a counter used for the turbo function, to pulse
   the button at every 4th and 8th frame. 
   
   On the six button Avenue6 controller, the enable pin is also used to
   check an additional two banks by alternatiing between then two each
   time enable is dropped. It is unknown if they games supporting 6 
   buttons dropped the enable line twice per frame, or if each set of 
   inputs were polled every other frame. 
   Secondary bank A consists of 'III', 'IV', 'V', and 'VI'
   Secondary bank B should be all low; Since it isnt physically possible
   to press all four directions on a pad, this was used to identify that
   a 6 button pad was connected. 
   
   The current way of playing the UPCB on TG-16 is as follows:
   2 button mode:
   		The default starting mode when connected to a TG-16. 
   		Short is button 'II'
   		Forward is button 'I'
   		Jab will toggle the autofire on button 'II'
   		Strong will toggle the autofire on button 'I'
   		Both autofire's will default to off, and cycle between every 4th frame,
   			every 8th frame, and back to off. 
   		Pressing Select + Start + Fierce + Roundhouse will enable 6 button mode
   	6 button mode:
   		No autofire available in 6 button mode. 
   		Roundhouse will be button 'III'
   		Jab Strong Fierce will be buttons 'IV', 'V', and 'VI' respectively.
   		Pressing Select + Start + Fierce + Roundhouse will toggle back to 2 button mode
   		
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

#define TG16_ENABLE			PORTEbits.RE0
#define TG16_BANKSEL		PORTAbits.RA5
#define TG16_DATA1			LATBbits.LATB0
#define TG16_DATA2			LATCbits.LATC6
#define TG16_DATA3			LATCbits.LATC7
#define TG16_DATA4			LATBbits.LATB1

/* Prototypes */
void TG16_main(void);
void TG16_Init_Pins(void);

#endif //TG16_H