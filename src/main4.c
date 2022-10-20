#include <stddef.h>
#include <stdint.h>

#include "DHT11.h"
#include "LCD_display.h"
#include "utilities.h"
// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2

int main(void) {
    start_lcd();
    uint8_t temperature = 10;
    uint8_t humidity = 10;
    int res;
    while (1) {
        res = read(&temperature, &humidity, NULL);
        if (res) {
            disp_num(1, 1, res);
            error_led();
        }

        disp_pos(1, 1, (int)temperature);
        disp_pos(1, 2, (int)humidity);

        // DHT11 sampling rate is 1HZ.
        _delay_ms(1000);
    }
}
