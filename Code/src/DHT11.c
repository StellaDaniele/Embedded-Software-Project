/*
This was written starting from the DHT11 datasheet
and the Arduino C library. The error codes are the same
so that it is easier to debug.
*/

#include "DHT11.h"

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>

int confirm(int us, uint8_t level);
uint8_t bits2byte(uint8_t data[8]);
int sample(uint8_t data[40]);
int parse(uint8_t data[40], uint8_t* ptemperature, uint8_t* phumidity);

int confirm(int us, uint8_t level) {
    // Check if the sensor responds as expected
    // wait one more count to ensure.
    int cnt = us / 10 + 1;

    bool ok = false;
    for (int i = 0; i < cnt; i++) {
        if (READ_DHT_PIN != level) {
            ok = true;
            break;
        }
        _delay_us(10);
    }
    return (!ok ? -1 : 0);
}

uint8_t bits2byte(uint8_t data[8]) {
    uint8_t v = 0;
    for (int i = 0; i < 8; i++)
        v += data[i] << (7 - i);
    return v;
}

int sample(uint8_t data[40]) {
    // empty output data.
    memset(data, 0, 40);

    // notify DHT11 to start:
    // PULL LOW 20ms.
    // PULL HIGH 20-40us.
    // SET TO INPUT.
    SET_OUTPUT_DHT
    SET_LOW_DHT
    _delay_ms(20);
    SET_HIGH_DHT
    _delay_us(30);
    SET_INPUT_DHT

    // DHT11 starting:
    // PULL LOW 80us
    // PULL HIGH 80us
    if (confirm(80, 0))
        return 100;
    if (confirm(80, 1))
        return 101;

    // DHT11 data transmite:
    // 1bit start, PULL LOW 50us
    // PULL HIGH 26-28us, bit(0)
    // PULL HIGH 70us, bit(1)
    for (int j = 0; j < 40; j++) {
        if (confirm(50, 0))
            return 102;
        // read a bit, should never call method,
        // for the method call use more than 20us,
        // so it maybe failed to detect the bit0.
        bool ok = false;
        int tick = 0;
        for (int i = 0; i < 8; i++, tick++) {
            if (READ_DHT_PIN != 1) {
                ok = true;
                break;
            }
            _delay_us(10);
        }
        if (!ok)
            return 103;
        data[j] = (tick > 3 ? 1 : 0);
    }

    // DHT11 EOF:
    // PULL LOW 50us.
    if (confirm(50, 0))
        return 104;
    return 0;
}

int parse(uint8_t data[40], uint8_t* ptemperature, uint8_t* phumidity) {
    // Extract values and compare checksum to ensure data accuracy
    uint8_t humidity = bits2byte(data);
    uint8_t humidity2 = bits2byte(data + 8);
    uint8_t temperature = bits2byte(data + 16);
    uint8_t temperature2 = bits2byte(data + 24);
    uint8_t check = bits2byte(data + 32);
    uint8_t expect = humidity + humidity2 + temperature + temperature2;
    if (check != expect)
        return 105;
    *ptemperature = temperature;
    *phumidity = humidity;
    return 0;
}

int read(uint8_t* ptemperature, uint8_t* phumidity, uint8_t pdata[40]) {
    int ret = 0;
    uint8_t data[40] = {0};
    if ((ret = sample(data)) != 0)
        return ret;
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    if ((ret = parse(data, &temperature, &humidity)) != 0)
        return ret;
    if (pdata)
        memcpy(pdata, data, 40);
    if (ptemperature)
        *ptemperature = temperature;
    if (phumidity)
        *phumidity = humidity;
    return ret;
}
