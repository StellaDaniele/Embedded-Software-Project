#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

// LCD
#include "LCD_display.h"
// CAMERA
#include "arduino_init.h"
#include "camera_setup.h"
#include "capture.h"
#include "utilities.h"

#define WIDTH 320
#define HEIGHT 240

#define MESSAGE "Will it work??"

int main(void) {
    // Setup
    arduinoUnoInit();
    camInit();
    setResolution();
    setColor();
    // setClocl(5);
    setClocl(10);
    start_lcd();
    disp_num(1, 1, -1234);
    disp_num(1, 2, 987);
    disp_str(1, 3, MESSAGE);
    uint32_t i = 0;
    while (true) {
        i++;
        captureImg(WIDTH, HEIGHT);
        disp_num(1, 4, i);
    }
    return 0;
}
