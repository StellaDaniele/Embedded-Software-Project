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

int main(void) {
    board_init();
    init_joystick();
    start_lcd();
    clrscr();
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
                    // disp_str(1, 1, "in SETTINGS");
                    settings_state();
                    break;
                case REBOOT:
                    // disp_str(1, 1, "in REBOOT");
                    reboot_state();
                    break;

                default:
                    error(UNEXPECTED_STATE_ERROR, 0);
            }
            init_menu();
            _delay_ms(1);  // debouncing central joystick button
            central_button_pressed_interrupt = false;
        }
    }
    // Should never be here
    error(GENERAL_ERROR, 0);
    return 0;
}
