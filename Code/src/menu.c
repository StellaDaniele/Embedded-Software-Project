#include "menu.h"

#include "LCD_display.h"
#include "joystick.h"
#include "utilities.h"

states curr_state;
bool new_state;
static int8_t selection_x = 1, selection_y = 1;

void init_menu(void) {
    curr_state = MENU;
    new_state = false;
    clrscr();
    disp_str(selection_x, selection_y, ">");
    disp_str(2, 1, "SENSORS");
    disp_str(2, 2, "CAMERA");
    disp_str(13, 1, "SETTINGS");
    disp_str(13, 2, "REBOOT");
}

void menu(enum joystick_dir current) {
    switch (current) {
        case D:
            if (selection_y + 1 > 2) break;  // Change if new states are added
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
                curr_state = SENSORS;
            } else if (selection_x == 1 && selection_y == 2) {
                new_state = true;
                curr_state = CAMERA;
            } else if (selection_x == 12 && selection_y == 1) {
                new_state = true;
                curr_state = SETTINGS;
            } else if (selection_x == 12 && selection_y == 2) {
                new_state = true;
                curr_state = REBOOT;
            }
            break;
        default:
            error(UNEXPECTED_JOYSTICK_DIR_ERROR, 0);
            break;
    }
}
