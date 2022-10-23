#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#include "LCD_display.h"
#include "joystick.h"

int selection_x = 1, selection_y = 1;

void menu(enum joystick_dir current) {
    switch (current) {
        case D:
            if (selection_y + 1 > 4) break;
            disp_str(selection_x, selection_y, " ");
            disp_str(selection_x, ++selection_y, ">");
            break;
        case U:
            if (selection_y - 1 < 1) break;
            disp_str(selection_x, selection_y, " ");
            disp_str(selection_x, --selection_y, ">");
            break;
        case R:
            if (selection_x + 11 > 20) break;
            disp_str(selection_x, selection_y, " ");
            selection_x += 11;
            disp_str(selection_x, selection_y, ">");
            break;
        case L:
            if (selection_x - 11 < 1) break;
            disp_str(selection_x, selection_y, " ");
            selection_x -= 11;
            disp_str(selection_x, selection_y, ">");
            break;
        case C:
            disp_str(selection_x, selection_y, " ");
            disp_str(selection_x, selection_y, "*");
            break;
        default:
            break;
    }
}

int main(void) {
    init_joystick();
    start_lcd();
    
    disp_str(selection_x, selection_y, ">");
    disp_str(2, 1, "FIRST");
    disp_str(2, 2, "SECOND");
    disp_str(13, 1, "THIRD");
    disp_str(13, 2, "FOURTH");
    while (1) {
        
        if (joystick_new_direction()) {
            menu(current);
        }
    }
}
