#ifndef _PWM_
#define _PWM_
#include "mcu.h"

void buzzer_beep_alarm(void);
void buzzer_alarm_tick_1ms(void);
void buzzer_pwm_config();
void buzzer_beep_ms(uint16_t ms);
void buzzer_fre_set();
#endif