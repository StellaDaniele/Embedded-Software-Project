#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "arduino_init.h"
#include "camera_setup.h"
#include "capture.h"
#include "utilities.h"

#define WIDTH 320
#define HEIGHT 240

//#define WIDTH 640
//#define HEIGHT 480


int main() {
    // Setup
    arduinoUnoInit();
    camInit();
    setResolution();
    setColor();
    // setClocl(5);
    setClocl(10);

    while (true) {
        captureImg(WIDTH, HEIGHT);
    }
    return 0;
}

