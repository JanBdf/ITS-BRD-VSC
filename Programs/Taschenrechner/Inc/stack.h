#ifndef stack_h
#define stack_h
#include "errorS.h"
#include <stdbool.h>
T_Error push(int value);
T_Error pop(int *value);
bool isEmpty(void);
int freeSpace(void);
int getTop(int decr);
int size(void);
void clear(void);
#endif