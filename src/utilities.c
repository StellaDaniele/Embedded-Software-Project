#include "utilities.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void error_led(void) {
    cli();
    DDRB |= (1 << PORTB7);
    while (1) {
        PORTB ^= (1 << PORTB7);
        _delay_ms(100);
    }
}
