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
// Starts up the LCD before using it
void start_lcd(void);
// Clears the entire display
void clrscr(void);
// Moves the cursor to the given x and y coordinates
void gotoxy(unsigned char x, unsigned char y);
// Displays the given string on the display, at the given coordinates
void disp_str(unsigned char x, unsigned char y, char str[]);
// Displays the given 16 bit positive number on the display, at the given coordinates
void disp_pos(unsigned char a, unsigned char b, unsigned int num);
// Displays the given number (both positive and negative) on the display, at the given coordinates
void disp_num(unsigned char a, unsigned char b, int num);
// Displays the given float number on the display, at the given coordinates
void disp_float(char a, char b, float num, unsigned char prec);
// Displays the given number on the display, after the string, at the given coordinates
void disp_str_num(unsigned char a, unsigned char b, char str[], int num);

#endif
