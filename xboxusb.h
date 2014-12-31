#ifndef XBOXUSB_H
#define XBOXUSB_H
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

/* UPCB module for Xbox support
	Pinout:
	Looking at the plug that goes into the Xbox 
	
	 /-\/\/--\
	|         |
	\4 5 3 2 1/
	 ---------       <-thicker wall with the five contacts on it.
	 
	Pin	 Name 	Function		Wire Color
	1 	VCC 	+5v 			Red
	2 	D- 		USB wire D-		White
	3 	D+ 		USB wire D+		Green
	4 	GND		Ground			Black/Blue
	5 	Unknown	Unknown			Yellow
	
	It's uncertain what exactly the fifth, yellow wire is for. Some say it is for 
	detecting the television refresh rate for use with light gun peripherals. All
	I do know for certain is that we don't need it. As always, the colors listed 
	are the usual suspects and seem fairly standard, but rather than put your 
	console at risk, ALWAYS use a multimeter to check the wires for their matching
	pins.
	 
	Xbox controller ports use a 5 pin connector and communicate using standard USB standards. 
	Unlike other controller, the Xbox controller is NOT releated to the HID class of devices, 
	nor any other class or standard. The genuine Xbox controllers actually contain a three port 
	USB hub, to which the controller itself is connected, along with the two available memory
	card and headset ports, which are themselves USB devices. This code will emulate only the
	controller; the UPCB doesn't have the means or interest to simultaneously support a hub and
	controller. 
	
	The controller is a high speed USB device. The endpoints EP0 OUT and EP0 IN are present 
	just like just about every other USB device. The main communication between the controller
	and the Xbox takes place in endpoints EP2 IN and EP2 OUT. EP2 IN is setup with interrupt 
	requests every 4 milliseconds; more than four updates per frame. Each transmission is a 
	twenty byte packet containing the digital and analog status of the controller. EP2 OUT 
	is setup similarly, but receives a 6 byte packet from the Xbox controlling the rumble 
	motors.
	
	To make a UPCB cable for this system:
	D-Sub Pin       Xbox Pin
	1				4 (GND)
	2				High (Connected to VCC; D-sub pin 8)
	3				Low  (Connected to GND; D-sub pin 1)
	4				Low
	5				High 
	6				Low
	7				Low
	8				1 (VCC)
	9				NC - Not connected to anything
	10				High
	11				High
	12				NC - Not connected to anything
	13				2 (D-)
	14				3 (D+)
	15				High

*/

#include "allusb.h"



#define	XBOXUSB_NUM_CONFIGURATIONS	1
#define	XBOXUSB_NUM_INTERFACES		1
#define XBOXUSB_NUM_STRINGS			0
#define XBOXUSB_EP0_PACKET_SIZE		0x20	// maximum packet size for low-speed peripherals is 8 bytes, for full-speed peripherals it can be 8, 16, 32, or 64 bytes
#define XBOXUSB_EP2_PACKET_SIZE		0x20 
#define XBOXUSB_REPORT_SIZE		20 //not a typo. twenty, decimal. Not to be confused with 0x20 buffers above

#define XBOXUSB_EP0_OUT_buffer USBBUFFER5[0]
#define XBOXUSB_EP0_IN_buffer USBBUFFER5[XBOXUSB_EP0_PACKET_SIZE]
#define XBOXUSB_EP2_IN_buffer USBBUFFER5[XBOXUSB_EP0_PACKET_SIZE*2]
#define XBOXUSB_EP2_OUT_buffer USBBUFFER5[(XBOXUSB_EP0_PACKET_SIZE*2) + XBOXUSB_EP2_PACKET_SIZE]


void XBOXUSB_main(void);
void XBOXUSB_SendReport(void);
void XBOXUSB_SendDescriptorPacket(void);
void XBOXUSB_ProcessOutToken(void);
void XBOXUSB_ProcessInToken(void);
void XBOXUSB_VendorRequests(void);
void XBOXUSB_ClassRequests(void);
void XBOXUSB_StandardRequests(void);
void XBOXUSB_ProcessSetupToken(void);
void XBOXUSB_ServiceUSB(void);
void XBOXUSB_InitUSB(void);


#endif