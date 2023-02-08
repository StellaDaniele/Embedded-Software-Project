#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <stdbool.h>
#include <stdint.h>

// Relay
#define DDR_RELAY DDRJ
#define PORT_RELAY PORTJ
#define RELAY PORTJ0
// PIR HC-SR501
#define DDR_PIR DDRK
#define PIN_PIR PINK
#define PIR PINK0
// Encoder
#define DDR_ENCODER_CLK DDRK
#define PIN_ENCODER_CLK PINK
#define PORT_ENCODER_CLK PORTK
#define ENCODER_CLK PINK1

#define DDR_ENCODER_DT DDRK
#define PIN_ENCODER_DT PINK
#define PORT_ENCODER_DT PORTK
#define ENCODER_DT PINK2

#define DDR_ENCODER_SW DDRK
#define PIN_ENCODER_SW PINK
#define PORT_ENCODER_SW PORTK
#define ENCODER_SW PINK3

extern volatile uint64_t curr_time_ms;
// This is the offset applied to the temperature threshold
extern volatile int8_t encoder_position;
extern volatile bool encoder_sw_pressed_interrupt;
extern volatile bool encoder_changed;

void board_init(void);
void enable_encoder_interrupt(void);
void disable_encoder_interrupt(void);

#endif
