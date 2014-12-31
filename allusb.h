#ifndef ALLUSB_H
#define ALLUSB_H

/*
UPCB - Universal Programmed Controller Board 
Copyright (C) 2007  Marcus Post marcus@marcuspost.com
Based on USB code framework 'lab2' Copyright (C) 2005 Bradley A. Minch

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


/* UPCB template module for USB support.
	For different USB based systems, I am expecting that the USB sections will 
	have to be used repeatedly in different manners. As of the 1.C (1.12) version, 
	the core USB code has been moved over to Brad Minch's USB stack for PICs. 
	
	This file, allusb.h, contains the #defines and declarations that will be used
	by every USB module, including the variables declared in allusb.c. Each USB 
	susbsystem, such as PC, Playstation 3, and Xbox, will have separate .c and .h
	files, reusing the data and functions from allusb.* as much as possible.  
	
	I will not go into the details of USB communication right now, mainly 
	because I do not understand it well, but also because it's complicated
	and just not neccessary. 
	
	USB Pinout
	(Looking at the end of a USB cable that plugs into a PC or other USB port)
	___________
	| 4 3 2 1 |
	| _ _ _ _ |
	-----------    <- thicker half of cable where the contacts are.
	
	Pin		Description
	1		VCC
	2		D-	
	3		D+
	4		GND
	
	Making a UPCB USB cable. 
	For each USB plug system supported, there will be a different pinout for a 
	system specific cable; i.e. a cable for PS3 only, a cable for PC only, a 
	cable for using the USB bootloader, etc. However, this does not mean you 
	HAVE to make a different cable for everything. There is also a button select
	cable that will allow you to use the same cable on all USB systems that use 
	a standard 'Type A' socket. The button select cable is HIGHLY recommended 
	for all users to make certain they have the ability to use the USB bootloader
	for future upgrades. 
	
	To Make a UPCB cable for Button Select USB
	D-Sub 15 Pin		USB Pin
	1					4 (GND)
	2					High
	3					Low
	4					Low
	5					Low
	6					Low
	7					Low
	8					1 (VCC)
	9					NC - Not connected to anything
	10					High
	11					High
	12					NC - Not connected to anything
	13					2 (D-)
	14					3 (D+)
	15					Low
	
	To Make a UPCB cable for USB Bootloader (reprogramming the UPCB)
	D-Sub 15 Pin		USB Pin
	1					4 (GND)
	2					High
	3					Low
	4					Low
	5					Low
	6					Low
	7					Low
	8					1 (VCC)
	9					NC - Not connected to anything
	10					High
	11					High
	12					NC - Not connected to anything
	13					2 (D-)
	14					3 (D+)
	15					High
	
	To Make a UPCB cable for PC USB HID
	D-Sub 15 Pin		USB Pin
	1					4 (GND)
	2					High
	3					Low
	4					Low
	5					High
	6					Low
	7					Low
	8					1 (VCC)
	9					NC - Not connected to anything
	10					High
	11					High
	12					NC - Not connected to anything
	13					2 (D-)
	14					3 (D+)
	15					Low
	
	To Make a UPCB cable for Playstation 3 USB
	D-Sub 15 Pin		USB Pin
	1					4 (GND)
	2					High
	3					Low
	4					High
	5					Low
	6					Low
	7					Low
	8					1 (VCC)
	9					NC - Not connected to anything
	10					High
	11					High
	12					NC - Not connected to anything
	13					2 (D-)
	14					3 (D+)
	15					High
	
*/

// Define the states that the USB interface can be in
#define	POWERED_STATE	0x00
#define	DEFAULT_STATE	0x01
#define	ADDRESS_STATE	0x02
#define	CONFIG_STATE	0x03

// Define the states for Control EndPoints
#define	EP_IDLE_STATE		0x00
#define	EP_SETUP_STATE		0x01
#define	EP_DISABLED_STATE	0xff

#define	ENDPT_DISABLED		0x00
#define ENDPT_IN_ONLY		0x12
#define ENDPT_OUT_ONLY		0x14
#define ENDPT_CONTROL		0x16	// enable for in, out and setup
#define ENDPT_NON_CONTROL	0x1E	// enable for in, and out

#define INT_STAT_MASK_RESET	0x01
#define INT_STAT_MASK_ERROR	0x02
#define INT_STAT_MASK_TOKEN_DONE	0x04
#define INT_STAT_MASK_SLEEP	0x08
#define INT_STAT_MASK_STALL	0x10

#define TOKEN_OUT	(0x01<<2)
#define TOKEN_ACK	(0x02<<2)
#define TOKEN_IN	(0x09<<2)
#define TOKEN_SETUP	(0x0D<<2)

// standard descriptor types
#define	DEVICE		1
#define	CONFIGURATION	2
#define	STRING		3
#define	INTERFACE	4
#define	ENDPOINT	5

// HID class descriptor types
#define HID			0x21
#define REPORT		0x22
#define PHYSICAL	0x23

// offsets from the beginning of the setup data record
#define	bmRequestType	0x00
#define	bRequest	0x01
#define	wValue		0x02
#define	wValueHigh	0x03
#define	wIndex		0x04
#define	wIndexHigh	0x05
#define	wLength		0x06
#define	wLengthHigh	0x07

// Standard USB requests
#define NO_REQUEST		0xFF
#define	GET_STATUS		0x00
#define	CLEAR_FEATURE	0x01
#define	SET_FEATURE		0x03
#define	SET_ADDRESS		0x05
#define	GET_DESCRIPTOR	0x06
#define SET_DESCRIPTOR	0x07
#define	GET_CONFIGURATION	0x08
#define	SET_CONFIGURATION	0x09
#define	GET_INTERFACE	0x0A
#define	SET_INTERFACE	0x0B
#define SYNCH_FRAME		0x0C

// HID Class requests
#define GET_REPORT		0x01
#define GET_IDLE		0x02
#define GET_PROTOCOL	0x03
#define SET_REPORT		0x09
#define SET_IDLE		0x0A
#define SET_PROTOCOL	0x0B

#define	GET_STRING_DESCRIPTOR	0x66
#define	HID_SET_REPORT		0x21
#define	VEND_SET_MEMORY		0x80

#define	SVCUSBINT		0x01 << 2
#define	SVCTOKENDONE		0x02 << 2
#define	SVCRESET		0x03 << 2
#define	SVCSLEEP		0x04 << 2
#define	SVCSTALL		0x05 << 2
#define	SVCERROR		0x06 << 2
#define	SVCACTIVITY		0x07 << 2
#define	TOKENOUT		0x08 << 2
#define	TOKENIN			0x09 << 2
#define	TOKENSETUP		0x0A << 2
#define	CLEARFEATURE		0x0B << 2
#define	GETCONFIG		0x0C << 2
#define	GETDESCRIPTOR		0x0D << 2
#define	GETINTERFACE		0x0E << 2
#define	GETSTATUS		0x0F << 2
#define	SETADDRESS		0x10 << 2
#define	SETCONFIG		0x11 << 2
#define	SETFEATURE		0x12 << 2
#define	SETINTERFACE		0x13 << 2
#define	FINISHSETADDRESS	0x14 << 2
#define	COPYDESC2EP0		0x15 << 2
#define	COPYSTRINGDESC2EP0	0x16 << 2
#define	ZEROLENPACKET		0x17 << 2

#define EP0				0x00 << 3
#define EP1				0x01 << 3
#define EP2				0x02 << 3

#define STANDARD		0x00 << 5
#define CLASS			0x01 << 5
#define VENDOR			0x02 << 5

#define RECIPIENT_DEVICE	0x00
#define RECIPIENT_INTERFACE	0x01
#define RECIPIENT_ENDPOINT	0x02

#define DEVICE_REMOTE_WAKEUP	0x01
#define ENDPOINT_HALT			0x00

typedef struct BUFDESC {
	unsigned char status;
	unsigned char bytecount;
	unsigned char *address;
} BUFDESC;


extern far unsigned char USBBUFFER4[0x100];
extern far unsigned char USBBUFFER5[0x100];
extern far BUFDESC *BDPTR;
extern far unsigned char *FREEUSBBUFFER;
extern unsigned char USB_curr_config;
extern unsigned char USB_idle_rate;
extern unsigned char USB_USWSTAT;
extern unsigned char USB_device_status;
extern unsigned char USB_protocol;
extern unsigned char USB_dev_req;

extern unsigned char USB_buffer_data[8];
extern unsigned char USB_error_flags;
extern unsigned char USB_address_pending;
extern unsigned char rom *USB_desc_ptr;
extern unsigned char USB_bytes_left;
extern unsigned char USB_packet_length;
extern unsigned char USB_USTAT;
extern BUFDESC USB_buffer_desc;

#define  BD0O BDPTR[0]
#define  BD0I BDPTR[1]
#define  BD1O BDPTR[2]
#define  BD1I BDPTR[3]
#define  BD2O BDPTR[4]
#define  BD2I BDPTR[5]
#define  BD3O BDPTR[6]
#define  BD3I BDPTR[7]
#define  BD4O BDPTR[8]
#define  BD4I BDPTR[9]
#define  BD5O BDPTR[10]
#define  BD5I BDPTR[11]
#define  BD6O BDPTR[12]
#define  BD6I BDPTR[13]
#define  BD7O BDPTR[14]
#define  BD7I BDPTR[15]
#define  BD8O BDPTR[16]
#define  BD8I BDPTR[17]
#define  BD9O BDPTR[18]
#define  BD9I BDPTR[19]
#define  BD10O BDPTR[20]
#define  BD10I BDPTR[21]
#define  BD11O BDPTR[22]
#define  BD11I BDPTR[23]
#define  BD12O BDPTR[24]
#define  BD12I BDPTR[25]
#define  BD13O BDPTR[26]
#define  BD13I BDPTR[27]
#define  BD14O BDPTR[28]
#define  BD14I BDPTR[29]
#define  BD15O BDPTR[30]
#define  BD15I BDPTR[31]




#endif 