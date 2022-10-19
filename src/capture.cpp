#include "capture.hpp"

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

void StringPgm(const char* str) {
    do {
        WAIT_FOR_BYTE_TRANSMISSION_USART
        UDR0 = pgm_read_byte_near(str);
        WAIT_FOR_BYTE_TRANSMISSION_USART
    } while (pgm_read_byte_near(++str));
}

/*static*/ void captureImg(uint16_t wg, uint16_t hg) {
    uint16_t y, x;
    StringPgm(PSTR("*RDY*"));
    /*
    while (!(PIND & (1 << PIND3)))
        ;  // wait for high
    */
    WAIT_FOR_PIN_HIGH(PIND, PIND3)
    /*
    while ((PIND & (1 << PIND3)))
        ;  // wait for low
    */
    WAIT_FOR_PIN_LOW(PIND, PIND3)
    y = hg;
    while (y--) {
        x = wg;
        // while (!(PIND & 256));//wait for high WAS COMMENTED
        while (x--) {
            /*
            while ((PIND & (1 << PIND2)))
                ;  // wait for low
            */
            WAIT_FOR_PIN_LOW(PIND, PIND2)
            // Transmit the content of the data pins
            // UDR0 = (PINC & 15) | (PIND & 240);
            UDR0 = (PINC & ((1 << PINC0) | (1 << PINC1) |
                            (1 << PINC2) | (1 << PINC3))) |
                   (PIND & ((1 << PIND4) | (1 << PIND5) |
                            (1 << PIND6) | (1 << PIND7)));
            WAIT_FOR_BYTE_TRANSMISSION_USART
            /*
            while (!(PIND & (1 << PIND2)))
                ;  // wait for high
            */
            WAIT_FOR_PIN_HIGH(PIND, PIND2)
            /*
            while ((PIND & (1 << PIND2)))
                ;  // wait for low
            */
            WAIT_FOR_PIN_LOW(PIND, PIND2)
            /*
            while (!(PIND & (1 << PIND2)))
                ;  // wait for high
            */
            WAIT_FOR_PIN_HIGH(PIND, PIND2)
        }
        // while ((PIND & 256));//wait for low WAS COMMENTED
    }
    _delay_ms(100);
}
