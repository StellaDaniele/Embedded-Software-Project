#ifndef H_MENU
#define H_MENU
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
