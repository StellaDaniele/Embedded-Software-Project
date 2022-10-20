#include <avr/io.h>
#include <stdint.h>
#ifndef H_DHT1
#define H_DHT1

#define PIN_DHT PINA
#define PORT_DHT PORTA
#define DDR_DHT DDRA
#define DHT_PIN PA6

#define READ_DHT_PIN (PIN_DHT & (1 << DHT_PIN) ? 1 : 0)
#define SET_HIGH_DHT PORT_DHT |= (1 << DHT_PIN);
#define SET_LOW_DHT PORT_DHT &= ~(1 << DHT_PIN);

#define SET_INPUT_DHT SET_LOW_DHT DDR_DHT &= ~(1 << DHT_PIN);
#define SET_OUTPUT_DHT DDR_DHT |= (1 << DHT_PIN);

int read(uint8_t* ptemperature, uint8_t* phumidity, uint8_t pdata[40]);

#endif
