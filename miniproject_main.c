#include<lpc21xx.h>              // Header file for LPC2148 registers
#include "types.h"               // User-defined data types
#include "delay.h"               // Delay functions
#include "miniproject_defines.h"    // Project macros
#include "miniproject.h"           // Function declarations
#include "struct.h"               // Structure for medicine schedule

s32 hour,min,sec,date,month,year,day;   // RTC variables
u32 flag=0, flag1=0;                    // Setup flag
u32 time=0, cnt=0;

extern medicine_time M[MAX];

int main()
{
		u32 i;
	
		IODIR0|=1<<EINT0_STATUS_BUZZER;   // Set buzzer pin as output
	
    // Initialize RTC 
		RTC_Init();
    // Initialize the LCD
		InitLCD();
		//Initialize the LCD
		InitKPM();
		//Enable interrupts
		enable_interrupts();

    while (1) 
    {
			if(flag==0)
			{
				//CmdLCD(CLEAR_LCD);
        // Get and display the current time info on LCD
				GetRTCTimeInfo(&hour,&min,&sec);
				DisplayRTCTime(hour,min,sec);
			
				// Get and display the current date info on LCD
				GetRTCDateInfo(&date,&month,&year);
				DisplayRTCDate(date,month,year);
			
				// Get and display the current day info on LCD
				GetRTCDay(&day);
				DisplayRTCDay(day);
			}	
			if(flag==1)   //switch1 is pressed
			{
				setup_mode();      // Configure timings
				flag=0;
			}
			if(flag==3)    //switch2 is pressed
      {
         //OFF buzzer as status that interrupt occured
         IOCLR0=1<<EINT0_STATUS_BUZZER;
         CmdLCD(CLEAR_LCD);              
         StrLCD("Medicine taken!");      
         delay_s(4);
         //clear reminder
         CmdLCD(CLEAR_LCD);
         flag=0;
			}
			
			// Compare medicine times
			for(i=0 ; i<cnt; i++)
			{
				if((M[i].hr==hour) && (M[i].minute==min)&& (M[i].second==sec) && (flag1==1))
				{
					 Medicine_Alert();       //alert message
				}
					
				GetRTCTimeInfo(&hour,&min,&sec);   // Get the current time info 

				// Check if 1 minute has passed after alert started
				// AND ensure system is currently in alert state (flag == 2)
				if((time!=min)&&(flag==2))
				{
					IOCLR0 = 1<<EINT0_STATUS_BUZZER;   // Turn OFF buzzer
					CmdLCD(CLEAR_LCD);                  // Clear LCD display
					flag=0;                            // Reset flag , exit alert state and return to normal operation
				}
			}			
    }
}
