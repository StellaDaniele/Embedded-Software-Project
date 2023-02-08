#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <stdint.h>

#define PORT_RELAY PORTJ
#define DDR_RELAY DDRJ
#define RELAY PORTJ0

extern volatile uint64_t curr_time_ms;

void board_init(void);

#endif
