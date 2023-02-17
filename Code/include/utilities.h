#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include <stdint.h>

/**
 * @brief Error types
 */
typedef enum error_type { GENERAL_ERROR,
                          CAMERA_TWI_ERROR,
                          CAMERA_ERROR,
                          ADC_MUX_SELECTION_ERROR,
                          DHT11_ERROR,
                          UNEXPECTED_STATE_ERROR,
                          UNEXPECTED_JOYSTICK_DIR_ERROR,
                          N_ERRORS } error_type;

/**
 * @brief Displays an error message on an LCD display and turns on an error LED
 *
 * This function disables all the peripherals in Power Reduction Registers 0 and 1,
 * displays the error code and additional information on the LCD display, and
 * enters an infinite loop. The error code and additional information are passed as
 * arguments to this function.
 *
 * @param err an `error_type` enumeration that represents the type of error that occurred.
 * @param additional_info an integer that represents additional information about the error.
 */
void error(error_type err, int additional_info);

/**
 * @brief Turns on an error LED and enters an infinite loop
 *
 * This function disables all the peripherals in Power Reduction Registers 0 and 1
 * and toggles the error LED every 100ms.
 */
void error_led(void);

/**
 * @brief Resets the board by enabling the watchdog timer and entering an infinite loop
 */
void reset_board(void);

#endif
