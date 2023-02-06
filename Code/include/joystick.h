#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <stdbool.h>
enum joystick_dir { dir_nothing,
                    U,
                    D,
                    R,
                    L,
                    C } current;

void init_joystick();
// returns true if new movement detected, sets "current" global variable to new movement
bool joystick_new_direction();

#endif
