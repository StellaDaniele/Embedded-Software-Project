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

uint8_t threshold_tmp = 18;

void sensors_state(void) {
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    int res;
    bool old_ir_state = false;
    disp_str(1, 1, "TEMP: ");
    disp_str(11, 1, "HEATER OFF");
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

            if (temperature < threshold_tmp) {
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
    disp_str(1, 2, "UART0 ...");
    _delay_ms(100);
    int i = 0;
    disp_str_num(1, 3, "Images sent: ", i);
    disp_str(1, 4, "Central btn to exit");
    central_button_pressed_interrupt = false;
    while (true) {
        captureImg(320, 240);
        // captureImg(640, 240);
        disp_num(14, 3, ++i);
        if (central_button_pressed_interrupt) {
            return;
        }
    }
}

void settings_state(void) {
    disp_str(1, 1, "Set threshold:");
    disp_str(1, 2, "Current value:");
    disp_num(15, 2, threshold_tmp);
    disp_str(1, 4, "Central btn to exit");
    central_button_pressed_interrupt = false;
    enable_encoder_interrupt();

    while (true) {
        if (encoder_changed) {
            encoder_changed = false;
            threshold_tmp += encoder_position;
            encoder_position = 0;
            disp_num(15, 2, threshold_tmp);
        }
        if (central_button_pressed_interrupt) {
            disable_encoder_interrupt();
            return;
        }
    }
}

void reboot_state(void) {
    reset_board();
}
