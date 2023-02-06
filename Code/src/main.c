#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

#include "DHT11.h"
#include "LCD_display.h"
#include "general.h"
#include "camera_setup.h"
#include "capture.h"
#include "joystick.h"
#include "menu.h"
#include "states_functions.h"
#include "utilities.h"

volatile uint64_t curr_time_ms = 0;

int main(void) {
    board_init();
    init_joystick();
    start_lcd();
    init_menu();

    while (1) {
        if (joystick_new_direction()) {
            menu(current);
        }
        if (new_state) {
            new_state = false;
            clrscr();
            switch (curr_state) {
                case SENSORS:
                    // disp_str(1, 1, "in TEMP_HUM");
                    sensors_state();
                    break;
                case CAMERA:
                    // disp_str(1, 1, "in CAMERA");
                    camera_state();
                    break;
                case SETTINGS:
                    // disp_str(1, 1, "in RELAY");
                    settings_state();
                    break;
                case REBOOT:
                    reboot_state();
                    break;

                default:
                    break;
            }
            init_menu();
        }
    }
    return 0;
}

ISR(TIMER1_COMPA_vect) {
    // called every 1ms
    ++curr_time_ms;
}
