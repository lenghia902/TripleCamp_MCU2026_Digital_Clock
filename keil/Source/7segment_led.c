#include "mcu.h"
#include "7segment_led.h"
#include "timer0.h"
#include "key.h"
#include "led.h"
#include "i2c.h"
uint8_t led_buff[4];  //Display cache register

code  uint8_t numble_tab[16]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};  //0~F

void segment_led_config()
{
  P3 = 0X00;
  P3M |= 0XFF;
  P5M &= 0XF0;
  P5M |= 0X0F;
}

void segment_led_scan()
{
  static uint8_t cnt = 0;
  SEG_COM0 = 0;
  SEG_COM1 = 0;
  SEG_COM2 = 0;
  SEG_COM3 = 0;  
  switch(cnt)
  {
    case 0:SEG_COM0 = 1; break;
    case 1:SEG_COM1 = 1; break;
    case 2:SEG_COM2 = 1; break;
    case 3:SEG_COM3 = 1; break;    
  }
  P3  = led_buff[cnt];

  
  if(++cnt > 3 )
    cnt = 0;
}

void segment_display(uint8_t sec, uint8_t min, uint8_t hour)
{
if((setup==1||alarm==1)&&blink_state==1){
		led_buff[1]=0x00;
		led_buff[0]=0x00;
		
	}
	else if((setup==2||alarm==2)&&blink_state==1){
		led_buff[3]=0x00;
		led_buff[2]=0x00;
	} 
	else if(alarm!=0){
    led_buff[3] = numble_tab[alarm_min%10];
 
    led_buff[2] = numble_tab[alarm_min/10];
  
    led_buff[1] = numble_tab[alarm_hour%10] | 0x80;
    
    led_buff[0] = numble_tab[alarm_hour/10];
  } 
else{
    led_buff[3] = numble_tab[min%10];
 
    led_buff[2] = numble_tab[min/10];
  
    led_buff[1] = numble_tab[hour%10] | 0x80;
	
    led_buff[0] = numble_tab[hour/10];
  } 
}
