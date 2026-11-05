#include "general.h"
#include "stm32f429xx.h"
#include <assert.h>
#include <stdint.h>
#include "gpio.h"

#define BUTTON_PORT_WIDTH 8
#define LED_PORT_WIDTH 8
#define BSRR_RESET_OFFSET 16

static char phases[] = {'a', 'd', 'b', 'c'};

void read_phase(char* phase) {
    int value = GPIOF->IDR & 3;
    *phase = phases[value];    
}

void read_button(unsigned int index, bool* value) {
    assert(index < BUTTON_PORT_WIDTH);
    *value = (GPIOF->IDR >> index) & 1;
}

void set_led(unsigned int index, bool value) {
    assert(index < (LED_PORT_WIDTH * 2));

    volatile uint32_t* bsrr_ptr;

    if (index >= LED_PORT_WIDTH) {
        bsrr_ptr = &GPIOE->BSRR;
        index -= LED_PORT_WIDTH;
    } else {
        bsrr_ptr = &GPIOD->BSRR;
    }

    if (!value) {
        index += BSRR_RESET_OFFSET;
    }

    *bsrr_ptr = 1 << index;
}
