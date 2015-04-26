#ifndef N64_H
#define N64_H
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
/* UPCB module for N64 support
	
   Looking into the socket/plug on the front of the N64

         _____
        /     \
       | 1 2 3 |
       ---------

       	Pin			Description
	1			+3.3v (VCC)
	2			Data
	3			Ground


	To make a UPCB cable for the N64, match up pins like this:
	D-Sub 15 Pin		N64 Pin
	1			3 (GND) 
	2			Low
	3			Low
	4			High
	5			Low
	6			Low
	7			Low
	8			1 (+3.3v)
	9			NC - Not connected to anything
	10			High
	11			High
	12			NC - Not connected to anything
	13			NC - Not connected to anything
	14			2 (Data)
	15			Low
		

	The controller identify command is 000000001 (eight bit 0x00, and stop
	bit). This command is just sent to check for the existence of a 
	controller. If this is not answered, the N64 will just believe 
	there is no controller plugged in. The appropriate response is 24 bits
	in length. The ID we will respond with is 0x050002, the standard N64 
	controller with no rumble pak or memory card attached.

	The controller polling command is 000000011 (eight bit 0x01, and stop bit).
	
	Our response to the polling command is 8 bytes. The digital inputs are
	in the first two bytes, while the analogs take up the remaining two bytes.
	The analog stick values range from -127 to +127 (two's complement), but
	an actual controller's range is only ~-81 to ~+81, so those are the max values
	we will supply.

	Byte0	A		B	Z	Start	U	D	L	R
	Byte1	JoystickReset	0	LT	RT	C-Up	C-Down	C-Left	C-Right
	Byte2 	Joystick X
	Byte3	Joystick Y

*/

//bcf = clear (0) = drive low, bsf = set (1) = let high
#define N64_ASM_HIGH	bsf TRISC, 7, ACCESS
#define N64_ASM_LOW	bcf TRISC, 7, ACCESS

// 1-byte commands sent from N64 to controller
#define N64_COMMAND_IDENTIFY	0x00
#define N64_COMMAND_STATUS	0x01
#define N64_COMMAND_READ_BUS	0x02
#define N64_COMMAND_WRITE_BUS	0x03
#define N64_COMMAND_OLD_IDENT   0xFF
	
#define N64_NOP _asm nop _endasm
#define N64_NOP2 N64_NOP N64_NOP
#define N64_NOP4 N64_NOP2 N64_NOP2
#define N64_NOP8 N64_NOP4 N64_NOP4
#define N64_NOP12 N64_NOP8 N64_NOP4
#define N64_NOP16 N64_NOP8 N64_NOP8
#define N64_NOP20 N64_NOP16 N64_NOP4
#define N64_NOP22 N64_NOP20 N64_NOP2
#define N64_NOP24 N64_NOP20 N64_NOP4

#define N64_2US_DELAY _endasm N64_NOP22 _asm
#define N64_4US_DELAY _endasm N64_NOP8 N64_NOP24 N64_NOP24 _asm

#define N64_ANALOG_HIGH 0x51 //81
#define N64_ANALOG_LOW  0xAF //-81
#define N64_ANALOG_MID  0x00 

#define N64_MODE_BASIC 		0x01
#define N64_MODE_DPAD_ONLY	0x02
#define N64_MODE_ANALOG_ONLY	0x03

#define N64_READBIT(bitnum) _endasm if(PORTCbits.RC5){byte_read|=1<<bitnum;} _asm
#define N64_OUT_HIGH()		TRISCbits.TRISC7=INPUT;
#define N64_OUT_LOW()		TRISCbits.TRISC7=OUTPUT;
#define N64_DATA_IN 		PORTCbits.RC7

/* macros */

/* Prototypes */
void N64_Init_Pins(void);
void N64_main(void);
void N64_SendResponse(unsigned char *buffer, unsigned char bitsize);

#endif //N64_H		
