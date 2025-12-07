#include <lpc214x.h>
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "delay.h"
#include "rtc.h"
#include "lcd.h"
#include "types.h"
#include <string.h>
void Init_EINT(void);
char pass1[6] = "123456";
char pass2[6];
s32 hour,min,sec,date,month,year,day;
s32 hr1=11,hr2=20, min1=0, min2=0;
char src[100] = "Thanks for coming In-Time, Press any key to exit                ";
// Main function
void init_system()
{
  	RTC_Init();	// Initialize RTC 	
  	Init_LCD();  // Initialize the LCD
	Init_KPM();  //Intilize the KPM
	Init_EINT(); //turn on the interuppt
	 
  // Set the initial time (hours, minutes, seconds)
	SetRTCTimeInfo(12,14,00);
	SetRTCDateInfo(30,10,2025);
	SetRTCDay(4);
}

 
void display_RTC(){
    // Get and display the current time on LCD
		GetRTCTimeInfo(&hour,&min,&sec);
		DisplayRTCTime(hour,min,sec);
		GetRTCDateInfo(&date,&month,&year);
		DisplayRTCDate(date,month,year);
		GetRTCDay(&day);
		DisplayRTCDay(day);
}

u32 password()
{
	s32 i;
	s32 res=1;
	char key;
	CmdLCD(0x01);
	StrLCD("Enter PIN");
	CmdLCD(0XC0);
	delay_ms(300);
	
	for(i=0; i<6; i++)
	{
		
		key = KeyScan();
		
		if(key=='C')
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
		}
		if(key!='C'){
			CmdLCD(0XC0+i);
			CharLCD(key);
			pass2[i] = key;
		}
		delay_ms(200);
	}
	for(i=0; i<6; i++)
	{
		if(pass1[i] != pass2[i]) res=0;
	}
	CmdLCD(0x01);
	return res;
}

s32 check_time(void)
{
        if(HOUR>=hr1 && HOUR<=hr2)
        {
            return 1;
        }
        else 
					return 0;
}

void operation()
{
	char temp;
	int i;
	CmdLCD(0x01);
	StrLCD("Loading");
	delay_ms(300);
	CmdLCD(0x01);
	StrLCD("Loading.");
	delay_ms(300);
	CmdLCD(0x01);
	StrLCD("Loading..");
	delay_ms(300);
	CmdLCD(0x01);
	StrLCD("Loading...");
	delay_ms(300);
	CmdLCD(0x01);
	
	while(1)
	{
		CmdLCD(0x80);
		StrLCD("Valid time");
		CmdLCD(0xc0);
		StrLCD(src);
		delay_ms(50);
		temp = src[0];
		for(i=0; src[i]; i++)
		{
			src[i] = src[i+1];
		}
		src[i-1]=temp;
		if(!ColScan()) {
			CmdLCD(0x01);
			return;
		}
	}
}
