#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

#include "utilities.h"

#define enable_ADC() ADCSRA |= (1 << ADSC);
#define stop_ADC() ADCSRA &= ~(1 << ADSC);

bool ADC_connected_pins[16] = {false};

#define connect_ADC_pin(x) \
    ADC_connected_pins[x] = true;

void set_ADC_MUX(uint8_t pin) {
    enable_ADC();
    if (pin < 0 || pin > 15) {
        error_led(ADC_MUX_SELECTION);
    }
    if (!ADC_connected_pins[pin]) {
        // If the pin is not selected for the ADC
        error_led(ADC_MUX_SELECTION);
    }
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) |
               (1 << MUX2) | (1 << MUX3) |
               (1 << MUX4));
    ADCSRB &= ~(1 << MUX5);

    if (pin <= 7) {
        ADMUX |= pin;
    } else {
        ADCSRB |= (1 << MUX5);
        ADMUX |= pin - 8;
    }
    enable_ADC();
}

void ADC_init(void) {
    // Connection of pins to ADC
    connect_ADC_pin(0);
    connect_ADC_pin(1);
    // connect_ADC_pin(2);

    // ADC setup
    ADMUX |= (1 << REFS0) | (1 << ADATE);
    // AVCC is used as ref
    // ADC is in auto-trigger free running mode

    ADCSRA |= ((1 << ADIE) | (1 << ADPS0) |
               (1 << ADPS1) | (1 << ADPS2));
    // Enable the ADC and set the prescaler @128
}

int main(void) {
    return 0;
}

ISR(ADC_vect) {
    // valore = ADCL | (ADCH << 8);
    ADCSRA |= (1 << ADIF);  //
}
