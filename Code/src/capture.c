#include "capture.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define WAIT_FOR_BYTE_TRANSMISSION_USART \
    while (!(UCSR0A & (1 << UDRE0)))     \
        ;

#define WAIT_FOR_PIN_HIGH(reg, pin) \
    while (!(reg & (1 << pin)))     \
        ;

#define WAIT_FOR_PIN_LOW(reg, pin) \
    while ((reg & (1 << pin)))     \
        ;

// Transfers string via USART0
void StringPgm(const char* str) {
    do {
        WAIT_FOR_BYTE_TRANSMISSION_USART
        UDR0 = pgm_read_byte_near(str);
        WAIT_FOR_BYTE_TRANSMISSION_USART
    } while (pgm_read_byte_near(++str));
}

void captureImg(uint16_t wg, uint16_t hg) {
    uint16_t y, x;
    /*
    "*RDY*" is a string to recognize the beginning of a
    new image in the device connected to the USART0
    */
    StringPgm(PSTR("*RDY*"));

    WAIT_FOR_PIN_HIGH(PIND, PIND3)
    WAIT_FOR_PIN_LOW(PIND, PIND3)
    
    y = hg;
    while (y--) {
        x = wg;
        while (x--) {
            WAIT_FOR_PIN_LOW(PIND, PIND2)
            // Transmit the content of the data pins
            UDR0 = (PINC & ((1 << PINC0) | (1 << PINC1) |
                            (1 << PINC2) | (1 << PINC3) |
                            (1 << PINC4) | (1 << PINC5) |
                            (1 << PINC6) | (1 << PINC7)));
            WAIT_FOR_BYTE_TRANSMISSION_USART
            WAIT_FOR_PIN_HIGH(PIND, PIND2)
            WAIT_FOR_PIN_LOW(PIND, PIND2)
            WAIT_FOR_PIN_HIGH(PIND, PIND2)
        }
    }
    _delay_ms(100);
}
