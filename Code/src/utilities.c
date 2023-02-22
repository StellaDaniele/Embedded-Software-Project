#include "utilities.h"

#include <LCD_display.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

#ifndef __GNUC__
#define __attribute__(x)
#endif

void error_led(void) {
    cli();
    PRR0 = 0xff;  // Disable peripherals in Power Reduction Register 0
    PRR1 = 0xff;  // Disable peripherals in Power Reduction Register 1
    DDRB |= (1 << PORTB7);
    while (1) {
        PORTB ^= (1 << PORTB7);
        _delay_ms(100);
    }
}

void error(error_type err, int32_t additional_info) {
    cli();
    PRR0 = 0xff;  // Disable peripherals in Power Reduction Register 0
    PRR1 = 0xff;  // Disable peripherals in Power Reduction Register 1
                  /*
                  The following peripherals are affected by these two lines:
                  PRR0:
                    PRADC       - Power Reduction ADC
                    PRUSART0    - Power Reduction USART0
                    PRSPI       - Power Reduction SPI
                    PRTIM1      - Power Reduction Timer/Counter1
                    PRTIM0      - Power Reduction Timer/Counter0
                    PRTIM2      - Power Reduction Timer/Counter2
                    PRTWI       - Power Reduction TWI
                  PRR1:
                    PRUSB       - Power Reduction USB
                    PRTIM3      - Power Reduction Timer/Counter3
                    PRTIM4      - Power Reduction Timer/Counter4
                    PRTIM5      - Power Reduction Timer/Counter5
                    PRTRX24     - Power Reduction 2.4GHz Transceiver
                  By setting the bits in the Power Reduction Registers to 1, the
                  associated peripherals are disabled and their power consumption is reduced.
                  */
    start_lcd();
    clrscr();
    disp_str(1, 1, "ERROR CODE: ");
    disp_num(1, 2, (int)err);
    disp_str(1, 3, "ADDITIONAL INFO: ");
    disp_num(1, 4, additional_info);
    error_led();
    DDRB |= (1 << PORTB7);
    while (1) {
        PORTB ^= (1 << PORTB7);
        _delay_ms(100);
    }
}

__attribute__((always_inline)) inline void reset_board(void) {
    cli();
    wdt_enable(WDTO_15MS);
    while (1)
        ;
}
