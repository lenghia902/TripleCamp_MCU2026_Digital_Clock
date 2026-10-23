#ifndef _KEY_
#define _KEY_
#define EEPROM_I2C_ADDR        0xA0
#define EEPROM_ADDR_ALARM_HOUR 0x00
#define EEPROM_ADDR_ALARM_MIN  0x01
#include "mcu.h"
extern	uint8_t	skeychat;											
extern uint8_t alarm, setup, keycvtbuf2;
extern void Mnkey(void);
extern	void Wr_FIFO(void);
//extern uint8_t alarm_data[3];
extern uint8_t alarm_hour_active;
extern uint8_t alarm_min_active;
extern uint8_t alarm_sec_active;
#endif