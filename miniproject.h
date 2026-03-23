#include "types.h"

//lcd function declarations
void WriteLCD(u8 data);
void CmdLCD(u8 cmd);
void CharLCD(u8 ascii);
void InitLCD(void);
void StrLCD(s8* p);
void U32LCD(u32 n);

//rtc function declarations
void RTC_Init(void);
void GetRTCTimeInfo(s32 *,s32 *,s32 *);
void DisplayRTCTime(u32,u32,u32);
void GetRTCDateInfo(s32 *,s32 *,s32 *);
void DisplayRTCDate(u32,u32,u32);

void SetRTCTimeInfo(u32,u32,u32);
void SetRTCDateInfo(u32,u32,u32);

void GetRTCDay(s32 *);
void DisplayRTCDay(u32);
void SetRTCDay(u32);

//kpm function declarations
void InitKPM(void);
u32 ColScan(void);
u32 RowCheck(void);
u32 ColCheck(void);
u32 KeyScan(void);
u32 ReadNum(void);

//interrupt function definitions
void enable_interrupts(void);
void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

void setup_mode(void);
void Edit_menu(void);
void Cfg_menu(void);
void Edit_RTC_Time(void);
void Edit_RTC_Date(void);
void Edit_RTC_Day(void);
void set_hour(void);
void set_min(void);
void set_date(void);
void set_month(void);
void Medicine_Alert(void);

