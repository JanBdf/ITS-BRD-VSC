#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

char read_phase();
bool read_button(unsigned int index);
void set_led(unsigned int index, bool value);
void display_binary(unsigned int value);

#endif // ifndef GPIO_H