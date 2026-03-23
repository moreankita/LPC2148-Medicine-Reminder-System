#include <lpc21xx.h>

#define MAX  4

//lcd defines
#define LCD_DATA 8   //P0.8
#define RS 16       //P0.16
#define RW 17       //P0.17
#define EN 18       //P0.18

//lcd_commands

#define CLEAR_LCD 0X01
#define RET_CUR_HOME 0x02
#define DISP_OFF 0x08
#define DISP_ON 0x0C
#define DISP_ON_CUR_ON 0x0E
#define DISP_ON_CUR_BLINK 0x0F
#define MODE_8BIT_1LINE 0X30
#define MODE_8BIT_2LINE 0X38
#define MODE_4BIT_1LINE 0X20
#define MODE_4BIT_2LINE 0X28
#define GOTO_LINE1_POS0 0X80
#define GOTO_LINE2_POS0 0XC0
#define GOTO_LINE3_POS0 0X94
#define GOTO_LINE4_POS0 0XD4
#define SHIFT_CUR_RIGHT 0X06
#define SHIFT_DISP_LEFT 0x10
#define SHIFT_DISP_RIGHT 0X14

//rtc defines
// System clock and peripheral clock Macros
#define FOSC 12000000
#define CCLK (5*FOSC)    
#define PCLK (CCLK/4)

// RTC Prescaler Calculation Macros
// RTC requires 32.768 kHz clock for 1-second increment.
// PREINT and PREFRAC registers divide PCLK to generate 32.768 kHz.

// PREINT = int (PCLK / 32768) - 1;
// PREFRAC = PCLK -((PREINT + 1) * 32768);
#define PREINT_VAL (int) ((PCLK / 32768) - 1)
#define PREFRAC_VAL (PCLK -((PREINT_VAL + 1) * 32768))

//RTC Control Register (CCR) Bit Definitions
// Bit 0 – Clock Enable --> 1 = Enable RTC counters  0 = Disable RTC counters
#define RTC_ENABLE (1<<0)

// Bit 1 – Clock Reset --> 1 = Reset RTC counters    0 = Normal operation
#define RTC_RESET (1<<1)

//only for LPC2148
// Bit 4 – Clock Source Select 
// 1 = Use external 32.768 kHz oscillator
// 0 = Use internal PCLK as RTC clock source
#define RTC_CLKSRC (1<<4)

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

#define _LPC2148

//kpm defines
#define ROW0 16 //p1.16
#define ROW1 17
#define ROW2 18
#define ROW3 19

#define COL0 20
#define COL1 21
#define COL2 22
#define COL3 23 //p1.23

//interrupts defines
#define EINT0_SWITCH1        0x0000000C
#define EINT0_VIC_CHNO       14
#define EINT0_STATUS_BUZZER  23      //P0.23


#define EINT1_SWITCH2        0x000000C0
#define EINT1_VIC_CHNO       15
  
