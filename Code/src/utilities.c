#include "utilities.h"

#include <LCD_display.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>  // watchdog timer
#include <util/delay.h>

void error_led(void) {
    cli();
    DDRB |= (1 << PORTB7);
    while (1) {
        PORTB ^= (1 << PORTB7);
        _delay_ms(100);
    }
}

void error(error_type err, int additional_info) {
    cli();
    PRR0 = 0xff;  // Disable peripherals in Power Reduction Register 0
    PRR1 = 0xff;  // Disable peripherals in Power Reduction Register 1

    start_lcd();
    clrscr();
    disp_str(1, 1, "Error code: ");
    disp_num(1, 2, (int)err);
    disp_str(1, 3, "Additional_info: ");
    disp_num(1, 4, additional_info);
    error_led();
    DDRB |= (1 << PORTB7);
    while (1) {
        PORTB ^= (1 << PORTB7);
        _delay_ms(100);
    }
    /*
    cli();
    start_lcd();
    disp_str(1, 1, "Error code: ");
    disp_num(1, 2, err);
    error_led();
    */
}

void reset_board(void) {
    cli();
    wdt_enable(WDTO_15MS);
    while (1)
        ;
}
