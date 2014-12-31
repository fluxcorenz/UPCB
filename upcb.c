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
#include "upcb.h"

void EEpromPut(unsigned char addr, unsigned char data) 
{ 
	unsigned short temp;
	
	
	EEADR = addr;
	EEDATA = data; 
	EECON1bits.EEPGD = 0; 
	EECON1bits.CFGS = 0; 
	EECON1bits.WREN = 1; 

	DISABLE_INTERRUPTS();
	_asm
	MOVLW 0x55
	MOVWF EECON2,0
	MOVLW 0xAA
	MOVWF EECON2,0
	_endasm
	EECON1bits.WR=1;
	while (EECON1bits.WR == 1);
	ENABLE_INTERRUPTS();

	EECON1bits.WREN = 0; 
} 

unsigned char EEpromGet(unsigned char addr) 
{ 
	volatile unsigned char eepTemp; 
	EEADR = addr; 
	EECON1bits.EEPGD = 0; 
	EECON1bits.CFGS = 0; 
	EECON1bits.RD = 1; 
	eepTemp = EEDATA; 
	return eepTemp; 
} 