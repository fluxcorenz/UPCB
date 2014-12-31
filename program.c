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
#include "program.h"
#include "upcb.h"
#include "config.h"
#include <p18cxxx.h>



	#pragma idata
	unsigned char tournamentmode=0;
	
	
	#pragma code
	
#ifdef PROGRAM_BUTTON 
	// all of this code is unneeded if the program_button
	//isnt being used

	#pragma idata // making some global vars
		//each button will be given an eight bit bitfield, representing the buttons that can
		//trigger that button. Sorry for the poor wording, examples are better
		// Let's say we're on a playstation. if we want to have the physical jab button  (normally square)
		// act so it presses fierce and roundhouse (R1 and R2 respectively, we would want the bitfields like so
		// buttonmapping[FIE]=0b11111110; buttonmapping[ROU]=0b11111110; 
		//That way, when fierce(R1) is checked and jab is down, it will show R1 as down, ditto for R2
		// if we want it so that absolutely every button is mapped to fierce (this one's for you Marvin)
		// and nothing else then we'd want buttonmapping[FIE]=0b00000000; and the rest of the array filled 
		//with 1's. 
	unsigned char programmode=PROGRAMMODE_OFF;
	unsigned char button_to_map=0;
	unsigned char recordmode=0;
	unsigned int  controllerstate=0;
	unsigned char startpressed=0;
	unsigned char startpresscount=0;
	unsigned char System_Mapping_Address=0;
	unsigned char turboadjuststickflag=0;
	unsigned char buttonmapping[8] = {
						0b11111110, // jab to jab
						0b11111101, //strong to strong
						0b11111011, //fierce to fierce, etc
						0b11110111,
						0b11101111,
						0b11011111,
						0b10111111,
						0b01111111 
						};
	//turboholder holds for each button (using the e_buttons) holds the turbo frame count
	//for each button. 0=no turb, 1= off one frame, on one frame, repeat, 2=on 2 frame, off 2 frame, etc.					
	//turbocount holds the number of frames that have passed since last toggle; when turbocount[BLAH]
	// == turboholder[BLAH], turbocount[BLAH]=0, and { turboavailable.bBLAH  = !turboavailable.bBLAH;}
	unsigned char turboholder[8]= {	0, 0, 0, 0, 0, 0, 0, 0 } ;
	unsigned char turbocount[8]= { 0, 0, 0, 0, 0, 0, 0, 0 } ;	
	//turboavailable stores a flag for each button saying whether it is available. a high flag will cause the button
	//to return a not pressed every time.
	// Controlled by the end of frame update; return (turboavailable.bJAB || Real_Stick_Jab) kind of thing
	unsigned char turboavailable=0x00; 
	
	
	#pragma udata gpr1=0x100
	unsigned char recordbuffer[256];
	#pragma udata gpr2=0x200
	unsigned char recordbuffer2[256];
	
	
	#pragma code
	void FrameUpdate(void)
	{
		unsigned char i;
		for(i=0;i<8;i++)
		{
			if (turboholder[i]!=0)
			{
				turbocount[i]++;
				if (turbocount[i] == turboholder[i])	
				{
					turbocount[i]=0;
//					if(turboavailable & (1<<i))
//					{ //bit is high
//						turboavailable -= (1<<i);
//					}
//					else
//					{ //bit is low
//						turboavailable += (1<<i);	
//					}
					turboavailable ^= (1<<i);
				}
			}
		}
		
	}	
	
	void Program_Mapping_Check(void)
	{	
		int counter=0;
		/* We need to check that the mappings in the EEPROM are valid; if the UPCB is reprogrammed, they get
		overwritten with 0xFF for one messed up configuration; no buttons work. This is silly and unneeded. So,
		on boot, we check the last byte of the EEPROM to see if our flag is there. If it is, everythings 
		kopasetic and we can continue. If not, we need to initiallize the EEPROM. */
		/* Update: added a button combination to do this as well to reset all mappings */
		if ((EEpromGet(MAPPING_SAVED_FLAG_LOCATION) != MAPPING_SAVED_FLAG) ||
				(!Real_Stick_Short && !Real_Stick_Forward && !Real_Stick_Roundhouse))
		{
			//flag check failed. initialize
			for (counter=0;counter<250;counter+=8)
			{
				EEpromPut(counter,   buttonmapping[0]);
				EEpromPut(counter+1, buttonmapping[1]);
				EEpromPut(counter+2, buttonmapping[2]);
				EEpromPut(counter+3, buttonmapping[3]);
				EEpromPut(counter+4, buttonmapping[4]);
				EEpromPut(counter+5, buttonmapping[5]);
				EEpromPut(counter+6, buttonmapping[6]);
				EEpromPut(counter+7, buttonmapping[7]);		
			}
			EEpromPut(MAPPING_SAVED_FLAG_LOCATION, MAPPING_SAVED_FLAG); //and set the flag
		}
		
		
	}
	
	void Program_Save_Mappings(void)
	{
		EEpromPut(System_Mapping_Address, buttonmapping[0]);
		EEpromPut(System_Mapping_Address+1, buttonmapping[1]);
		EEpromPut(System_Mapping_Address+2, buttonmapping[2]);
		EEpromPut(System_Mapping_Address+3, buttonmapping[3]);
		EEpromPut(System_Mapping_Address+4, buttonmapping[4]);
		EEpromPut(System_Mapping_Address+5, buttonmapping[5]);
		EEpromPut(System_Mapping_Address+6, buttonmapping[6]);
		EEpromPut(System_Mapping_Address+7, buttonmapping[7]);		
		EEpromPut(MAPPING_SAVED_FLAG_LOCATION, MAPPING_SAVED_FLAG);		
	}
	
	void Program_Load_Mappings(unsigned char sma_address)
	{
		Program_Mapping_Check(); 
		System_Mapping_Address=sma_address; //save the SMA so we know where to write later
		buttonmapping[0]=EEpromGet((System_Mapping_Address)); 		
		buttonmapping[1]=EEpromGet((System_Mapping_Address+1)); 		
		buttonmapping[2]=EEpromGet((System_Mapping_Address+2)); 		
		buttonmapping[3]=EEpromGet((System_Mapping_Address+3)); 		
		buttonmapping[4]=EEpromGet((System_Mapping_Address+4)); 		
		buttonmapping[5]=EEpromGet((System_Mapping_Address+5)); 		
		buttonmapping[6]=EEpromGet((System_Mapping_Address+6)); 		
		buttonmapping[7]=EEpromGet((System_Mapping_Address+7)); 		
	
	}
	
	unsigned char S_Jab(void)  //return 0 if pressed, anything else otherwise
	{
		unsigned char temp;
		if (programmode) return 1;
		#ifdef EXTRA_BUTTONS

			temp=( PORTD & 0b00111111);
			if (Real_Stick_Extra0) temp |=0b01000000;
			if (Real_Stick_Extra1) temp |=0b10000000;
			//return ( turboavailable.bJAB || ((temp | buttonmapping[JAB])==0xFF));		
			return ((temp | turboavailable | buttonmapping[JAB])==0xFF);
		#else
			return (( turboavailable | PORTD | buttonmapping[JAB])==0xFF);
		#endif //EXTRA_BUTTONS
	}
	
	unsigned char S_Strong(void)	
	{
		unsigned char temp;
		if (programmode) return 1;
		#ifdef EXTRA_BUTTONS
			temp=( PORTD & 0b00111111);
			if (Real_Stick_Extra0) temp |=0b01000000;
			if (Real_Stick_Extra1) temp |=0b10000000;
			return ((( turboavailable | temp | buttonmapping[STR])==0xFF));		
		#else
			return ((( turboavailable | PORTD | buttonmapping[STR])==0xFF));
		#endif //EXTRA_BUTTONS
	}
		
	unsigned char S_Fierce(void)	
	{
		unsigned char temp;
		if (programmode) return 1;
		#ifdef EXTRA_BUTTONS
			temp=( PORTD & 0b00111111);
			if (Real_Stick_Extra0) temp |=0b01000000;
			if (Real_Stick_Extra1) temp |=0b10000000;
			return ((( turboavailable | temp | buttonmapping[FIE])==0xFF));		
		#else
			return ((( turboavailable | PORTD | buttonmapping[FIE])==0xFF));
		#endif //EXTRA_BUTTONS
	}
	unsigned char S_Short(void)	
	{
		unsigned char temp;
			/* Every console will report Short, so check for programstuff here */
		if (programmode) 
		{
			if (tournamentmode)
			{
				programmode=PROGRAMMODE_OFF;	
				return 1;
			}
			if ((Real_Stick_Program) && (programmode==PROGRAMMODE_REMAPPING)) 
			{	//button no longer pressed and we're doing mapping
				/* for each button of the 8, see if its pressed. if it is, unset the bits in the map
					that are currently 0 in the button_to_map. if it isnt, set the bits that are 0 in 
					the map */
				if (!Real_Stick_Jab) 		buttonmapping[JAB] &= button_to_map; else buttonmapping[JAB] |= (0xFF ^ button_to_map);
				if (!Real_Stick_Strong) 	buttonmapping[STR] &= button_to_map; else buttonmapping[STR] |= (0xFF ^ button_to_map);
				if (!Real_Stick_Fierce) 	buttonmapping[FIE] &= button_to_map; else buttonmapping[FIE] |= (0xFF ^ button_to_map);
				if (!Real_Stick_Short)		buttonmapping[SHO] &= button_to_map; else buttonmapping[SHO] |= (0xFF ^ button_to_map);
				if (!Real_Stick_Forward) 	buttonmapping[FOR] &= button_to_map; else buttonmapping[FOR] |= (0xFF ^ button_to_map);
				if (!Real_Stick_Roundhouse) buttonmapping[ROU] &= button_to_map; else buttonmapping[ROU] |= (0xFF ^ button_to_map);
				#ifdef EXTRA_BUTTONS
					if (!Real_Stick_Extra0) 	buttonmapping[EX0] &= button_to_map; else buttonmapping[EX0] |= (0xFF ^ button_to_map);
					if (!Real_Stick_Extra1)	 	buttonmapping[EX1] &= button_to_map; else buttonmapping[EX1] |= (0xFF ^ button_to_map);
				#else
					if (!Real_Stick_Select) 	buttonmapping[SEL] &= button_to_map; else buttonmapping[SEL] |= (0xFF ^ button_to_map);
					if (!Real_Stick_Start)	 	buttonmapping[STA] &= button_to_map; else buttonmapping[STA] |= (0xFF ^ button_to_map);
				#endif
				
				
				
				programmode=PROGRAMMODE_OFF;
			}
			if ((Real_Stick_Program) && (programmode==PROGRAMMODE_TURBOSET))
			{
				programmode=PROGRAMMODE_OFF;	
			}
			if ((Real_Stick_Program) && (programmode==PROGRAMMODE_NORMAL))
			{
				programmode=PROGRAMMODE_OFF;	
			}
			if (tournamentmode)
			{
				programmode=PROGRAMMODE_OFF;	
			}
			if ((!Real_Stick_Program) && (!startpressed) && (!Real_Stick_Start) && (Real_Stick_Up) && (Real_Stick_Down))
			{ /* In programmode, start is pressed, and we didn't know it was pressed before...*/
				startpressed=1; //set the flag so we know it was pressed.
				startpresscount++; //increment the count of how many times its pressed				
			}
			if ((!Real_Stick_Program) && (startpressed) && (Real_Stick_Start))
			{ /* In programmode, start is NOT pressed, and it was pressed before...*/
				startpressed=0; //clear the flag
				if (startpresscount > 2)
				/* if its been pressed and released three times.... */
				{
					startpresscount=0; //reset the count
					Program_Save_Mappings();
				}	
			}	
			if (programmode == PROGRAMMODE_TURBOSET)
			{
				if (Real_Stick_Left) 
				{//clear the timer count to zero and clear turboavailable flag.
					for(temp=0;temp<8;temp++) turbocount[temp]=0;
					turboavailable=0x00;
				}
				if (turboadjuststickflag != 0)
				{
					//clear the flag if stick is in neutral
					if (Real_Stick_Up && Real_Stick_Down) turboadjuststickflag=0;
					
				}
				else
				{ //the stick was last in neutral
					if (!Real_Stick_Up)
					{
						turboadjuststickflag=1;
						if (!Real_Stick_Jab && (turboholder[JAB] != 0 ))	turboholder[JAB]--;
						if (!Real_Stick_Strong && (turboholder[STR] != 0 )) 	turboholder[STR]--;
						if (!Real_Stick_Fierce && (turboholder[FIE] != 0 )) 	turboholder[FIE]--;
						if (!Real_Stick_Short && (turboholder[SHO] != 0 ))		turboholder[SHO]--;
						if (!Real_Stick_Forward && (turboholder[FOR] != 0 )) 	turboholder[FOR]--;
						if (!Real_Stick_Roundhouse && (turboholder[ROU] != 0 )) turboholder[ROU]--;
						#ifdef EXTRA_BUTTONS
							if (!Real_Stick_Extra0 && (turboholder[EX0] != 0 )) 	turboholder[EX0]--;
							if (!Real_Stick_Extra1 && (turboholder[EX1] != 0 ))	 	turboholder[EX1]--;
						#else
							if (!Real_Stick_Select && (turboholder[SEL] != 0 )) 	turboholder[SEL]--;
							if (!Real_Stick_Start && (turboholder[STA] != 0 ))	 	turboholder[STA]--;
						#endif
					}
					if (!Real_Stick_Down)
					{
						turboadjuststickflag=1;	
						if (!Real_Stick_Jab && (turboholder[JAB] != 0xFF ))	turboholder[JAB]++;
						if (!Real_Stick_Strong && (turboholder[STR] != 0xFF )) 	turboholder[STR]++;
						if (!Real_Stick_Fierce && (turboholder[FIE] != 0xFF )) 	turboholder[FIE]++;
						if (!Real_Stick_Short && (turboholder[SHO] != 0xFF ))		turboholder[SHO]++;
						if (!Real_Stick_Forward && (turboholder[FOR] != 0xFF )) 	turboholder[FOR]++;
						if (!Real_Stick_Roundhouse && (turboholder[ROU] != 0xFF )) turboholder[ROU]++;
						#ifdef EXTRA_BUTTONS
							if (!Real_Stick_Extra0 && (turboholder[EX0] != 0xFF )) 	turboholder[EX0]++;
							if (!Real_Stick_Extra1 && (turboholder[EX1] != 0xFF ))	 	turboholder[EX1]++;
						#else
							if (!Real_Stick_Select && (turboholder[SEL] != 0xFF )) 	turboholder[SEL]++;
							if (!Real_Stick_Start && (turboholder[STA] != 0xFF ))	 	turboholder[STA]++;
						#endif
					}
					
				}
			}
			return 1;
		}
		else
		{
			if (!Real_Stick_Program  && !tournamentmode) //mode is not active, but program button is pressed
			{
				
				#ifdef EXTRA_BUTTONS
						unsigned char tempmap;
						tempmap=( PORTD & 0b00111111);
						if (Real_Stick_Extra0)   		tempmap |= 0b01000000;
						
						if (Real_Stick_Extra1)   		tempmap |= 0b10000000;
						
						button_to_map=tempmap;
				#else
						button_to_map=PORTD;
				#endif
				if (button_to_map==0xFF)
				{
					programmode=PROGRAMMODE_TURBOSET;
				}
				else
				{
					programmode=PROGRAMMODE_REMAPPING;
				}
				startpresscount=0; //reset the startpresscount 
				return 1;	
			}
		}
		
		
		#ifdef EXTRA_BUTTONS
			temp=( PORTD & 0b00111111);
			if (Real_Stick_Extra0) temp |=0b01000000;
			if (Real_Stick_Extra1) temp |=0b10000000;
			return ((temp | buttonmapping[SHO] | turboavailable )==0xFF);		
		#else
			return ((turboavailable | PORTD | buttonmapping[SHO])==0xFF);
		#endif //EXTRA_BUTTONS
	}
	
	unsigned char S_Forward(void)	
	{
		unsigned char temp;
		if (programmode) return 1;
		#ifdef EXTRA_BUTTONS
			temp=( PORTD & 0b00111111);
			if (Real_Stick_Extra0) temp |=0b01000000;
			if (Real_Stick_Extra1) temp |=0b10000000;
			return ((turboavailable | temp | buttonmapping[FOR])==0xFF);		
		#else
			return ((turboavailable | PORTD | buttonmapping[FOR])==0xFF);
		#endif //EXTRA_BUTTONS
	}
	
	unsigned char S_Roundhouse(void)	
	{
		unsigned char temp;
		if (programmode) return 1;
		#ifdef EXTRA_BUTTONS
			temp=( PORTD & 0b00111111);
			if (Real_Stick_Extra0) temp |=0b01000000;
			if (Real_Stick_Extra1) temp |=0b10000000;
			return ((turboavailable | temp | buttonmapping[ROU])==0xFF);		
		#else
			return ((turboavailable | PORTD | buttonmapping[ROU])==0xFF);
		#endif //EXTRA_BUTTONS
	}
	
	#ifdef EXTRA_BUTTONS
	
		unsigned char S_Select(void)
		{
			if (programmode) return 1;
			return Real_Stick_Select;
		}	
		
		unsigned char S_Start(void)
		{
			if (programmode) return 1;
			if (tournamentmode)
			{
				if (tournamentmode==2 && Real_Stick_Start)
				{ //Command to disable tournament mode seen, and start is NOT pressed.
				  //go ahead and disable tournament mode.
				  tournamentmode=0;				
				}
				if ((!Real_Stick_Start) && (!Stick_Select) && (!Stick_Down))
				{ //set flag to exit tournament mode
					tournamentmode=2;
					return 1;
				}
				else
				{
					return 1; //dont show start pressed, even if it is
				}
			}
			else
			{ //not in tournament mode
				if ((!Stick_Select) && (!Stick_Up) && (!Real_Stick_Start))
				{ // enter tournament mode.
					tournamentmode=1;
					return 1;
				}
				else
				{ //not tournament mode, and not setting tournament mode, so return start
					return Real_Stick_Start;
				}
			}
		}
		unsigned char S_Extra0(void)
		{
			unsigned char temp;
			if (programmode) return 1;
			temp=( PORTD & 0b00111111);
			if (Real_Stick_Extra0) { temp |=0b01000000; } 
			if (Real_Stick_Extra1) { temp |=0b10000000; } 
			return ((turboavailable | temp | buttonmapping[EX0])==0xFF);		
		}
			
		unsigned char S_Extra1(void)
		{
			unsigned char temp;
			if (programmode) return 1;
			temp=( PORTD & 0b00111111 );
			if (Real_Stick_Extra0) { temp |=0b01000000; } 
			if (Real_Stick_Extra1) { temp |=0b10000000; } 
			return ((turboavailable | temp | buttonmapping[EX1])==0xFF);		
		}		
	#else
		unsigned char S_Select(void)	
		{
			unsigned char temp;
			if (programmode) return 1;
			return ((turboavailable | PORTD | buttonmapping[SEL])==0xFF);	
		}
		unsigned char S_Start(void)
		{
			if (tournamentmode)
			{
				if (tournamentmode==2 && Real_Stick_Start)
				{ //Command to disable tournament mode seen, and start is NOT pressed.
				  //go ahead and disable tournament mode.
				  tournamentmode=0;				
				}
				if ((!Real_Stick_Start) && (!Real_Stick_Select) && (!Stick_Down))
				{ //set flag to exit tournament mode
					tournamentmode=2;
					return 1;
				}
				else
				{
					return 1; //dont show start pressed, even if it is
				}
			}
			else
			{ //not in tournament mode
				if ((!Real_Stick_Select) && (!Stick_Up) && (!Real_Stick_Start))
				{ // enter tournament mode.
					tournamentmode=1;
					return 1;
				}
				else
				{ //not tournament mode, and not setting tournament mode, so return start
					if (programmode) return 1;
					return ((turboavailable | PORTD | buttonmapping[STA])==0xFF);	
				}
			}
		}
	#endif // EXTRA_BUTTONS
	
#else // PROGRAM_BUTTON
	
	unsigned int S_Start(void)
	{
		if (tournamentmode)
		{
			if ((!Real_Stick_Start) && (!Stick_Select) && (!Stick_Down))
			{ //exit tournament mode
				tournamentmode=0;
				return 1;
			}
			else
			{
			return 1; //dont show start pressed, even if it is
			}
		}
		else
		{ //not in tournament mode
			if ((!Stick_Select) && (!Stick_Up) && (!Real_Stick_Start))
			{ // enter tournament mode.
				tournamentmode=1;
				return 1;
			}
			else
			{ //not tournament mode, and not setting tournament mode, so return start
				return Real_Stick_Start;
			}
		}
	}


#endif  //PROGRAM_BUTTON

