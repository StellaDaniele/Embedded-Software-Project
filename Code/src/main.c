#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

#include "DHT11.h"
#include "LCD_display.h"
#include "board_init.h"
#include "camera_setup.h"
#include "capture.h"
#include "joystick.h"
#include "menu.h"
#include "utilities.h"
#include "states_functions.h"


int main(void) {

    // Set up timer 1 in normal mode
    TCCR1A = 0x00;
    TCCR1B = 0x00;

    // Set the prescaler
    TCCR1B |= (1 << CS11);

    // Set the compare value
    OCR1A = F_CPU / 8 / 1000;

    // Enable CTC mode and timer 1 overflow interrupt
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);
    sei();

    DDRJ |= (1 << DDJ0);
    PORTJ |= (1 << PORTJ0);
    //arduinoUnoInit();
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
                    sensors_state();
                    break;
                case CAMERA:
                    // disp_str(1, 1, "in CAMERA");
                    camera_state();
                    break;
                case RELAY:
                    // disp_str(1, 1, "in RELAY");
                    relay_state();
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
}
