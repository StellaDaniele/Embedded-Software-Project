#include "menu.h"

#include "LCD_display.h"
#include "joystick.h"

static int selection_x = 1, selection_y = 1;

void init_menu() {
    curr_state = MENU;
    new_state = false;
    disp_str(selection_x, selection_y, ">");
    disp_str(2, 1, "TEMP_HUM");
    disp_str(2, 2, "CAMERA");
    disp_str(13, 1, "RELAY");
    // disp_str(13, 2, "FOURTH");
}

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
            /*
            disp_str(selection_x, selection_y, " ");
            disp_str(selection_x, selection_y, "*");
            */
            if (selection_x == 1 && selection_y == 1) {
                new_state = true;
                curr_state = TEMP_HUM;
            } else if (selection_x == 1 && selection_y == 2) {
                new_state = true;
                curr_state = CAMERA;
            } else if (selection_x == 12 && selection_y == 1) {
                new_state = true;
                curr_state = RELAY;
            }
            break;
        default:
            break;
    }
}
