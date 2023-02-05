#ifndef HPP_UTILITIES
#define HPP_UTILITIES
#include <stdint.h>

enum error_types { GENERAL_ERROR,
                   ADC_MUX_SELECTION };

static enum error_types error;
typedef uint8_t err_number;

//void error_led(err_number err);
void error_led();

#endif