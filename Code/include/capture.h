/**
 * @file capture.h
 * @brief Capture image from the OV7670 camera module on an ATmega2560 microcontroller
 */

#ifndef __CAPTURE_H__
#define HPP_CAPTURE

#include <stdint.h>

/**
 * @brief Captures an image from the OV7670 camera module and transfers it via USART0
 *
 * This function captures an image from the OV7670 camera module and transfers it via USART0.
 *
 * @param wg A `uint16_t` value indicating the width of the image to be captured.
 * @param hg A `uint16_t` value indicating the height of the image to be captured.
 *
 * @details This function first sends the string `"*RDY*"` to the device connected to the USART0 to recognize the beginning of a new image. Then, it waits for the `RDY` pin to go high, which indicates that the OV7670 module is ready to transfer the image. Once the `RDY` pin goes high, the function enters a loop to capture the image pixel by pixel.
 *
 * The image is captured by reading the data pins of the OV7670 module through the `PINC` register. The function reads the data pins, transmits the pixel data via USART0, and waits for the `VSYNC` pin to go high and low before reading the next pixel. This process is repeated until the entire image is captured.
 *
 * After capturing the image, the function waits for 100ms before returning.
 */
void captureImg(uint16_t wg, uint16_t hg);

#endif
