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
#include <p18cxxx.h>
#include "ps3usb.h"
#include "upcb.h"
#include "allusb.h"
#include "timers.h"
	//#pragma udata gpr1=0x100
	//unsigned char recordbuffer[256];
#pragma udata
unsigned char PS3USB_USBbuffer[PS3USB_REPORT_SIZE];
unsigned char SETADDR_BEFORE_GETDESCR;
unsigned char GETDESCR_BEFORE_SETADDR;
unsigned char PS3_DETECTED;

#pragma romdata
//Updated for VSHG - checked
rom const unsigned char PS3USB_Device_Descriptor[] = {
	0x12, //size of this descriptor in bytes
	DEVICE, //descriptor type = PS3USB_Device_Descriptor
	0x10, 0x01,	 //USB Spec 2.0
	0x00, 		//class code
	0x00,		//subclass code
	0x00,		//protocol code
	0x40,		//EPO buffer size
	 0xC4, 0x10,	//vendor ID 
	 0xC0, 0x82, //product ID
	//0xC5, 0x11,	//vendor ID 
	//0xC1, 0x83, //product ID
	0x06, 0x00, //device release number
	0x01, 		//manufacturer string index
	0x02, 		//product string index
	0x00,		//device serial number string index
	0x01		//# of possible configurations
};

rom const unsigned char PCUSB_Device_Descriptor[] = {
	0x12, //size of this descriptor in bytes
	DEVICE, //descriptor type = PS3USB_Device_Descriptor
	0x10, 0x01,	 //USB Spec 2.0
	0x00, 		//class code
	0x00,		//subclass code
	0x00,		//protocol code
	0x40,		//EPO buffer size
	0xD8, 0x04,	//vendor ID 04D8
	REVISION_BYTE, CONFIG_BYTE, //product ID
	0x06, 0x00, //device release number
	0x01, 		//manufacturer string index
	0x02, 		//product string index
	0x00,		//device serial number string index
	0x01		//# of possible configurations
};


//partially updated to match VSHG; checked
 rom const unsigned char PS3USB_Configuration_Descriptor[] = {
	 0x09, 			//size of this desc in bytes
	 CONFIGURATION, //type of desc
	 0x22, 	0x00,		//size of total descriptor
	 0x01, 			//# of interfaces
	 0x01,			//index number of this config
	 0x00,			//config string index
	 0x80, 			//attributes:Default
	 0x32,			//power consumption * 2mA
	//Interface portion
	 0x09,			//size of this desc in bytes
	 INTERFACE,		//type of desc
	 0x00,			//interface number (ordinal)
	 0x00,			//alternate setting number
	 0x01,			//number of endpoints (not including EP0)
	 0x03,			//Interface class - HID
	 0x00,			//sublass 
	 0x00,			//protocol code - mouse
	 0x00,			//interface string index
	 
	 //HID portion
	 0x09,			//size of this descriptor in bytes
	 HID,			//type of descriptor: HID
	 0x11, 0x01,	//HID spec release number
	 0x00,			//country code (0x00 for not supported)
	 0x01,			//number of class descriptors
	 REPORT,			//Report descriptor type
	 0x70, 0x00, 	//size of report descriptor
	 
	 //endpoint portion
	 0x07,		//size of desc in bytes
	 ENDPOINT,	//type od desc
	 0x81,		//endpoint address EP1 IN
	 0x03,		//transfer style: interrupt
	 0x40, 0x00,	//max packet size : 64 bytes
	 0x0A,		//interval: 10ms 
	 
};

//HID report from VSHG; need to double check.
rom const unsigned char PS3USB_Report_Descriptor[] = { 
 0x05, 0x01,					// USAGE_PAGE (Generic Desktop)
 0x09, 0x05,	  				//Usage gamepad
 0xA1, 0x01,					// COLLECTION (Application)
 0x15, 0x00,					//   LOGICAL_MINIMUM (0)
 0x25, 0x01,					//   LOGICAL_MAXIMUM (1) --10
 0x35, 0x00,					//   PHYSICAL_MINIMUM (0)
 0x45, 0x01, 					 //	PHYSICAL_MAXIMUM (1)
 0x75, 0x01, 					//   REPORT_SIZE (1)
 0x95, 0x0d,					//   REPORT_COUNT (13)
 0x05, 0x09,					//   USAGE_PAGE (Button) --20
 0x19, 0x01, 					//   USAGE_MINIMUM (Button 1)
 0x29, 0x0d, 					//   USAGE_MAXIMUM (Button 13)
 0x81, 0x02, 					//   INPUT (Data,Var,Abs)
 0x95, 0x03, 					//     REPORT_COUNT (3)
 0x81, 0x01, 					//Input something or other --30
 0x05, 0x01,					// USAGE_PAGE (Generic Desktop)
 0x25, 0x07, 					//   LOGICAL_MAXIMUM (7)
 0x46, 0x3b, 0x01, 				//physical maximum 315
 0x75, 0x04, 					//Report size 4    --39
 0x95, 0x01, 					//Report count 1
 0x65, 0x14,					//unit: English angular position
 0x09, 0x39, 					//usage: hat switch
 0x81, 0x42,					// Input something or other
 0x65, 0x00, 					//Unit:none    --49
 0x95, 0x01, 					//report count 1
 0x81, 0x01, 					//Input something or other
 0x26, 0xff, 0x00, 				// LOGICAL_MAXIMUM (255)
 0x46, 0xff, 0x00, 			// PHYSICAL_MAXIMUM (255)  --59
 0x09, 0x30, 
 0x09, 			//--62
 0x31, 0x09,
 0x32, 0x09, 
 0x35, 0x75, 
 0x08, 0x95, 
 0x04, 0x81,   //--72
 0x02, 0x06, 
 0x00, 0xff, 
 0x09, 0x20, 
 0x09, 0x21,
 0x09, 0x22,   //82
 0x09, 0x23, 
 0x09, 0x24, 
 0x09, 0x25, 
 0x09, 0x26,   //90
 0x09, 0x27, 
 0x09, 0x28, 
 0x09, 0x29,
 0x09, 0x2a, 
 0x09, 0x2b,  //100
 0x95, 0x0c, 
 0x81, 0x02, 
 0x0a, 0x21, 
 0x26, 0x95, 
 0x08, 0xb1, 
 0x02, 0xc0   //--112 == 0x70
 };
 
/*	
rom const unsigned char PS3USB_String0[] = {
	4,	// bLength
	STRING,	// bDescriptorType
	0x09,	// wLANGID[0] (low byte)
	0x04	// wLANGID[0] (high byte)
};

rom const unsigned char PS3USB_String1[] = {
	10,	// bLength
	STRING,	// bDescriptorType
	'S',0x00, 'o', 0x00, 'n', 0x00, 'y', 0x00};

rom const unsigned char PS3USB_String2[] = {
	54,	// bLength -1
	STRING,	// bDescriptorType  //-2
	'P', 0x00,'L', 0x00,'A', 0x00,'Y', 0x00,'S', 0x00, //12
	'T', 0x00,'A', 0x00,'T', 0x00,'I', 0x00,'O', 0x00, //22
	'N', 0x00,'(', 0x00,'R', 0x00, ')', 0x00,'3', 0x00, //32
	' ', 0x00, 'C', 0x00, 'o', 0x00,'n', 0x00,'t', 0x00,//42
	'r', 0x00,'o', 0x00,'l', 0x00,'l', 0x00,'e', 0x00,  //52
	'r', 0x00,  //54
	};
*/

//checked with VSHG
rom const unsigned char PS3USB_String0[] = {
	4,	// bLength
	STRING,	// bDescriptorType
	0x09,	// wLANGID[0] (low byte)
	0x04	// wLANGID[0] (high byte)
};

//from VSHG
rom const unsigned char PS3USB_String1[] = {
	10, 3, 83, 0, 69, 0, 71, 0, 65, 0 
	}; 
//From VSHG
rom const unsigned char PS3USB_String2[] = {
	48, 3, 86, 0, 73, 0, 82, 0, 84, 0, 85, 0, //12
	65, 0, 32, 0, 83, 0, 84, 0, 73, 0, 67, 0, //24
	75, 0, 32, 0, 72, 0, 105, 0, 103, 0, 104, 0, //36
	32, 0, 71, 0, 114, 0, 97, 0, 100, 0, 101, 0 }; //48
	

rom const unsigned char PCUSB_String0[] = {
	4,	// bLength
	STRING,	// bDescriptorType
	0x09,	// wLANGID[0] (low byte)
	0x04	// wLANGID[0] (high byte)
};
 
rom const unsigned char PCUSB_String1[] = {
	26,	// bLength  --1
	STRING,	// bDescriptorType --2
	'T', 0x00,'o', 0x00,'o', 0x00, //--8
	'd', 0x00,'l', 0x00,'e', 0x00,'s', 0x00,' ', 0x00, //-18
	'2', 0x00,'0', 0x00,'0', 0x00, '7', 0x00}; //-26

rom const unsigned char PCUSB_String2[] = {
	42,	// bLength
	STRING,	// bDescriptorType
	'U', 0x00,'n', 0x00,'i', 0x00,  // -8
	'v', 0x00,'e', 0x00,'r', 0x00,'s', 0x00,'a', 0x00, //-18
	'l', 0x00,' ', 0x00,'P', 0x00,'C', 0x00,'B', 0x00, //-28
	' ', 0x00, 'v', 0x00, MAJOR_REVISION, 0x00, '.', 0x00, MINOR_REVISION, 0x00,  //-38
	OPTION_MODE, 0x00, PROG_OPTION_MODE, 0x00 }; //-42	

#pragma code

void PS3USB_InitUSB(void) {
	UIE = 0x00;					// mask all USB interrupts
	UIR = 0x00;					// clear all USB interrupt flags
	//UCFG = 0x10;				// configure USB for low-speed transfers and to use the on-chip transciever and pull-up resistor
	UCFG = 0x14;		//we need full speed
	UCON = 0x08;				// enable the USB module and its supporting circuitry
	USB_curr_config = 0x00;
	USB_idle_rate = 0x00;
	USB_USWSTAT = 0x00;			// default to powered state
	USB_device_status = 0x01;
	USB_protocol = 0x01;		// default to report protocol initially
	USB_dev_req = NO_REQUEST;	// No device requests in process
	while (UCONbits.SE0);		// wait for the first SE0 to end
}

void PS3USB_ServiceUSB(void) {
	BUFDESC *buf_desc_ptr;

	if (UIRbits.UERRIF) { //if there's been a USB error of some sort...
		UEIR = 0x00;   //Clear out all of the error flags 
	} else if (UIRbits.SOFIF) { //if there's a start of frame error
		UIRbits.SOFIF = 0;      //clear it
	} else if (UIRbits.IDLEIF) {  //if there's an idle detect (3ms of silence)
		UIRbits.IDLEIF = 0;    //clear the flag
		UCONbits.SUSPND = 1;  //suspend the USB module
	} else if (UIRbits.ACTVIF) { //activity on the D+/D- lines
		UIRbits.ACTVIF = 0;     //clear the flag
		UCONbits.SUSPND = 0;    //and wakeup from suspend 
			/* what if not suspended? does the ACTIVF not get set for good communication?
				or is the Interrupt enable for the ACTVIF flag only enabled on suspend?
				Because the IDLE doesn't look like it sets it. */
	} else if (UIRbits.STALLIF) { //if a STALL handshake received..
		UIRbits.STALLIF = 0;		//clear it
	} else if (UIRbits.URSTIF) { //if we're told to reset
		/* drop everything. Set address to 0x00, flush every buffer
			enable the EP0 control buffer as normal. */
		USB_curr_config = 0x00;  
		UIRbits.TRNIF = 0;		// clear TRNIF four times to clear out the USTAT FIFO
		UIRbits.TRNIF = 0;
		UIRbits.TRNIF = 0;
		UIRbits.TRNIF = 0;
		UEP0 = 0x00;				// clear all EP control registers to disable all endpoints
		UEP1 = 0x00;
		UEP2 = 0x00;
		UEP3 = 0x00;
		UEP4 = 0x00;
		UEP5 = 0x00;
		UEP6 = 0x00;
		UEP7 = 0x00;
		UEP8 = 0x00;
		UEP9 = 0x00;
		UEP10 = 0x00;
		UEP11 = 0x00;
		UEP12 = 0x00;
		UEP13 = 0x00;
		UEP14 = 0x00;
		UEP15 = 0x00;   		
		BD0O.bytecount = PS3USB_EP0_PACKET_SIZE;
		BD0O.address = &PS3USB_EP0_OUT_buffer;	// EP0 OUT gets a buffer
		BD0O.status = 0x88;				// set UOWN bit (USB can write)
		BD0I.address = &PS3USB_EP0_IN_buffer;	// EP0 IN gets a buffer
		BD0I.status = 0x08;				// clear UOWN bit (MCU can write)
		UADDR = 0x00;				// set USB Address to 0
		UIR = 0x00;				// clear all the USB interrupt flags
		UEP0 = ENDPT_CONTROL;	// EP0 is a control pipe and requires an ACK
		UEIE = 0xFF;			// enable all error interrupts
		USB_USWSTAT = DEFAULT_STATE;
		USB_device_status = 0x01;	// self powered, remote wakeup disabled
	} else if (UIRbits.TRNIF) { //transcation complete flag. Some transaction has completed properly
			//need to check the stats to see what happened. 
	
		buf_desc_ptr = (BUFDESC *)((unsigned char *)(&BD0O)+(USTAT&0x7C));	// mask out bits 0, 1, and 7 of USTAT for offset into the buffer descriptor table
		//the masked buffer descriptor should tell us which endpoint something happened in.
		//Copy the contents of the buffer descriptor to the global variable so we can examine it.
		USB_buffer_desc.status = buf_desc_ptr->status;
		USB_buffer_desc.bytecount = buf_desc_ptr->bytecount;
		USB_buffer_desc.address = buf_desc_ptr->address;
		USB_USTAT = USTAT;		// save the USB status register
		UIRbits.TRNIF = 0;		// clear TRNIF interrupt flag
		USB_error_flags = 0x00;	// clear USB error flags
		
		//now that we have the details from the buffer descriptor, figure out what to do with it.
		
		switch (USB_buffer_desc.status&0x3C) {	// extract PID bits to see what kind of token it is.
				//and then process it.
			case TOKEN_SETUP:
				PS3USB_ProcessSetupToken();
				break;
			case TOKEN_IN:
				PS3USB_ProcessInToken();
				break;
			case TOKEN_OUT:
				PS3USB_ProcessOutToken();
				break;
			case TOKEN_ACK:
				break;	
			default:
				mLED_2_Toggle();
				mLED_2_Toggle();
				mLED_2_Toggle();
				break;
		}
		if (USB_error_flags&0x01) {		// if there was a Request Error...
			BD0O.bytecount = PS3USB_EP0_PACKET_SIZE;	// ...get ready to receive the next Setup token...
			BD0I.status = 0x84;
			BD0O.status = 0x84;					// ...and issue a protocol stall on EP0
		}
	}
}

void PS3USB_ProcessSetupToken(void) {
	unsigned char n;
	//okay, if we're here, we successfully finished some chunk of a transfer, and the PID from the 
	//transfer told us it was a setup token. 
	
	//I dont like the hard coded 8 bytes here, but Setup packets are all 8 bytes according to spec.
	for (n = 0; n<8; n++) {
		USB_buffer_data[n] = USB_buffer_desc.address[n];
		//recordbuffer[saveptr] = USB_buffer_desc.address[n];
		//saveptr++;
		//if (0xF9 < saveptr)
		//{
		//	mLED_2_Off();
		//	mLED_2_On();
		//	mLED_2_Off();
		//}
	}
	
	BD0O.bytecount = PS3USB_EP0_PACKET_SIZE;	// reset the EP0 OUT byte count
	/* So the bytecount for a buffer descriptor counts DOWN? not to sure I like this. 
		Need to read up on BD's in the datasheet. */
	BD0I.status = 0x08;			// return the EP0 IN buffer to us (dequeue any pending requests)			
	BD0O.status =  //chopping this line up for readability
		(!(USB_buffer_data[bmRequestType]&0x80) && (USB_buffer_data[wLength] || USB_buffer_data[wLengthHigh])) 
		? 0xC8 : 0x88;	// set EP0 OUT UOWN back to USB and DATA0/DATA1 packet according to the request type
		/* Translation: if the length is non-zero and the data was from the host to the device
			then set status to 0xC8; otherwise 0x88.
			Need more information on the details of the status byte. The Summary on datasheet 
			page 178 doesn't help too much. The first bit, set on both to 1, is the ownership of the 
			buffer. bits 6 and 3 I dont get yet.
		*/
	UCONbits.PKTDIS = 0;			// assuming there is nothing to dequeue, clear the packet disable bit
		//PKTDIS is supposedly automagically set when a setup token is received, this clears it so work may
		//continue
	USB_dev_req = NO_REQUEST;		// clear the device request in process
		//USB_dev_req seems to be a global flag, almost a state machine indicator. Holds the current
		//type of request received. 
	switch (USB_buffer_data[bmRequestType]&0x60) {	// extract request type bits
		//get bits 6 and 5 from the first byte. These bits hold the Type of request. 
		//Standard=00, class=01, vendor=10, reserved=11 (defines are the number >>5)
		//figure which it is, and process with that function.
		
		//Damn, so the setup packet has multiple sub packet possibilities. lots to keep track of. :(
		case STANDARD:
			PS3USB_StandardRequests();
			break;
		case CLASS:
			PS3USB_ClassRequests();
			break;
		case VENDOR:
			PS3USB_VendorRequests();
			break;
		default:
			USB_error_flags |= 0x01;	// set Request Error Flag
	}
}

void PS3USB_StandardRequests(void) {
	//we've successfully got a token in, identified it as a Setup token, flushed and took over the EP0
	//buffer, and identified it as a Standard request. 
	unsigned char *UEP;
	unsigned char n;
	BUFDESC *buf_desc_ptr;

	switch (USB_buffer_data[bRequest]) {  //finally checking the second bit, data[1]
 
		case GET_STATUS:  // 0x00. Expects a two byte status result; only a couple of bits are actually 
			//defined. Different results based on if its to the device, interface or endpoint.
			switch (USB_buffer_data[bmRequestType]&0x1F) {	// extract request recipient bits
				//pulls out the lowest 5 bits from the first byte to see if its for device, intf, or endpoint
				case RECIPIENT_DEVICE:
					BD0I.address[0] = USB_device_status;
					//bit 0 : true if device is self powered. bit 1: true if device can wake up host
					//rest of bits always 0. Put the data in the buffer and send it off.
					BD0I.address[1] = 0x00;
					BD0I.bytecount = 0x02;
					BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
					break;
				case RECIPIENT_INTERFACE: //0x01. All 16 bits must be 0.
					switch (USB_USWSTAT) {
					//weird. checking the current state of the device, I guess the check isn't done 
					//for the first device because its needed to be done during configuration
						case ADDRESS_STATE:
							USB_error_flags |= 0x01;	// set Request Error Flag
							break;
						case CONFIG_STATE:
						//we're up and running fine. send out the message, as long as it is to one of the 
						//actual interfaces. throw an error if it is checking and interface beyond the existing
						//one(s)
							if (USB_buffer_data[wIndex]<PS3USB_NUM_INTERFACES) {
								BD0I.address[0] = 0x00;
								BD0I.address[1] = 0x00;
								BD0I.bytecount = 0x02;
								BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
							} else {
								USB_error_flags |= 0x01;	// set Request Error Flag
							}
					}
					break;
				case RECIPIENT_ENDPOINT:
					//once again, responses based on current configured state.
					switch (USB_USWSTAT) {
						case ADDRESS_STATE:
							//device has a configured address.
							if ((USB_buffer_data[wIndex]&0x0F)==0x00) {	// get EP, strip off direction bit and see if it is EP0
							
								BD0I.address[0] = (((USB_buffer_data[wIndex]&0x80) ? BD0I.status:BD0O.status)&0x04)>>2;	// return the BSTALL bit of EP0 IN or OUT, whichever was requested
								BD0I.address[1] = 0x00;
								BD0I.bytecount = 0x02;
								BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
							} else {
							//whoa whoa whoa. Was worried at first about no reponses other endpoints other than EP0, but in ADDRESS_STATE, 
							//EP1 hasnt been setup yet. So this part is fine.
								USB_error_flags |= 0x01;	// set Request Error Flag
								//TODO: Find out where the hell USB_error_flags actually causes something to happen.
							}
							break;
						case CONFIG_STATE:
							//Ah, system is fully up and operational.
							UEP = (unsigned char *)&UEP0;
							//set UEP to point at UEP0 register
							//kinda neat how they're changing the pointer to point at the descriptor for that endpoint buffer. Covers all possible endpoints.
							//good stuff.
							n = USB_buffer_data[wIndex]&0x0F;	// get EP and strip off direction bit for offset from UEP0
							buf_desc_ptr = &BD0O+((n<<1)|((USB_buffer_data[wIndex]&0x80) ? 0x01:0x00));	// compute pointer to the buffer descriptor for the specified EP
							//seems to expect the buffer descriptors to be in a versy specific order, apparently without pingpong buffers
							//checking the declarations in usb_defs.h confirms this. 
							//NOTE: Buffer descriptors MUST be delared together sequentially! otherwise this will break.

							if (UEP[n]&((USB_buffer_data[wIndex]&0x80) ? 0x02:0x04)) { // if the specified EP is enabled for transfers in the specified direction...
								BD0I.address[0] = ((buf_desc_ptr->status)&0x04)>>2;	// ...return the BSTALL bit of the specified EP
								BD0I.address[1] = 0x00;
								BD0I.bytecount = 0x02;
								BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
							} else {
								USB_error_flags |= 0x01;	// set Request Error Flag
							}
							break;
						default:
							USB_error_flags |= 0x01;	// set Request Error Flag
					}
					break;
				default:
					USB_error_flags |= 0x01;	// set Request Error Flag
			}
			break;
		case CLEAR_FEATURE: //BOOKMARK
		case SET_FEATURE:
			//notice the lack of Break; this chunk handles both the CLEAR and SET versions of the request.
			switch (USB_buffer_data[bmRequestType]&0x1F) {	// extract request recipient bits
				case RECIPIENT_DEVICE:
					//only request for devices should be remote wakeup. 
					switch (USB_buffer_data[wValue]) {
						case DEVICE_REMOTE_WAKEUP:
							if (USB_buffer_data[bRequest]==CLEAR_FEATURE)
								USB_device_status &= 0xFE;
							else
								USB_device_status |= 0x01;
							BD0I.bytecount = 0x00;		// set EP0 IN byte count to 0
							BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
							break;
						default:
							USB_error_flags |= 0x01;	// set Request Error Flag
					}
					break;
				case RECIPIENT_ENDPOINT:
					//only endpoint feature is endpoint halt. 
					switch (USB_USWSTAT) {
						//if we're in the address state, there is only EP0
							case ADDRESS_STATE:
							if (!(USB_buffer_data[wIndex]&0x0F)) {	// get EP, strip off direction bit, and see if its EP0
								BD0I.bytecount = 0x00;		// set EP0 IN byte count to 0
								BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
							} else {
								USB_error_flags |= 0x01;	// set Request Error Flag
							}
							break;
						case CONFIG_STATE:
							UEP = (unsigned char *)&UEP0;
							if (n = USB_buffer_data[wIndex]&0x0F) {	// get EP and strip off direction bit for offset from UEP0, if not EP0...
								buf_desc_ptr = &BD0O+((n<<1)|((USB_buffer_data[wIndex]&0x80) ? 0x01:0x00));	// compute pointer to the buffer descriptor for the specified EP
								if (USB_buffer_data[wIndex]&0x80) {	// if the specified EP direction is IN...
									if (UEP[n]&0x02) {	// if EPn is enabled for IN transfers...
										buf_desc_ptr->status = (USB_buffer_data[bRequest]==CLEAR_FEATURE) ? 0x00:0x84;
										//shutdown the EP or enable the EP.
									} else {
										USB_error_flags |= 0x01;	// set Request Error Flag									
									}
								} else {	// ...otherwise the specified EP direction is OUT, so...
									if (UEP[n]&0x04) {	// if EPn is enabled for OUT transfers...
										buf_desc_ptr->status = (USB_buffer_data[bRequest]==CLEAR_FEATURE) ? 0x88:0x84;
									} else {
										USB_error_flags |= 0x01;	// set Request Error Flag									
									}
								}
							}
							if (!(USB_error_flags&0x01)) {	// if there was no Request Error...
							//NOTE: Is this how one sends an ACK?
								BD0I.bytecount = 0x00; 
								BD0I.status = 0xC8;		// ...send packet as DATA1, set UOWN bit
							}
							break;
						default:
							USB_error_flags |= 0x01;	// set Request Error Flag
					}
					break;
				default:
					USB_error_flags |= 0x01;	// set Request Error Flag
			}
			break;
		case SET_ADDRESS:
			if (USB_buffer_data[wValue]>0x7F) {	// if new device address is illegal, send Request Error
				USB_error_flags |= 0x01;	// set Request Error Flag
			} else {
				USB_dev_req = SET_ADDRESS;	// processing a SET_ADDRESS request
				USB_address_pending = USB_buffer_data[wValue];	// save new address
				//NOTE: When done the actuall address get changed? USB_address_pending is a global, so hunt that down.
				//prolly checks USB_dev_req
				BD0I.bytecount = 0x00;		// set EP0 IN byte count to 0
				BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
				if (!GETDESCR_BEFORE_SETADDR)
					{ //setting the address before querying device descriptor
						SETADDR_BEFORE_GETDESCR=1;
						//which means it is likely the PS3 BC mode
						PS3_DETECTED=1;
					}
			}
			break;
		case GET_DESCRIPTOR:
			USB_dev_req = GET_DESCRIPTOR;	// processing a GET_DESCRIPTOR request
			if (!SETADDR_BEFORE_GETDESCR)
					{ //querying the descriptors before setting the address
						GETDESCR_BEFORE_SETADDR=1;
						//which means it is likely a proper PC connection
						//PS3_DETECTED=0;
					}
			switch (USB_buffer_data[wValueHigh]) {
				case DEVICE:
					if(PS3_DETECTED)
					{
						USB_desc_ptr = PS3USB_Device_Descriptor;
					}
					else
					{
						USB_desc_ptr = PCUSB_Device_Descriptor;
					}	
					USB_bytes_left = USB_desc_ptr[0];// aka the size of that descriptor
					if ((USB_buffer_data[wLengthHigh]==0x00) && (USB_buffer_data[wLength]<USB_bytes_left)) {
							//NOTE: kinda worried about this check. Can a device descriptor be >0xFF in size?
						USB_bytes_left = USB_buffer_data[wLength];
					}
					PS3USB_SendDescriptorPacket();	//Send the next packet width check of the descriptor			
					break;
				case CONFIGURATION:
					switch (USB_buffer_data[wValue]) {
						case 0:
							USB_desc_ptr = PS3USB_Configuration_Descriptor; //I guess this means there is only support for one configuration
							break;
						default:
							USB_error_flags |= 0x01;	// set Request Error Flag
					}
					if (!(USB_error_flags&0x01)) {
						USB_bytes_left = USB_desc_ptr[2];	// wTotalLength at an offset of 2
						if ((USB_buffer_data[wLengthHigh]==0x00) && (USB_buffer_data[wLength]<USB_bytes_left)) {
							USB_bytes_left = USB_buffer_data[wLength];
						}
						PS3USB_SendDescriptorPacket();
					}
					break;
				case STRING:
					switch (USB_buffer_data[wValue]) {
						case 0:
							if(PS3_DETECTED) USB_desc_ptr = PS3USB_String0; else USB_desc_ptr = PCUSB_String0;
							break;
						case 1:
							if(PS3_DETECTED) USB_desc_ptr = PS3USB_String1; else USB_desc_ptr = PCUSB_String1;
							break;
						case 2:
							if(PS3_DETECTED) USB_desc_ptr = PS3USB_String2; else USB_desc_ptr = PCUSB_String2;;
							break;
						default:
							USB_error_flags |= 0x01;	// set Request Error Flag
					}
					if (!(USB_error_flags&0x01)) {
						USB_bytes_left = USB_desc_ptr[0];
						if ((USB_buffer_data[wLengthHigh]==0x00) && (USB_buffer_data[wLength]<USB_bytes_left)) {
							USB_bytes_left = USB_buffer_data[wLength];
						}
						PS3USB_SendDescriptorPacket();
					}
					break;
				case HID:
					switch (USB_buffer_data[wValue]) {
						case 0:
							USB_desc_ptr = PS3USB_Configuration_Descriptor+0x12;		// add offset of HID1 descriptor from the beginning of PS3USB_Configuration_Descriptor descriptor
							//whoa. this is crap. there's gotta be a better way to do that.
							//NOTE: Make this better
							break;
						default:
							USB_error_flags |= 0x01;	// set Request Error Flag
					}
					if (!(USB_error_flags&0x01)) {
						USB_bytes_left = USB_desc_ptr[0];	// wTotalLength at an offset of 2
						if ((USB_buffer_data[wLengthHigh]==0x00) && (USB_buffer_data[wLength]<USB_bytes_left)) {
							USB_bytes_left = USB_buffer_data[wLength];
						}
						PS3USB_SendDescriptorPacket();
					}
					break;
				case REPORT:
					switch (USB_buffer_data[wValue]) {
						case 0:
						//FLAG! The report descriptor here should be automagic from the declaration
							USB_bytes_left = 0x94;		// total length of PS3USB_Report_Descriptor descirptor in HID1 descriptor, rather than PS3USB_Report_Descriptor
							//#error size of sizeof(PS3USB_Report_Descriptor)
							USB_desc_ptr = PS3USB_Report_Descriptor;
							break;
						default:
							USB_error_flags |= 0x01;	// set Request Error Flag
					}
					if (!(USB_error_flags&0x01)) {
						if ((USB_buffer_data[wLengthHigh]==0x00) && (USB_buffer_data[wLength]<USB_bytes_left)) {
							USB_bytes_left = USB_buffer_data[wLength];
						}
						PS3USB_SendDescriptorPacket();
					}
					break;
				default:
					USB_error_flags |= 0x01;	// set Request Error Flag
			}
			break;
						
		case GET_CONFIGURATION:
			//interesting. the configuration is only set in the SET_CONFIGURATION call below, and frankly doesn't
			//do anything but sit there in a global var. Grab the config number the host set, return it in a 1 byte pack
			BD0I.address[0] = USB_curr_config;	// copy current device configuration to EP0 IN buffer
			BD0I.bytecount = 0x01;
			BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
			break;
		case SET_CONFIGURATION:
			if (USB_buffer_data[wValue]<=PS3USB_NUM_CONFIGURATIONS) {
				//host gave us the ok to setup shop. 
				UEP1 = 0x00;	// clear all EP control registers except for EP0 to disable EP1-EP15 prior to setting configuration
				UEP2 = 0x00;
				UEP3 = 0x00;
				UEP4 = 0x00;
				UEP5 = 0x00;
				UEP6 = 0x00;
				UEP7 = 0x00;
				UEP8 = 0x00;
				UEP9 = 0x00;
				UEP10 = 0x00;
				UEP11 = 0x00;
				UEP12 = 0x00;
				UEP13 = 0x00;
				UEP14 = 0x00;
				UEP15 = 0x00;
				switch (USB_curr_config = USB_buffer_data[wValue]) {
					case 0:
						USB_USWSTAT = ADDRESS_STATE;
						//config 0 unsets everything, like a new plugin I guess. Set state machine to know we need to get an address.
						break;
					default:
						//if it aint 0, we dont really care what it is. Just enable the other EP's

						USB_USWSTAT = CONFIG_STATE; //tell state machine we configured.
						BD1I.bytecount = PS3USB_EP1_PACKET_SIZE;	// set EP1 IN byte count 
						BD1I.address = &PS3USB_EP1_IN_buffer;	// set EP1 IN buffer address
						BD1I.status = 0x48;		// clear UOWN bit (PIC can write EP1 IN buffer)
						UEP1 = ENDPT_IN_ONLY;	// enable EP1 for interrupt in transfers
						//NOTE: This seems to be where we can specify between the four transfer types.
						//ENDPOINT SETUPS GO HERE!
				}
				BD0I.bytecount = 0x00;		// set EP0 IN byte count to 0
				BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
			} else {
				USB_error_flags |= 0x01;	// set Request Error Flag
			}
			break;
		case GET_INTERFACE:
			switch (USB_USWSTAT) {
				case CONFIG_STATE:
					if (USB_buffer_data[wIndex]<PS3USB_NUM_INTERFACES) {
						//still quite a bit fuzzy on what this should do if anything. 
						BD0I.address[0] = 0x00;	// always send back 0 for bAlternateSetting
						BD0I.bytecount = 0x01;
						BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
					} else {
						USB_error_flags |= 0x01;	// set Request Error Flag
					}
					break;
				default:
					USB_error_flags |= 0x01;	// set Request Error Flag
			}
			break;
		case SET_INTERFACE:
			switch (USB_USWSTAT) {
				case CONFIG_STATE:
					if (USB_buffer_data[wIndex]<PS3USB_NUM_INTERFACES) {
						switch (USB_buffer_data[wValue]) {
							case 0:		// currently support only bAlternateSetting of 0
								BD0I.bytecount = 0x00;		// set EP0 IN byte count to 0
								BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
								break;
							default:
								USB_error_flags |= 0x01;	// set Request Error Flag
						}
					} else {
						USB_error_flags |= 0x01;	// set Request Error Flag
					}
					break;
				default:
					USB_error_flags |= 0x01;	// set Request Error Flag
			}
			break;
		case SET_DESCRIPTOR:
		case SYNCH_FRAME:
		default:
			USB_error_flags |= 0x01;	// set Request Error Flag
	}
}

void PS3USB_ClassRequests(void) {
	unsigned char n;

	switch (USB_buffer_data[bRequest]) {
		case GET_REPORT:
			//FLAG! set to report size
			for (n = 0; n<PS3USB_REPORT_SIZE; n++)
				//BD0I.address[n] = PS3USB_USBbuffer[n];	// copy Key_buffer to EP0 IN buffer
				BD0I.address[n] = 0;
			BD0I.address[0] = 33;
			BD0I.address[1] = 38;
			//BD0I.bytecount = PS3USB_REPORT_SIZE;			// set byte count to 8
			BD0I.bytecount = 8;
			BD0I.status = 0xC8;				// send packet as DATA1, set UOWN bit
			//Whoa whoa whoa. So the report can be sent out via EP0 here, or via EP1 in the PS3USB_SendReport function?
			//what if the report is >8 bytes? Are there multiple requests?
			break;
		case SET_REPORT:
			USB_dev_req = SET_REPORT;	// processing a SET_REPORT request
			//I guess there's not much to do with the SET_REPORT; setting the dev_req just tells
			//to clear out the dev_req on the next outtoken. Doesn't DO anything. Not even an acknowledge or
			//zero byte answer.
			break;
		case GET_PROTOCOL:
			BD0I.address[0] = USB_protocol;	// copy current protocol into EP0 IN buffer
			BD0I.bytecount = 0x01;			// set byte count to 1
			BD0I.status = 0xC8;				// send packet as DATA1, set UOWN bit
			//similar to the GET_CONFIGURATION from Standard requests; SET stashes the protocol in a global, and spits it back
			//if we get a GET
			break;
		case SET_PROTOCOL:
			USB_protocol = USB_buffer_data[wValue];		// update the new protocol value
			BD0I.bytecount = 0x00;			// set byte count to 0
			BD0I.status = 0xC8;				// sent packet as DATA1, set UOWN bit
			//stash in global and done.
			break;
		case GET_IDLE:
			BD0I.address[0] = USB_idle_rate;	// copy current idle rate into EP0 IN buffer
			BD0I.bytecount = 0x01;				// set byte count to 1
			BD0I.status = 0xC8;					// send packet as DATA1, set UOWN bit
			//Same; SET stashes it in a global, and GET spits it back.
			break;
		case SET_IDLE:
			USB_idle_rate = USB_buffer_data[wValueHigh];	// update the new idle rate
			BD0I.bytecount = 0x00;			// set byte count to 0
			BD0I.status = 0xC8;				// sent packet as DATA1, set UOWN bit
			break;
		default:
			USB_error_flags |= 0x01;	// set Request Error Flag
	}
}

void PS3USB_VendorRequests(void) {
	switch (USB_buffer_data[bRequest]) {
		default:
			USB_error_flags |= 0x01;	// set Request Error Flag
	}
}

void PS3USB_ProcessInToken(void) {
	//these In tokens seem to be continuations, the host saying 'Im expecting more data: send it'
	//TODO: Need to verify if the larger HID reports will require multiple InTokens. 
	switch (USB_USTAT&0x18) {	// extract the EP bits
		case EP0:
			switch (USB_dev_req) {
				case SET_ADDRESS:
					switch (UADDR = USB_address_pending) {
						case 0:
							USB_USWSTAT = DEFAULT_STATE;
							break;
						default:
							USB_USWSTAT = ADDRESS_STATE;
					}
					break;
				case GET_DESCRIPTOR:
					PS3USB_SendDescriptorPacket();
					break;
			}
			break;
		case EP1:
			break;
		case EP2:
			break;
	}
}

void PS3USB_ProcessOutToken(void) {
	switch (USB_USTAT&0x18) {	// extract the EP bits
		case EP0:
			switch (USB_dev_req) {
				case SET_REPORT:
					USB_dev_req = NO_REQUEST;		// clear device request
			}
			BD0O.bytecount = PS3USB_EP0_PACKET_SIZE;
			BD0O.status = 0x88;
			BD0I.bytecount = 0x00;		// set EP0 IN byte count to 0
			BD0I.status = 0xC8;		// send packet as DATA1, set UOWN bit
			//TODO: I really need to get down the buffer descriptor status bytes.
			break;
		case EP1:
			break;
		case EP2:
			break;
	}
}

void PS3USB_SendDescriptorPacket(void) {
	unsigned char n;

	if (USB_bytes_left<PS3USB_EP0_PACKET_SIZE) {
		USB_dev_req = NO_REQUEST;	// sending a short packet, so clear device request
		USB_packet_length = USB_bytes_left;
		USB_bytes_left = 0x00;
	} else {
		USB_packet_length = PS3USB_EP0_PACKET_SIZE;
		USB_bytes_left -= PS3USB_EP0_PACKET_SIZE;
	}
	for (n = 0; n<USB_packet_length; n++) {
		BD0I.address[n] = *USB_desc_ptr++;
	}
	BD0I.bytecount = USB_packet_length;
	BD0I.status = ((BD0I.status^0x40)&0x40)|0x88; // toggle the DATA01 bit, clear the PIDs bits, and set the UOWN and DTS bits
	//so wait a tic. I know that EP0 buffer is prolly 8 in everything here; thats cool, well kinda. But if the descriptor is 
	// > UBS_packet_length, then the host will have to request the rest of the descriptor, right? maybe it does already
	//checking the Made Simple site, it does appear from the mouse description on page 5 to be the case. THe host repeats the
	//request, and the device ships another chunk out. 
	//looks like USB_desc_ptr is the pointer to where the descriptor is at in the process of being sent. Part I dont understand
	//is that the USB_desc_ptr is overwritten to the beginning of the descriptor will each call. Hmm. I'm sure it works, just am 
	//missing something.
	//it looks like the rest of the data is being sent when this function is being called by PS3USB_ProcessInToken.
}

void PS3USB_SendReport(void) {
	unsigned char n;

	for (n = 0; n<PS3USB_REPORT_SIZE; n++)
		BD1I.address[n] = PS3USB_USBbuffer[n];			// copy Key_buffer to EP1 IN buffer
	BD1I.bytecount = PS3USB_REPORT_SIZE;							// set the EP1 IN byte count to 8
	BD1I.status = ((BD1I.status&0x40)^0x40)|0x88;	// toggle the DATA01 bit of the EP1 IN status register and set the UOWN and DTS bits
}

void PS3USB_main(void) {
	unsigned char USB_STICK_MODE;
	unsigned char n;
	unsigned char *key_buff_ptr;
	unsigned char timertemp;
	
	USB_STICK_MODE=USM_BOTH;
	if(!Real_Stick_Start) USB_STICK_MODE=USM_DIGITAL_ONLY;
	if(!Real_Stick_Select) USB_STICK_MODE=USM_ANALOG_ONLY;
	SETADDR_BEFORE_GETDESCR=0;
	GETDESCR_BEFORE_SETADDR=0;
	PS3_DETECTED=0;
	if(!Real_Stick_Jab && !Real_Stick_Short) PS3_DETECTED=1;

	 OpenTimer0(TIMER_INT_OFF
	 		& T0_16BIT
	 		& T0_SOURCE_INT 
	 		& T0_PS_1_8  );
	  //with a 1:8 prescale, 1/60th of a second is 0x61A8	ticks of the clock.
	  // 0xFFFF-0x61A8 ==	9E57
	  //so if we start the timer at 0x9E57, it will turn over after 1 frame. 
	 TMR0H=0x9E;TMR0L=0x57;
	 

	
	
	PS3USB_InitUSB();			// initialize the USB registers and serial interface engine
	while (USB_USWSTAT!=CONFIG_STATE) {		// while the peripheral is not configured...
		PS3USB_ServiceUSB();						// ...service USB requests
	} 
	
 
	while (1) 
	{
		if (!(BD1I.status&0x80)) // see if UOWN bit of EP1 IN status register is clear (i.e., PIC owns EP1 IN buffer)
		{	
			
			PS3USB_USBbuffer[0]=PS3USB_USBbuffer[1]=0;
			PS3USB_USBbuffer[2]=0x08;
			PS3USB_USBbuffer[3]=PS3USB_USBbuffer[4]=PS3USB_USBbuffer[5]=PS3USB_USBbuffer[6]=0x80;
			PS3USB_USBbuffer[7]=PS3USB_USBbuffer[8]=PS3USB_USBbuffer[9]=PS3USB_USBbuffer[10]=PS3USB_USBbuffer[11]=PS3USB_USBbuffer[12]=PS3USB_USBbuffer[13]=0;
			PS3USB_USBbuffer[14]=PS3USB_USBbuffer[15]=PS3USB_USBbuffer[16]=PS3USB_USBbuffer[17]=PS3USB_USBbuffer[18]=0;

			if(!Stick_Jab) { PS3USB_USBbuffer[0]|=0b00000001; PS3USB_USBbuffer[14]=0xFF; } //NO idea about the [14]
			if(!Stick_Short) { PS3USB_USBbuffer[0]|=0b00000010; PS3USB_USBbuffer[13]=0xFF; } 
			if(!Stick_Forward) { PS3USB_USBbuffer[0]|=0b00000100; PS3USB_USBbuffer[12]=0xFF; } 
			if(!Stick_Strong) { PS3USB_USBbuffer[0]|=0b00001000; PS3USB_USBbuffer[11]=0xFF; } 
#ifdef EXTRA_BUTTONS					
			if(!Stick_Extra0) { PS3USB_USBbuffer[0] |=0b00010000; PS3USB_USBbuffer[15]=0xFF; } //4th punch ==L1
			if(!Stick_Extra1) { PS3USB_USBbuffer[0] |=0b01000000; PS3USB_USBbuffer[17]=0xFF; } //4th kick==L2	
#endif
			if(!Stick_Fierce) {  PS3USB_USBbuffer[0] |=0b00100000; PS3USB_USBbuffer[16]=0xFF; } //Fierce == R1
			if(!Stick_Roundhouse) {  PS3USB_USBbuffer[0] |=0b10000000; PS3USB_USBbuffer[18]=0xFF; } //Roundhouse == R2
	
			if(!Stick_Start && !Stick_Select && Stick_Jab)  { PS3USB_USBbuffer[1]|=0b00010000; } //PS HOME button bitches!
				else //one or both is not pressed or Jab IS pressed. 
				{
					if(!Stick_Start) { PS3USB_USBbuffer[1] |=0b00000010;} 	
					if(!Stick_Select) { PS3USB_USBbuffer[1] |=0b00000001;} 
				}
		//button numbers are based on the wiring of a hacked VSHG in a HRAP case. god only knows what
		//may have been switched around. 
		//buttons: jab short forward strong RH fierce 
		// #10 is start, #9 is definitely select #13 is Home
		//7 and 8 are the extendeds? R1 and R2 I hope
		//no idea about 11 and 12, prolly L3 and R3. Nothing on the VSHG PCB activates it. 
			if(USB_STICK_MODE!=USM_ANALOG_ONLY)
				{
					if(!Stick_Up)
					{
						PS3USB_USBbuffer[2]=0x00;
						if(!Stick_Right) PS3USB_USBbuffer[2]=0x01;
						if(!Stick_Left) PS3USB_USBbuffer [2]=0x07;
					}
					else
					{
						if(!Stick_Down)
						{
							PS3USB_USBbuffer[2]=0x04;
							if(!Stick_Right) PS3USB_USBbuffer[2]=0x03;
							if(!Stick_Left) PS3USB_USBbuffer [2]=0x05;
						}
						else
						{//neither up nor down
							if(!Stick_Right) PS3USB_USBbuffer[2]=0x02;
							if(!Stick_Left) PS3USB_USBbuffer [2]=0x06;
						}
					}
				}
			
			if(USB_STICK_MODE!=USM_DIGITAL_ONLY)
			{
				if(!Stick_Right) PS3USB_USBbuffer[3]=0xFF;
				if(!Stick_Left) PS3USB_USBbuffer[3]=0x00;
				if(!Stick_Up) PS3USB_USBbuffer[4]=0x00;
				if(!Stick_Down) PS3USB_USBbuffer[4]=0xFF;
			}
				
							
			PS3USB_SendReport();
			mLED_1_Toggle();
		}
		PS3USB_ServiceUSB();
		
		
		 timertemp=TMR0L;  //reading L so high will be refreshed.
		 timertemp=TMR0H; // reading H for real
		 if (timertemp < 0x9D)
		 {
		 	TMR0H=0x9E;
		 	TMR0L=0x57; //reset the timer
		 	FrameUpdate();
		 }
		 

	}
}
