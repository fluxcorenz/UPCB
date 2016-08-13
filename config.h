#ifndef CONFIG_H
#define CONFIG_H
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

/* Definitions for UPCB hardware setup. */

/* DEBUG - Used to disable piggyback modes that use the pins used for ICSP debugging. 
	ONLY define it if you are using the debugging tools in MPLAB. */
//#define DEBUG
/* STATUS_LEDS - If the controller is setup with two LEDs on the RA0 and RA1 lines */
//#define STATUS_LEDS

/* EXTRA_BUTTONS_LEFT - If the controller is setup with two playbuttons on RA0 and RA1 
	 	on the LEFT side of the main six buttons. */
//#define EXTRA_BUTTONS_LEFT

/* EXTRA_BUTTONS_RIGHT - If the controller is setup with two playbuttons on RA0 and RA1
		on the RIGHT side of the main six buttons. */
//#define EXTRA_BUTTONS_RIGHT

/* PROGRAM_BUTTON - If the controller is setup with a record/playback button on RA5 */
//#define PROGRAM_BUTTON

#define MAJOR_REV_NUMBER 2
#define MINOR_REV_NUMBER 9

/* Defines from here on down setup the extra stuff needed automatically, and should not be changed */


/* Define the major revision number */
#define MAJOR_REVISION ('0' + MAJOR_REV_NUMBER )
/* minor revision number */
#define MINOR_REVISION ('0' + MINOR_REV_NUMBER )
#define REVISION_BYTE ((MAJOR_REV_NUMBER * 16) + (MINOR_REV_NUMBER ))

/* If either extra buttons defines are set, also set the generic extra buttons flag */
#ifdef EXTRA_BUTTONS_LEFT
	#define EXTRA_BUTTONS
#endif
#ifdef EXTRA_BUTTONS_RIGHT
	#define EXTRA_BUTTONS
#endif

// if no options, we're using BASE_OPTION
#ifndef EXTRA_BUTTONS
	#ifndef STATUS_LEDS
		#define BASE_OPTION
	#endif
#endif

//Let's define the OPTION_MODE character based on the options selected
#ifdef EXTRA_BUTTONS
	#define OPTION_MODE 'B'
	#ifdef PROGRAM_BUTTON
		#define CONFIG_BYTE 0x12
	#else
		#define CONFIG_BYTE 0x13
	#endif
#endif
#ifdef STATUS_LEDS
	#define OPTION_MODE 'L'
	#ifdef PROGRAM_BUTTON
		#define CONFIG_BYTE 0x14
	#else
		#define CONFIG_BYTE 0x15
	#endif
#endif
#ifdef BASE_OPTION
	#define OPTION_MODE 'X'
	#ifdef PROGRAM_BUTTON
		#define CONFIG_BYTE 0x16
	#else
		#define CONFIG_BYTE 0x17
	#endif
#endif
	
//Let's define the PROG_OPTION_MODE character based on whether we're using the program button
#ifdef PROGRAM_BUTTON
	#define PROG_OPTION_MODE 'P'
#else
	#define PROG_OPTION_MODE ' '
#endif

#ifdef EXTRA_BUTTONS
	#ifdef STATUS_LEDS
		#error _STATUS_LEDS and _EXTRA_BUTTONS are mutually exclusive. Please correct in config.h
	#endif
#endif

#ifdef EXTRA_BUTTONS_RIGHT
	#ifdef EXTRA_BUTTONS_LEFT
		#error _EXTRA_BUTTONS_RIGHT and _EXTRA_BUTTONS_LEFT are mutually exclusive. Please correct in config.h
	#endif
#endif


#endif //CONFIG_H
