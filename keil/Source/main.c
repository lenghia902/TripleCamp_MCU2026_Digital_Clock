#define __XRAM_SFR_H__
#include "mcu.h"
#include "timer0.h"
#include "7segment_led.h"
#include "led.h"
#include "key.h"
#include "i2c.h"
//#include "spi.h"

void delay_ms(uint16_t ms);

uint8_t sec = 0, min=0, hour =0;
uint8_t setup=0, alarm=0;
bit blink_state=0;
uint8_t alarm_hour;
uint8_t alarm_min ;
uint8_t alarm_sec ;
//uint8_t alarm_data[3]= {0,0,0};

int main(void)
{ 
  uint8_t alarm_load_buf[2];
	
	CKCON 	=	0X70;
	CLKSEL 	=	0X05;								//Fcpu = 32MHz/4=8MHz 
	CLKCMD	=	0X69;								//Tcpu = 0,125 us
	CKCON 	=	0X00;	  
  timer0_config();
  segment_led_config();
	led_config();
	I2C_Init(); 
//	spi_master_init();
	
	   // Doc gio hen da luu tu lan truoc, chi 1 lan luc khoi dong
   
		EP_MultiRead(EEPROM_I2C_ADDR, EEPROM_ADDR_ALARM_HOUR, alarm_load_buf, 2);
		alarm_hour_active = alarm_load_buf[0];
		alarm_min_active  = alarm_load_buf[1];
		alarm_sec_active  = 0;

	  
	  if(alarm_hour_active > 23) alarm_hour_active = 0;
    if(alarm_min_active  > 59) alarm_min_active  = 0;
    
	
	while(1)
  {
		WDTR = 0x5A;                //CLR WDTR
		Mnkey();		
    segment_led_scan();		
    segment_display(sec,min,hour);
  }
}