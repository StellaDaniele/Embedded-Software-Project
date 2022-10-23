#include <stddef.h>
#include <stdint.h>

#include "DHT11.h"
#include "LCD_display.h"
#include "utilities.h"

int main(void) {
    start_lcd();
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    int res;
    while (1) {
        res = read(&temperature, &humidity, NULL);
        if (res) {
            disp_num(1, 1, res);
            error_led();
        }
        //disp_str_num(1,1,"TEMP: ", (int)temperature);
        //disp_str_num(1,2,"HUMI: ", (int)humidity);

        disp_str(1,1,"TEMP: ");
        disp_num(7,1, (int)temperature);
        disp_str(9,1,"C");
        disp_str(1,2,"HUMI: ");
        disp_num(7,2, (int)humidity);
        disp_str(9,2,"%");
        // DHT11 sampling rate is 1HZ.
        _delay_ms(1000);
    }
}
