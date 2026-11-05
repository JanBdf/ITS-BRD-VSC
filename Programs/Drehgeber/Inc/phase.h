#ifndef PHASE_H
#define PHASE_H

#include <stdint.h>

int run_state();
int get_counter(int* value);
int get_timestamp(int* value);

typedef struct {
    char phase;
    uint32_t timestamp;
} phase_t;

#endif // ifndef PHASE_H