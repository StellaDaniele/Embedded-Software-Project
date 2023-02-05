#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void configurePorts() {
}

typedef enum {
    STATE_INIT,
    STATE_GREEN,
    STATE_RED,
    STATE_BLUE,
    NUM_STATES
} State_t;

typedef struct {
    State_t state;
    void (*state_function)(void);
} StateMachine_t;

void fn_INIT(void);
void fn_GREEN(void);
void fn_RED(void);
void fn_BLUE(void);

State_t current_state = STATE_INIT;

StateMachine_t fsm[] = {
    {STATE_INIT, fn_INIT},
    {STATE_GREEN, fn_GREEN},
    {STATE_RED, fn_RED},
    {STATE_BLUE, fn_BLUE}};

void fn_INIT() {
    checkS1();
    current_state = STATE_GREEN;
}

void fn_GREEN() {
    GreenOn();
    checkS2();
    current_state = STATE_RED;
}

void fn_RED() {
    RedOn();
    checkS1();
    current_state = STATE_BLUE;
}

void fn_BLUE() {
    BlueOn();
    checkS2();
    current_state = STATE_GREEN;
}

int main(void) {
    configurePorts();

    while (1) {
        if (current_state < NUM_STATES) {
            (*fsm[current_state].state_function)();
        } else {
            /* serious error */
        }
    }
}
