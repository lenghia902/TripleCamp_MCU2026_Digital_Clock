#include "key.h"
#include "led.h"
#include "timer0.h"
#include "i2c.h"
#include "pwm.h"
//#include "spi.h"


#define	keyout0		P44																					
#define	keyout1		P45
#define	keyout2		P46
#define	keyout3		P47

#define	keyin0		P24
#define	keyin1		P25
#define	keyin2		P26
#define	keyin3		P27

#define	delaytime							10																

uint8_t alarm_hour_active;
uint8_t alarm_min_active;
uint8_t alarm_sec_active = 0;

uint8_t	keyinbuf1,keyinbuf2;																		
uint8_t	keychkbuf1,keychkbuf2;																	
uint8_t	keycvtbuf1,keycvtbuf2;																	
uint8_t	keyoldbuf1 =0 ,keyoldbuf2=0;																	
uint8_t	skeychat;																							
bit			fkeyin;				
bit 		save_alarm;


void	Mnkey()
{

  uint8_t i ;
  
  P2UR |= 0XF0;
  P2M  &= 0X0F;
  P4M  &= ~0xF0;	
	
	P4M |= 0x10;																									
	keyout0 = 0;																									
  for(i = 0;i< 8;i++);                                          
	keyinbuf1 = keyin3*8 | keyin2*4 | keyin1*2 | keyin0;			
	keyout0 = 1;																								
  P4M &= ~0x10;	
	keyinbuf1 |= 0xF0;
	
	P4M |= 0x80;																									
	keyout3 = 0;																									
  for(i = 0;i< 8;i++);                                          
	keyinbuf2 = keyin1*8 | keyin2*4 | keyin3*2 | keyin0;			
	keyout3 = 1;																								
  P4M &= ~0x80;	
	keyinbuf2 |= 0xF0;

	keyinbuf1 = ~ keyinbuf1;																			
	keyinbuf2 = ~ keyinbuf2;			

	if ((keyinbuf1 == keychkbuf1)&&(keyinbuf2 == keychkbuf2))		{	
		if ((1 == fkeyin) && (0 == skeychat))														
		{																														
				keycvtbuf1 = keychkbuf1;												
				keycvtbuf2 = keychkbuf2;	
				fkeyin = 0;
		}
	}
	else
	{
		keychkbuf1 = keyinbuf1;																			
		keychkbuf2 = keyinbuf2;	
		fkeyin = 1;
		skeychat = delaytime;																				
	}	
	
  if(skeychat != 0)
    skeychat -- ;
	

	if ((keycvtbuf1 != keyoldbuf1)||(keycvtbuf2 != keyoldbuf2))	//tranh nhan giu
	{
		if ((0 == keyoldbuf1)&&(0==keyoldbuf2))								//chi xu ly khi truoc do la nhan len
		{
			if (keycvtbuf1& 0x01){
				if(alarm==0){
					if(++setup>2){
						setup=0;
						blink_led=1;
					}
					buzzer_beep_ms(300);
					no_key_timer=0;
				}
			}			
			else if(keycvtbuf1& 0x08){
				if(setup==0){
					if (++alarm > 2){
            alarm = 0;
						save_alarm=1;
						blink_led=1;
					} else save_alarm=0;					
					if(alarm == 1){
						
						/*alarm_sec  = 0;			
						alarm_min = min;      //  lay tu GIO HIEN TAI
						alarm_hour = hour;*/
						
						// Lay gio hen truoc do
						alarm_hour = alarm_hour_active;
						alarm_min  = alarm_min_active;
						alarm_sec  = 0;
					} 
					
					buzzer_beep_ms(300);
					no_key_timer=0;
					
					if(save_alarm==1){
						uint8_t alarm_save_buf[2];
						alarm_save_buf[0] = alarm_hour;
						alarm_save_buf[1] = alarm_min;
						EP_MultiWrite(EEPROM_I2C_ADDR, EEPROM_ADDR_ALARM_HOUR, alarm_save_buf, 2);
						  
						
						alarm_hour_active = alarm_hour;
            alarm_min_active  = alarm_min;
            alarm_sec_active  = 0;
						
						/*alarm_data[0] = alarm_hour;
            //alarm_data[1] = alarm_min;
            //alarm_data[2] = alarm_sec;
            w25q_write_256bytes(0, alarm_data, 3);
					*/
				}
			}
		}
			else if (keycvtbuf2& 0x01){
				if(alarm==0){	
					if(setup==1){
				
						buzzer_beep_ms(300);
						no_key_timer=0;
						if(++hour>23){
							hour=0;
						}
					}
					if(setup==2){
					
					buzzer_beep_ms(300);
						no_key_timer=0;
						if(++min>59){
							min=0;
						}
					}
				}
				if(setup==0){
					if(alarm==1){
				
					buzzer_beep_ms(300);
						no_key_timer=0;
						if(++alarm_hour>23){
							alarm_hour=0;
						}
					}
					if(alarm==2){

					buzzer_beep_ms(300);
						no_key_timer=0;
						if(++alarm_min>59){
							alarm_min=0;
						}
					}
				}
			}			
		else if (keycvtbuf2& 0x08){
				if(alarm==0){
					
					if(setup==1){
					
					buzzer_beep_ms(300);
						no_key_timer=0;
						if(hour==0){
							hour=23;
						}
						else hour--;
					}
					if(setup==2){
					
					buzzer_beep_ms(300);
						no_key_timer=0;
						if(min==0){
							min=59;
						}
						else min --;
					}
				}
				if(setup==0){
					if(alarm==1){
			
					buzzer_beep_ms(300);
						no_key_timer=0;
						if(alarm_hour==0){
							alarm_hour=23;
						}
						else alarm_hour --;
					}
					if(alarm==2){
						
					buzzer_beep_ms(300);
						no_key_timer=0;
						if(alarm_min==0){
							alarm_min=59;
						}
						else alarm_min--;
					}
				}
			}				
		}
		keyoldbuf1 = keycvtbuf1;
		keyoldbuf2 = keycvtbuf2;			
	}
}