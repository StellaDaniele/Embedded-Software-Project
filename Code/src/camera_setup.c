#include "camera_setup.h"

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
        error_led();
}

void twiWriteByte(uint8_t DATA, uint8_t type) {
    TWDR = DATA;
    TWCR = (1 << TWINT) | (1 << TWEN);
    WAIT_FOR_TRANSMISSION_TWI
    if ((TWSR & 0xF8) != type)
        error_led();
}

void twiAddr(uint8_t addr, uint8_t typeTWI) {
    TWDR = addr;                       // send address
    TWCR = (1 << TWINT) | (1 << TWEN); /* clear interrupt to start transmission */
    WAIT_FOR_TRANSMISSION_TWI
    if ((TWSR & 0xF8) != typeTWI)
        error_led();
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
    writeReg(0x12, 0x80);
    _delay_ms(100);
    wrSensorRegs8_8(ov7670_default_regs);
    writeReg(REG_COM10, 32);  // PCLK does not toggle on HBLANK.
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
