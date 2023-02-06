#include "states_functions.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

#include "DHT11.h"
#include "LCD_display.h"
#include "arduino_init.h"
#include "camera_setup.h"
#include "capture.h"
#include "joystick.h"
#include "menu.h"
#include "utilities.h"
#include "states_functions.h"

void sensors_state(void) {
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    int res;
    while (1) {
        res = read(&temperature, &humidity, NULL);
        if (res) {
            disp_num(1, 1, res);
            error_led();
        }
        // disp_str_num(1,1,"TEMP: ", (int)temperature);
        // disp_str_num(1,2,"HUMI: ", (int)humidity);

        disp_str(1, 1, "TEMP: ");
        disp_num(7, 1, (int)temperature);
        disp_str(9, 1, "C");
        disp_str(1, 2, "HUMI: ");
        disp_num(7, 2, (int)humidity);
        disp_str(9, 2, "%");
        // DHT11 sampling rate is 1HZ.
        _delay_ms(1000);
    }
}

void relay_state(void) {
    PORTJ &= ~(1 << PORTJ0);
    disp_str(1, 1, "Relay ON");
    while (true) {
        if (joystick_new_direction()) {
            if (current == C) {
                PORTJ ^= (1 << PORTJ0);
                clrscr();
                disp_str(1, 1, (PORTJ & (1 << PORTJ0)) ? "Relay OFF" : "Relay ON");
            }
        }
    }
}

void camera_state(void) {
    // Setup
    // arduinoUnoInit();
    camInit();
    setResolution();
    setColor();
    // setClocl(5);
    setClocl(10);
    disp_str(1, 1, "Sending images on");
    disp_str(1, 2, "serial 0...");
    _delay_ms(100);
    int i = 0;
    disp_str_num(1, 3, "Images sent: ", i);
    while (true) {
        captureImg(320, 240);
        disp_num(14, 3, ++i);
    }
}

void settings(void){

}
