#ifndef PSX_H
#define PSX_H
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
/* UPCB module for Playstation and Playstation 2 support
  From GameSX.com
  
              Looking at the plug
       -------------------------------
PIN 1- | o  o  o | o  o  o | o  o  o |
       \_____________________________/
       
        1	DATA	To PSX	 	This signal is an 8 bit serial transmission synchronous 
        						to the falling edge of clock (That is both the incoming 
        						and outgoing signals change on a high to low transition 
        						of clock. All the reading of signals is done on the 
        						leading edge to allow settling time.)
		2	COMMAND	From PSX	This signal is the counterpart of DATA. It is again an 
								8 bit serial transmission on the falling edge of clock.
		3	VCC2	From PSX	7 to 9V. Supplies power through fuse for memory cards/
								rumble motors. Drawing excess power will rupture the 
								surface mount fuse))  (Not used in the UPCB)
		4	GND		N/A			Ground
		5	VCC 	(3.4V)		Main power. Supplies power through a fuse for memory 
								cards/controllers. Drawing excess power will rupture the 
								surface mount fuse, disconnecting the power from all 
								controller and memory card sockets!
		6	ATT		From PSX	ATT is used to get the attention of the controller. 
								This signal will go low for the duration of a 
								transmission. This pin has also been called called 
								Select, DTR and Command.
		7	CLK		From PSX	Clock signal to keep PSX and controller in sync with 
								each other
		8	N/C		N/A			Not connected
		9	ACK		To PSX		This signal should go low for at least one clock period 
								after each 8 bits are sent and ATT is still held low. 
								If the ACK signal does not go low within about 60 us the 
								PSX will then start interogating other devices.
								
	As of this writing, only the original digital PSX pad is simulated. Eventually the 
	Dual Shock 1 and Dual Shock 2 will be supported. When they are, you can create the
	cable to tell the UPCB which controller to emulate, but there isn't much of a need.
	The pinout described here is for a 'button select' playstation cable. When the 
	other pads are supported, you can choose which one to simulate by holding down a 
	key combination, or letting it default to Digital. The button select version is the
	most recommended choice.
	
	To create a button select Playstion cable for UPCB
	D-Sub Pin 			PSX pin
	1					4 (GND)
	2					Low
	3					Low
	4					Low
	5					6 (ATT)
	6					Low
	7					Low
	8					5 (VCC)
	9					NC - Not connected to anything
	10					High
	11					Low
	12					2 (CMD)
	13					9 (ACK)
	14					1 (DATA)
	15					7 (CLK)

	To create a dedicated Digital Playstion cable for UPCB
	D-Sub Pin 			PSX pin
	1					4 (GND)
	2					Low
	3					Low
	4					High
	5					6 (ATT)
	6					Low
	7					Low
	8					5 (VCC)
	9					NC - Not connected to anything
	10					High
	11					Low
	12					2 (CMD)
	13					9 (ACK)
	14					1 (DATA)
	15					7 (CLK)
	
	To create a dedicated Dual Shock 1 Playstion cable for UPCB (once supported)
	D-Sub Pin 			PSX pin
	1					4 (GND)
	2					Low
	3					High
	4					Low
	5					6 (ATT)
	6					Low
	7					Low
	8					5 (VCC)
	9					NC - Not connected to anything
	10					High
	11					Low
	12					2 (CMD)
	13					9 (ACK)
	14					1 (DATA)
	15					7 (CLK)
	
	To create a dedicated Dual Shock 2 Playstion cable for UPCB (once supported)
	D-Sub Pin 			PSX pin
	1					4 (GND)
	2					Low
	3					High
	4					High
	5					6 (ATT)
	6					Low
	7					Low
	8					5 (VCC)
	9					NC - Not connected to anything
	10					High
	11					Low
	12					2 (CMD)
	13					9 (ACK)
	14					1 (DATA)
	15					7 (CLK)

*/

#define PSX_ATT PORTAbits.RA5

// Prototypes
void PSX_ACK(void);
void PSX_Init_Pins(void);
void PSX_DIG_main (void);

#endif //PSX_H