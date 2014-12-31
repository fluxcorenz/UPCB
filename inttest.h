#ifndef INTTEST_H
#define INTTEST_H
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
/* Routines to help with enabling interrupts for any supported console that
	needs them. */



void high_int_handler(void); //the func pointed to by the ISR. Calls the HIGH_INT function
void low_int_handler(void); //the func pointed to by the low ISR. Calls the LOW_INT function
void high_interrupt(void);
void low_interrupt(void);
void inttest(void);
void intfunc(void);

extern void (*LOW_INT_HOOK)(void);
extern void (*HIGH_INT_HOOK)(void);


