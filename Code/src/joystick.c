#include "joystick.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#include "LCD_display.h"
#include "utilities.h"

#ifndef __GNUC__
#define __attribute__(x)
#endif

#define CLEAR_MUX_ADC \
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4));

#define SELECT_ADC_JOYPAD_X CLEAR_MUX_ADC

#define SELECT_ADC_JOYPAD_Y \
    CLEAR_MUX_ADC           \
    ADMUX |= (1 << MUX0);

joystick_dir current;
static int32_t value_x = 0, value_y = 0;
// static enum joystick_dir old = dir_nothing;
static bool new_value = false, first = true;
volatile bool central_button_pressed_interrupt = false;

#define UP "U"
#define DOWN "D"
#define RIGHT "R"
#define LEFT "L"

#define LOW_ADC 200
#define HIGH_ADC 800

void init_joystick(void) {
    // DDRF &= ~((1 << DDD0) | (1 << DDD1));
    ADMUX |= (1 << REFS0);  // | (1 << ADLAR);  // AVCC, Left Adjust Result
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    DDRJ &= ~(1 << DDJ1);    // Central button
    PORTJ |= (1 << PORTJ1);  // pull-up resistor
    enable_central_button_interrupt();
}

__attribute__((always_inline)) inline int read_ADC(void) {
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)))
        ;
    ADCSRA |= (1 << ADIF);
    return ADCL | (ADCH << 8);
}

enum joystick_dir disp_joystick_pos(void) {
    SELECT_ADC_JOYPAD_X
    value_x = read_ADC();
    SELECT_ADC_JOYPAD_Y
    value_y = read_ADC();
    // disp_num(1, 2, value_x);
    // disp_num(1, 3, value_y);
    if (value_x < LOW_ADC) {
        // disp_str(1, 1, UP);
        return U;
    } else if (value_x > HIGH_ADC) {
        // disp_str(1, 1, DOWN);
        return D;
    } else {
        if (value_y < LOW_ADC) {
            // disp_str(1, 1, RIGHT);
            return R;
        } else if (value_y > HIGH_ADC) {
            // disp_str(1, 1, LEFT);
            return L;
        }
    }
    // if (!(PINJ & (1 << PINJ1))){
    //     return C;
    // }

    if (central_button_pressed_interrupt) {
        central_button_pressed_interrupt = false;
        return C;
    }
    return dir_nothing;
}

bool joystick_new_direction(void) {
    switch (disp_joystick_pos()) {
        case dir_nothing:
            first = true;
            // disp_str(1, 1, "W");
            break;
        case U:
            if (first) new_value = true;
            first = false;
            current = U;
            break;
        case D:
            if (first) new_value = true;
            first = false;
            current = D;
            // disp_str(1, 1, DOWN);
            break;
        case L:
            if (first) new_value = true;
            first = false;
            current = L;
            // disp_str(1, 1, LEFT);
            break;
        case R:
            if (first) new_value = true;
            first = false;
            current = R;
            // disp_str(1, 1, RIGHT);
            break;
        case C:
            if (first) new_value = true;
            first = false;
            current = C;
            // disp_str(1, 1, "C");
            break;
    }
    if (new_value) {
        // disp_num(1, 1, current);
        new_value = false;
        return true;
    }
    return false;
}

void enable_central_button_interrupt(void) {
    // Enable pin change interrupt for PJ1
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10);
    central_button_pressed_interrupt = false;
    sei();
}

void disable_central_button_interrupt(void) {
    PCICR &= ~(1 << PCIE1);
    PCMSK1 &= ~(1 << PCINT10);
}

ISR(PCINT1_vect) {
    if (!(PINJ & (1 << PINJ1)))
        central_button_pressed_interrupt = true;
    PCIFR |= (1 << PCIF1);  // Reset interrupt flag
}
