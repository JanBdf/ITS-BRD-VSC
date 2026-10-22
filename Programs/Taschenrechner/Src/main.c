/**
 ******************************************************************************
 * @file    main.c
 * @author  Jan Bittendorf
 * @brief   Simple calculator for the ITS-BRD using polish notation
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

#include "main.h"

#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "init.h"
#include "keypad.h"
#include "lcd.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "display.h"
#include "scanner.h"
#include "terminal.h"
#include "token.h"
#include "stack.h"
#include <stdio.h>

static Terminal terminal;
static bool error_state;

#define BUFFER_SIZE 32
#define TESTING true

void print_err(char* message) {
   setTextColorTerm(RED, &terminal);
   printTerm(message, &terminal);
   setTextColorTerm(BLACK, &terminal);
}

void handle_error(char* message) {
   clearTerm(&terminal);
   print_err(message);
   print_err("\n\nPress C to reset");
   error_state = true;
}

void print_num(int num) {
   char buf[BUFFER_SIZE];
   sprintf(buf, "%d", num);
   printTerm(buf, &terminal);
}

void token_unknown() {
   handle_error("Unknown Input");
}

void token_overflow() { // Never sent by scanner
   handle_error("Input overflow");
}

void token_unexpected() {
   handle_error("Unexpected Input");
}

void token_enter() {} // Never sent by scanner

void token_double() {
   int stack_ptr = 0;

   stack_get_ptr(&stack_ptr);

   if (stack_ptr == -1) {
      handle_error("Stack is empty");
      return;
   }

   int stack_val = 0;

   if (stack_get(stack_ptr, &stack_val) == INVALID_STACK_INDEX) {
      handle_error("Invalid Stack Index");
      return;
   }

   if (stack_push(stack_val) == STACK_OVERFLOW) {
      handle_error("Stack Overflow");
      return;
   }
}

void reset() {
   clearTerm(&terminal);
   //setPosTerm(0, 0, &terminal);
   stack_clear();
   error_state = false;
}

void token_clear() {
   reset();
}

void token_prt_all() { // TODO: only print 3 in reverse order
   int stack_ptr = -1;
   
   stack_get_ptr(&stack_ptr);

   if (stack_ptr == -1) {
      handle_error("Stack is empty");
      return;
   }

   int min = (stack_ptr > 2) ? (stack_ptr - 2) : 0;

   clearTerm(&terminal);

   for (int i=min; i<=stack_ptr; i++) {
      int value = 0;

      if (stack_get(i, &value) == INVALID_STACK_INDEX) {
         handle_error("Invalid Stack pointer");
         return;
      }

      print_num(value);
      printTerm("\n", &terminal);
   }
}

void token_prt() {
   int stack_ptr = 0;

   stack_get_ptr(&stack_ptr);

   if (stack_ptr == -1) {
      handle_error("Stack is empty");
      return;
   }

   int stack_val = 0;

   if (stack_get(stack_ptr, &stack_val) == INVALID_STACK_INDEX) {
      handle_error("Stack is empty");
      return;
   }

   clearTerm(&terminal);
   print_num(stack_val);
}

void token_swap() {
   int first = 0;
   int second = 0;

   if ((stack_pop(&first) == STACK_UNDERFLOW) || (stack_pop(&second) == STACK_UNDERFLOW)) {
      handle_error("Stack Underflow");
      return;
   }

   if ((stack_push(second) == STACK_OVERFLOW) || (stack_push(first) == STACK_OVERFLOW)) {
      handle_error("Stack Overflow");
      return;
   }
}

int add(int left, int right, int* result) {
   if (((left > 0) && (right > 0) && ((INT32_MAX - left) < right)) ||
       ((left < 0) && (right < 0) && ((INT32_MIN - left) > right))) {
      return -1;
   }

   *result = left + right;
   return SUCCESS;
}

int sub(int left, int right, int* result) {
   if (((left > 0) && (right < 0) && ((INT32_MAX - left) < -right)) ||
       ((left < 0) && (right > 0) && ((INT32_MIN - left) > -right))) {
      return -1;
   }

   *result = left - right;
   return SUCCESS;
}

int mul(int left, int right, int* result) {
   if (((left > 0) && (right > 0) && ((INT32_MAX / left) < right)) ||
       ((left < 0) && (right < 0) && ((INT32_MAX / left) > right)) ||
       ((left < 0) && (right > 0) && ((INT32_MIN / left) < right)) ||
       ((left > 0) && (right < 0) && ((INT32_MIN / left) > right))) {
      return -1;
   }

   *result = left * right;
   return SUCCESS;
}

int div(int left, int right, int* result) {
   if (right == 0) {
      return -1;
   }

   if ((left == INT32_MIN) && (right == -1)) {
      return -1;
   }

   *result = left / right;
   return SUCCESS;
}

void token_op(int (*func)(int, int, int*)) {
   int left = 0;
   int right = 0;

   if ((stack_pop(&right) == STACK_UNDERFLOW) || (stack_pop(&left) == STACK_UNDERFLOW)) {
      handle_error("Stack Underflow");
      return;
   }

   int result = 0;
   
   if (func(left, right, &result) != SUCCESS) {
      handle_error("Illegal Operation");
      return;
   }

   if (stack_push(result) == STACK_OVERFLOW) {
      handle_error("Stack Overflow");
      return;
   }
}

void token_number(int value) {
   if (stack_push(value) == STACK_OVERFLOW) {
      handle_error("Stack Overflow");
      return;
   }
}

void handle_token(T_token token) {
   int value = token.val;

   if (error_state && (token.tok != CLEAR)) {
      return;
   }

   switch (token.tok) {
      case NUMBER:
         token_number(value);
         break;
      case PLUS:
         token_op(add);
         break;
      case MINUS:
         token_op(sub);
         break;
      case MULT:
         token_op(mul);
         break;
      case DIV:
         token_op(div);
         break;
      case PRT:
         token_prt();
         break;
      case SWAP:
         token_swap();
         break;
      case PRT_ALL:
         token_prt_all();
         break;
      case CLEAR:
         token_clear();
         break;
      case DOUBLE:
         token_double();
         break;
      case ENTER:
         token_enter();
         break;
      case UNEXPECTED:
         token_unexpected();
         break;
      case OVERFLOW:
         token_overflow();
         break;
      default:
         token_unknown();
         break;
   }
}

void run_test() {
   // NOLINTBEGIN(readability-magic-numbers)
   int value;
   T_token token;

   token.tok = 'n';
   token.val = 3;
   handle_token(token);
   stack_get(0, &value);
   if (value != 3) {
      printTerm("Failed stack push test\n", &terminal);
      return;
   }

   token.val = 5;
   handle_token(token);
   token.tok = '*';
   handle_token(token);
   stack_get(0, &value);
   if (value != 15) {
      printTerm("Failed mul test\n", &terminal);
      return;
   }

   token.tok = 'd';
   for (int i=0; i<10; i++) {
      handle_token(token);
   }
   if (error_state != true) {
      printTerm("Failed error test 1\n", &terminal);
      return;
   }

   token.tok = 'C';
   handle_token(token);
   if (error_state != false) {
      printTerm("Failed error test 2\n", &terminal);
      return;
   }

   token.tok = 'n';
   token.val = 0;
   handle_token(token);
   token.val = 1024;
   for (int i=0; i<3; i++) {
      handle_token(token);
   }
   token.val = 0;
   handle_token(token);
   token.val = 2;
   handle_token(token);
   token.tok = '-';
   handle_token(token);
   token.tok = '*';
   for (int i=0; i<3; i++) {
      handle_token(token);
   }
   token.tok = '-';
   handle_token(token);
   if (error_state != false) {
      printTerm("Failed error test 3\n", &terminal);
      return;
   }

   token.tok = 'n';
   token.val = 0;
   handle_token(token);
   token.val = 1024;
   for (int i=0; i<3; i++) {
      handle_token(token);
   }
   token.val = 0;
   handle_token(token);
   token.val = 2;
   handle_token(token);
   token.tok = '-';
   handle_token(token);
   token.tok = '*';
   for (int i=0; i<3; i++) {
      handle_token(token);
   }
   token.tok = 'n';
   token.val = 0;
   handle_token(token);
   token.val = 1;
   handle_token(token);
   token.tok = '-';
   handle_token(token);
   token.tok = '*';
   handle_token(token);
   token.tok = '-';
   handle_token(token);
   if (error_state != true) {
      printTerm("Failed error test 4\n", &terminal);
      return;
   }

   token.tok = 'C';
   handle_token(token);

   token.tok = 'n';
   token.val = 5;
   handle_token(token);
   token.val = 0;
   handle_token(token);
   token.tok = '/';
   handle_token(token);
   if (error_state != true) {
      printTerm("Failed error test 5\n", &terminal);
      return;
   }

   token.tok = 'C';
   handle_token(token);

   token.tok = 'O';
   handle_token(token);
   if (error_state != true) {
      printTerm("Failed error test 6\n", &terminal);
      return;
   }

   token.tok = 'C';
   handle_token(token);

   token.tok = '-';
   handle_token(token);
   if (error_state != true) {
      printTerm("Failed error test 7\n", &terminal);
      return;
   }

   token.tok = 'C';
   handle_token(token);

   token.tok = 'n';
   token.val = 20;
   handle_token(token);
   token.val = 7;
   handle_token(token);
   token.tok = '-';
   handle_token(token);
   stack_get(0, &value);
   if (value != 13) {
      printTerm("Failed sub test\n", &terminal);
      return;
   }

   token.tok = 'n';
   token.val = 20;
   handle_token(token);
   token.val = 7;
   handle_token(token);
   token.tok = '+';
   handle_token(token);
   stack_get(1, &value);
   if (value != 27) {
      printTerm("Failed add test\n", &terminal);
      return;
   }

   token.tok = 'n';
   token.val = 20;
   handle_token(token);
   token.val = 7;
   handle_token(token);
   token.tok = '/';
   handle_token(token);
   stack_get(2, &value);
   if (value != 2) {
      printTerm("Failed div test\n", &terminal);
      return;
   }

   printTerm("All tests passed\n", &terminal);
   // NOLINTEND(readability-magic-numbers)
}

int main(void) {
   initITSboard(); // Initialisierung des ITS Boards
   
   init_display();
   terminal = makeKeyPad();
   error_state = false;

   if (TESTING) {
      run_test();
      printTerm("Press C to continue", &terminal);
      error_state = true;
   }

   while (1) {
      T_token token = next_token();
      handle_token(token);
   }
}

// EOF
