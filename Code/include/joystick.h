/**
 * @file joystick.h
 * @brief Functions for using the joystick on an ATmega2560 microcontroller
 */

#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <stdbool.h>

/**
 * @brief Joystick directions
 */
typedef enum joystick_dir {
    dir_nothing, /**< No direction */
    U,           /**< Up direction */
    D,           /**< Down direction */
    R,           /**< Right direction */
    L,           /**< Left direction */
    C            /**< Central button direction */
} joystick_dir;

extern joystick_dir current;
extern volatile bool central_button_pressed_interrupt;

/**
 * @brief Initializes the joystick
 */
void init_joystick();

/**
 * @brief Checks if the joystick has a new direction
 * @return True if the joystick has a new direction, false otherwise
 */
bool joystick_new_direction();

/**
 * @brief Enables the central button interrupt
 */
void enable_central_button_interrupt(void);

/**
 * @brief Disables the central button interrupt
 */
void disable_central_button_interrupt(void);

#endif
