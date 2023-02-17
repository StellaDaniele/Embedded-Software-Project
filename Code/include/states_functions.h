#ifndef __STATES_FUNCTIONS_H__
#define __STATES_FUNCTIONS_H__

#include <stdint.h>

#define MIN_THRESHOLD_TEMP 6
#define MAX_THRESHOLD_TEMP 30

// Initial value is 18°C
extern uint8_t threshold_temp;

void sensors_state(void);
void camera_state(void);
void settings_state(void);
void reboot_state(void);

#endif
