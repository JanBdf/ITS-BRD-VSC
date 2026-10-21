#include "stack.h"

static const int STACK_SIZE = 10;
static int stack[STACK_SIZE];
static int stack_ptr = 0;

int stack_push(int value) {
    if (stack_ptr >= STACK_SIZE) {
        return STACK_OVERFLOW;
    }

    stack[stack_ptr++] = value;

    return SUCCESS;
}

int stack_pop(int* value) {
    if (stack_ptr <= 0) {
        return STACK_UNDERFLOW;
    }

    *value = stack[--stack_ptr]; 

    return SUCCESS;
}

void stack_clear() {
    stack_ptr = 0;
}

void stack_get_ptr(int* value) {
    *value = stack_ptr - 1;
}

int stack_get(int index, int* value) {
    if (index < 0) {
        return INVALID_STACK_INDEX;
    }

    if (index >= STACK_SIZE) {
        return INVALID_STACK_INDEX;
    }

    *value = stack[index];
    return SUCCESS;
}
