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

void print_err(char* message) {
   clearTerm(&terminal);
   setTextColorTerm(RED, &terminal);
   printTerm(message, &terminal);
   setTextColorTerm(BLACK, &terminal);
}

void handle_stack_overflow() {
   print_err("Stack Overflow!\nResetting in 1 sec");
   HAL_Delay(1000);
   token_clear(0);
}

void print_num(int num) {
   char buf[32];
   sprintf(buf, "%d", num);
   printTerm(buf, &terminal);
}

void token_unknown(int value) {
   printTerm("Unknown Input\nReset using C", &terminal);
}

void token_overflow(int value) { // Never sent by scanner
   printTerm("Input overflow", &terminal);
}

void token_unexpected(int value) {
   printTerm("Unexpected Input\nReset using C", &terminal);
}

void token_enter(int value) {} // Never sent by scanner

void token_double(int value) {
   int stack_ptr = 0;

   stack_get_ptr(&stack_ptr);

   int stack_val = 0;

   if (stack_get(stack_ptr, &stack_val) != SUCCESS) {
      print_err("Invalid Stack Index");
      return;
   }

   if (stack_push(stack_val) != SUCCESS) {
      handle_stack_overflow();
      return;
   }
}

void token_clear(int value) {
   clearTerm(&terminal);
   //setPosTerm(0, 0, &terminal);
   stack_clear();
}

void token_prt_all(int value) { // TODO: only print 3 in reverse order
   int stack_ptr = -1;
   
   stack_get_ptr(&stack_ptr);

   for (int i=(stack_ptr - 1); i>=0; i--) {
      int value = 0;

      if (stack_get(i, &value) != SUCCESS) {
         printTerm("I don't even know", &terminal);
         return;
      }

      print_num(value);
      printChar(' ', &terminal);
   }
}

void token_prt(int value) {
   int stack_ptr = 0;

   stack_get_ptr(&stack_ptr);

   int stack_val = 0;

   if (stack_get(stack_ptr - 1, &stack_val) != SUCCESS) {
      printTerm("Stack is empty", &terminal);
      return;
   }

   print_num(stack_val);
}

void token_swap(int value) {
   int a = 0; // NOLINT
   int b = 0; // NOLINT

   if ((stack_pop(&a) != SUCCESS) || (stack_pop(&b) != SUCCESS)) {
      printTerm("Stack Underflow", &terminal);
      return;
   }

   if ((stack_push(b) != SUCCESS) || (stack_push(a) != SUCCESS)) {
      printTerm("Stack Overflow (how?)", &terminal);
      return;
   }
}

int add(int left, int right, int* result) {
   *result = left + right;
   return SUCCESS;
}

int sub(int left, int right, int* result) {
   *result = left - right;
   return SUCCESS;
}

int mul(int left, int right, int* result) {
   *result = left * right;
   return SUCCESS;
}

int div(int left, int right, int* result) {
   *result = left / right;
   return SUCCESS;
}

void token_op(int (*func)(int, int, int*)) {
   int left = 0;
   int right = 0;

   if ((stack_pop(&right) != SUCCESS) || (stack_pop(&left) != SUCCESS)) {
      printTerm("Stack Underflow", &terminal);
      return;
   }

   int result = 0;
   
   if (func(left, right, &result) != SUCCESS) {
      printTerm("Illegal Operation", &terminal);
      return;
   }

   if (stack_push(result) != SUCCESS) {
      printTerm("Stack Overflow (how?)", &terminal);
      return;
   }
}

void token_number(int value) {
   if (stack_push(value) != SUCCESS) {
      printTerm("Stack Overflow", &terminal);
      return;
   }
}

int main(void) {
   initITSboard(); // Initialisierung des ITS Boards

   GUI_init(DEFAULT_BRIGHTNESS); // Initialisierung des LCD Boards mit Touch
   TP_Init(false); // Initialisierung des LCD Boards mit Touch
   
   init_display();
   terminal = makeKeyPad();

   while (1) {
      T_token token = next_token(); 
      int value = token.val;

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
            token_prt(value);
            break;
         case SWAP:
            token_swap(value);
            break;
         case PRT_ALL:
            token_prt_all(value);
            break;
         case CLEAR:
            token_clear(value);
            break;
         case DOUBLE:
            token_double(value);
            break;
         case ENTER:
            token_enter(value);
            break;
         case UNEXPECTED:
            token_unexpected(value);
            break;
         case OVERFLOW:
            token_overflow(value);
            break;
         default:
            token_unknown(value);
            break;
      }
   }
}

// EOF
