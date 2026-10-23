#include "mcu.h"
#include "led.h"

void led_config()
{
  P0 |= 0X18;
  P0M |= 0X18;
	
}

void led_ctr(uint8_t led,uint8_t cmd)
{
  switch(led)
  {
    case 0:LED0_PIN = cmd;break;
    case 1:LED1_PIN = cmd;break;
  }
}