/*
UPCB - Universal Programmed Controller Board 

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
#include "n64.h"
#include "upcb.h"
#include <delays.h>

void N64_Init_Pins(void);
void N64_main(void);


void N64_Init_Pins(void)
{
	LATCbits.LATC7=0;
	TRISCbits.TRISC7=INPUT;
	UCONbits.USBEN=0;
	UCFGbits.UTRDIS=1;
}


void N64_main(void)
{
	unsigned char byte_read, N64_mode, N64_stored_x, N64_stored_y;
	unsigned char N64buffer[10];
	N64buffer[0]=N64buffer[1]=N64buffer[2]=N64buffer[3]=N64buffer[4]=0;
	N64buffer[5]=N64buffer[6]=N64buffer[7]=N64buffer[8]=N64buffer[9]=0;
	N64_Init_Pins();
	//check for which N64 mode to use.  Default to basic (both dpad and analog)
	N64_mode=N64_MODE_BASIC;
	//start only: dpad only
	if (!Stick_Start && Stick_Select) N64_mode=N64_MODE_DPAD_ONLY;
	//select only: analog only
	if (Stick_Start && !Stick_Select) N64_mode=N64_MODE_ANALOG_ONLY;
			
	while (1) {
		byte_read=0; 
 
		_asm
			//wait for 4us of high (idle)
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

			//Bit 7
			//wait for line to drop
			wait_for_drop7: 
				btfsc PORTC, 5, ACCESS //skip next instruction if NOT high
				goto wait_for_drop7
			//line is down. wait for 2us (to sample centre of signal)
			N64_2US_DELAY
			N64_READBIT(7)
			line_high6:
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_high6
			wait_for_drop6:
			btfsc PORTC, 5, ACCESS //wait for low (start of next bit)
			goto wait_for_drop6
			N64_2US_DELAY
			N64_READBIT(6)
			line_high5:
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_high5
			wait_for_drop5:
			btfsc PORTC, 5, ACCESS //wait for low (start of next bit)
			goto wait_for_drop5
			N64_2US_DELAY
			N64_READBIT(5)
			line_high4:
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_high4
			wait_for_drop4:
			btfsc PORTC, 5, ACCESS //wait for low (start of next bit)
			goto wait_for_drop4
			N64_2US_DELAY
			N64_READBIT(4)
			line_high3:
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_high3
			wait_for_drop3:
			btfsc PORTC, 5, ACCESS //wait for low (start of next bit)
			goto wait_for_drop3
			N64_2US_DELAY
			N64_READBIT(3)
			line_high2:
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_high2
			wait_for_drop2:
			btfsc PORTC, 5, ACCESS //wait for low (start of next bit)
			goto wait_for_drop2
			N64_2US_DELAY
			N64_READBIT(2)
			line_high1:
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_high1
			wait_for_drop1:
			btfsc PORTC, 5, ACCESS //wait for low (start of next bit)
			goto wait_for_drop1
			N64_2US_DELAY
			N64_READBIT(1)
			line_high0:
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_high0
			wait_for_drop0:
			btfsc PORTC, 5, ACCESS //wait for low (start of next bit)
			goto wait_for_drop0
			N64_2US_DELAY
			N64_READBIT(0)

			line_high_stop:
			btfss PORTC, 5, ACCESS //if line is high, skip next instruction
			goto line_high_stop
			wait_for_drop_stop:
			btfsc PORTC, 5, ACCESS //wait for low (start of next bit)
			goto wait_for_drop_stop
			N64_4US_DELAY //skip over entire stop bit
		_endasm 
			
		//ok line should be high and waiting for us to respond
		if (byte_read == N64_COMMAND_IDENTIFY || byte_read == N64_COMMAND_OLD_IDENT) {
			N64buffer[0]=0x05; 
		 	N64buffer[1]=0x00;
		 	N64buffer[2]=0x02;
		 	N64_SendResponse(N64buffer, 24);
		} else if (byte_read == N64_COMMAND_STATUS) { //polling command. 
			if ((N64_mode == N64_MODE_BASIC) 
				|| (N64_mode == N64_MODE_DPAD_ONLY) 
				|| (N64_mode == N64_MODE_ANALOG_ONLY))
			{//added to make directions move both dpad and right analog 
				N64buffer[0]=0;
				if (!Stick_Short) N64buffer[0]	|=0b10000000; //A
				if (!Stick_Jab) N64buffer[0]	|=0b01000000; //B
				if (!Stick_Select) N64buffer[0] |=0b00100000; //Z
				if (!Stick_Start) N64buffer[0]	|=0b00010000; //Start
				N64buffer[3] = N64_ANALOG_MID;
				if (!Stick_Up) {
					if ((N64_mode==N64_MODE_BASIC)||(N64_mode==N64_MODE_DPAD_ONLY))	{
						N64buffer[0] |= 0b00001000;
					}
					if ((N64_mode==N64_MODE_BASIC)||(N64_mode==N64_MODE_ANALOG_ONLY)) {
						N64buffer[3] = N64_ANALOG_HIGH;
					}
				} else if (!Stick_Down) {
					if ((N64_mode==N64_MODE_BASIC)||(N64_mode==N64_MODE_DPAD_ONLY)) {
						N64buffer[0] |= 0b00000100;
					}
					if ((N64_mode==N64_MODE_BASIC)||(N64_mode==N64_MODE_ANALOG_ONLY)) {
						N64buffer[3] = N64_ANALOG_LOW;
					}
				}
				N64buffer[2] = N64_ANALOG_MID;
				if (!Stick_Left) {
					if ((N64_mode==N64_MODE_BASIC)||(N64_mode==N64_MODE_DPAD_ONLY)) {
						N64buffer[0] |= 0b00000010;
					}
					if ((N64_mode==N64_MODE_BASIC)||(N64_mode==N64_MODE_ANALOG_ONLY)) {
						N64buffer[2] = N64_ANALOG_LOW;
					}
				} else if (!Stick_Right) {
					if ((N64_mode==N64_MODE_BASIC)||(N64_mode==N64_MODE_DPAD_ONLY)) {
						N64buffer[0] |= 0b00000001;
					}
					if ((N64_mode==N64_MODE_BASIC)||(N64_mode==N64_MODE_ANALOG_ONLY)) {
						N64buffer[2] = N64_ANALOG_HIGH;
					}
				}

				N64buffer[1]=0b00000000;
				#ifdef EXTRA_BUTTONS
					if (!Stick_Extra0) N64buffer[1]		|=0b00100000; //left trigger
					if (!Stick_Extra1) N64buffer[1]		|=0b00010000; //right trigger
				#endif
				if (!Stick_Fierce) N64buffer[1]		|=0b00001000; //c-up
				if (!Stick_Forward) N64buffer[1]	|=0b00000100; //c-down
				if (!Stick_Strong) N64buffer[1]		|=0b00000010; //c-left
				if (!Stick_Roundhouse) N64buffer[1]	|=0b00000001; //c-right
			}
			N64_SendResponse(N64buffer, 32);
			FrameUpdate();
		}
	}
}




void N64_SendResponse(unsigned char *buffer, unsigned char bitsize)
{
	unsigned char lfsr, hfsr, bitcount;
	lfsr=FSR1L;
	hfsr=FSR1H;
	bitcount=0;
	
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
		N64_ASM_LOW // 1 cycle.. clear for output (low), set for input (high)
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
		N64_ASM_HIGH //13  1us. If we're here, the bit is 1, and the lines high right on time
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
			N64_ASM_HIGH //37
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
			N64_ASM_HIGH //37 (regardless of what the last bit is, it should be high now)
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
			nop //48 (end of last bit)
			N64_ASM_LOW// 1 (start of stop bit)
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
			N64_ASM_HIGH //high rise of stop bit. Done.
	_endasm
	//put the FSR back the way we found it
	FSR1L=lfsr;
	FSR1H=hfsr;
	TRISCbits.TRISC7=INPUT; //#define N64_DATA_IN
}
