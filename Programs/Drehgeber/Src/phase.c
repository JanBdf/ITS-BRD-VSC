#include "lcd.h"
#include "timer.h"
#include "phase.h"
#include "general.h"
#include "gpio.h"
#include <stdbool.h>
#include <stddef.h>

typedef void (*state_func)(void);

static void poll_phase(phase_t* phase);
static void state_forward();
static void state_backward();
static void state_init();
static void state_error();

#define F state_forward
#define B state_backward
#define I state_init
#define E state_error
#define N NULL

static bool error_state;
static int counter = 0;
static phase_t last_phase;
static state_func states[4][4] = {
    //       a      b      c      d
    {N, F, E, B}, // a
    {B, N, F, E}, // b
    {E, B, N, F}, // c
    {F, E, B, N}, // d
};

static void poll_phase(phase_t* phase) {
    read_phase(&phase->phase);
    phase->timestamp = getTimeStamp();
}

static void state_forward() {
    counter++;
    set_led(16, true);
    set_led(15, false);
}

static void state_backward() {
    counter--;
    set_led(16, false);
    set_led(15, true);
}

static void state_init() {
    set_led(14, false);
    set_led(15, false);
    set_led(16, false);
    error_state = false;
}

static void state_error() {
    set_led(14, true);
    error_state = true;
}

int run_state() {
    phase_t new_phase;
    poll_phase(&new_phase);

    int old_state_index = last_phase.phase - 'a';
    int new_state_index = new_phase.phase - 'a';

    state_func func = states[old_state_index][new_state_index];
    func();

    return SUCCESS;
}

int get_counter(int* value) {
    return SUCCESS;
}

int get_timestamp(int* value) {
    return SUCCESS;
}
