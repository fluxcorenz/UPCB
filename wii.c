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
#include "wii.h"
#include "upcb.h"
#include <i2c.h>
void WII_main(void)
{
	
	OpenI2C(SLAVE_7, SLEW_ON); //setup I2C
	SSPADD = 0x52; //i2c address of the 'chuck. Let's see if its the same for the CC
	//if needed, look into ADMSK2 through ADMSK5 to catch requests to many addresses
	
	while(1)
	{
		
		
	}	
}