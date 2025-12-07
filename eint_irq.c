//eint_irq
#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "lcd.h"
#include "kpm.h"
extern char pass1[6];
void Edit_RTC_Info(void);  /*declaration for Edit RTC info*/
void Edit_HOUR(void);  /*declaration for Edit HOUR*/ 
void Edit_MIN(void);   /*declaration for Edit minute*/
void Edit_SEC(void);  /*declaration for Edit second*/
void Edit_DAY(void);  /*declaration for Edit day*/
void Edit_DATE(void); /*declaration for Edit date*/
void Edit_MONTH(void); /*declaration for Edit month*/
void Edit_YEAR(void);  /*declaration for Edit year*/
void SetRTCTimeInfo(u32,u32,u32); 
void SetRTCDay(u32);
void SetRTCDateInfo(u32, u32, u32);
void CHANGE_PASSWORD(void);
void Edit_DEVICE_ALLOW_TIME(void);
void open_edit_menu(void);
u32 password(void);
#define EINT0_VIC_CHNO 14  
#define EINT0_STATUS_LED 30 //@p1.16
void einto_isr(void) __irq;
extern s32 hr1,hr2, min1, min2;
u32 Password_flag=0;
void Init_EINT()
{
	IODIR1 |= 1<<EINT0_STATUS_LED;	//cfg EINTO status led as gpio out;
	PINSEL0 |= 0x0000000C;	 //cfg p0.1 pin as EINT0 input pin
    
	VICIntEnable = 1<<EINT0_VIC_CHNO;  //Enabling EINTO via VIC
	VICVectCntl0 = (1<<5) | EINT0_VIC_CHNO;  //cfg EINT0 as highest priority 0
	VICVectAddr0 = (u32)einto_isr;  //loading eint0 address

	EXTMODE = 1<<0; //cfg EINT0 for edge trigger
}

void einto_isr(void) __irq
{
	
	if((!Password_flag) && password())
	{
		Password_flag=1;
		open_edit_menu();
		CmdLCD(0x01);
	}
	else if(Password_flag){
		open_edit_menu();
		CmdLCD(0x01);
	}
	else
	{
		CmdLCD(0x01);
		StrLCD("Wrong PIN");
		delay_ms(1000);
		CmdLCD(0x01);
	}

	EXTINT = 1<<0;
	VICVectAddr = 0;
}

void open_edit_menu()
{
	s32 flag=0;
	CmdLCD(0x01);
	while(1)
	{
		CmdLCD(0x80);
		StrLCD("1 E.RTC");
		CmdLCD(0x80+8);
		StrLCD("3 P CHG");
		CmdLCD(0xc0);
		StrLCD("2 E.D.A.T");
		CmdLCD(0xc0+8);
		StrLCD("4 EXIT");

		switch(KeyScan())
		{
			case '1': Edit_RTC_Info();    /*function call for Edit RTC_info*/ 
								break;
			case '2': Edit_DEVICE_ALLOW_TIME(); /*function call for Edit Allowablt time*/
								break;
			case '3': CHANGE_PASSWORD(); /*function call for password chnage*/
								break;
			case '4': CmdLCD(0x01); flag=1;
								return;
			default : {
								CmdLCD(0x01); StrLCD("Invalid Input");  
								delay_ms(1000);
								CmdLCD(0x01);
								break;
			}
		}
		if(flag==1) break;
	}
}

void Edit_RTC_Info(void) /*function defination for Edit RTC_Info*/
{
	u32 flag=0;
	while(1){
		CmdLCD(0x01);
		StrLCD("1-H 2-MI 3-S 4-D");
		CmdLCD(0xc0);
		StrLCD("5-D 6-MO 7-Y 8-E");
		delay_ms(300);
	
		switch(KeyScan())
		{
			case '1': Edit_HOUR(); /*Function call for Edit Hour*/break;
			case '2': Edit_MIN(); /*Function call for Edit minute*/break;
			case '3': Edit_SEC(); /*Function call for Edit second*/break;
			case '4': Edit_DAY(); /*Function call for Edit day*/break;
			case '5': Edit_DATE(); /*Function call for Edit date*/break;
			case '6': Edit_MONTH(); /*Function call for Edit date*/break;
			case '7': Edit_YEAR(); /*Function call for Edit date*/break;
			case '8': CmdLCD(0x01); flag=1; break;
			default : CmdLCD(0x01); StrLCD("Invalid Input");  
								delay_ms(1000);
								CmdLCD(0x01);
								break;
		}
		if(flag){
				delay_ms(200);			
			return;
		}
	}
}

void Edit_HOUR() /*Function defination for Edit Hour*/
{
	s32 hr=0, i;
	char key;
	CmdLCD(0x01);
	StrLCD("ENTER HOUR");
	CmdLCD(0xc0);
	StrLCD("CHr:");
	U32LCD(HOUR);
	StrLCD("  EHr:");
	CmdLCD(0xc0+12);
	for(i=12; i<17; i++)
	{
		delay_ms(300);
		key = KeyScan();
		if(key=='C' && i!=13)
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
		}
		if(key=='E') break;
		if(key!='C'){
			CmdLCD(0XC0+i);
			CharLCD(key);
			hr = (key-48)+hr*10;
		}
	}
	if(hr>=0 && hr<=23)
		SetRTCTimeInfo(hr, MIN, SEC);
	else
	{
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("Invalid HOURS");
		delay_ms(500);
		CmdLCD(0x01);
	}
	delay_ms(500);
	CmdLCD(0x01);
}

void Edit_MIN() /*Function defination for Edit minute*/
{
	s32 min=0, i;
	char key;
	CmdLCD(0x01);
	StrLCD("ENTER MINUTES");
	CmdLCD(0xc0);
	StrLCD("CMI:");
	U32LCD(MIN);
	StrLCD("  EMI:");
	CmdLCD(0xc0+12);
	for(i=12; i<17; i++)
	{
		delay_ms(300);
		key = KeyScan();
		if(key=='C' && i!=13)
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
		}
		if(key=='E') break;
		if(key!='C'){
			CmdLCD(0XC0+i);
			CharLCD(key);
			min = (key-48)+min*10;
		}
	}
	if(min>=0 && min<=59)
		SetRTCTimeInfo(HOUR, min, SEC);
	else
	{
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("Invalid minutes");
		delay_ms(500);
		CmdLCD(0x01);
	}
	delay_ms(500);
	CmdLCD(0x01);
}

void Edit_SEC() /*Function defination for Edit second*/
{
	s32 sec=0, i;
	char key;
	CmdLCD(0x01);
	StrLCD("Enter SECONDS");
	CmdLCD(0xc0);
	StrLCD("CSE:");
	U32LCD(SEC);
	StrLCD("  ESE:");
	CmdLCD(0xc0+12);
	for(i=12; i<17; i++)
	{
		delay_ms(300);
		key = KeyScan();
		if(key=='C' && i!=13)
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
		}
		if(key=='E') break;
		if(key!='C'){
			CmdLCD(0XC0+i);
			CharLCD(key);
			sec = (key-48)+sec*10;
		}
	}
	if(sec>=0 && sec<=59)
		SetRTCTimeInfo(HOUR, MIN, sec);
	else
	{
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("Invalid Seconds");
		delay_ms(500);
		CmdLCD(0x01);
	}
	delay_ms(500);
	CmdLCD(0x01);
}

void Edit_DAY(void) /*Function defination for Edit day*/
{
	s32 day;
	CmdLCD(0x01);
	StrLCD("Chose Choice");
	delay_ms(500);
	CmdLCD(0x01);
	StrLCD("0SU 1MO 2TU 3WED");
	CmdLCD(0xc0);
	StrLCD("4THU 5FRI 6SAT");
	day = KeyScan()-48;
	if(day>=0 && day<=6)
	{
		SetRTCDay(day);
		CmdLCD(0x01);
	}
	else
	{
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("Invalid day");
		delay_ms(500);
		CmdLCD(0x01);
	}
}

void Edit_DATE() /*Function defination for Edit date*/
{
	s32 date=0, i;
	char key;
	CmdLCD(0x01);
	StrLCD("Enter DATE");
	CmdLCD(0xc0);
	StrLCD(" Date:-");
	CmdLCD(0xc0+7);
	for(i=7; i<11; i++)
	{
		delay_ms(300);
		key = KeyScan();
		if(key=='C' && i!=7)
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
			date /= 10;
		}
		if(key=='E') break;
		if(key!='C'){
			CmdLCD(0XC0+i);
			CharLCD(key);
			date = (key-48)+date*10;
		}
	}
	if(date>=1 && date<=31)
		SetRTCDateInfo(date, MONTH, YEAR);
	else
	{
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("Invalid date");
		delay_ms(500);
		CmdLCD(0x01);
	}
	delay_ms(500);
	CmdLCD(0x01);
}

void Edit_MONTH() /*Function defination for Edit month*/
{
	s32 month=0, i;
	char key;
	CmdLCD(0x01);
	StrLCD("Enter MONTH");
	CmdLCD(0xc0);
	StrLCD("Month:-");
	CmdLCD(0xc0+7);
	for(i=7; i<11; i++)
	{
		delay_ms(300);
		key = KeyScan();
		if(key=='C' && i!=7)
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
			month /= 10;
		}
		if(key=='E') break;
		if(key!='C'){
			CmdLCD(0XC0+i);
			CharLCD(key);
			month = (key-48)+month*10;
		}
	}
	if(month>=1 && month<=12)
		SetRTCDateInfo(DOM, month, YEAR);
	else
	{
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("Invalid MONTH");
		delay_ms(500);
		CmdLCD(0x01);
	}
	delay_ms(500);
	CmdLCD(0x01);
}

void Edit_YEAR() /*Function defination for Edit year*/
{
	s32 year=0, i;
	char key;
	CmdLCD(0x01);
	StrLCD("Enter YEAR");
	CmdLCD(0xc0);
	StrLCD(" Year:-");
	CmdLCD(0xc0+7);
	for(i=7; i<11; i++)
	{
		delay_ms(300);
		key = KeyScan();
		if(key=='C' && i!=7)
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
				year /= 10;
		}
		if(key=='E') break;
		if(key!='C'){
			CmdLCD(0XC0+i);
			CharLCD(key);
			year = (key-48)+year*10;
		}
	}
	if(year>=1 && year<=4095)
		SetRTCDateInfo(DOM, MONTH, year);
	else
	{
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("Invalid YEAR");
		delay_ms(500);
		CmdLCD(0x01);
	}
	delay_ms(500);
	CmdLCD(0x01);
}

void CHANGE_PASSWORD(void)
{
	char Npass[6], Cpass[6], ch;
	s32 i, res=1;
	CmdLCD(0x01);
	StrLCD("Enter Old Pin");
	CmdLCD(0xc0);
	for(i=0; i<6; i++)
	{
		delay_ms(300);
		ch = KeyScan();
		if(ch=='C')
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
		}
		if(ch=='E') break;
		if(ch!='C'){
			CmdLCD(0XC0+i);
			CharLCD(ch);
			if(ch!=pass1[i]) res=0;
		}
	}
	if(res==0) return;
	CmdLCD(0x01);
	StrLCD("NEW PIN");
	CmdLCD(0xc0);
	for(i=0; i<6; i++)
	{
		delay_ms(300);
		ch = KeyScan();
		if(ch=='C')
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
		}
		if(ch=='E') break;
		if(ch!='C'){
			CmdLCD(0XC0+i);
			CharLCD(ch);
			Npass[i] = ch;
		}
	}
	
	CmdLCD(0x01);
	StrLCD("CFORM PIN");
	CmdLCD(0xc0);
	for(i=0; i<6; i++)
	{
		delay_ms(300);
		ch = KeyScan();
		if(ch=='C')
		{
				--i;
				CmdLCD(0XC0+i);
			 	CharLCD(' ');
				--i;
		}
		if(ch=='E') break;
		if(ch!='C'){
			CmdLCD(0XC0+i);
			CharLCD(ch);
			Cpass[i] = ch;
		}
	}
	
	for(i=0; i<6; i++)
	{
		if(Npass[i]!=Cpass[i]) res=0;
	}
	
	if(res==1)
	{
		for(i=0; i<6; i++)
			pass1[i] = Npass[i];	
		CmdLCD(0x01);
		StrLCD("PIN Updated");
		delay_ms(500);
		CmdLCD(0x01);
	}
	else
	{
		CmdLCD(0x01);
		StrLCD("NEWPIN and Cpass");
		CmdLCD(0xc0);
		StrLCD("Not Matched");
		delay_ms(1000);
	}
	CmdLCD(0x01);
	return;
}

void Edit_DEVICE_ALLOW_TIME(void)
{
	u32 flag1=0, flag2=0;
	while(1){
        s32 i,hr=0, min=0,key;
        CmdLCD(0x01);
        StrLCD("Edit Start Time");
        CmdLCD(0xc0);
        StrLCD("Hr:-");
        CmdLCD(0xc0+4);
        delay_ms(500);
        for(i=4; i<9; i++)
        {
								delay_ms(200);
                key = KeyScan();
          if(key=='C' && (i!=4))
          {
                --i;
               CmdLCD(0XC0+i);
               CharLCD(' ');
							 hr /=10;
								--i;
         }
          if(key=='E') break;
          if(key!='C')
           {
						 CmdLCD(0XC0+i);
             CharLCD(key);
             hr = hr*10 + (key-48);
           }
        }
				
				CmdLCD(0xc0+9);
        StrLCD("MI:-");
        CmdLCD(0xc0+13);
        delay_ms(500);
        for(i=13; i<16; i++)
        {
								delay_ms(200);
                key = KeyScan();
          if(key=='C' && (i!=4))
          {
                --i;
               CmdLCD(0XC0+i);
               CharLCD(' ');
							 min /=10;
								--i;
         }
          if(key=='E') break;
          if(key!='C')
           {
						 CmdLCD(0XC0+i);
             CharLCD(key);
             min = min*10 + (key-48);
           }
        }
				
        if((hr>=0 && hr<=23) && (min>=0 && min<=59)){
                hr1 = hr;
								min1 = min;
								flag1=1;
        }
        else
        {
                CmdLCD(0x01);
                StrLCD("invalid Time");
								delay_ms(500);
								return;
        }
				if(flag1==1) break;
			}
				CmdLCD(0x01);
			
			while(1){
        s32 i,hr=0,min=0,key;
        CmdLCD(0x01);
        StrLCD("Edit End Time");
        CmdLCD(0xc0);
        StrLCD("Hr:-");
        CmdLCD(0xc0+4);
        delay_ms(500);
        for(i=4; i<9; i++)
        {
								delay_ms(200);
                key = KeyScan();
          if(key=='C' && (i!=4))
          {
                --i;
               CmdLCD(0XC0+i);
               CharLCD(' ');
							 hr /=10;
								--i;
         }
          if(key=='E') break;
          if(key!='C')
           {
						 CmdLCD(0XC0+i);
             CharLCD(key);
             hr = hr*10 + (key-48);
           }
        }
				
				CmdLCD(0xc0+9);
        StrLCD("MI:-");
        CmdLCD(0xc0+13);
        delay_ms(500);
        for(i=13; i<16; i++)
        {
								delay_ms(200);
                key = KeyScan();
          if(key=='C' && (i!=4))
          {
                --i;
               CmdLCD(0XC0+i);
               CharLCD(' ');
							 min /=10;
								--i;
         }
          if(key=='E') break;
          if(key!='C')
           {
						 CmdLCD(0XC0+i);
             CharLCD(key);
             min = min*10 + (key-48);
           }
        }
				
				
        if((hr>=0 && hr<=23) &&(min>=0 && min<=59)){
								min2 = min;
                hr2 = hr;
								flag2=1;
        }
        else
        {
                CmdLCD(0x01);
                StrLCD("invalid Time");
								delay_ms(500);
								return;
        }
				if(flag2==1) break;
			}
				CmdLCD(0x01);
			delay_ms(300);
}
