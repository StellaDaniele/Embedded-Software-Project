#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include <stdint.h>

typedef enum error_type { GENERAL_ERROR,
                          CAMERA_TWI_ERROR,
                          CAMERA_ERROR,
                          ADC_MUX_SELECTION_ERROR,
                          DHT11_ERROR,
                          UNEXPECTED_STATE_ERROR,
                          UNEXPECTED_JOYSTICK_DIR_ERROR,
                          N_ERRORS } error_type;

// Principal error function, with the error_type list of errors, uses the LCD screen and the LED
void error(error_type err, int additional_info);
// Used for fast debugging, uses only the LED
void error_led(void);
// Enables the minimum watchdog (15ms), disables all the interrupts and enters an infinite loop
void reset_board(void);

#endif
