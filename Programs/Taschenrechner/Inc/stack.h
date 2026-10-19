#ifndef STACK_H
#define STACK_H

#define SUCCESS 0
#define STACK_OVERFLOW (-1)
#define STACK_UNDERFLOW (-2)

int stack_push(int value);
int stack_pop(int* value);
int stack_clear();
int stack_used(int* value);
int stack_get(int index, int* value);

#endif // STACK_H