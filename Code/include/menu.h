#ifndef __MENU_H__
#define __MENU_H__
#include <stdbool.h>

#include "joystick.h"
typedef enum states { MENU,
              SENSORS,
              CAMERA,
              SETTINGS,
              REBOOT } states;

extern states curr_state;
extern bool new_state;

void init_menu();
void menu(enum joystick_dir current);

#endif
