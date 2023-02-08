#include "states_functions.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

#include "DHT11.h"
#include "LCD_display.h"
#include "camera_setup.h"
#include "capture.h"
#include "general.h"
#include "joystick.h"
#include "menu.h"
#include "states_functions.h"
#include "utilities.h"

void sensors_state(void) {
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    int res;
    bool old_ir_state = false;
    disp_str(1, 1, "TEMP: ");
    disp_str(1, 2, "HUMI: ");
    disp_str(1, 3, "MOVEMENT: NO");
    disp_str(1, 4, "Central btn to exit");
    central_button_pressed_interrupt = false;
    while (1) {
        if ((curr_time_ms % 1000) == 0) {
            // DHT11 sampling rate is 1HZ.
            res = read(&temperature, &humidity, NULL);
            if (res)
                error(DHT11_ERROR, res);
            // disp_str_num(1,1,"TEMP: ", (int)temperature);
            // disp_str_num(1,2,"HUMI: ", (int)humidity);

            disp_num(7, 1, (int)temperature);
            disp_str(9, 1, "C");
            disp_num(7, 2, (int)humidity);
            disp_str(9, 2, "%");
        }
        if (old_ir_state != ((PIN_PIR & (1 << PIR)) != 0)) {
            old_ir_state = ((PIN_PIR & (1 << PIR)) != 0);
            disp_str(11, 3, ((PIN_PIR & (1 << PIR)) != 0) ? "YES" : "NO ");
        }
        if (central_button_pressed_interrupt) {
            return;
        }
    }
}

void camera_state(void) {
    // Setup
    // arduinoUnoInit();
    disp_str(1, 1, "Initializing");
    disp_str(1, 2, "camera...");
    camInit();
    setResolution();
    setColor();
    // setClocl(5);
    setClocl(10);
    disp_str(1, 1, "Sending images on");
    disp_str(1, 2, "UART 0...");
    _delay_ms(100);
    int i = 0;
    disp_str_num(1, 3, "Images sent: ", i);
    disp_str(1, 4, "Central btn to exit");
    central_button_pressed_interrupt = false;
    while (true) {
        captureImg(320, 240);
        disp_num(14, 3, ++i);
        if (central_button_pressed_interrupt) {
            return;
        }
    }
}

void settings_state(void) {
    // TODO
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

void reboot_state(void) {
    reset_board();
}
