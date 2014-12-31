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
#include "tg16.h"
#include "upcb.h"

void TG16_Init_Pins()
{
	TRISEbits.TRISE0=INPUT;	//#define TG16_ENABLE			PORTEbits.RE0
	TRISAbits.TRISA5=INPUT;	//#define TG16_BANKSEL		PORTAbits.RA5
	TRISBbits.TRISB0=OUTPUT; //#define TG16_DATA1			LATBbits.LATB0
	TRISCbits.TRISC6=OUTPUT; //#define TG16_DATA2			LATCbits.LATC6
	TRISCbits.TRISC7=OUTPUT; //#define TG16_DATA3			LATCbits.LATC7
	TRISBbits.TRISB1=OUTPUT; //#define TG16_DATA4			LATBbits.LATB1
}

void TG16_main()
{
	// we need a flag to see if we're in sixbutton mode or not, a flag for if the sixbutton mode
	//button combination is currently held down, and a count of the number of time's Enable has run
	unsigned char sixbuttonmode, sixbuttonpress, enablecount;
	// we need a flag to see if the button for 'I' autofire and 'II' autofire is held down, 
	// and markers to see what the autofire for the I and II is. 0=off, 1= every 8 frames,2=every 4 frames
	unsigned char IAFpress, IIAFpress, IAF, IIAF;
	sixbuttonmode=sixbuttonpress=enablecount=0;
	IAFpress=IIAFpress=IAF=IIAF=0;
	
	while(1)
	{
		while(!sixbuttonmode)
		{ //default two button mode
			//we dont do anything unless Enable is low
			
			if (TG16_ENABLE) // if enable is high...
			{	
				while (TG16_ENABLE);//sit and wait for the enable to drop 
				//kick enablecount up a notch, or return it to 0 if 7 or more
				if (enablecount>6)  
				{
					enablecount=0;
				}
				else
				{
					enablecount++;
				}
			}
			/* this enables us to keep polling the select line repeated without increasing the 
				enable count except on enables transition to low */  
			
			//check which bank it wants data for
			if (TG16_BANKSEL)
			{  //bank B, Up Right Down Left
				TG16_DATA1=Stick_Up;
				TG16_DATA2=Stick_Right;
				TG16_DATA3=Stick_Down;
				TG16_DATA3=Stick_Left;
			}
			else
			{ //bank A. I II Select Start
				//now we have to check the IAF settings
				if (IAF==1) //if we press every 8 frames
				{
					if (enablecount==0) //and this is that frame
					{
						TG16_DATA1=Stick_Forward; //then the data shoudl be the same as the button
					}
					else 
					{  //if its NOT that frame
						TG16_DATA1=1; // then the button wont be pressed no matter what
					}
				}
				if (IAF==2) //if we press every 4 frames
				{
					if ((enablecount==0) || (enablecount==4))
					{ //and this is one of those frames
						TG16_DATA1=Stick_Forward;
					}
					else
					{ //if not one of those frames
						TG16_DATA1=1; //then the button wont be pressed no matter what
					}
				}
				if (IAF==0) //normal operation
				{
					TG16_DATA1=Stick_Forward;
				}
				//now do the same for IIAF
				if (IIAF==1) //if we press every 8 frames
				{
					if (enablecount==0) //and this is that frame
					{
						TG16_DATA2=Stick_Short; //then the data shoudl be the same as the button
					}
					else 
					{  //if its NOT that frame
						TG16_DATA2=1; // then the button wont be pressed no matter what
					}
				}
				if (IIAF==2) //if we press every 4 frames
				{
					if ((enablecount==0) || (enablecount==4))
					{ //and this is one of those frames
						TG16_DATA2=Stick_Short;
					}
					else
					{ //if not one of those frames
						TG16_DATA2=1; //then the button wont be pressed no matter what
					}
				}
				if (IIAF==0) //normal operation
				{
					TG16_DATA2=Stick_Short;
				}
				// lastly, the super easy select and start
				TG16_DATA3=Stick_Select;
				TG16_DATA4=Stick_Start;
			}  //else banksel
			

/* Process the autofire button for 'I' ***************************************************/			
			//check for 'I' autofire button
			if (!Stick_Strong) //IAF button pressed
			{	//but we only care if we havent seen it before
				if (!IAFpress)  //we havent seen it before
				{
					IAFpress=0x1; //set the flag so we know we've seen it
					if (IAF>1)  //if already at fastest,
					{
						IAF=0; //disable autofire
					}
					else
					{	//kick it up a notch
						IAF++;
					}
				}
			}
			else //if the IAF is not pressed
			{
				if (IAFpress) //and we thought it was
				{
					IAFpress=0x0; //mark the flag that it is not held down
				}
			}
/* Process the autofire button for 'II' ***************************************************/			
			//check for 'II' autofire button
			if (!Stick_Jab) //IIAF button pressed
			{	//but we only care if we havent seen it before
				if (!IIAFpress)  //we havent seen it before
				{
					IIAFpress=0x1; //set the flag so we know we've seen it
					if (IIAF>1)  //if already at fastest,
					{
						IIAF=0; //disable autofire
					}
					else
					{	//kick it up a notch
						IIAF++;
					}
				}
			}
			else //if the IIAF is not pressed
			{
				if (IIAFpress) //and we thought it was
				{
					IIAFpress=0x0; //mark the flag that it is not held down
				}
			}			
/* Process the buttons for toggling between modes ****************************************/
			//check for button combination to switch modes
			if ((!Stick_Start) && (!Stick_Select) && (!Stick_Fierce) && (!Stick_Roundhouse))
			{  //key combination is currently pressed
				// but since it will be checked tons if held down, we only care if we did NOT know 
				//about it before now
				if (!sixbuttonpress)
				{ //we didnt know about it before now.
					// so set the flag so we know it now
					sixbuttonpress=0x1;
					//and switch to six button mode next.
					sixbuttonmode=0x01;
				}	
			}
			else // key combination is not held
			{
				if (sixbuttonpress) //but we dont care unless we thought it was.
									//clear the flag so we'll catch it the next time 
									//its pressed.
				{ 
					sixbuttonpress=0x00;
				}
			}
		} //While not sixbuttonmode
/* End 2 button mode while loop ************************************************************/
		
		while(sixbuttonmode)
		{
			if (TG16_ENABLE) // if enable is high...
			{	
				while (TG16_ENABLE);//sit and wait for the enable to drop 
				//toggler enablecount between 0 and 1
				if (enablecount)  
				{
					enablecount=0;
				}
				else
				{
					enablecount=1;
				}
			}
			/* this enables us to keep polling the select line repeated without increasing the 
				enable count except on enables transition to low */  
			// no autofire to process in six button mode, so let's get to it
			if (enablecount)
			{ //first time through enable, so let's give it the standard two bank from the two
				// button controller
				if (TG16_BANKSEL)
				{ ////bank B, Up Right Down Left
					TG16_DATA1=Stick_Up;
					TG16_DATA2=Stick_Right;
					TG16_DATA3=Stick_Down;
					TG16_DATA4=Stick_Left;
				}
				else
				{
					//bank A. I, II, Select, Start
					TG16_DATA1=Stick_Forward;
					TG16_DATA2=Stick_Short;
					TG16_DATA3=Stick_Select;
					TG16_DATA4=Stick_Start;
				}
			}
			else
			{ //second batch, so with the extended buttons
				if (TG16_BANKSEL)
				{ //bank extended B, which is all Low 
					TG16_DATA1=0;
					TG16_DATA2=0;
					TG16_DATA3=0;
					TG16_DATA4=0;
				}
				else
				{ //bank extended A. III, IV, V, VI
					TG16_DATA1=Stick_Roundhouse;
					TG16_DATA2=Stick_Jab;
					TG16_DATA3=Stick_Strong;
					TG16_DATA4=Stick_Fierce;
				}
			}

/* Process keys to toggle between modes ****************************************************/		
		//check for button combination to switch modes
			if ((!Stick_Start) && (!Stick_Select) && (!Stick_Fierce) && (!Stick_Roundhouse))
			{  //key combination is currently pressed
				// but since it will be checked tons if held down, we only care if we did NOT know 
				//about it before now
				if (!sixbuttonpress)
				{ //we didnt know about it before now.
					// so set the flag so we know it now
					sixbuttonpress=0x1;
					//and switch back to two button mode next.
					sixbuttonmode=0x00;
				}	
			}
			else // key combination is not held
			{
				if (sixbuttonpress) //but we dont care unless we thought it was.
									//clear the flag so we'll catch it the next time 
									//its pressed.
				{ 
					sixbuttonpress=0x00;
				}
			}

					
		}
/* End Six button while loop ****************************************************************/
		
		
	}
	
}