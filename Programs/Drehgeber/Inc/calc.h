#ifndef calc_h
#define calc_h
#include <stdbool.h>
#include <stdint.h>
bool calc_update(uint32_t current, double *angle, double *angle_speed);
void calc_init(uint32_t current);
#endif