# camera_setup.c
twiStart()
This function sends the start condition for TWI (Two-Wire Interface), which is used for communication with the camera module. It waits for the transmission to complete, and if an error occurs, it calls the error() function with the CAMERA_TWI_ERROR error code.

twiWriteByte(uint8_t DATA, uint8_t type)
This function writes a byte of data to the TWI interface. It takes two arguments - DATA, which is the data byte to be transmitted, and type, which is the expected TWI transmission type (e.g. TW_MT_DATA_ACK for a data transmission with an acknowledgement). It waits for the transmission to complete, and if an error occurs, it calls the error() function with the CAMERA_TWI_ERROR error code and a second argument indicating the error source.

twiAddr(uint8_t addr, uint8_t typeTWI)
This function sends the address of the camera module over the TWI interface. It takes two arguments - addr, which is the camera module address, and typeTWI, which is the expected TWI transmission type for the address (e.g. TW_MT_SLA_ACK for a master transmitter mode with an acknowledgement from the slave). It waits for the transmission to complete, and if an error occurs, it calls the error() function with the CAMERA_TWI_ERROR error code and a second argument indicating the error source.

writeReg(uint8_t reg, uint8_t dat)
This function writes a byte of data to a camera register. It sends a start condition over the TWI interface, followed by the camera module address and the register and data bytes to be written. It then sends a stop condition to end the TWI transmission.

wrSensorRegs8_8(const struct regval_list reglist[])
This function writes a sequence of camera register values to the camera module. It takes an array of regval_list structs as an argument, where each struct contains a register address and a corresponding value. The function uses the writeReg() function to write each value to its corresponding register.

camInit()
This function initializes the camera module and the TWI interface. It sets up the PWM clock output for the camera module, sets the data direction register for the camera module data pins, initializes the serial interface for debugging purposes, and writes default register values to the camera module. It also sets the REG_COM10 register to configure the camera module to output PCLK only during active pixel data.

setResolution()
This function sets the camera module resolution to QVGA (320x240). It enables scaling and sets the register values for QVGA resolution.

setColor()
This function sets the camera module color format to YUV422. It sets the register values for YUV422 color format.

setClocl(uint8_t x)
This function sets the camera module clock rate. It writes the value of x to the REG_CLKRC register, which sets the camera module internal clock frequency.

# capture.c
void StringPgm(const char* str)
This function takes in a const char* pointer to a null-terminated string stored in program memory and transfers the string via USART0. This function is used to transmit a string of characters to the device connected to the USART0.

Parameters
str: A const char* pointer to a null-terminated string stored in program memory.

void captureImg(uint16_t wg, uint16_t hg)
This function captures an image from the OV7670 camera module and transfers it via USART0.

Parameters
wg: A uint16_t value indicating the width of the image to be captured.
hg: A uint16_t value indicating the height of the image to be captured.
Description
This function first sends the string "*RDY*" to the device connected to the USART0 to recognize the beginning of a new image. Then, it waits for the RDY pin to go high, which indicates that the OV7670 module is ready to transfer the image. Once the RDY pin goes high, the function enters a loop to capture the image pixel by pixel.

The image is captured by reading the data pins of the OV7670 module through the PINC register. The function reads the data pins, transmits the pixel data via USART0, and waits for the VSYNC pin to go high and low before reading the next pixel. This process is repeated until the entire image is captured.

After capturing the image, the function waits for 100ms before returning.

# DHT11.c
confirm
int confirm(int us, uint8_t level);
This function checks whether the DHT11 sensor responds as expected. It waits for a specified amount of time, and then checks whether the data pin is at a specified logic level. If the pin is at the expected level, the function returns 0; otherwise, it returns -1.

bits2byte
uint8_t bits2byte(uint8_t data[8]);
This function converts an array of 8 bits into a byte (uint8_t). It reads each bit in the array and sets the corresponding bit in the byte, with the most significant bit first. The function then returns the resulting byte.

sample
int sample(uint8_t data[40]);
This function samples data from the DHT11 sensor. It first initializes the sensor by sending a start signal and waiting for the sensor to respond. Then, it reads 40 bits of data from the sensor. If any errors occur during the data transmission, the function returns an error code; otherwise, it stores the data in an array and returns 0.

parse
int parse(uint8_t data[40], uint8_t* ptemperature, uint8_t* phumidity);
This function parses the data obtained from the sample function. It reads the humidity and temperature values from the data array, calculates a checksum, and compares it to the checksum in the data array. If the checksum matches, the function stores the humidity and temperature values in the specified memory locations (ptemperature and phumidity), and returns 0. If the checksum does not match, the function returns an error code.

read
int read(uint8_t* ptemperature, uint8_t* phumidity, uint8_t pdata[40]);
This function reads data from the DHT11 sensor by calling the sample and parse functions. It first calls sample to sample data from the sensor, and then calls parse to parse the data. If both functions succeed, it stores the resulting data in the specified memory locations (ptemperature, phumidity, and pdata), and returns 0. If either function fails, the function returns an error code.

Note: The SET_OUTPUT_DHT, SET_LOW_DHT, SET_HIGH_DHT, SET_INPUT_DHT, and READ_DHT_PIN macros used in these functions are not defined in the code provided, so their implementation cannot be described in this documentation.

# general.c
board_init(void)
This function initializes the microcontroller by setting up the timer 1 to generate an interrupt every 1ms in order to keep track of time. It also sets up the pins for the relay, IR sensor, and the encoder. The relay pin is set as an output and initially set high (relay is low level trigger). The IR sensor pin is set as an input. The encoder pins for CLK, DT, and SW are set as inputs with pull-up resistors enabled. The function also enables global interrupts.

enable_encoder_interrupt(void)
This function enables the interrupt for the encoder by setting up the pin change interrupt for PCINT16 (CLK), PCINT17 (DT), and PCINT18 (SW). It also sets the encoder_position variable to 0.

disable_encoder_interrupt(void)
This function disables the interrupt for the encoder by clearing the pin change interrupt for PCINT16 (CLK), PCINT17 (DT), and PCINT18 (SW).

ISR(TIMER1_COMPA_vect)
This interrupt service routine (ISR) is called every 1ms by timer 1. It increments the curr_time_ms variable.

ISR(PCINT2_vect)
This ISR is called when there is a pin change interrupt for PCINT16 (CLK), PCINT17 (DT), or PCINT18 (SW). It first checks if enough time has elapsed since the last debounce before updating the encoder_position variable based on the current states of CLK and DT. If the encoder switch is pressed, it sets the encoder_sw_pressed_interrupt variable to true. It then resets the interrupt flag for PCINT2.

# joystick.c
init_joystick()
This function initializes the joystick by setting up the Analog-to-Digital Converter (ADC) for the X and Y axes of the joystick and enabling the interrupt for the central button on the joystick.

read_ADC()
This function reads the value from the ADC for the joystick position.

enum joystick_dir disp_joystick_pos()
This function reads the joystick position by first reading the X and Y values from the ADC, and then comparing the values to certain thresholds to determine the direction of the joystick. It returns an enum type joystick_dir which specifies the direction of the joystick.

bool joystick_new_direction()
This function checks if the joystick has moved in a new direction. It calls disp_joystick_pos() to determine the current direction of the joystick, and compares it to the previous direction of the joystick. If the current direction is different from the previous direction, it returns true.

void enable_central_button_interrupt(void)
This function enables the interrupt for the central button on the joystick.

void disable_central_button_interrupt(void)
This function disables the interrupt for the central button on the joystick.

ISR(PCINT1_vect)
This is the Interrupt Service Routine (ISR) for the pin change interrupt on Port J. It sets a flag to indicate that the central button on the joystick has been pressed.

# LCD_display.c
Function: void start_lcd(void)
This function initializes the LCD display. It configures the data direction registers for the control pins of the display and sends the initialization commands to the display to set the function, display, and entry mode. This function must be called before any other display function.

Function: void display(unsigned char car, unsigned char stat)
This function sends a character to the display. It takes in two arguments:

car: The character to be displayed
stat: The status of the data to be sent to the display (CONF for configuration data or DATA for display data)
Function: void clrscr(void)
This function clears the display by sending the 0x01 command to the display.

Function: void disp_str(unsigned char x, unsigned char y, char str[])
This function displays a string at a specified location on the display. It takes in three arguments:

x: The x-coordinate of the starting position of the string (1-based)
y: The y-coordinate of the starting position of the string (1-based)
str: The string to be displayed
Function: void gotoxy(unsigned char x, unsigned char y)
This function sets the cursor position on the display. It takes in two arguments:

x: The x-coordinate of the cursor position (1-based)
y: The y-coordinate of the cursor position (1-based)
Function: void disp_pos(unsigned char a, unsigned char b, unsigned int num)
This function displays an integer at a specified location on the display. It takes in three arguments:

a: The x-coordinate of the starting position of the integer (1-based). If this argument is 0, the current cursor position will be used.
b: The y-coordinate of the starting position of the integer (1-based)
num: The integer to be displayed
Function: void float_to_int(float number, char digits, int *integer_part, int *floating_part)
This function converts a float number to two integer parts, one representing the integer portion of the number and the other representing the fractional portion of the number. It takes in four arguments:

number: The float number to be converted
digits: The number of decimal places to be used for the fractional part
integer_part: A pointer to an integer where the integer part of the number will be stored
floating_part: A pointer to an integer where the fractional part of the number will be stored
Function: int power(int base, int exponent)
This function returns the result of raising base to the power of exponent. It takes in two arguments:

base: The base of the exponentiation
exponent: The exponent of the exponentiation
Note: This code is intended to be used on an AVR ATmega2560 or equivalent microcontroller.

# menu.c
void init_menu()
This function initializes the menu by setting the current state to MENU, displaying menu items on the LCD display, and highlighting the first item with a ">" symbol.

void menu(enum joystick_dir current)
This function handles user inputs from the joystick and updates the menu accordingly. It takes an enum value current that represents the current direction of the joystick (Up, Down, Left, Right, or Center). Depending on the direction of the joystick, the function updates the selected item on the menu by moving the ">" symbol accordingly. If the center button of the joystick is pressed, the function sets a flag new_state and updates the curr_state variable to represent the selected menu option.

states curr_state
This is a global variable that represents the current state of the system. It can have one of the four values: MENU, SENSORS, CAMERA, SETTINGS, or REBOOT.

bool new_state
This is a global boolean variable that is set to true if the center button of the joystick is pressed and a new state is selected from the menu.

static int selection_x = 1, selection_y = 1;
These are global variables that represent the current position of the ">" symbol on the menu. selection_x and selection_y are used to keep track of the selected item on the menu.

enum joystick_dir current
This is an enumerated type that represents the current direction of the joystick. It can have one of the five values: U (Up), D (Down), L (Left), R (Right), or C (Center).

void disp_str(int x, int y, char *str)
This function displays the string str on the LCD display at the position (x, y). It takes two integer parameters x and y that represent the position of the string on the display and a character pointer *str that represents the string to be displayed.

void clrscr()
This function clears the LCD display. It is used to clear the display before updating it with new information.

void error(error_type error, int data)
This function is not defined in the code provided but it is mentioned in the menu function. It is likely a function that handles errors and displays them on the LCD display. error is an enumerated type that represents the type of error and data is an integer that provides additional information about the error.

# utilities.c
void error_led(void);
This function turns on an error LED and enters an infinite loop. It disables all the peripherals in Power Reduction Registers 0 and 1 and toggles the error LED every 100ms.

void error(error_type err, int additional_info);
This function displays an error message on an LCD display and turns on an error LED. It disables all the peripherals in Power Reduction Registers 0 and 1, displays the error code and additional information on the LCD display, and enters an infinite loop. The error code and additional information are passed as arguments to this function.

Parameters
err: an error_type enumeration that represents the type of error that occurred.

additional_info: an integer that represents additional information about the error.

void reset_board(void);
This function resets the board by enabling the watchdog timer and entering an infinite loop.
