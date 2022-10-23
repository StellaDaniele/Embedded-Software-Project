#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "LCD_display.h"

#define MESSAGE "Will it work??"

void lcd_test(void) {
    start_lcd();
    disp_num(1, 1, -1234);
    disp_num(1, 2, 987);
    disp_str(1, 3, MESSAGE);
}

/*
int main(void) {
    lcd_test();
    return 0;
}
*/
