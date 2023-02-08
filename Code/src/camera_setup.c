#include "camera_setup.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "macros_regs_ov7670.h"
#include "utilities.h"

#define WAIT_FOR_TRANSMISSION_TWI  \
    while (!(TWCR & (1 << TWINT))) \
        ;

void twiStart() {
    TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));  // send start
    WAIT_FOR_TRANSMISSION_TWI
    if ((TWSR & 0xF8) != TW_START)
        error(CAMERA_TWI_ERROR, 0);
}

void twiWriteByte(uint8_t DATA, uint8_t type) {
    TWDR = DATA;
    TWCR = (1 << TWINT) | (1 << TWEN);
    WAIT_FOR_TRANSMISSION_TWI
    if ((TWSR & 0xF8) != type)
        error(CAMERA_TWI_ERROR, 1);
}

void twiAddr(uint8_t addr, uint8_t typeTWI) {
    TWDR = addr;                       // send address
    TWCR = (1 << TWINT) | (1 << TWEN); /* clear interrupt to start transmission */
    WAIT_FOR_TRANSMISSION_TWI
    if ((TWSR & 0xF8) != typeTWI)
        error(CAMERA_TWI_ERROR, 2);
}

void writeReg(uint8_t reg, uint8_t dat) {
    // send start condition
    twiStart();
    twiAddr(camAddr_WR, TW_MT_SLA_ACK);
    twiWriteByte(reg, TW_MT_DATA_ACK);
    twiWriteByte(dat, TW_MT_DATA_ACK);
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);  // send stop
    _delay_ms(1);
}

void wrSensorRegs8_8(const struct regval_list reglist[]) {
    uint8_t reg_addr, reg_val;
    const struct regval_list *next = reglist;
    while ((reg_addr != 0xff) | (reg_val != 0xff)) {
        reg_addr = pgm_read_byte(&next->reg_num);
        reg_val = pgm_read_byte(&next->value);
        writeReg(reg_addr, reg_val);
        next++;
    }
}

void camInit() {
    //cli();  // disable interrupts

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
    _delay_ms(3000); // THIS IS NEEDED, DO NOT REMOVE

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

    writeReg(0x12, 0x80);
    _delay_ms(100);
    wrSensorRegs8_8(ov7670_default_regs);
    writeReg(REG_COM10, 32);  // PCLK does not toggle on HBLANK.
    //sei();
}

void setResolution() {
    writeReg(REG_COM3, 4);  // REG_COM3 enable scaling
    wrSensorRegs8_8(qvga_ov7670);
}

void setColor() {
    wrSensorRegs8_8(yuv422_ov7670);
    // wrSensorRegs8_8(qvga_ov7670);
}

void setClocl(uint8_t x) {
    writeReg(REG_CLKRC, x);
}
