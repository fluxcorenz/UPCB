#ifndef JAGUAR_H
#define JAGUAR_H
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
/* UPCB module for Jaguar/EJP support.

Information from http://www.gamesx.com/controldata/ejp_faq.htm
The most important information in this FAQ is the circuit schematic,
supposedly the one used inside the actual Jaguar controller. 

15 Pin High Density
D Socket

Looking into the port on the computer, the connector looks something 
like this:-
(Female)

	--------------------------------------------------
	\          *       *       *      *      *       /
	 \         5       4       3      2      1      /
	  \                                            /
	   \          *       *       *      *     *  /
	    \        10       9       8      7     6 /
	     \                                      /
	      \   *       *       *      *      *  /
	       \ 15      14      13     12     11 /
	        \________________________________/
	 
So the male cable (the one you'll make) would look like this:

	--------------------------------------------------
	\          *       *       *      *      *       /
	 \         1       2       3      4      5      /
	  \                                            /
	   \   *      *       *       *      *        /
	    \  6      7       8       9      10      /
	     \                                      /
	      \   *       *       *      *      *  /
	       \ 11      12      13     14     15 /
	        \________________________________/    
	

 7 O---------*---------*--*--*--*--*--+
             |         |  |  |  |  |  |
 1 O---------|---------|--|--|--|--|--|---------------------+
 2 O---------|---------|--|--|--|--|--|---------------------|--+
 3 O---------|---------|--|--|--|--|--|---------------------|--|--+
 4 O---------|---------|--|--|--|--|--|---------------------|--|--|--+
             |         |  |  |  |  |  |                     |  |  |  | 
             |         |  |  |  |  |  |       *--|>+--N/O---*  |  |  |
             |         |  |  |  |  |  |       |     (OPTION)|  |  |  |
             |         |  |  |  |  |  |       |             |  |  |  |
             | 20      |  |  |  |  |  |       *--|>+--N/O---|--*  |  |
        +----+----+    |  |  |  |  |  |       |       (C)   |  |  |  |
        |         |    #  #  #  #  #  # All   |             |  |  |  |
        | 74HC244 |    #  #  #  #  #  # 4k7   *--|>+--N/O---|--|--*  |
        |         |    #  #  #  #  #  #       |       (B)   |  |  |  |
      16|         |4   |  |  |  |  |  |       |             |  |  |  |
10 O----+---SS----+----*--|--|--|--|--|-------*--|>+--N/O---|--|--|--*
        |         |       |  |  |  |  |               (A)   |  |  |  |
        |         |       |  |  |  |  |                     |  |  |  | 
        |         |       |  |  |  |  |       *--|>+--N/O---*  |  |  |
        |         |       |  |  |  |  |       |       (3)   |  |  |  |
        |         |       |  |  |  |  |       |             |  |  |  |
        |         |       |  |  |  |  |       *--|>+--N/O---|--*  |  |
        |         |       |  |  |  |  |       |       (2)   |  |  |  |
        |         |       |  |  |  |  |       |             |  |  |  |
        |         |       |  |  |  |  |       *--|>+--N/O---|--|--*  |
        |         |       |  |  |  |  |       |       (1)   |  |  |  |
       6|         |14     |  |  |  |  |       |             |  |  |  |
11 O----+---SS----+-------*--|--|--|--|-------*--|>+--N/O---|--|--|--*
        |         |          |  |  |  |             (RIGHT) |  |  |  |
        |         |          |  |  |  |                     |  |  |  | 
        |         |          |  |  |  |       *--|>+--N/O---*  |  |  |
        |         |          |  |  |  |       |       (6)   |  |  |  |
        |         |          |  |  |  |       |             |  |  |  |
        |         |          |  |  |  |       *--|>+--N/O---|--*  |  |
        |         |          |  |  |  |       |       (5)   |  |  |  |
        |         |          |  |  |  |       |             |  |  |  |
        |         |          |  |  |  |       *--|>+--N/O---|--|--*  |
        |         |          |  |  |  |       |       (4)   |  |  |  |
      12|         |8         |  |  |  |       |             |  |  |  |
12 O----+---SS----+----------*--|--|--|-------*--|>+--N/O---|--|--|--*
        |         |             |  |  |              (LEFT) |  |  |  |
        |         |             |  |  |                     |  |  |  | 
        |         |             |  |  |       *--|>+--N/O---*  |  |  |
        |         |             |  |  |       |       (9)   |  |  |  |
        |         |             |  |  |       |             |  |  |  |
        |         |             |  |  |       *--|>+--N/O---|--*  |  |
        |         |             |  |  |       |       (8)   |  |  |  |
        |         |             |  |  |       |             |  |  |  |
        |         |             |  |  |       *--|>+--N/O---|--|--*  |
        |         |             |  |  |       |       (7)   |  |  |  |
       9|         |11           |  |  |       |             |  |  |  |
13 O----+---SS----+-------------*--|--|-------*--|>+--N/O---|--|--|--*
        |         |                |  |              (DOWN) |  |  |  |
        |         |                |  |                     |  |  |  | 
        |         |                |  |       *--|>+--N/O---+  |  |  |
        |         |                |  |       |       (#)      |  |  |
        |         |                |  |       |                |  |  |
        |         |                |  |       *--|>+--N/O------+  |  |
        |         |                |  |       |       (0)         |  |
        |         |                |  |       |                   |  |
        |         |                |  |       *--|>+--N/O---------+  |
        |         |                |  |       |       (*)            |
       7|         |13              |  |       |                      |
14 O----+---SS----+----------------*--|-------*--|>+--N/O------------*
        |         |                   |               (UP)           |
        |         |                   |                              |
      18|         |2                  |                              |
6  O----+---SS----+-------------------*----------|>+--N/O------------+
        |         |                                 (PAUSE)           
        |         |
        |         |10
        |         +-----+
        |         |     |
        +-+---+---+     |
         1|   |19       |
		  |   |         |
		  |   |         |
 9 O------*---*---------+
	
From this, it is pretty easy to understand how it works. The EJP uses 
bank switching just like other controllers like the Saturn and Genesis.
Instead of using two pins to select from 4 banks like the Saturn, it 
uses four pins; the pin that is low is the pin that is active, while 
the rest are high (or high impedence. Just as long as its not low). 
Pins 1, 2, 3, and 4 are the bank select pins. Pins 6, and 10-14 are
the data pins. Pins are active low. 

		 Pin 10   Pin 11   Pin 12   Pin 13   Pin 14   Pin 6    
Bank 1   Option    (3)       (6)     (9)       (#)     N/A (High)
Bank 2     (C)     (2)       (5)     (8)       (0)     N/A (High)
Bank 3     (B)     (1)       (4)     (7)       (*)     N/A (High)
Bank 4     (A)     Right      Left    Down     Up     Pause

So the pinout would look like this.
EJP Pin			Description
1				Bank Select 1
2				Bank Select 2
3				Bank Select 3
4				Bank Select 4
5				N/C
6				Data 6
7				VCC
8				N/C
9				GND
10				Data 1
11 				Data 2
12				Data 3
13				Data 4
14				Data 5
15				N/C

So to make a UPCB cable for the Jaguar/EJP:
UPCB D-Sub Pin				Jaguar Pin
1								9 (GND)
2								1 (Bank Select 1)
3								2 (Bank Select 2)
4								3 (Bank Select 3)
5								4 (Bank Select 4)
6								High
7								High
8								7 (VCC)
9								NC (Not connected)
10								10 (Data 1)
11								11 (Data 2)
12								12 (Data 3)
13								13 (Data 4)
14								14 (Data 5)
15								6  (Data 6)

How it'll behave: 
I don't have a Jaguar to test. From what I can tell,
the *, 0, and # buttons have functionality in most
games for various options. So, ABC will be short, 
forward, roundhouse, Select will be Option, Start
will be pause, and * 0 # will be jab strong fierce.

*/


#define JAGUAR_BANK1 		PORTEbits.RE2
#define JAGUAR_BANK2 		PORTEbits.RE1
#define JAGUAR_BANK3 		PORTEbits.RE0
#define JAGUAR_BANK4 		PORTAbits.RA5
#define JAGUAR_DATA1		LATBbits.LATB3
#define JAGUAR_DATA2		LATBbits.LATB2
#define JAGUAR_DATA3		LATBbits.LATB0
#define JAGUAR_DATA4		LATCbits.LATC6
#define JAGUAR_DATA5		LATCbits.LATC7
#define JAGUAR_DATA6		LATBbits.LATB1

/* Prototypes */
void JAGUAR_Init_Pins(void);
void JAGUAR_main(void);

#endif //JAGUAR_H