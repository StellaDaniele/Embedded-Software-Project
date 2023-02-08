

#include "general.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "utilities.h"

#define ENCODER_CLK_STATE ((PIN_ENCODER_CLK & (1 << ENCODER_CLK)) != 0)
#define ENCODER_DT_STATE ((PIN_ENCODER_DT & (1 << ENCODER_DT)) != 0)

volatile uint64_t curr_time_ms = 0;
volatile int8_t encoder_position = 0;
volatile bool encoder_sw_pressed_interrupt = false;
volatile bool encoder_changed = false;
// These two variables are the parameters used
// for the encoder's debouncing
volatile uint64_t lastDebounceTime = 0;
volatile const uint64_t debounceDelay = 50;

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
    // IR sensor
    DDR_PIR &= ~(1 << PIR);  // Set as intput
    // Encoder
    DDR_ENCODER_CLK &= ~(1 << ENCODER_CLK);
    PORT_ENCODER_CLK |= (1 << ENCODER_CLK);  // pull-up resistor

    DDR_ENCODER_DT &= ~(1 << ENCODER_DT);
    PORT_ENCODER_DT |= (1 << ENCODER_DT);  // pull-up resistor

    DDR_ENCODER_SW &= ~(1 << ENCODER_SW);
    PORT_ENCODER_SW |= (1 << ENCODER_SW);  // pull-up resistor

    sei();
}

void enable_encoder_interrupt(void) {
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT16);  // CLK
    PCMSK2 |= (1 << PCINT17);  // DT
    PCMSK2 |= (1 << PCINT18);  // SW
    encoder_position = 0;
}

void disable_encoder_interrupt(void) {
    PCICR &= ~(1 << PCIE2);
    PCMSK2 &= ~(1 << PCINT16);
    PCMSK2 &= ~(1 << PCINT17);
    PCMSK2 &= ~(1 << PCINT18);
}

ISR(TIMER1_COMPA_vect) {
    // called every 1ms
    ++curr_time_ms;
}

ISR(PCINT2_vect) {
    int encoder_value = ENCODER_CLK_STATE | (ENCODER_DT_STATE << 1);
    if ((curr_time_ms - lastDebounceTime) > debounceDelay) {
        switch (encoder_value) {
            case 0b00:
                break;
            case 0b01:
                encoder_position++;
                encoder_changed = true;
                break;
            case 0b10:
                encoder_position--;
                encoder_changed = true;
                break;
            case 0b11:
                break;
        }
        lastDebounceTime = curr_time_ms;
    }
    if (!(PIN_ENCODER_SW & (1 << ENCODER_SW)))
        encoder_sw_pressed_interrupt = true;
    PCIFR |= (1 << PCIF2); // Reset interrupt flag
}