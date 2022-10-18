#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

int main(void) {
    DDRB |= _BV(PORTB7);
    while (true) {
        PORTB ^= _BV(PORTB7);
        _delay_ms(100);
    }
    return 0;
}
