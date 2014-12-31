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
#include "gc.h"
#include "upcb.h"
#include <delays.h>

void GC_Init_Pins(void);
void GC_main(void);


void GC_Init_Pins(void)
{
	LATCbits.LATC7=0;
	TRISCbits.TRISC7=INPUT; //#define GC_DATA_IN
//	LATBbits.LATB4=0; //set output pin 9 to '0' (default)
//	TRISBbits.TRISB4=OUTPUT; //it'll be clearer if I just make it a regular output.
	UCONbits.USBEN=0;
	UCFGbits.UTRDIS=1;
	#ifdef GC_DEBUG
	TRISCbits.TRISC6=OUTPUT; //for debugging only.
	LATCbits.LATC6=1;
	#endif
}


void GC_main(void)
{
	unsigned char byte_read, GC_mode, GC_stored_x, GC_stored_y;
	unsigned char GCbuffer[10];
	//"frame since last" direction counters for basic SSB mode
	unsigned char FSL_Up, FSL_Down, FSL_Left, FSL_Right;
	//"double tap" direction flag for basic SSB mode
	unsigned char DT_Up, DT_Down, DT_Left, DT_Right;
	GCbuffer[0]=GCbuffer[1]=GCbuffer[2]=GCbuffer[3]=GCbuffer[4]=0;
	GCbuffer[5]=GCbuffer[6]=GCbuffer[7]=GCbuffer[8]=GCbuffer[9]=0;
	GC_Init_Pins();
	//check for which GC mode to use.  Default to basic (both dpad and analog)
	GC_mode=GC_MODE_BASIC;
	//start only: dpad only
	if (!Stick_Start && Stick_Select) GC_mode=GC_MODE_DPAD_ONLY;
	//select only: analog only
	if (Stick_Start && !Stick_Select) GC_mode=GC_MODE_ANALOG_ONLY;
	//strong & forward: SSB basic 
	if (!Stick_Strong && !Stick_Forward) GC_mode=GC_MODE_SSB_BASIC;
	//fierce & roundhouse: SSB advanced
	if (!Stick_Fierce && !Stick_Roundhouse) GC_mode=GC_MODE_SSB_ADV;
	
	while(1)
	{
		/*New tactic: read the first byte only. */
		byte_read=0; 

		_asm
			//Bit 7
			//wait for line top drop
			GCRD_HIGH
			wait_for_drop7: 
				btfsc PORTC, 5, ACCESS
				goto wait_for_drop7
			//line is down. wait for 2us
			//nop
			GC_2US_DELAY
			GC_READBIT(7)

			GC_4US_DELAY
			GC_READBIT(6)
			GC_4US_DELAY
			GC_READBIT(5)
			GC_4US_DELAY
			GC_READBIT(4)
			GC_4US_DELAY
			GC_READBIT(3)
			GC_4US_DELAY
			GC_READBIT(2)
			GC_4US_DELAY
			GC_READBIT(1)
			GC_4US_DELAY
			GC_READBIT(0)

			/* First byte read. Let's wait until the transmission is complete */
			/* by waiting until we have 4 us of uninterupted high */
			line_got_dropped_low:
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_got_dropped_low
			//ok line should be high and waiting for us to respond
			GCRD_LOW
			_endasm 
			if (!Stick_Jab)
			{
				mLED_1_Off();
				mLED_2_Off();
			}
			if (byte_read == 0x00)
			{ //probe
				mLED_1_Toggle();
				GCbuffer[0]=0x09; 
			 	GCbuffer[1]=0x00;
			 	GCbuffer[2]=0x00;
			 	SendResponse(GCbuffer, 24);
			}
			if (byte_read == 0x41)
			{ //get origins
				GCbuffer[0]=0x00;
				GCbuffer[1]=0x80;
				GCbuffer[2]=0x83;
				GCbuffer[3]=0x7A;
				GCbuffer[4]=0x81;
				GCbuffer[5]=0x7E;
				GCbuffer[6]=0x16;
				GCbuffer[7]=0x13;
				GCbuffer[8]=0x00;
				GCbuffer[9]=0x00;
				SendResponse(GCbuffer, 80);
			}
			if (byte_read == 0x40)  			
			{ //polling command. 
				if (  (GC_mode == GC_MODE_BASIC) 
				|| (GC_mode == GC_MODE_DPAD_ONLY) 
				|| (GC_mode == GC_MODE_ANALOG_ONLY)  )
				{//added to make directions move both dpad and right analog 
					GCbuffer[0]=0;
					if(!Stick_Start)GCbuffer[0]		|=0b00010000;
					if(!Stick_Jab)GCbuffer[0]		|=0b00001000;
					if(!Stick_Strong)GCbuffer[0]	|=0b00000100;
					if(!Stick_Short)GCbuffer[0]		|=0b00000010;
					if(!Stick_Forward)GCbuffer[0]	|=0b00000001;
					GCbuffer[4]=0x80; //cstick x
					GCbuffer[5]=0x80; //cstick y
					GCbuffer[1]=0b10000000;
					
					if(!Stick_Fierce)
					{
							GCbuffer[1]		|=0b01000000;
							GCbuffer[6]=GC_ANALOG_HIGH; //trigger L squeezed
					}
					else 
					{
						GCbuffer[6]=GC_ANALOG_LOW; //trigger L neutral
					}	
					
					if(!Stick_Roundhouse)
					{
						GCbuffer[1]		|=0b00100000;
						GCbuffer[7]=GC_ANALOG_HIGH; //trigger R squeezed
					}
					else
					{
						GCbuffer[7]=GC_ANALOG_LOW; //trigger R neutral
					}
					GCbuffer[3]=0x80;
					GCbuffer[2]=0x80;
					if(!Stick_Select)
					{
						GCbuffer[1]		|=0b00010000;
						if(!Stick_Up){ GCbuffer[5]=GC_ANALOG_HIGH; GCbuffer[1]&=0b11101111;}
						if(!Stick_Down){ GCbuffer[5]=GC_ANALOG_LOW; GCbuffer[1]&=0b11101111;}
						if(!Stick_Left){ GCbuffer[4]=GC_ANALOG_LOW; GCbuffer[1]&=0b11101111;}
						if(!Stick_Right){ GCbuffer[4]=GC_ANALOG_HIGH; GCbuffer[1]&=0b11101111;}						
					}
					else
					{
						if(!Stick_Up)
						{
							if((GC_mode==GC_MODE_BASIC)||(GC_mode==GC_MODE_DPAD_ONLY))	GCbuffer[1]	|=0b00001000;
							if((GC_mode==GC_MODE_BASIC)||(GC_mode==GC_MODE_ANALOG_ONLY))GCbuffer[3]=GC_ANALOG_HIGH;
						}
						else
						{				
							if(!Stick_Down)
							{
								if((GC_mode==GC_MODE_BASIC)||(GC_mode==GC_MODE_DPAD_ONLY)) GCbuffer[1] |=0b00000100;
								if((GC_mode==GC_MODE_BASIC)||(GC_mode==GC_MODE_ANALOG_ONLY))GCbuffer[3]=GC_ANALOG_LOW;
							}
						}
										
						if(!Stick_Right)
						{
							if((GC_mode==GC_MODE_BASIC)||(GC_mode==GC_MODE_DPAD_ONLY)) GCbuffer[1]		|=0b00000010;
							if((GC_mode==GC_MODE_BASIC)||(GC_mode==GC_MODE_ANALOG_ONLY))GCbuffer[2]=GC_ANALOG_HIGH;
						}
						else
						{
							if(!Stick_Left)
							{
								if((GC_mode==GC_MODE_BASIC)||(GC_mode==GC_MODE_DPAD_ONLY))GCbuffer[1]		|=0b00000001;
								if((GC_mode==GC_MODE_BASIC)||(GC_mode==GC_MODE_ANALOG_ONLY))GCbuffer[2]=GC_ANALOG_LOW;
							}					
						}
					}										
					
				}
//advanced SSB				
				if (GC_mode == GC_MODE_SSB_ADV)
				{
				
#ifdef SMASH_ADVANCED_ORIG			
					//short - shield
					//jab - A
					//strong - B
					//fierce - Run
					//forward - jump
					//roundhouse - lock
					//select - dpad down
					GCbuffer[0]=0;
					GCbuffer[1]=0b10000000;
					GCbuffer[2]=0x80; //right analog x
					GCbuffer[3]=0x80; //right analog y
					GCbuffer[4]=0x80; //cstick x
					GCbuffer[5]=0x80; //cstick y
					GCbuffer[6]=GC_ANALOG_LOW;
					GCbuffer[7]=GC_ANALOG_LOW;	
					//easy stuff first; start
					if(!Stick_Start)	GCbuffer[0]	|=0b00010000;
					//and select for taunt (up taunt in Brawl)
					if (!Stick_Select)	GCbuffer[1] |=0b00001000;
					//jab for A
					if(!Stick_Jab)GCbuffer[0]		|=0b00000001;
					//strong for B
					if(!Stick_Strong)GCbuffer[0]	|=0b00000010;
					//forward for jump
					if(!Stick_Forward)GCbuffer[0]		|=0b00001000;
					if(!Stick_Roundhouse)
					{ //lock is pressed. report stored direction for analog stick, 
					  //actual direction (100%)for C stick
						GCbuffer[2]=GC_stored_x;
						GCbuffer[3]=GC_stored_y;
						if(!Stick_Up)  GCbuffer[5]=GC_ANALOG_HIGH;
						if(!Stick_Down)  GCbuffer[5]=GC_ANALOG_LOW;
						if(!Stick_Left) GCbuffer[4]=GC_ANALOG_LOW;
						if(!Stick_Right) GCbuffer[4]=GC_ANALOG_HIGH;		
					}
					else
					{ //lock NOT pressed. check if 'run' is pressed
						if(!Stick_Fierce)
						{//run pressed, use 100% values
							if(!Stick_Up)  GCbuffer[3]=GC_ANALOG_HIGH;
							if(!Stick_Down)  GCbuffer[3]=GC_ANALOG_LOW;
							if(!Stick_Left) GCbuffer[2]=GC_ANALOG_LOW;
							if(!Stick_Right) GCbuffer[2]=GC_ANALOG_HIGH;	
						}
						else
						{ //run not pressed, use 50% values for analog stick
							if(!Stick_Up)  GCbuffer[3]=GC_ANALOG_MID_HIGH;
							if(!Stick_Down)  GCbuffer[3]=GC_ANALOG_MID_LOW;
							if(!Stick_Left) GCbuffer[2]=GC_ANALOG_MID_LOW;
							if(!Stick_Right) GCbuffer[2]=GC_ANALOG_MID_HIGH;
						}						
						//and in either case, store the analog values in case lock is pressed
						GC_stored_x=GCbuffer[2];
						GC_stored_y=GCbuffer[3];
					}
					if(!Stick_Short)
					{ //shield pressed. Check 'run to see if 50% or 100% 
						if (!Stick_Fierce) 
						{//100%. need full trigger + digital click
							GCbuffer[6]=GC_ANALOG_HIGH;
							GCbuffer[1]		|=0b01000000;
						}
						else GCbuffer[6]=GC_ANALOG_MID;
					}
#endif
#ifdef SMASH_ADVANCED_GK
					//short - shield
					//jab - A
					//strong - B
					//fierce - Run
					//forward - jump
					//roundhouse - lock
					//select - dpad down
					GCbuffer[0]=0;
					GCbuffer[1]=0b10000000;
					GCbuffer[2]=0x80; //right analog x
					GCbuffer[3]=0x80; //right analog y
					GCbuffer[4]=0x80; //cstick x
					GCbuffer[5]=0x80; //cstick y
					GCbuffer[6]=GC_ANALOG_LOW;
					GCbuffer[7]=GC_ANALOG_LOW;	
					//easy stuff first; start
					if(!Stick_Start)	GCbuffer[0]	|=0b00010000;
					//and select for taunt (up taunt in Brawl)
					if (!Stick_Select)	GCbuffer[1] |=0b00001000;
					//jab for A
					if(!Stick_Jab)GCbuffer[0]		|=0b00000001;
					//strong for B
					if(!Stick_Strong)GCbuffer[0]	|=0b00000010;
					//fierce for jump
					if(!Stick_Fierce)GCbuffer[0]		|=0b00001000;
					if(!Stick_Roundhouse)
					{ //lock is pressed. report stored direction for analog stick, 
					  //actual direction (100%)for C stick
						GCbuffer[2]=GC_stored_x;
						GCbuffer[3]=GC_stored_y;
						if(!Stick_Up)  GCbuffer[5]=GC_ANALOG_HIGH;
						if(!Stick_Down)  GCbuffer[5]=GC_ANALOG_LOW;
						if(!Stick_Left) GCbuffer[4]=GC_ANALOG_LOW;
						if(!Stick_Right) GCbuffer[4]=GC_ANALOG_HIGH;		
					}
					else
					{ //lock NOT pressed. check if 'run' is pressed
						if(!Stick_Fierce)
						{//run pressed, use 100% values
							if(!Stick_Up)  GCbuffer[3]=GC_ANALOG_HIGH;
							if(!Stick_Down)  GCbuffer[3]=GC_ANALOG_LOW;
							if(!Stick_Left) GCbuffer[2]=GC_ANALOG_LOW;
							if(!Stick_Right) GCbuffer[2]=GC_ANALOG_HIGH;	
						}
						else
						{ //run not pressed, use 50% values for analog stick
							if(!Stick_Up)  GCbuffer[3]=GC_ANALOG_MID_HIGH;
							if(!Stick_Down)  GCbuffer[3]=GC_ANALOG_MID_LOW;
							if(!Stick_Left) GCbuffer[2]=GC_ANALOG_MID_LOW;
							if(!Stick_Right) GCbuffer[2]=GC_ANALOG_MID_HIGH;
						}						
						//and in either case, store the analog values in case lock is pressed
						GC_stored_x=GCbuffer[2];
						GC_stored_y=GCbuffer[3];
					}
					if(!Stick_Short)
					{ //shield pressed. Check 'run to see if 50% or 100% 
						if (!Stick_Fierce) 
						{//100%. need full trigger + digital click
							GCbuffer[6]=GC_ANALOG_HIGH;
							GCbuffer[1]		|=0b01000000;
						}
						else GCbuffer[6]=GC_ANALOG_MID;
					}
#endif					
				} //end SSB advanced
//basic SSB
				if(GC_mode == GC_MODE_SSB_BASIC)
				{
					//basic moveset, but difficult to code. 
					//start:duh
					//select: down taunt
					//short:shield
					//jab:A
					//strong:B
					//double tap directions for 100%, 50% otherwise.
					//A+B will use C stick for smashing. 
					GCbuffer[0]=0;
					GCbuffer[1]=0b10000000;
					GCbuffer[2]=0x80; //right analog x
					GCbuffer[3]=0x80; //right analog y
					GCbuffer[4]=0x80; //cstick x
					GCbuffer[5]=0x80; //cstick y
					GCbuffer[6]=GC_ANALOG_LOW;
					GCbuffer[7]=GC_ANALOG_LOW;	
					//easy stuff first; start
					if(!Stick_Start)	GCbuffer[0]	|=0b00010000;
					//and select for taunt (up taunt in Brawl)
					if (!Stick_Select)	GCbuffer[1] |=0b00001000;
					//and shield will always be 100%
					if (!Stick_Short) 
					{//100%. need full trigger + digital click
						
						if(Stick_Jab)
						{
							GCbuffer[6]=GC_ANALOG_HIGH;
							GCbuffer[1]		|=0b01000000;
						}
						else //shield + A = Z trigger
						{
							GCbuffer[1] |=0b00010000;	
						}
					}
					else 
					{
						GCbuffer[6]=GC_ANALOG_LOW;
						//jab for A 
						if(!Stick_Jab)GCbuffer[0]		|=0b00000001;
					}
					//strong for B
					if(!Stick_Strong)GCbuffer[0]	|=0b00000010;
				
					//fierce for jump
					if(!Stick_Fierce)GCbuffer[0]	|=0b00001000;
						
					//Update 'frame since last' counters, and clear DT flags if not pressed
					if(Stick_Up && (FSL_Up < 200)) { FSL_Up++; DT_Up=0; }
					if(Stick_Down && (FSL_Down < 200)) { FSL_Down++; DT_Down=0; }
					if(Stick_Left && (FSL_Left < 200)) { FSL_Left++; DT_Left=0; }
					if(Stick_Right && (FSL_Right < 200)) { FSL_Right++; DT_Right=0; }
					if(!Stick_Up)
					{ //up is pressed. is it being held after a doubletap?
						if (DT_Up)
						{ //yup, so it's 100%
							GCbuffer[3]=GC_ANALOG_HIGH;
						}
						else
						{ //no DT flag, so check if it should be. it should be if the FSL is < GC_DOUBLETAP_WINDOW
							if ((FSL_Up < GC_DOUBLETAP_WINDOW) && (FSL_Up > 0))
							{ //doubletap up. Set the flag, report 100%
								DT_Up=1;
								GCbuffer[3]=GC_ANALOG_HIGH;
							}
							else
							{ //not a new double tap nor a held double tap. just report 50%
								GCbuffer[3]=GC_ANALOG_MID_HIGH;
							}
						}
						//no matter what, clear FSL 
						FSL_Up=0;
					}
					if(!Stick_Down)
					{ //down is pressed. is it being held after a doubletap?
						if (DT_Down)
						{ //yup, so it's 100%
							GCbuffer[3]=GC_ANALOG_LOW;
						}
						else
						{ //no DT flag, so check if it should be. it should be if the FSL is < GC_DOUBLETAP_WINDOW
							if ((FSL_Down < GC_DOUBLETAP_WINDOW) && (FSL_Down > 0))
							{ //doubletap down. Set the flag, report 100%
								DT_Down=1;
								GCbuffer[3]=GC_ANALOG_LOW;
							}
							else
							{ //not a new double tap nor a held double tap. just report 50%
								GCbuffer[3]=GC_ANALOG_MID_LOW;
							}
						}
						//no matter what, clear FSL 
						FSL_Down=0;
					}
					if(!Stick_Left)
					{ //Left is pressed. is it being held after a doubletap?
						if (DT_Left)
						{ //yup, so it's 100%
							GCbuffer[2]=GC_ANALOG_LOW;
						}
						else
						{ //no DT flag, so check if it should be. it should be if the FSL is < GC_DOUBLETAP_WINDOW
							if ((FSL_Left < GC_DOUBLETAP_WINDOW) && (FSL_Left > 0))
							{ //doubletap Left. Set the flag, report 100%
								DT_Left=1;
								GCbuffer[2]=GC_ANALOG_LOW;
							}
							else
							{ //not a new double tap nor a held double tap. just report 50%
								GCbuffer[2]=GC_ANALOG_MID_LOW;
							}
						}
						//no matter what, clear FSL 
						FSL_Left=0;
					}					
					if(!Stick_Right)
					{ //Right is pressed. is it being held after a doubletap?
						if (DT_Right)
						{ //yup, so it's 100%
							GCbuffer[2]=GC_ANALOG_HIGH;
						}
						else
						{ //no DT flag, so check if it should be. it should be if the FSL is < GC_DOUBLETAP_WINDOW
							if ((FSL_Right < GC_DOUBLETAP_WINDOW) && (FSL_Right > 0))
							{ //doubletap Right. Set the flag, report 100%
								DT_Right=1;
								GCbuffer[2]=GC_ANALOG_HIGH;
							}
							else
							{ //not a new double tap nor a held double tap. just report 50%
								GCbuffer[2]=GC_ANALOG_MID_HIGH;
							}
						}
						//no matter what, clear FSL 
						FSL_Right=0;
					}							
					//if A+B+direction pressed, smash with C stick
					if(!Stick_Jab && !Stick_Strong && (!Stick_Up || !Stick_Down || !Stick_Left || !Stick_Right))
					{ 
						if(!Stick_Up)  GCbuffer[5]=GC_ANALOG_HIGH;
						if(!Stick_Down)  GCbuffer[5]=GC_ANALOG_LOW;
						if(!Stick_Left) GCbuffer[4]=GC_ANALOG_LOW;
						if(!Stick_Right) GCbuffer[4]=GC_ANALOG_HIGH;
						//can't smash in the air :) Reset left analog to neutral.
						GCbuffer[3]=GCbuffer[2]=GC_ANALOG_MID;
					}			
					//just to be safe, if the combinations of shield+A=Z, or A+B=Cstick are used, 
					//let's make sure A and B are not reported as pressed. 
					if(!Stick_Jab)
					{ //jab is pressed
						if (!Stick_Strong) 
						{ //A+B combination
							GCbuffer[0] &= 0b11111100;
						}
						if (!Stick_Short)
						{ //A+Shield combo
							GCbuffer[0] &= 0b11111100;
						}
					}
				}
				SendResponse(GCbuffer, 64);
				FrameUpdate();
			}
				
/*			if ( (byte_read != 0x41) 
					&& (byte_read != 0x00)
					&& (byte_read != 0x40)
				)
			{
				mLED_2_Toggle();
				GCbuffer[0]=byte_read;
				GCbuffer[1]=byte_read;
				GCbuffer[2]=byte_read;
			}			 
*/
			
			
			
	}
}




void SendResponse(unsigned char *buffer, unsigned char bitsize)
{
	unsigned char lfsr, hfsr, bitcount;
	lfsr=FSR1L;
	hfsr=FSR1H;
	bitcount=0;
	
/*	_asm
		movlw bitsize
		decf PLUSW2, 1, ACCESS
	_endasm  */
	/* to access bitsize, movlw bitsize, then use PLUSW2 as normal. Same as the local vars */
	
  /* Okay, so the path is clear. Put buffer into FSR1L, put buffer+1 into FSR1H, 
  	and direct everything with INDF1. */
	_asm
	movlw buffer
	movff PLUSW2, FSR1L
	movlw buffer+1
	movff PLUSW2, FSR1H
	/* Now we can access buffer with INDF1, and we just need to increment FSR1L every 
		8 bits */
	
	begin_of_response_loop:
		//drop the line low
		GC_ASM_LOW // 1 cycle.. clear for output (low), set for input (high)
		//rotate the byte at buffer to get the the current bit into carry
		nop // 2
		nop // 3
		nop // 4
		nop // 5
		nop // 6 
		nop // 7
		nop // 8
		nop // 9
		nop // 10
		rlcf INDF1, 1, ACCESS //11,  carry holds the bit
		//if carry is 1, release line high
		bnc bit_zero // 12~13   ; one cycle if no branch, 2 cycle if branched
	bit_one:	
		GC_ASM_HIGH //13  1us. If we're here, the bit is 1, and the lines high right on time
		goto bit_midgame//15
	bit_zero:
		nop //15
		nop //15
	bit_midgame:
		//Alright, we got just under 2 us for housekeeping. Make sure we set the TRISC on cycle 36
		//since there's no effect if its set when already set, we only need ot check the bit we're
		//sending once. 
		//first, decrement the bitsize, and head to stopbit if zero
		movlw bitsize //16
		decf PLUSW2, 1, ACCESS //17
		//btfss is 1 cycle if no skip, 2 cycle to skip over 1 cycle instruction, and 3 to skip a 2
		//cycle instruction like goto. 
		btfsc STATUS, 2, ACCESS //18~20 (if skip next) Have to do it this way since there is no 'branch if zero'. 
		goto  send_stop_bit //20
		//increment the count of bits we've sent
		movlw bitcount // 21
		incf PLUSW2, 1, ACCESS //22
		// check if 8
		//btfss is 1 cycle if no skip, 2 cycle to skip over 1 cycle instruction, and 3 to skip a 2
		//cycle instruction like goto. 
		btfss PLUSW2, 3, ACCESS //23~25
		goto do_not_update_byte_pointer //25
	update_byte_pointer:
			//its 8, clear it
			movlw bitcount //26
			clrf PLUSW2, ACCESS  //27
			//and increment the buffer pointer
			incf FSR1L, 1, ACCESS //28
			goto end_of_update_byte_pointer //30		
	do_not_update_byte_pointer:
			nop //26
			nop //27
			nop //28
			nop //29
			nop //30
	end_of_update_byte_pointer:
			// okay, so we grabbed the bit, released the line if needed, incremented number read, decremented
			//the number to read, checked the number read to see if pointer needed incremented. Nothing to do
			//but wait now. 
			nop //31
			nop //32
			nop //33
			nop //34
			nop //35
			nop //36
			//3us on the dot. Release line to high; no affect if previously released.
			GC_ASM_HIGH //37
			nop //38
			nop //39
			nop //40
			nop //41
			nop //42
			nop //43
			nop //44
			nop //45
			nop //46
			// goto is two cycles, taking us right back where we need. 
			goto begin_of_response_loop
	send_stop_bit:
			//if we've been sent here, then this is the last bit. Just sit tight, wait for the 
			//36th instruction, release the line high, wait 12 cycles, drop the line, wait 12
			//cycles, release the line, and exit routine. 
			nop //21
			nop //22
			nop //23
			nop //24
			nop //25
			nop //26
			nop //27
			nop //28
			nop //29
			nop //30
			nop //31
			nop //32
			nop //33
			nop //34
			nop //35
			nop //36
			GC_ASM_HIGH //37
			nop //38
			nop //39
			nop //40
			nop //41
			nop //42
			nop //43
			nop //44
			nop //45
			nop //46
			nop //47
			nop //48
			GC_ASM_LOW// 1
			nop //2
			nop //3
			nop //4
			nop //5
			nop //6
			nop //7
			nop //8
			nop //9
			nop //10
			nop //11
			nop //12
			GC_ASM_HIGH //finally, done.
	_endasm
	//put the FSR back the way we found it
	FSR1L=lfsr;
	FSR1H=hfsr;
	TRISCbits.TRISC7=INPUT; //#define GC_DATA_IN
}