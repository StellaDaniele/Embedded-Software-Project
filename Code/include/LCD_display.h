/**
 * @file LCD_display.h
 * @brief Functions for using the LCD on an ATmega2560 microcontroller
 */

#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

#include <avr/io.h>
#include <util/delay.h>

/*Set up LCD 20X4 on ATmega2560:
    The numbers are referred to the digital pins of the Arduino board
    In the .c file there are all the macros to easily change the pins
    VSS : GND
    VDD : 5V
    VE : Connected to a 10k potentiometer in order to adjust screen contrast
    RS : 24 - PORTA0
    RW : GND
    E : 25 - PORTA1
    D0 :
    D1 :
    D2 :
    D3 :
    D4 : 26 - PORTA2
    D5 : 27 - PORTA3
    D6 : 28 - PORTA4
    D7 : 29 - PORTA5
    A : 56 ohm resistor to set the backlight brightness
        There is also a resistor in the LCD board itself
    K : GND
*/

/**
 * @brief Initializes the LCD display.
 *
 * This function initializes the LCD display by setting the data direction registers
 * and writing to the display to configure it.
 */
void start_lcd(void);

/**
 * @brief Clears the LCD display.
 *
 * This function clears the LCD display.
 */
void clrscr(void);

/**
 * @brief Sets the cursor to a specified position on the LCD display.
 *
 * This function sets the cursor to a specified position on the LCD display.
 *
 * @param x The horizontal position of the cursor.
 * @param y The vertical position of the cursor.
 */
void gotoxy(unsigned char x, unsigned char y);

/**
 * @brief Displays a string at a specified position.
 *
 * This function displays a string at a specified position on the LCD display.
 *
 * @param x The horizontal position of the string.
 * @param y The vertical position of the string.
 * @param str The string to be displayed.
 */
void disp_str(unsigned char x, unsigned char y, char str[]);

/**
 * @brief Displays an unsigned number at a specified position on the LCD display.
 *
 * This function displays a number at a specified position on the LCD display.
 *
 * @param a The horizontal position of the number.
 * @param b The vertical position of the number.
 * @param num The number to be displayed.
 */
void disp_pos(unsigned char a, unsigned char b, unsigned int num);

/**
 * @brief Displays a signed number at a specified position on the LCD display.
 *
 * This function displays a number at a specified position on the LCD display.
 *
 * @param a The horizontal position of the number.
 * @param b The vertical position of the number.
 * @param num The number to be displayed.
 */
void disp_num(unsigned char a, unsigned char b, int num);

/**
 * @brief Displays a floating point number at a specified position on the LCD display.
 *
 * This function displays a floating point number at a specified position on the LCD display.
 *
 * @param a The horizontal position of the number.
 * @param b The vertical position of the number.
 * @param num The number to be displayed.
 * @param prec The precision (digits to display after the point).
 */
void disp_float(char a, char b, float num, unsigned char prec);

/**
 * @brief Displays a number at a specified position on the LCD display, after the string.
 *
 * This function displays a number at a specified position on the LCD display, after the string.
 *
 * @param a The horizontal position of the number.
 * @param b The vertical position of the number.
 * @param str The string to be displayed.
 * @param num The number to be displayed.
 */
void disp_str_num(unsigned char a, unsigned char b, char str[], int num);

#endif
