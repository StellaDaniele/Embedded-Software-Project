#include "camera_setup.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <util/twi.h>

#include "macros_regs_ov7670.h"
#include "utilities.h"

#ifndef __GNUC__
#define __attribute__(x)
#endif

#define WAIT_FOR_TRANSMISSION_TWI  \
    while (!(TWCR & (1 << TWINT))) \
        ;

void twiStart(void) {
    // send start
    TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
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
    // send address
    TWDR = addr;
    /* clear interrupt to start transmission */
    TWCR = (1 << TWINT) | (1 << TWEN);
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
        ++next;
    }
}

void camInit(void) {
    // 8mhz PWM master clock
    DDRB |= (1 << PORTB4);  // pin 11 (clock output)
    // Disable external clock input and asynchronous operation
    ASSR &= ~((1 << EXCLK) | (1 << AS2));
    // Fast PWM non-inverted out on OC2A and TOP=0xFF
    TCCR2A = (1 << COM2A0) | (1 << WGM21) | (1 << WGM20);
    // CLK no prescaling, Mode 7 TOP=0xFF
    TCCR2B = (1 << WGM22) | (1 << CS20);
    OCR2A = 2;  //(F_CPU)/(2*(X+1))

    DDRC = 0;  // all data bits connected to the ddrc

    DDRD &= ~((1 << PORTD2) | (1 << PORTD3));
    /*
    THIS DELAY IS NEEDED, DO NOT REMOVE!!
    The camera module needs some time to initialize.
    I ran some tests as I wanted to recude the time
    but sometimes it went in error during the
    twiAddr function therefore I decided to keep
    it 3s.
    */
    _delay_ms(3000);

    // Set up twi for 100khz
    TWSR &= ~((1 << TWPS0) | (1 << TWPS1));
    TWBR = 72;  // set to 100khz
    // TWBR = (1 << TWBR3) | (1 << TWBR6);

    // Enable serial
    // 1M baud rate
    UBRR0H = 0;
    UBRR0L = 1;
    // double speed aysnc
    UCSR0A |= (1 << U2X0);
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // async 1 stop bit 8bit char no parity bits
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

    writeReg(0x12, 0x80);
    _delay_ms(100);
    wrSensorRegs8_8(ov7670_default_regs);
    writeReg(REG_COM10, 32);  // PCLK does not toggle on HBLANK.
}

__attribute__ ((always_inline)) inline void setResolution(void) {
    writeReg(REG_COM3, 4);  // REG_COM3 enable scaling
    wrSensorRegs8_8(qvga_ov7670);
}

__attribute__ ((always_inline)) inline void setColor(void) {
    wrSensorRegs8_8(yuv422_ov7670);
    // wrSensorRegs8_8(qvga_ov7670);
}

__attribute__ ((always_inline)) inline void setClocl(uint8_t x) {
    writeReg(REG_CLKRC, x);
}
