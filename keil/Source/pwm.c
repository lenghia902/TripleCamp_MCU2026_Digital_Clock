#include "mcu.h"
#include "pwm.h"

code uint16_t musical_tab = 65369;

uint16_t t2_load_value;
uint16_t buzzer_cnt = 0;

static uint8_t  alarm_beep_left  = 0;   // so pip con lai (dem lui tu 5)
static uint16_t alarm_next_delay = 0;   // so ms con lai truoc pip tiep theo

void buzzer_pwm_config()
{
  buzzer_fre_set();
  T2CON = 0x41; //fcpu 12=> T= 1.5 us

  CCEN = 0X02;
  
  TH2 = t2_load_value>>8;
  TL2 = t2_load_value;
  ET2 = 1;
}

void buzzer_beep_ms(uint16_t ms)
{
	buzzer_cnt = ms*4; 
	buzzer_pwm_config();
}

void T2COM0Interrupt(void) interrupt ISRTimer2 
{
  TH2 = t2_load_value>>8;
  TL2 = t2_load_value;
  TF2 = 0;
	//CRCH/CRCL- TH2/TL2 =667 ticks * 1.5us = 1 ms
	if (buzzer_cnt > 0){
			buzzer_cnt--;
			if (buzzer_cnt == 0)
			{
					CCEN &= ~0x02; //tat buzzer
			}
	}
}

// Goi 1 LAN khi phat hien trung gio hen, de bat dau chuoi pip-pip 5s
void buzzer_beep_alarm(void)
{
  alarm_beep_left  = 5;    // 5 pip x (0.5s ON + 0.5s OFF) = 5000ms
  alarm_next_delay = 0;    // phat pip dau tien ngay
}

// Goi MOI 1ms tu timer0_isr de duy tri nhip. Tu thoat neu khong co chuoi nao dang chay.
void buzzer_alarm_tick_1ms(void)
{
  if(alarm_beep_left == 0) 
		return;

  if(alarm_next_delay == 0){
    buzzer_beep_ms(500);      // bat pip 0.5s 
    alarm_next_delay = 1000;  // 1000ms = 0.5s ON + 0.5s OFF, roi moi bat pip ke tiep
    alarm_beep_left--;
  } 
	else {
    alarm_next_delay--;
  }
}

void buzzer_fre_set()
{

  t2_load_value = musical_tab;
  CRCH =(t2_load_value +(65536-t2_load_value)/2) >> 8;
  CRCL =(t2_load_value +(65536-t2_load_value)/2);
}