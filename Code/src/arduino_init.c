#include "arduino_init.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void arduinoUnoInit() {
    cli();  // disable interrupts

    // 8mhz PWM master clock
    DDRB |= (1 << PORTB4);  // pin 11 (clock output)
    ASSR &= ~((1 << EXCLK) | (1 << AS2));
    TCCR2A = (1 << COM2A0) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << WGM22) | (1 << CS20);
    OCR2A = 2;  //(F_CPU)/(2*(X+1)) previous values: 0, 2
                // DDRC &= ~15;   // low d0-d3 camera
                /*
                DDRC &= ~((1 << PORTC0) | (1 << PORTC1) |
                          (1 << PORTC2) | (1 << PORTC3));
                */
    DDRC = 0;   // all data bits connected to the ddrc
    // DDRD &= ~252;  // d7-d4 and interrupt pins
    DDRD &= ~((1 << PORTD2) | (1 << PORTD3) /*|
              (1 << PORTD4) | (1 << PORTD5) |
              (1 << PORTD6) | (1 << PORTD7)*/
    );
    //_delay_ms(3000);

    // set up twi for 100khz
    // TWI = Two Wire Interface (I2C)
    // TWSR &= ~3;  // disable prescaler for TWI
    TWSR &= ~((1 << TWPS0) | (1 << TWPS1));
    TWBR = 72;  // set to 100khz
    // TWBR = (1 << TWBR3) | (1 << TWBR6);

    // enable serial
    UBRR0H = 0;
    UBRR0L = 1;  // 0 = 2M baud rate. 1 = 1M baud. 3 = 0.5M. 7 = 250k 207 is 9600 baud rate.
    // UCSR0A |= 2;                           // double speed aysnc
    UCSR0A |= (1 << U2X0);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable receiver and transmitter
    // UCSR0C = 6;                            // async 1 stop bit 8bit char no parity bits
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}
