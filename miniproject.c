#include<lpc21xx.h>          // Header file for LPC2148 registers
#include<stdlib.h>
#include "miniproject_defines.h"  // Project macros
#include "miniproject.h"         // Function declarations
#include "types.h"            // User-defined data types
#include "defines.h"           
#include "delay.h"            // Delay functions
#include "struct.h"           // Structure for medicine schedule

u32 val;               // Variable to store keypad input value
s32 digit=0;           // Counter to track number of digits entered
s32 t1, t2, t3;         // Temporary variables
extern u32 time, cnt;   // time stores minute when buzzer started  
												// cnt counts number of medicine schedules stored

extern u32 flag, flag1;                         // Setup flag
extern s32 hour,min,sec,date,month,year,day;    //RTC variables
medicine_time M[MAX];	                          // Array of structures to store multiple medicine schedules

// Keypad Look-Up Table (maps row & column to actual key values)
u8 kpmLUT[4][4]={{'7','8','9','/'},
									{'4','5','6','*'},
									{'1','2','3','-'},
									{'C','0','=','+'}};

// Array to hold names of days of the week
char week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

//lcd funtion definitions

/*This function sends one byte (8-bit data)/command
	to the LCD data pins
*/
void WriteLCD(u8 data)
{
	SCLRBIT(IOCLR0,RW);             //set RW=0, selects write operation
	WBYTE(IOPIN0,LCD_DATA,data);    // Send 8-bit data to LCD data pins (P0.8–P0.15)
	SSETBIT(IOSET0,EN);            //EN=1, Enable signal HIGH(start data latch)
	delay_us(1);                   // Small delay for LCD to read the data
	SCLRBIT(IOCLR0,EN);            //EN=0,  Falling edge -> LCD latches the data
	delay_ms(2);                  // for internal process
}

//This function sends a command to the LCD.
void CmdLCD(u8 cmd)
{
	SCLRBIT(IOCLR0,RS);        //RS=0, command reg selected
	WriteLCD(cmd);             // Send command to LCD 
}

//This function displays a single character on the LCD.
void CharLCD(u8 ascii)
{
	SSETBIT(IOSET0,RS);        //RS=1, data reg selected
	WriteLCD(ascii);           // Send ASCII character to LCD for display
}
void InitLCD(void)
{
	WBYTE(IODIR0,LCD_DATA,255);    //P0.8 to P0.15 as outputs
	SETBIT(IODIR0,RS);             //p0.16 as output
	SETBIT(IODIR0,RW);						 //p0.17 as output
	SETBIT(IODIR0,EN);             //p0.18 as output
	
	delay_ms(15);
	CmdLCD(MODE_8BIT_1LINE);
	delay_ms(5);
	CmdLCD(MODE_8BIT_1LINE);
	delay_us(100);
	CmdLCD(MODE_8BIT_1LINE);
	
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DISP_ON);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}

void StrLCD(s8* p)
{
	while(*p)
	CharLCD(*p++);
}

void U32LCD(u32 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n)
		{
			a[i++]=(n%10)+48;
			n/=10;
		}
		for(--i;i>=0;i--)
		CharLCD(a[i]);
	}
	
}

//rtc function definitions
/*
Initialize the Real-Time Clock (RTC)
This function disables the RTC, sets the prescaler values, 
and then enables the RTC.
*/
void RTC_Init(void) 
{
  // Disable and reset the RTC
	CCR = RTC_RESET;
	
	#ifndef _LPC2148
  
  // Set prescaler integer and fractional parts
	PREINT = PREINT_VAL;
	PREFRAC = PREFRAC_VAL;
  
  // Enable the RTC
	CCR = RTC_ENABLE; 

	#else
  // Enable the RTC with external clock source
	CCR = RTC_ENABLE | RTC_CLKSRC;	
	#endif
}

/*
Get the current RTC time
hour Pointer to store the current hour
minute Pointer to store the current minute
second Pointer to store the current second
*/
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
	*hour = HOUR;
	*minute = MIN;
	*second = SEC;
}

/*
Display the RTC time on LCD
hour value (0-23)
minute value (0-59)
second value (0-59) seperated by ':'
*/
void DisplayRTCTime(u32 hour, u32 minute, u32 second)
{
	CmdLCD(GOTO_LINE1_POS0);
	CharLCD(hour/10+48);
	CharLCD(hour%10+48);
	CharLCD(':');
	CharLCD(minute/10+48);
	CharLCD(minute%10+48);
	CharLCD(':');
	CharLCD(second/10+48);		
	CharLCD(second%10+48);	
}

/*
Get the current RTC date
day Pointer to store the current date (1-31)
month Pointer to store the current month (1-12)
year Pointer to store the current year (four digits)
*/
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year)
{
	*date = DOM;
	*month = MONTH;
	*year = YEAR;
}

/*
Display the RTC date on LCD
Day of month (1-31)
Month (1-12)
Year (four digits) and seperated by '/'
*/
void DisplayRTCDate(u32 date, u32 month, u32 year)
{
	CmdLCD(GOTO_LINE2_POS0);
	CharLCD(date/10+48);
	CharLCD(date%10+48);
	CharLCD('/');
	CharLCD(month/10+48);
	CharLCD(month%10+48);
	CharLCD('/');
	U32LCD(year);
}

/*
Set the RTC time
Hour to set (0-23)
Minute to set (0-59)
Second to set (0-59)
*/
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second)
{
	HOUR = hour;
	MIN = minute;
	SEC = second;
}

/*
Set the RTC date
day of month to set (1-31)
month to set (1-12)
year to set (four digits)
*/
void SetRTCDateInfo(u32 date, u32 month, u32 year)
{
	DOM = date;
	MONTH = month;
	YEAR = year;
}

/*
Get the current day of the week
dow Pointer to store Day of Week (0=Sunday, ..., 6=Saturday)
*/
void GetRTCDay(s32 *dow)
{
	*dow = DOW; 
}

/*
Display the current day of the week on LCD
dow (Day of Week) (0=Sunday, ..., 6=Saturday)
*/
void DisplayRTCDay(u32 dow)
{
	CmdLCD(GOTO_LINE1_POS0+10);
	StrLCD(week[dow]);
  
}

/*
Set the day of the week in RTC
Day of Week to set (0=Sunday, ..., 6=Saturday)
*/
void SetRTCDay(u32 dow)
{
	DOW = dow;
}

//kpm function definitions
void InitKPM(void)
{
	WNIBBLE(IODIR1,ROW0,15);
}

u32 ColScan(void)
{
	return((RNIBBLE(IOPIN1,COL0)<15)?0:1);
}
u32 RowCheck(void)
{
	u32 rno;
	for(rno=0; rno<4; rno++)
	{
		WNIBBLE(IOPIN1,ROW0,~(1<<rno));
		if(ColScan()==0)
		{
			break;
		}
	}
	//make rows as default
	WNIBBLE(IOPIN1,ROW0,0x0);
	return rno;
}
u32 ColCheck(void)
{
	u32 cno;
	for(cno=0; cno<4; cno++)
	{
		if(RBIT(IOPIN1,(COL0+cno))==0)
		{
			break;
		}
	}
	return cno;
}

u32 KeyScan(void)
{
	u32 rno,cno,keyv;
	//wait for switch press
	while(ColScan());
	//find the row_no
	rno=RowCheck();
	//find the col_no
	cno=ColCheck();
	//get the key value using kpmLUT
	keyv=kpmLUT[rno][cno];
	//wait for switch release
	while(!ColScan());
	return keyv;
}
u32 ReadNum(void)
{
	u8 key;
	u32 sum=0;
	while(1)
	{
		key=KeyScan();
		if(key>='0'&&key<='9')
		{
			digit++;
			CharLCD(key);
			sum=(sum*10)+(key-'0');
			
		}
		else if((key=='C')&&(digit>0))
		{
			digit--;
			if(digit==-1)
			{
				digit=0;
			}
			sum/=10;
			CmdLCD(0x10);
			CharLCD(' ');
			CmdLCD(0x10);
		}
		else if(key=='=')
		{
			break;
		}
	}
	return sum;
}

//interrupt
void enable_interrupts(void)
{
  //cfg P0.1, P0.3 pin as EINT0, EINT1 input pins
	//clear  bit 2&3, 6&7 without affecting other bits
	PINSEL0&=((u32)~3<<2) | ((u32)~3<<6);
	//update bit 2&3, bit 6&7 for EINT0, EINT1 pin function
	PINSEL0|= EINT0_SWITCH1 | EINT1_SWITCH2;
	
	//cfg VIC peripheral block 
	//allow EINT0, EINT1 as irq type
	//VICIntSelect=0;    //default
	//enable EINT0, EINT1 through channel
	VICIntEnable=1<<EINT0_VIC_CHNO | 1<<EINT1_VIC_CHNO;
	
	//cfg EINT0 as v.irq with highest priority(0)
	VICVectCntl0=(1<<5)|EINT0_VIC_CHNO;
	
	//load eint0_isr address into LUT sfr
	VICVectAddr0=(u32)eint0_isr;
	
	//cfg EINT1 as v.irq with next highest priority(0)
	VICVectCntl1=(1<<5)|EINT1_VIC_CHNO;
	
	//load eint1_isr address into LUT sfr
	VICVectAddr1=(u32)eint1_isr;
	
	//cfg EINT0, EINT1 via external interrupt peripheral
	//enable EINT0, EINT1
	//EXTINT=0;   //default
	
	//cfg EINT0, EINT1 as edge triggered interrupt
	EXTMODE=((1<<1) | (1<<0));
	
	//cfg EINT0, EINT1 as falling edge triggered
	//EXTPOLAR=0;   //default

}

void eint0_isr(void) __irq
{
	flag=1;	
	
	//clear EINT0 status in EXT INT peripheral
	EXTINT=1<<0;
	
	//clear EINT0 status in VIC peripheral
	VICVectAddr=0;
}

void eint1_isr(void) __irq
{
	flag=3;
	
	//clear EINT0 status in EXT INT peripheral
	EXTINT=1<<1;
	
	//clear EINT0 status in VIC peripheral
	VICVectAddr=0;
}

void setup_mode(void)
{
	while(1)
	{
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("1.Edit RTC Time");
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("2.Cfg Medi");
		CmdLCD(GOTO_LINE2_POS0+10);
		StrLCD("3.Exit");
		val=KeyScan();
		CmdLCD(0x01);
		switch(val)
		{
			case '1':Edit_menu();
							break;
			case '2':Cfg_menu();
							break;
			case '3':return;
		}
	}
}
void Edit_menu(void)
{
	while(1)
	{
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("1.Time 2.Date");
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("3.Day 4.Exit");
		val=KeyScan();
		switch(val)
		{
			case '1':Edit_RTC_Time();
							break;
			case '2':Edit_RTC_Date();
							break;
			case '3':Edit_RTC_Day();
							break;
			case '4':return;
		}
		// Set the time (hours, minutes, seconds)
		SetRTCTimeInfo(hour,min,sec);
	
    // Set the data (date, month, year)
		SetRTCDateInfo(date,month,year);
	
    // Set day (SUN to SAT )
		SetRTCDay(day); 
	}
}
void Cfg_menu(void)
{	
	u8 c;
		do
		{
			flag1=1;
			CmdLCD(0x01);
			CmdLCD(0x80);
			StrLCD("Enter medi time");
			delay_ms(500);
			CmdLCD(0x01);
			
			//configure hour
	    l1:StrLCD("Enter hr(0-23):");
			CmdLCD(GOTO_LINE2_POS0);
			t1=ReadNum();
			CmdLCD(0x01);
			if((t1>=0) && (t1<=23))
			{
				M[cnt].hr=t1;
			}
			else
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("Invalid input");
				delay_ms(500);
				CmdLCD(0x01);
				goto l1;
			}
			
			//configure minute
			l2:StrLCD("Enter min(0-59):");
			CmdLCD(GOTO_LINE2_POS0);
			t2=ReadNum();
			CmdLCD(0x01);
			if((t2>=0) && (t2<=59))
			{
				M[cnt].minute=t2;
			}
			else
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("Invalid input");
				delay_ms(500);
				CmdLCD(0x01);
				goto l2;
			}
			
			//configure second
			l3:StrLCD("Enter sec(0-59):");
			CmdLCD(GOTO_LINE2_POS0);
			t3=ReadNum();
			CmdLCD(0x01);
			if((t3>=0) && (t3<=59))
			{
				M[cnt].second=t3;
			}
			else
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("Invalid input");
				delay_ms(500);
				CmdLCD(0x01);
				goto l3;
			}
	
			//display configured medicine time
			CmdLCD(0x80);
			StrLCD("medicine time:");
			CmdLCD(0xC0);
			U32LCD(M[cnt].hr);
			CharLCD(':');
			U32LCD(M[cnt].minute);
			CharLCD(':');
			U32LCD(M[cnt].second);
			delay_s(3);
			CmdLCD(0x01);
			StrLCD("Enter + to add ");
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("one more medi time");
			delay_s(3);
			CmdLCD(0x01);
			cnt++;
			c=KeyScan();
		}while((c=='+') && (cnt<MAX));
}

void Edit_RTC_Time(void)
{
	while(1)
	{
		CmdLCD(0x01);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("1.hour 2.min");
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("3.Both 4.Exit");
		val=KeyScan();
		CmdLCD(0x01);
		switch(val)
		{
			case '1':set_hour();
							break;
			case '2':set_min();
							break;
			case '3':set_hour();
							CmdLCD(0x01);
							set_min();
							break;
			case '4':return;
		}
	}
}
void Edit_RTC_Date(void)
{	
	while(1)
	{
		CmdLCD(0x01);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("1.date 2.month");
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("3.yr 4.all 5.Exit");
		val=KeyScan();
		CmdLCD(0x01);
		switch(val)
		{
			case '1':set_date();
							break;
			case '2':set_month();
							break;
			case '3':StrLCD("Enter year:");
							year=ReadNum();
							break;
			case '4':set_date();
							set_month();
							StrLCD("Enter year:");
							year=ReadNum();
							break;
			case '5':return;
		}
	}
}
void Edit_RTC_Day(void)
{
	l1:CmdLCD(0x01);
		StrLCD("Enter day(0-6):");
		delay_s(2);
		CmdLCD(0x01);
		StrLCD("SUN,MON,TUE,WED");
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("THU,FRI,SAT");
		delay_s(2);
		CmdLCD(0x01);
		StrLCD("Day:");
		day=ReadNum();
		if((day<0)||(day>6))
		{
			CmdLCD(0x01);
			StrLCD("Invalid Input!");
			delay_s(1);
			goto l1;
		}

}
void set_hour(void)
{
	l1:StrLCD("Enter hr(0-23):");
		 CmdLCD(GOTO_LINE2_POS0);
		 t1=ReadNum();
		 CmdLCD(0x01);
		 if((t1>=0) && (t1<=23))
		 {
			 hour=t1;
		 }
		 else
		 {
			 CmdLCD(GOTO_LINE1_POS0);
			 StrLCD("Invalid input");
			 delay_ms(500);
			 CmdLCD(0x01);
			 goto l1;
		 }
}
void set_min(void)
{
	l2:StrLCD("Enter min(0-59):");
	CmdLCD(GOTO_LINE2_POS0);
	t2=ReadNum();
	CmdLCD(0x01);
	if((t2>=0) && (t2<=59))
	{
		min=t2;
	}
	else
	{
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("Invalid input");
		delay_ms(500);
		CmdLCD(0x01);
		goto l2;
	}
}
void set_date(void)
{
	l1:StrLCD("Enter date(0-31):");
	CmdLCD(GOTO_LINE2_POS0);
	t1=ReadNum();
	CmdLCD(0x01);
	if((t1>=0) && (t1<=31))
	{
		date=t1;
	}
	else
	{
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("Invalid input");
		delay_ms(500);
	  CmdLCD(0x01);
		goto l1;
	}
}
void set_month(void)
{
	l2:StrLCD("Enter month(0-12):");
	CmdLCD(GOTO_LINE2_POS0);
	t2=ReadNum();
	CmdLCD(0x01);
	if((t2>=0) && (t2<=12))
	{
		month=t2;
	}
	else
	{
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("Invalid input");
		delay_ms(500);
		CmdLCD(0x01);
	  goto l2;
	}
}
void Medicine_Alert(void)
{
	flag=2;
	time=min;
	IOSET0 = 1<<EINT0_STATUS_BUZZER;		 //buzzer ON
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("Take Medicine");
}
