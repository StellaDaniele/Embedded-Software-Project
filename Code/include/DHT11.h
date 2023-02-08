#ifndef __DHT11_H__
#define __DHT11_H__

#include <avr/io.h>
#include <stdint.h>

#define PIN_DHT PINA
#define PORT_DHT PORTA
#define DDR_DHT DDRA
#define DHT_PIN PA6

#define READ_DHT_PIN ((PIN_DHT & (1 << DHT_PIN)) != 0)
#define SET_HIGH_DHT PORT_DHT |= (1 << DHT_PIN);
#define SET_LOW_DHT PORT_DHT &= ~(1 << DHT_PIN);

/*
It needs to also set down the output first in order not
to enable the pull-up resistor
*/
#define SET_INPUT_DHT SET_LOW_DHT DDR_DHT &= ~(1 << DHT_PIN);
#define SET_OUTPUT_DHT DDR_DHT |= (1 << DHT_PIN);

/*
Reads the temperature and the humidity and stores the value
in the two variables passed as parameters. pdata is the buffer,
useful for debugging purposes. To use normally, call it with nullptr
as pdata.
*/
int read(uint8_t* ptemperature, uint8_t* phumidity, uint8_t pdata[40]);

#endif
