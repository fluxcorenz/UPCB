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
#include "upcb.h"
#include <p18cxxx.h>
#pragma udata USB_BUFFER_DESCRIPTOR_TABLE=0x0400
unsigned char USBBUFFER4[0x100];
#pragma udata usbram5=0x0500
unsigned char USBBUFFER5[0x100];

#pragma udata
BUFDESC *BDPTR=USBBUFFER4;
//far unsigned char *FREEUSBBUFFER=&BDPTR[32];
far unsigned char *FREEUSBBUFFER=&USBBUFFER4[0x80];

unsigned char USB_curr_config;
unsigned char USB_idle_rate;
unsigned char USB_USWSTAT;
unsigned char USB_device_status;
unsigned char USB_protocol;
unsigned char USB_dev_req;

unsigned char USB_buffer_data[8];
unsigned char USB_error_flags;
unsigned char USB_address_pending;
unsigned char rom *USB_desc_ptr;
unsigned char USB_bytes_left;
unsigned char USB_packet_length;
unsigned char USB_USTAT;
BUFDESC USB_buffer_desc;

//#pragma code
