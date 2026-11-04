#include "phase.h"
#include "general.h"
#include "gpio.h"
#include "timer.h"
#include <stddef.h>
#include <stdint.h>

static int counter;

static direction_t direction = UNKNOWN;
static bool error = false;

static struct Phaset last_phase;

#define f state_forward
#define b state_backward
#define e state_error

typedef void (*state_functions)(void);

static void state_forward(void);
static void state_backward(void);
static void state_error(void);

static const state_functions STATES_ARR[4][4] = {
    {NULL, b, f, e}, // a
    {f, NULL, e, b}, // d
    {b, e, NULL, f}, // b
    {e, f, b, NULL}, // c

};

#undef f
#undef b
#undef e

struct Phaset poll_phase(void) {
   struct Phaset cphase;
   cphase.phase = (read_pin(SY) << 1) | read_pin(SX);
   cphase.timestamp = getTimeStamp();

   return cphase;
}

static void state_forward(void) {
   counter++;
   direction = FORWARD;
}

static void state_backward(void) {
   counter--;
   direction = BACKWARD;
}

static void state_error(void) { error = true; }

int run_state(struct Phaset current) {
   state_functions sfs = STATES_ARR[last_phase.phase][current.phase];
   if (sfs) {
      sfs();
      last_phase = current;
   }
   return 0;
}

int get_counter(void) { return counter; }

uint32_t get_last_timestamp(void) { return last_phase.timestamp; }

void phase_init(struct Phaset init) {
   last_phase = init;
   counter = 0;
   error =  false;
   direction = UNKNOWN;
}

direction_t get_direction(void) { return direction; }

bool has_error(void) { return error; }
