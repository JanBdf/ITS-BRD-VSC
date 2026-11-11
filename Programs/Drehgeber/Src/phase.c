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
static void state_error();
static void state_none();

#define F state_forward
#define B state_backward
#define E state_error
#define N state_none

static state_func states[4][4] = {
    //       a      b      c      d
    {N, F, E, B}, // a
    {B, N, F, E}, // b
    {E, B, N, F}, // c
    {F, E, B, N}, // d
};

#undef F
#undef B
#undef E
#undef N

static bool error_state = false;
static direction_t direction = UNKNOWN;
static int counter = 0;
static phase_t last_phase = {0};

static void poll_phase(phase_t* phase) {
    phase->phase = read_phase();
    phase->timestamp = getTimeStamp();
}

static void state_none() {}

static void state_forward() {
    counter++;
    direction = FORWARD;
}

static void state_backward() {
    counter--;
    direction = BACKWARD;
}

static void state_error() {
    error_state = true;
}

void run_state() {
    phase_t new_phase;
    poll_phase(&new_phase);
    state_func func;

    if (last_phase.phase == 0) { // TODO: Improve error handling
        func = state_none;
    } else {
        int old_state_index = last_phase.phase - 'a';
        int new_state_index = new_phase.phase - 'a';

        func = states[old_state_index][new_state_index];
    }

    func();

    last_phase = new_phase;
}

int get_counter() {
    return counter;
}

uint32_t get_timestamp() {
    return last_phase.timestamp;
}

bool get_error_state() {
    return error_state;
}

direction_t get_direction() {
    return direction;
}

void reset_state() {
    error_state = false;
    direction = UNKNOWN;
    last_phase.phase = 0;
}
