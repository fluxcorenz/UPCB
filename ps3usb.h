
#ifndef PS3USB_H
#define PS3USB_H

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

#include "allusb.h"

/* Report details:
			0 - report id? Constant 0x01
			1 - unknown - constant  0x00
			2 - digital bits
				(Dpad) LDRU Start R3 L3 Select
			3 - digital bits
				square x circle triangle R1(?) L1 R2 L2
			4 - digital bits 
				LSB is 'home' button. rest are unknown (byte is 0x01 if home pressed, 0x00 otherwise)
			5 - unknown - constant 0x00				
			6 - left analog X axis - unsigned left low(0x00), right high(0xFF), center 0x80
			7 - left analog Y axis - unsigned up low(0x00), down high(0xFF), center 0x80
			8 - right analog X axis - unsigned left low(0x00), right high(0xFF), center 0x80
			9 - right analog Y axis - unsigned up low(0x00), down high(0xFF), center 0x80
			10, 11, 12, 13  -  unknown - constant 0x00
			14 - Dpad Up analog - 0x00 unpressed, 0xFF max pressed 
			15 - Dpad Right analog - 0x00 unpressed, 0xFF max pressed
			16 - Dpad Down analog - 0x00 unpressed, 0xFF max pressed
			17 - Dpad Left analog - 0x00 unpressed, 0xFF max pressed
			18 - Dpad L2 analog - 0x00 unpressed, 0xFF max pressed
			19 - Dpad R2 analog - 0x00 unpressed, 0xFF max pressed
			20 - Dpad L1 analog - 0x00 unpressed, 0xFF max pressed
			21 - Dpad R1 (??) analog - 0x00 unpressed, 0xFF max pressed
			22 - Dpad triangle analog - 0x00 unpressed, 0xFF max pressed
			23 - Dpad circle analog - 0x00 unpressed, 0xFF max pressed
			24 - Dpad X analog - 0x00 unpressed, 0xFF max pressed
			25 - Dpad square (??) analog - 0x00 unpressed, 0xFF max pressed
			26, 27, 28 - unknown - constant 0x00
			29 - unknown, constant 0x03 //pelican has 0x02
			30 - unknown constant 0xEF //matches pelican
			31 - unknown constant 0x14 // matches pelican
			32, 33, 34, 35 - unknown constant 0x00 //matches pelican
			36 - unknown constant 0x23 //matches pelican
			37 - unknown constant 0x7f //pelican has 0x18
			38 - unknown constant 0x77 //matches pelican
			39 - unknown constant 0x01 //matches pelican
			40 - unknown constant 0x81 //pelican has 1D
			(three axises from the accelerometer, two bytes each
			41, 42, 43, 44, 45, 46 - accellerometer data
			first byte of each pair seems to vary from 01 to 02, in a way that definitely implies
			the accelerometer is reporting 10 bits of data per axis. What's weird is that the 
			pelican always reports the first byte of each pair as 02, and 0x00 for the second byte
			47 - unknown constant 0x00 //matches pelican
			48 - unknown constant 0x02 //matches pelican
						
			*/

#define	PS3USB_NUM_CONFIGURATIONS	1
#define	PS3USB_NUM_INTERFACES		1
#define PS3USB_NUM_STRINGS			2
#define PS3USB_EP0_PACKET_SIZE		0x40	// maximum packet size for low-speed peripherals is 8 bytes, for full-speed peripherals it can be 8, 16, 32, or 64 bytes
#define PS3USB_EP1_PACKET_SIZE		0x40 


#define PS3USB_REPORT_SIZE			0x13 

#define PS3USB_EP0_OUT_buffer USBBUFFER5[0]
#define PS3USB_EP0_IN_buffer USBBUFFER5[PS3USB_EP0_PACKET_SIZE]
#define PS3USB_EP1_IN_buffer USBBUFFER5[PS3USB_EP0_PACKET_SIZE*2]
//#define PS3USB_EP2_OUT_buffer USBBUFFER5[(PS3USB_EP0_PACKET_SIZE*2) + PS3USB_EP1_PACKET_SIZE]


//Defines for USB_STICK_MODE. 
#define USM_BOTH 0
#define USM_DIGITAL_ONLY 1
#define USM_ANALOG_ONLY 2

void PS3USB_main(void);
void PS3USB_SendReport(void);
void PS3USB_SendDescriptorPacket(void);
void PS3USB_ProcessOutToken(void);
void PS3USB_ProcessInToken(void);
void PS3USB_VendorRequests(void);
void PS3USB_ClassRequests(void);
void PS3USB_StandardRequests(void);
void PS3USB_ProcessSetupToken(void);
void PS3USB_ServiceUSB(void);
void PS3USB_InitUSB(void);


#endif