#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include <stdint.h>

typedef enum error_type { GENERAL_ERROR,
                          CAMERA_TWI_ERROR,
                          CAMERA_ERROR,
                          ADC_MUX_SELECTION_ERROR,
                          DHT11_ERROR,
                          UNEXPECTED_STATE_ERROR,
                          N_ERRORS } error_type;

void error(error_type err, int additional_info);
void error_led(void);
void reset_board(void);

#endif
