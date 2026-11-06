#ifndef PHASE_H
#define PHASE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    FORWARD,
    BACKWARD,
    UNKNOWN,
} direction_t;

typedef struct {
    char phase;
    uint32_t timestamp;
} phase_t;

void run_state();
int get_counter();
int get_timestamp(uint32_t* value);
bool get_error_state();
direction_t get_direction();
void reset_state();

#endif // ifndef PHASE_H