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
#include <p18cxxx.h>
#include "3DO.h"
#include "upcb.h"


void THREEDO_Init_Pins(void)
{
	TRISAbits.TRISA5=INPUT; //#define THREEDO_DATAIN 	PORTAbits.RA5
	TRISCbits.TRISC7=INPUT; //#define THREEDO_CLOCK	PORTCbits.RC7
	TRISCbits.TRISC6=INPUT; //#define THREEDO_LATCH	PORTCbits.RC6
	TRISBbits.TRISB0=OUTPUT; //#define THREEDO_DATAOUT	LATBbits.LATB0	
}

void THREEDO_main(void)
{
	unsigned char loopcount; 
	//Trying the buffer using an unsigned int; should be 16 bit 
	
	unsigned int buffer;
	/* to make things easier in my head, the buffer will be a queue traveling
		left. The MSB is the bit to send next, and the LSB is the bit we last 
		received from the downstream controller. So we need to set the buffer
		as the 	bitwise reverse of the description, like this.
			bit7	bit6	bit5	bit4	bit3	bit2	bit1	bit0
	Highbyte 0		0		Down	Up		Right	Left	A		B
	Lowbyte	 C		P		X		R		L		0		0		1 */
	

	
	
	THREEDO_Init_Pins();
	while(1)
	{
		THREEDO_DATAOUT=1; //data should be high while waiting for Latch to drop
		loopcount=0;
		buffer=0b0000000000000001;  //sets the first 2 and last 3 bits to the
		 //hard coded values the 3DO expects. Also clears all controller bits.
		/* let's build the buffer for certain first, and then check each bit in turn
			while waiting for our turn to talk */
		
		if (!Stick_Down) 		buffer |= 0b0010000000000000;
		if (!Stick_Up) 			buffer |= 0b0001000000000000;
		if (!Stick_Right) 		buffer |= 0b0000100000000000;				
		if (!Stick_Left) 		buffer |= 0b0000010000000000;				
		if (!Stick_Short)		buffer |= 0b0000001000000000;
		if (!Stick_Forward)		buffer |= 0b0000000100000000;
		if (!Stick_Roundhouse)	buffer |= 0b0000000010000000;
		if (!Stick_Strong)		buffer |= 0b0000000001000000;
		if (!Stick_Start)		buffer |= 0b0000000000100000;
		if (!Stick_Fierce)		buffer |= 0b0000000000010000;
		if (!Stick_Jab)			buffer |= 0b0000000000001000;
		
		do
		{ 
			/* in order to make the controller response as snappy as possible, we 
			   want to continuously poll it while waiting for our the latch to 
			   drop. The problem is that we want to avoid bad timings. The group
			   of instructions that entirely build up the buffer above can take 
			   some microseconds to run. We only have 4us between the rise and
			   fall of the clock. So, instead we are going to fully populate the
			   buffer once, in the very instant the latch goes high from reading
			   the controller data, and we know we have plenty of time. Then, 
			   while waiting for the latch to drop again, we'll check a single bit,
			   then check the latch line, check another bit, check the latch line,
			   repeat. In seems convoluted, but it makes sure we see the dropped 
			   latch and have the first bit out in <1us, 3us before the 3DO reads 
			   the line. */
			loopcount++; //increment loop counter
			switch(loopcount) 
			{
				case 1: 	if (!Stick_Down) 		buffer |= 0b0010000000000000;
							else buffer &= 0b1101111111111111;
							break;
				case 2: 	if (!Stick_Up) 			buffer |= 0b0001000000000000;
							else buffer &= 0b1110111111111111;
							break;
				case 3: 	if (!Stick_Right) 		buffer |= 0b0000100000000000;
							else buffer &= 0b1111011111111111;
							break;
				case 4: 	if (!Stick_Left) 		buffer |= 0b0000010000000000;
							else buffer &= 0b1111101111111111;
							break;
				case 5: 	if (!Stick_Short) 		buffer |= 0b0000001000000000;
							else buffer &= 0b1111110111111111;
							break;	
				case 6: 	if (!Stick_Forward) 	buffer |= 0b0000000100000000;
							else buffer &= 0b1111111011111111;
							break;		
				case 7: 	if (!Stick_Roundhouse) 	buffer |= 0b0000000010000000;
							else buffer &= 0b1111111101111111;
							break;			
				case 8: 	if (!Stick_Strong) 		buffer |= 0b0000000001000000;
							else buffer &= 0b1111111110111111;
							break;	
				case 9: 	if (!Stick_Start) 		buffer |= 0b0000000000100000;
							else buffer &= 0b1111111111011111;
							break;		
				case 10: 	if (!Stick_Fierce) 		buffer |= 0b0000000000010000;
							else buffer &= 0b1111111111101111;
							break;	
				case 11: 	if (!Stick_Jab) 		buffer |= 0b0000000000001000;
							else buffer &= 0b1111111111110111;
							loopcount=0; // this is the last one, so reset the counter
							break;		
				default: loopcount=0; // in case I made an error in counting, start over
			}							
		} while (THREEDO_LATCH); //and repeat until latch drops
		//yeah, latch dropped. Start the cycle. 
		
		while (!THREEDO_LATCH) //while latch is low
		{
			/* the idea here is rather simple. We have a 16 bit buffer we've already
			filled with the controller status. Each bit, we pop off the top bit, send
			it down the line, shift everything over, and at the bit read to the end,
			and repeat. If there is a controller downstream, we'll be receiving their 
			information and sending it out as well. If there is one downstream of THAT
			controller, we'll be getting that too, for as long as the latch is down. 
			Whenever we reach the end of the controllers, whether there is 0 or 20, the
			first bits of their data will be high, which will tell the console there is 
			no more so it can raise the latch. If there are no downstream controllers, 
			we will force the bits read to high with the pull up resistor (for UPCB 
			cables made with the additional controller connector) or by reading the input
			we tied to high in the system select lines (UPCB cable with no additional 
			connector). */
			//send out the MSB of byteone
			THREEDO_DATAOUT=(buffer & 0b1000000000000000);
			//shuffle the bits left by one
			buffer = buffer << 1;
			while(!THREEDO_CLOCK); //and wait for the clock to rise. 
			
			if (THREEDO_DATAIN)
			{
				buffer |= 0x0001; // line is high, so make the LSB high
			}
			else
			{
				buffer &= 0xFFFE; // line is low, so make the LSB low
			}
			//process complete. wait for clock to fall so we can repeat
			while(THREEDO_CLOCK);
		}
	}
}

