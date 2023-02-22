#include "LCD_display.h"
// Displays a character on the display, first it writes the 4 bits
void display(uint8_t car, uint8_t stat);
void float_to_int(float number, uint8_t digits, int32_t *integer_part, int32_t *floating_part);
uint32_t power(uint32_t base, uint32_t exponent);

#ifndef __GNUC__
#define __attribute__(x)
#endif

#define CONF 0
#define DATA 1

#ifndef __AVR_ATmega2560__
#warning This works on AVR ATmega2560 or equivalent.
#else

#define PORT_EN PORTA
#define DDR_EN DDRA
#define EN 1

#define PORT_RS PORTA
#define DDR_RS DDRA
#define RS 0

#define PORT_DISP PORTA
#define DDR_DISP DDRA
#define MASK_DISP (0x0F << 2)

#define WR_DISP(n)           \
    PORT_DISP &= ~MASK_DISP; \
    PORT_DISP |= ((n << 2) & MASK_DISP)
#endif

#define SET_EN PORT_EN |= (1 << EN)
#define RES_EN PORT_EN &= ~(1 << EN)
#define SET_RS PORT_RS |= (1 << RS)
#define RES_RS PORT_RS &= ~(1 << RS)

void start_lcd(void) {
    DDR_DISP |= MASK_DISP;
    DDR_RS |= (1 << RS);
    DDR_EN |= (1 << EN);

    RES_RS;
    WR_DISP(0x03);
    _delay_us(100);
    RES_EN;
    _delay_us(100);
    SET_EN;
    _delay_us(100);
    RES_EN;
    _delay_ms(5);

    WR_DISP(0x03);
    _delay_us(100);
    RES_EN;
    _delay_us(100);
    SET_EN;
    _delay_us(100);
    RES_EN;
    _delay_us(1000);

    WR_DISP(0x03);
    _delay_us(100);
    RES_EN;
    _delay_us(100);
    SET_EN;
    _delay_us(100);
    RES_EN;
    _delay_us(1000);

    WR_DISP(0x02);
    _delay_us(100);
    RES_EN;
    _delay_us(100);
    SET_EN;
    _delay_us(100);
    RES_EN;
    _delay_us(1000);

    display(0x28, CONF); /* function set    [ 0010 1000 ] */
    _delay_us(1000);
    display(0x08, CONF); /* display off     [ 00001DCB ] */
    _delay_us(1000);
    display(0x01, CONF); /* display on      [ 00000001 ] */
    _delay_us(1000);
    display(0x06, CONF); /* entry mode set  [ 000001 I/D S ] */
    _delay_us(1000);
    display(0x0c, CONF); /* display on      [ 00001DCB ] */
    _delay_us(1000);
}

void display(uint8_t car, uint8_t stat) {
    if (stat)
        SET_RS;
    else
        RES_RS;

    WR_DISP(car >> 4);
    _delay_us(100);
    SET_EN;
    _delay_us(100);
    RES_EN;

    WR_DISP(car);
    _delay_us(100);
    SET_EN;
    _delay_us(100);
    RES_EN;

    _delay_ms(2);
}

__attribute__((always_inline)) inline void clrscr(void) {
    display(0x01, CONF);
    _delay_ms(2);
}

void disp_str(uint8_t x, uint8_t y, char str[]) {
    uint8_t num;
    gotoxy(x, y);
    for (num = 0; str[num] != 0; num++)
        display(str[num], DATA);
}

void gotoxy(uint8_t x, uint8_t y) {
    x = (x > 20) ? 20 : x;
    y = (y > 4) ? 4 : y;
    x += (y - 1) * 20;
    if (x >= 21 && x <= 40)  // Second line
        x += 44;
    else if (x >= 41 && x <= 60)  // Third line
        x -= 20;
    else if (x >= 61 && x <= 80)  // Forth line
        x += 24;
    x -= 1;
    x |= 0x80;
    display(x, CONF);
    _delay_ms(2);
}

void disp_pos(uint8_t a, uint8_t b, uint32_t num) {
    uint8_t i, zeros, digits[6];
    if (a != 0)
        gotoxy(a, b);
    for (i = 0; i < 5; i++) {  // Conversion from int to str
        digits[4 - i] = num % 10 + '0';
        num = num / 10;
    }
    digits[5] = '\0';  // Null character after the 5 digits to terminate the string
    zeros = 0;
    for (i = 0; i < 4; i++) {  // Counting of zeros before the number
        if (digits[0] != '0') break;
        if (digits[i] == '0') zeros++;
        if (digits[i + 1] != '0') break;
    }
    for (i = 0; i < 5; i++)  // Moving the string to the left
        digits[i] = digits[i + zeros];
    for (i = 0; digits[i] != 0; i++)
        display(digits[i], DATA);
    if (a != 0)
        display(' ', DATA);
}

void disp_float(uint8_t a, uint8_t b, float num, uint8_t prec) {
    int32_t integer_part = 0, floating_part = 0;

    prec = (prec < 0) ? 2 : (prec > 5) ? 5
                                       : prec;

    float_to_int(num, prec, &integer_part, &floating_part);

    gotoxy(a, b);

    disp_pos(0, 0, integer_part);
    display('.', DATA);
    disp_pos(0, 0, floating_part);
}

void disp_num(uint8_t a, uint8_t b, int32_t num) {
    if (num < 0) {
        gotoxy(a, b);
        display('-', DATA);
        disp_pos(0, 0, (num * (-1)));
        display(' ', DATA);
    } else
        disp_pos(a, b, num);
}

uint32_t power(uint32_t base, uint32_t exponent) {
    int result = 1;
    for (; exponent > 0; exponent--)
        result *= base;
    return result;
}

void float_to_int(float number, uint8_t digits, int32_t *integer_part, int32_t *floating_part) {
    *integer_part = (int32_t)number;
    *floating_part = (number - (float)*integer_part) * power(10, digits);
}

void disp_str_num(uint8_t a, uint8_t b, char str[], int32_t num) {
    disp_str(a, b, str);
    uint8_t len = 0;
    for (len = 0; str[len] != '\0'; len++)
        ;
    disp_num(a + len, b, num);
}
