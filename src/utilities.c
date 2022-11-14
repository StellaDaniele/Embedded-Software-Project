#include "utilities.h"

#include <LCD_display.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void error_led(void) {
    cli();
    DDRB |= (1 << PORTB7);
    while (1) {
        PORTB ^= (1 << PORTB7);
        _delay_ms(100);
    }
}
/*
void error_led(err_number err) {
    cli();
    start_lcd();
    disp_str(1, 1, "Error code: ");
    disp_num(1, 2, err);
    error_led();
}
*/