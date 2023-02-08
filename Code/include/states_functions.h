#ifndef __STATES_FUNCTIONS_H__
#define __STATES_FUNCTIONS_H__

#include <stdint.h>

// Initial value is 18°C
extern uint8_t threshold_tmp;

void sensors_state(void);
void camera_state(void);
void settings_state(void);
void reboot_state(void);

#endif
