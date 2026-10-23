#ifndef _TIMER0_
#define _TIMER0_
#include "mcu.h"
void timer0_config(void);
extern uint8_t sec, min, hour;
extern uint8_t alarm_sec, alarm_min, alarm_hour;
extern bit blink_state, blink_led;
extern uint8_t no_key_timer;
#endif