#include "timer0.h"
#include "7segment_led.h"
#include "key.h"
#include "pwm.h"
//#include "spi.h"
#include "led.h"
#include "i2c.h"

uint16_t count_1s;
uint16_t count_500ms;
uint16_t count_out;
uint16_t count_pippip;
uint8_t no_key_timer=0;
bit blink_led=1;

void timer0_config(void)
{
  TMOD = 0x05;          //mode 1 16 bit timer0 
	TCON0 = 0x20;         // fexto /32 = 32MHZ/32
  TH0 = (65536 - 1000)>>8;             
  TL0 = (65536 - 1000);		
	TR0 = 1;										  
  ET0 = 1;   
  EAL = 1;
}

void timer0_isr(void) interrupt ISRTimer0   
{
  TH0 = (65536 - 1000)>>8;    //        
  TL0 = (65536 - 1000);		 
	TF0 = 0;	
	
	if(setup==0){
  if(++count_1s >= 1000) //thêm "=" de k du tgian
  {
    count_1s = 0;
    if(++sec >= 60){
			sec=0;
			min++;
			if(min>59){
				min=0;
				hour ++;
				if(hour>23){
					hour =0;
				}
			}
		}
			//w25q_read_data(0,alarm_data,3);
		if(hour==alarm_hour_active && min==alarm_min_active && sec==alarm_sec_active){
		   buzzer_beep_alarm();  
		}
	}
}
	if(alarm ==0&&setup==0){
		led_ctr(LED0,1);
	}
	if(alarm!=0||setup!=0){
		if(keycvtbuf2!=0){
			blink_state=0;
			count_500ms=0;
		}
		else if(++count_500ms >= 500){
				count_500ms=0;
				blink_state =!blink_state;
			}
		
			//Bo doan nay vi nhay ca 2 che do
			/*if(++count_pippip>500){
			count_pippip=0;
			blink_led=!blink_led;
			led_ctr(LED0,blink_led);
		}*/    
		if(++ count_out>=1000){
				count_out=0; 	
				if (++no_key_timer >= 30){
					setup=0;
					alarm =0;
					no_key_timer=0;
					buzzer_beep_ms(300);
				}
		}	
	}
	//Tach rieng che do hen gio phut
	 if(alarm!=0){
    if(++count_pippip >= 500){
      count_pippip=0;
      blink_led=!blink_led;
      led_ctr(LED0,blink_led);
    }
  } else {
    count_pippip=0;
    blink_led=1;              // reset pha, de lan sau vao alarm bat dau dung nhip
    led_ctr(LED0,1); 
}
	  buzzer_alarm_tick_1ms();   // duy tri nhip pip-pip, phai goi moi lan ngat

}

