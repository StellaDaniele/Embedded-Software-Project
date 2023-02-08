#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <stdint.h>

#define PORT_RELAY PORTJ
#define DDR_RELAY DDRJ
#define RELAY PORTJ0
#define PIN_PIR PINK
#define DDR_PIR DDRK
#define PIR PINK0

extern volatile uint64_t curr_time_ms;

void board_init(void);

#endif
