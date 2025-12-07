#include <LPC21xx.h>
#include "kpm.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"
void init_system(void);
void display_RTC(void);
u32 password(void);
void operation(void);
void einto_isr(void) __irq;
s32 check_time(void); /*function declaration for valid time*/
char key;
main()
{
	init_system();
	
	while(1){
		do{
			display_RTC();
		
		}while(ColScan());
		if(KeyScan()=='E')
		{
				if(password())
				{
					CmdLCD(0x01);
					if(check_time())
					{
						operation();
					}
					else
					{
						StrLCD("Invalid Time");
						delay_s(1);
						CmdLCD(0x01);
					}
				}
			 	else{
					CmdLCD(0x01);
					StrLCD("Wrong PIN");
					delay_ms(1000);
					CmdLCD(0X01);
				}
		}
	}
}
