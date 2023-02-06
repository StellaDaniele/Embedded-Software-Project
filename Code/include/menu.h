#ifndef __MENU_H__
#define __MENU_H__
#include <stdbool.h>

#include "joystick.h"
enum states { MENU,
              TEMP_HUM,
              CAMERA,
              RELAY } curr_state;
bool new_state;

void init_menu();
void menu(enum joystick_dir current);

#endif
