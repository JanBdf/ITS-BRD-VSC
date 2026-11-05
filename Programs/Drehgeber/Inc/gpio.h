#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

void read_phase(char* phase);
void read_button(unsigned int index, bool* value);
void set_led(unsigned int index, bool value);

#endif // ifndef GPIO_H