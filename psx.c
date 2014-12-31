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
#include "psx.h"
#include "upcb.h"
#include <spi.h>
#include <delays.h>

void PSX_DIG_main (void)
{
	unsigned char holder;
	PSX_Init_Pins();
	OpenSPI(SLV_SSON, MODE_10, SMPMID);
	mLED_1_Off();
	mLED_2_Off();
	while(1)
	{	
		FrameUpdate();
		while(PSX_ATT); //Sit and sping until the PSX decides it wants to talk to us. 
						//The ATT line should be low for the duration of the transmission
		// Here we are at first byte. Load up dummy data to go
		SSPBUF=0xFF; 
		while ((!SSPSTATbits.BF) && (!PSX_ATT)); 	// wait for a read cycle to complete
	
		if ((SSPBUF == 0x80) && (!PSX_ATT))  // 0x80 is 0x01 bit reverse; SPI uses MSB first, PSX used LSB first
		{					// so we have to reverse it for our comparisons
			//mLED_2_On();
			// sweet, so the first byte was received correctly. 
			SSPBUF=0x82; // Load the next byte to send into the buffer
						// 0x82 is the bit reverse of 0x41, the controller ID of the 
						//original PSX digital pad. 
			PSX_ACK(); //acknowledge
			
			while ((!SSPSTATbits.BF ) && (!PSX_ATT)); 	// wait for a read cycle to complete
			// now we've read and sent the second byte. If its 0x42, the status query command,
			//then start sending the data. Otherwise, its an unsupported command we ignore.
			if ((SSPBUF == 0x42) && (!PSX_ATT)) //0x42 is 0x42 reverse. Binary palindrome.
			{
				// correct command received so we're in sync. Time to send data.
				
				SSPBUF=0x5A; // sending the 'here comes the data byte' 0x5a, another palindrome
				PSX_ACK(); //acknowledge
				
				/* Now that the acknowledge is done, the PSX will start the clock again and shuffling
					the bits for the next byte transfer. Instead of going straight into waiting for it 
					to finish, let's use this time to construct the next byte we'll be sending. */
				holder=0xFF; //clear the byte out. 0 bits are pressed buttons, 
							// 1 bits are not pressed.
				/* Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7
     			   SLCT    1    1 STRT UP   RGHT DOWN LEFT  
     			   That's the layout for the PSX, But we'll be sending the byte backwards so:
     			   Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0       is how it really is
     			   SLCT    1    1 STRT UP   RGHT DOWN LEFT  		   */
     			if (!Stick_Select) holder &= 0b01111111;
     			if (!Stick_Start)  holder &= 0b11101111;
     			if (!Stick_Up)     holder &= 0b11110111;
     			if (!Stick_Right)  holder &= 0b11111011;
     			if (!Stick_Down)   holder &= 0b11111101;
     			if (!Stick_Left)   holder &= 0b11111110;
     			// all done building, wait for the third byte to finish
     			while ((!SSPSTATbits.BF) && (!PSX_ATT));
     			// all done, load the contructed byte into the buffer
     			SSPBUF=holder;
     			PSX_ACK(); // finished so acknowledge

     			/* again, ACK is down, and the first contructed byte is being sent. 
     				let's construct the final byte with the time we have
     				Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7  - PSX style
     				L2   R2    L1  R1   /\   O    X    |_|
     				Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0  - our MSB style */
     			holder=0xFF;
     			#ifdef EXTRA_BUTTONS
     				if(!Stick_Extra1)  holder &= 0b01111111; //map extra1 to L2
     				if(!Stick_Extra0)  holder &= 0b11011111; //map extra0 to L1
     			#endif
     			if (!Stick_Roundhouse) holder &= 0b10111111; // map RH to R2
     			if (!Stick_Fierce)     holder &= 0b11101111; // map fierce to R1
     			if (!Stick_Strong)     holder &= 0b11110111; //strong to tri
     			if (!Stick_Forward)    holder &= 0b11111011; //forward to O
     			if (!Stick_Short)      holder &= 0b11111101; //short to X
     			if (!Stick_Jab)        holder &= 0b11111110; //jab to square
     			// all done building, wait for the fourth byte to finish
     			while ((!SSPSTATbits.BF) && (!PSX_ATT));
     			// all done, load the contructed byte into the buffer
     			SSPBUF=holder;  //load fifth byte into buffer
     			PSX_ACK(); //ack receipt of fourth byte
     			
     			while ((!SSPSTATbits.BF) && (!PSX_ATT)); //wait for last byte to xfer
     			while(!PSX_ATT); //Let's sit and spin until the ATT line is released to high
 			}
 			else //command after the 0x01 was NOT 0x42
 			{
	 			//	mLED_1_On();
	 		}
		}
		else //first byte was NOT 0x01, or ATT line raised
		{
			if (PSX_ATT)
			{
				mLED_1_On();
			}
			else
			{
				mLED_2_On();
			}
		}
			
	}//while(1)
	
}//PSX_main()

void PSX_Init_Pins(void)
{
	TRISAbits.TRISA5=1; //SS line - input
	TRISCbits.TRISC7=0; //SDO - output
	TRISBbits.TRISB1=1; //Clock - input
   	TRISBbits.TRISB0=1; //SDI - input
   	TRISCbits.TRISC6=0; //Line we're using for Ack
}

void PSX_ACK(void)
{
	LATCbits.LATC6=0; // make sure the pin will go low when turned to output
	TRISCbits.TRISC6=0; // make it an output to pull the ACK low
	Delay10TCYx(3); // Wait for 30 instruction cycles (about 2.5 usec)
	TRISCbits.TRISC6=1; // return the pin to an input high impedence
}

