/**
 ******************************************************************************
 * @file    stack.c
 * @author  Lasse Boll
 * @brief   stack implementation
 ******************************************************************************
 */
#include "stack.h"
#include "errorS.h"
#include <stdbool.h>
#define STACK_SIZE 10

static int stack[STACK_SIZE];
static int current = -1;

T_Error push(int value) {
   if (current >= STACK_SIZE - 1) {
      return ERR_STACK_FULL;
   }
   stack[++current] = value;
   return ERR_OK;
}

T_Error pop(int *value) {
   if (current < 0) {
      return ERR_STACK_EMPTY;
   }
   *value = stack[current--];
   return ERR_OK;
}

bool isEmpty() { return current == -1; }

int freeSpace() { return STACK_SIZE - 1 - current; }

int getTop(int decr) { return stack[current - decr]; }

int size() { return current + 1; }
void clear(void) { current = -1; }