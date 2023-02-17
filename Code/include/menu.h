/**
 * @file menu.h
 * @brief Functions for the menu on an ATmega2560 microcontroller, using the LCD and the joystick.
 */

#ifndef __MENU_H__
#define __MENU_H__
#include <stdbool.h>

#include "joystick.h"

/**
 * @brief Available states
 */
typedef enum states { MENU,
                      SENSORS,
                      CAMERA,
                      SETTINGS,
                      REBOOT } states;

extern states curr_state;
extern bool new_state;

/**
 *@brief Initializes the menu by setting the current state to MENU, displaying menu items on the LCD display, and highlighting the first item with a ">" symbol.
 */
void init_menu(void);

/**
 *@brief Handles user inputs from the joystick and updates the menu accordingly.
 *@param current An enumerated value that represents the current direction of the joystick (Up, Down, Left, Right, or Center).
 */
void menu(enum joystick_dir current);

#endif
