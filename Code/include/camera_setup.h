/**
 * @file camera_setup.h
 * @brief Functions for initializing the OV7670 camera module on an ATmega2560 microcontroller
 */

#ifndef __CAMERA_SETUP_H__
#define __CAMERA_SETUP_H__

#include <stdint.h>

/**
 * @brief Initializes the camera module.
 */
void camInit();

/**
 * @brief Sets the camera resolution to QVGA.
 */
void setResolution();

/**
 * @brief Sets the camera color format to YUV422.
 */
void setColor();

/**
 * @brief Sets the camera clock frequency.
 *
 * @param x The clock frequency value to set.
 */
void setClocl(uint8_t);

#endif
