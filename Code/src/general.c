#include "general.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

volatile uint64_t curr_time_ms = 0;

void board_init(void) {
    /*
    The timer 1 is used to generate an interrupt
    every 1ms in order to keep track of time
    */
    // Set up timer 1 in normal mode
    TCCR1A = 0x00;
    TCCR1B = (1 << CS11);      // Set the prescaler
    OCR1A = F_CPU / 8 / 1000;  // 1ms

    // Enable CTC mode and timer 1 overflow interrupt
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);

    // Setup pins
    // Relay
    DDR_RELAY |= (1 << DDR_RELAY);  // Set as output
    PORT_RELAY |= (1 << RELAY);     // Set high (relay is low level trigger)

    sei();
}

ISR(TIMER1_COMPA_vect) {
    // called every 1ms
    ++curr_time_ms;
}
