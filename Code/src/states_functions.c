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

#ifndef __GNUC__
#define __attribute__(x)
#endif

uint8_t threshold_temp = 18;

inline __attribute__((always_inline)) void sensors_state(void) {
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    int res;
    bool old_ir_state = false;
    disp_str(1, 1, "TEMP: ");
    disp_str(11, 1, "HEATER OFF");
    disp_str(1, 2, "HUMI: ");
    disp_str(1, 3, "MOVEMENT: NO");
    disp_str(1, 4, "CENTRAL BTN TO EXIT");
    central_button_pressed_interrupt = false;
    while (1) {
        // DHT11 sampling rate is 1HZ.
        if ((curr_time_ms % 1000) == 0) {
            res = read(&temperature, &humidity, NULL);
            if (res)
                error(DHT11_ERROR, res);
            // disp_str_num(1,1,"TEMP: ", (int)temperature);
            // disp_str_num(1,2,"HUMI: ", (int)humidity);

            disp_num(7, 1, (int)temperature);
            disp_str(9, 1, "C");
            disp_num(7, 2, (int)humidity);
            disp_str(9, 2, "%");

            if (temperature < threshold_temp) {
                disp_str(18, 1, "ON ");
                PORTJ &= ~(1 << PORTJ0);
            } else {
                disp_str(18, 1, "OFF");
                PORTJ |= (1 << PORTJ0);
            }
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

inline __attribute__((always_inline)) void camera_state(void) {
    // Setup
    // arduinoUnoInit();
    disp_str(1, 1, "INITIALIZING");
    disp_str(1, 2, "CAMERA...");
    camInit();
    setResolution();
    setColor();
    // setClocl(5);
    setClocl(10);
    disp_str(1, 1, "SENDING IMAGES ON");
    disp_str(1, 2, "UART0 ...");
    _delay_ms(100);
    int i = 0;
    disp_str_num(1, 3, "IMAGES SENT: ", i);
    disp_str(1, 4, "CENTRAL BTN TO EXIT");
    central_button_pressed_interrupt = false;
    while (true) {
        // The camera module used supports up to 640X480
        captureImg(320, 240);
        disp_num(14, 3, ++i);
        if (central_button_pressed_interrupt) {
            return;
        }
    }
}

inline __attribute__((always_inline)) void settings_state(void) {
    disp_str(1, 1, "SET THRESHOLD:");
    disp_str(1, 2, "CURRENT VALUE:");
    disp_str(18, 2, "C");
    disp_str(1, 3, "ENC BTN TO DISCARD");
    disp_num(15, 2, threshold_temp);
    disp_str(1, 4, "CENTRAL BTN TO EXIT");
    central_button_pressed_interrupt = false;
    enable_encoder_interrupt();
    int old_threshold = threshold_temp;

    while (true) {
        if (encoder_changed) {
            encoder_changed = false;
            threshold_temp += encoder_position;
            if (threshold_temp < MIN_THRESHOLD_TEMP)
                threshold_temp = MIN_THRESHOLD_TEMP;
            else if (threshold_temp >= MAX_THRESHOLD_TEMP)
                threshold_temp = MAX_THRESHOLD_TEMP;
            encoder_position = 0;
            disp_num(15, 2, threshold_temp);
        }
        if (encoder_sw_pressed_interrupt) {
            encoder_sw_pressed_interrupt = false;
            threshold_temp = old_threshold;
            disp_num(15, 2, threshold_temp);
        }
        if (central_button_pressed_interrupt) {
            disable_encoder_interrupt();
            return;
        }
    }
}

inline __attribute__((always_inline)) void reboot_state(void) {
    reset_board();
}
