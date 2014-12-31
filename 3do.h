#ifndef THREEDO_H
#define THREEDO_H
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
/* UPCB module for 3DO support. 
All of the information on the 3DO protocol has come from one site
http://kaele.com/~kashima/games/3do.html

It's in Japanese, but google translates it pretty well. 
The 3DO uses a normal female DB-9 connector, but the way it 
communicates is different from almost every controller. Most 
systems have multiple ports on the front and reads from each
controller separately. The 3DO has only one controller port
on the front, and each subsequent controller is chained to 
the controller in front of it. Closer to the console, the 
lower the player number; the one directly connected to the
console is player 1. Even stranger is the fact that two of
the wires are used for stereo audio, with a headphone jack 
in each controller. 

The description and waveforms on the site above detail the 
communication pretty well. Each controller has a 16 bit 
parallel to shift function, almost identical to the ones
used by the NES and SNES. What makes this different is that 
the additional controller add on to the the one before it. 
The console notice to the controllers saying 'Start pushing
me the data' by dropping the P/S line. The controller starts
sending the data bit by bit in time with the clock line. Each 
time it sends a bit, it is also receiving a bit from the 
controller after it. After the first controller has sent all
16 bits for it's status, it has received the 16 bits from the 
controller after it. THAT second controller has received the 
16 bits from the third controller, the third controller has 
received the 16 bits from the fourth, and so on. I seem to 
remember that the game FIFA claimed you could field an entire
team of 11 players with 11 controllers all daisy chained.

So how does it know if there is even a controller next in line,
or even any controllers at all plugged in? I'm guessing the 
data line is an open collector (tied high at the console); 
if nothing is there, it's read as high. The first two bits of 
each 16 bit transmission is low. So, when the console checks 
the first two bits, if they are high, there are no more 
controllers connected, or no controllers connected at all if 
the very first two bits read are high.

The data sent, in left to right order, is as follows
(site linked above shows it reversed. The data is the same, 
but changed to left to right order to read easier.)
bit0	bit1	bit2	bit3	bit4	bit5	bit6	bit7
0		0		Down	Up		Right	Left	A		B
C		P		X		R		L		0		0		1

(Site linked showed R for both bit3 and bit4 of the second byte.
I changed bit4 to show L based on one of the schematics on his site:
http://kaele.com/~kashima/games/3do_tr.pdf ) This same schematic 
shows the output to DATA coming from a normal high or low OR gate;
Because this works, I wont have to imitate an open collector.

One last thing is that the bits are inverted from what I'm used
to. A bit is high if the button or direction is pressed, and low
if it is not. 

Bits change on the falling edge of clock and assumed to be read on 
the rising edge. Data line should be released to high when P/S is not
pulled low. 

3DO pinout:
Pin				Description
1				GND
2				VCC
3				Audio 1
4				Audio 2
5				VCC
6				P/S (Latch)
7				Clock
8				GND
9				Data

Constructing a UPCB cable is still easy, but slightly different 
from other consoles. For the first time, one of the pins will NOT
be going to the console; it'll be going to the male connector for
any additional controllers. You can skip having a male connector
along with the female cable, but then you'd always have to be 
player 2 in any versus games of Super Turbo. You can also have a 
headphone plug, but this is optional and doesn't affect play. 
Making a UPCB cable with the additional connector is recommended. 

UPCB cable for 3DO with additional controller connector
D-Sub 15			Female DB-9 to console			Male DB-9 to controllers	Headphone jack
1					1 (GND) & 8 (GND)				1 (GND) & 8 (GND)			Audio GND
2					High
3					Low
4					High
5													9 (Data) & Pull up resistor
6					Low
7					Low
8					2 (VCC) & 5 (VCC)				2 (VCC) & 5 (VCC)
9					NC (Not connected)				
10					High
11					Low
12					9 (Data)								
13					6 (Latch)						6 (Latch)
14					7 (Clock)						7 (Clock)
15					NC (Not connected)
					3 (Audio 1)						3 (Audio 1)					Audio Left
					4 (Audio 2)						4 (Audio 2)					Audio Right
					
-It is unknown which audio line is left, and which is right. Please let 
me know if I have this backwards.
-The Pull up resistor is required if using the controller connector. 
Just place a suitable(4.7K Ohm - 10K Ohm) resistor between the 
DB-15 UPCB pin 5, and one of the high pins such as 2, 4, 8 or 10. 4
is probably easiest.
- If you don't want to use the headphone jack, just ignore it. But it 
would be best to keep pins 3 and 4 connected between the two DB-9 
connectors, in case a player after you wanted to use headphones. 


UPCB cable for 3DO WITHOUT an additional controller connector. 
***Since it doesn't use the male DB-9 or the pull up resistor, this is 
easier to put together, but not recommended. Better to do it right 
than to do it twice, so use the table above unless you have a real
good reason not to. *************************************************
D-Sub 15			Female DB-9 to console		Headphone jack (optional)
1					1 (GND) & 8 (GND)			Audio ground
2					Low
3					Low
4					Low
5					High
6					Low
7					Low
8					2 (VCC) & 5 (VCC)
9					NC (Not connected)
10					High
11					High
12					9 (Data)
13					6 (Latch)
14					7 (Clock)
15					Low
					3 (Audio 1)					Audio Left
					4 (Audio 2)					Audio Right

Note: I'm doing a little bit of cheating here. In the system select pins
for the 3do solo, I'm making sure that pin 5 is High. Because the data in
line will always be high, I can use the same function for both 3do solo, 
and 3do with an additional connector. 

*/

#define THREEDO_DATAIN 	PORTAbits.RA5
#define THREEDO_CLOCK	PORTCbits.RC7
#define THREEDO_LATCH	PORTCbits.RC6
#define THREEDO_DATAOUT	LATBbits.LATB0

/* Prototypes */
void THREEDO_Init_Pins(void);
void THREEDO_main(void);

#endif //THREEDO_H