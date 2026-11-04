#ifndef gpio_h
#define gpio_h
#include <stdbool.h>
#include <stdint.h>
bool read_pin(int index);
bool read_button(int index);
int set_led(int index, bool);
void print_angle_speed(double angle, double speed);
void set_counter(int count);
void print_init(void);
#endif