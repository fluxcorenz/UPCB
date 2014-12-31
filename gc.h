#ifndef GC_H
#define GC_H
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
/* UPCB module for Gamecube support
	The Gamecube pinout from http://www.int03.co.uk/crema/hardware/gamecube/gc-control.htm
		Looking at the socket/plug on the front of the Gamecube
		  
		  	_7_
		   /   \
		  /1 2 3\
		  |-----|
		  \4 5 6/
		   -----
		   
		Pin 		Description
		1			+5v (See note below)
		2			Data
		3			Ground
		4			Ground
		5			Unknown/Not connected
		6			+3.43V (VCC) (See note below)
		7			Cable shielding/Ground
		
		The Gamecube has two different sources of power, one at 3.43v, the
		other at 5v. We want to use only one. Because the data line's High
		value is at 3.43v, we will be tieing the 3.43v line 6 to the Data 
		with a pull up resister. We will be using the +5v to power the UPCB.
		
		Information on the protocol taken from many sites, such as:
		http://www.int03.co.uk/crema/hardware/gamecube/gc-control.htm
		http://www.gc-linux.org/docs/yagcd/chap9.html
		http://www.gifford.co.uk/~coredump/gpad.htm
		http://www.raphnet.net/electronique/snes2wii/index_en.php
		(The project on raphnet is good, but I must point out that none of
		the code used in this project came from any of his.)
		
		The communication both ways takes place over the single Data line. The
		line should be pulled high at the console. The line is treated as an 
		open collector; when either device wants wants the line low, it 
		connects the line to ground (output 0). When it wants it high, it 
		releases the line (goes	high impedence aka changes to an input in PIC 
		speak) This prevents contention on the line that could happen if one 
		device was output high while the other was output low; it'd be a short circuit.
		
		Each bit of communication is 4us (microseconds) in duration. A low bit
		is 3us of the line pulled low, followed by 1us of the line being high.
		Conversly, a High bit is 1us of the line being low, followed by 3us of
		the line being high. Either way, the first part of the bit is always low,
		while the last half is always high. This pulling low in the first part
		of every bit is one way each device tells the other 'Hey, I'm talking 
		here.'
		
		After each full sequence of bits is sent either way, it is ended with 
		an additional high bit (1us low, and then the line is just left high).
		
		The following is a collection of logical assumptions based on 
		information from
		http://www.int03.co.uk/crema/hardware/gamecube/gc-control.htm
		and
		http://www.gc-linux.org/docs/yagcd/chap9.html
		
		There are two main commands we are expected and interpreting from the 
		console: The controller probing, and the controller polling. 
		
		The controller probing command is 000000001 (eight bit 0x00, and stop
		bit). This command is just sent to check for the existance of a 
		controller. If this is not answered, the Gamecube will just believe 
		there is no controller plugged in. The appropriate response is 24 bits
		in length. The first two bytes are the device ID. The ID we will respond
		with is 0x0900, the standard wired GC controller. The last byte is 
		a set of status flags. The meaning of these bits is unknown at this time,
		although it is believes bit 3 is the indicating if the rumble is running.
		For now, we will send 0x00 for this last byte. 
		
		The controller polling command is sent to check the current status of 
		all of the buttons and sticks. It also sends the command to the
		controller to turn on the rumble motors. This command is 24 bits in 
		length, and the first byte is always 0x40. The other two bytes are 
		arguments to this command. The only one we know for certain is the 
		very last bit of the last byte; if it is on, it wants the rumble 
		motor turned on. The polling command most often seen is 
		0x0302 or 0x0303 (same as the first, but activates rumble). Until/
		unless we get a better idea of what the two argument bytes are, we will
		only be checking the first byte; the other bits wont change how we 
		respond.
		
		Our response to the polling command is 8 bytes. The digital inputs are
		in the first two bytes, while the analogs take up the last six bytes.
	Byte0	0	0	0	Start	Y	X		B		A
	Byte1	1	L	R	Z		Up	Down	Right	Left
	Byte2 	Joystick X
	Byte3	Joystick Y
	Byte4	C-stick X
	Byte5	C-stick Y
	Byte6	Left trigger analog
	Byte7	Right trigger analog
	
		The analogs are all believed to be unsigned, most significant bit first.
		The midpoints for both sticks is 128, and the triggers are believed to
		be low numbers for released, and high numbers for pressed. The digital 
		L and R must be the tactile button that clicks when the trigger is 
		entirely pressed. For inputs showing the triggers as pressed, we should
		do both set the digital and change the analog.
		
		Oscope waveform in http://www.int03.co.uk/ shows that 0 means not pressed,
		and 1 means pressed; opposite of most systems.
		
		The first two bits in the response are the ERRSTAT and ERRLATCH. Both
		sound bad, and return 0 in a working controller. The third bit has been
		recorded as both 0 and 1, with no idea what it signifies.
		
		There are other commands, such as 0x41 and 0x42, that the Gamecube may
		send. Since they both deal with analog (get origins, and recalibrate)
		and we only have digital inputs, plus the fact we dont know the proper
		acknowledgement, these will be ignored.
		
		I recorded an interesting first command: 3 bytes, 0x01 0x81 0x35.
		I have no idea what this is for. 
		
		I am noticing that every byte I have seen that should be sent begins 
		with a 0. I wonder if this is on purpose. It'd be very nice if it was
		meant to be read on both sides like this:
			wait for initial 0
		begin byteread:
			read 7 bits
			read last bit
			if last bit is low, goto begin byteread; // 'nother byte coming
			else process the information just received. 
		This is probably a pipe dream though.	
	
		
	To make a UPCB cable for the GameCube, match up pins like this:
	D-Sub 15 Pin		GC Pin
	1					3, 4, or 7 (GND) 
	2					Low
	3					Low
	4					Low
	5					High
	6					Low
	7					Low
	8					1 (+5v)
	9					NC - Not connected to anything
	10					High
	11					High
	12					NC - Not connected to anything
	13					NC - Not connected to anything
	14					2 (Data)
	15					High
	The ground lines from the gamecube (GC pins 3, 4, and 7) are all tied 
	together on the console. It will be easier if you pick whichever one of
	the three looks easiest, and solder that to pin 1. You can clip the other
	two to make sure they stay out of the way, along with the +3.4 wire (GC pin 6).
		
*/

/* GC_DEBUG sets the output asm routines to set RC6's latch, instead of RC7's TRIS */
//#define GC_DEBUG

#ifndef GC_DEBUG
	#define GC_ASM_HIGH 		bsf TRISC, 7, ACCESS
#else
	#define GC_ASM_HIGH			bsf LATC, 6, ACCESS
#endif

#ifndef GC_DEBUG
	#define GC_ASM_LOW			bcf TRISC, 7, ACCESS
#else
	#define GC_ASM_LOW			bcf LATC, 6, ACCESS
#endif

//#define GC_READ_DEBUG
#ifdef GC_READ_DEBUG
	#define GCRD_HIGH bsf LATB, 4, ACCESS
	#define GCRD_LOW  bcf LATB, 4, ACCESS
#else
	#define GCRD_HIGH nop
	#define GCRD_LOW nop
#endif
	
#define GC_NOP _asm nop _endasm
#define GC_NOP2 GC_NOP GC_NOP
#define GC_NOP4 GC_NOP2 GC_NOP2
#define GC_NOP8 GC_NOP4 GC_NOP4
#define GC_NOP12 GC_NOP8 GC_NOP4
#define GC_NOP16 GC_NOP8 GC_NOP8
#define GC_NOP20 GC_NOP16 GC_NOP4
#define GC_NOP22 GC_NOP20 GC_NOP2
#define GC_NOP24 GC_NOP20 GC_NOP4

#define GC_2US_DELAY GCRD_LOW _endasm GC_NOP22 _asm GCRD_HIGH
#define GC_4US_DELAY GCRD_LOW _endasm GC_NOP8 GC_NOP24 GC_NOP24 _asm GCRD_HIGH

#define GC_ANALOG_HIGH 225
#define GC_ANALOG_LOW  32
#define GC_ANALOG_MID_HIGH 192
#define GC_ANALOG_MID_LOW  64
#define GC_ANALOG_MID  128

#define GC_MODE_BASIC 			0x01
#define GC_MODE_DPAD_ONLY		0x02
#define GC_MODE_ANALOG_ONLY		0x03
#define GC_MODE_SSB_BASIC		0x04
#define GC_MODE_SSB_ADV			0x05

#define GC_DOUBLETAP_WINDOW		45 //number of frames for a directional doubletap to register in SSB_BASIC mode

//#define GC_READBIT(bitnum) _endasm _asm btfsc PORTC, 5, ACCESS _endasm _asm bsf 0xfdf, bitnum, ACCESS 
#define GC_READBIT(bitnum) _endasm if(PORTCbits.RC5){byte_read|=1<<bitnum;} _asm
#define GC_OUT_HIGH()		TRISCbits.TRISC7=INPUT;
#define GC_OUT_LOW()		TRISCbits.TRISC7=OUTPUT;
#define GC_DATA_IN 			PORTCbits.RC7
#define GC_SET_INPUT()		TRISCbits.TRISC7=INPUT;
//#define DEBUGLINE			LATBbits.LATB4

#define SMASH_ADVANCED_ORIG
//#define SMASH_ADVANCED_GK



/* macros */


/* Prototypes */
void GC_Init_Pins(void);
void GC_main(void);
void SendResponse(unsigned char *buffer, unsigned char bitsize);
void GC_Answer_Query(void);

#endif //GC_H		