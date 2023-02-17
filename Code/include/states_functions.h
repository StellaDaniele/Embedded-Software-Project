#ifndef __STATES_FUNCTIONS_H__
#define __STATES_FUNCTIONS_H__

#include <stdint.h>

#define MIN_THRESHOLD_TEMP 6
#define MAX_THRESHOLD_TEMP 30

// Initial value is 18°C
extern uint8_t threshold_temp;

/**
 * @brief Monitors temperature, humidity, and movement using DHT11 and PIR sensors.
 * 
 * This function continuously samples temperature and humidity using DHT11 sensor, and
 * detects movement using PIR sensor. If the temperature falls below a predefined threshold,
 * it turns on the heater. The current temperature and humidity readings, along with the
 * status of the heater and movement detection, are displayed on the LCD screen.
 * 
 * @return void
 */
void sensors_state(void);

/**
 * @brief Captures and transmits images from the camera module.
 * 
 * This function initializes and configures the camera module, and continuously captures
 * images with a resolution of 320x240 pixels. Each captured image is transmitted over UART0
 * to a connected device. The number of images transmitted is displayed on the LCD screen.
 * 
 * @return void
 */
void camera_state(void);

/**
 * @brief Allows the user to adjust the temperature threshold.
 * 
 * This function displays the current temperature threshold on the LCD screen and allows
 * the user to adjust it using an encoder. The new threshold value is immediately displayed
 * on the screen. Pressing the encoder button saves the new threshold value and exits this
 * state. Pressing the central button exits this state without saving any changes.
 * 
 * @return void
 */
void settings_state(void);

/**
 * @brief Reboots the microcontroller.
 * 
 * This function reboots the microcontroller by calling the reset_board() function.
 * 
 * @return void
 */
void reboot_state(void);

#endif
