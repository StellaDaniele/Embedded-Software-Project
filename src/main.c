#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

#include "DHT11.h"
#include "LCD_display.h"
#include "joystick.h"
#include "menu.h"
#include "utilities.h"

void temp_humi_state(void) {
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
}

int main(void) {
    DDRJ |= (1 << DDJ0);
    PORTJ |= (1 << PORTJ0);
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
                case TEMP_HUM:
                    // disp_str(1, 1, "in TEMP_HUM");
                    temp_humi_state();
                    break;
                case CAMERA:
                    disp_str(1, 1, "in CAMERA");
                    break;
                case RELAY:
                    // disp_str(1, 1, "in RELAY");
                    relay_state();
                    break;

                default:
                    break;
            }
        }
    }
    return 0;
}
