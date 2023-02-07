#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <stdbool.h>
typedef enum joystick_dir { dir_nothing,
                    U,
                    D,
                    R,
                    L,
                    C } joystick_dir;



extern joystick_dir current;
extern volatile bool central_button_pressed_interrupt;

void init_joystick();
// returns true if new movement detected, sets "current" global variable to new movement
bool joystick_new_direction();
void enable_central_button_interrupt(void);
void disable_central_button_interrupt(void);

#endif
