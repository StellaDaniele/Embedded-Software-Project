/**
 * @file DHT11.h
 * @brief Functions for using the DHT11 temperature and humidity sensor on an ATmega2560 microcontroller
 */

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

/**
 *@brief Reads data from the DHT11 sensor and extracts temperature and humidity values.
 *@param ptemperature Pointer to where the temperature value should be stored.
 *@param phumidity Pointer to where the humidity value should be stored.
 *@param pdata Optional output buffer to store the sampled data.
 *@return 0 if successful, or an error code if an error occurs during sampling or parsing.
 */
int8_t read(uint8_t* ptemperature, uint8_t* phumidity, uint8_t pdata[40]);

#endif
