#ifndef phase_h
#define phase_h
#include <stdbool.h>
#include <stdint.h>

struct Phaset {
   uint8_t phase;
   uint32_t timestamp;
};

typedef enum { FORWARD, BACKWARD, UNKNOWN } direction_t;

direction_t get_direction(void);

bool has_error(void);

struct Phaset poll_phase(void);

extern int run_state(struct Phaset current);

int get_counter(void);

uint32_t get_last_timestamp(void);

void phase_init(struct Phaset init);

#endif