#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#include "LCD_display.h"
#include "joystick.h"
#include "test5_menu.h"

int main(void) {
    init_joystick();
    start_lcd();
    init_menu();

    while (1) {
        if (joystick_new_direction()) {
            menu(current);
        }
    }
}
