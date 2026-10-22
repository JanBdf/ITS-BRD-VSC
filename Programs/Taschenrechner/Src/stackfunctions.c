/**
 ******************************************************************************
 * @file    stackfunctions.c
 * @author  Lasse Boll
 * @brief   stackfunction implementation
 ******************************************************************************
 */
#include "stackfunctions.h"
#include "errorS.h"
#include "stack.h"
#include "terminal.h"
#include "token.h"
#include <limits.h>
#include <stdio.h>

#define BUFFER_SIZE 40

T_Error safeAdd(int a, int b, int *result) {
   if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
      return ERR_ADD_OVERFLOW;
   }
   *result = a + b;
   return ERR_OK;
}

T_Error safeSub(int a, int b, int *result) {
   if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b)) {
      return ERR_SUB_OVERFLOW;
   }
   *result = a - b;
   return ERR_OK;
}

T_Error safeMul(int a, int b, int *result) {
   long long res = (long long)a * b;
   if (res > INT_MAX || res < INT_MIN) {
      return ERR_MUL_OVERFLOW;
   }
   *result = (int)res;
   return ERR_OK;
}

T_Error safeDiv(int a, int b, int *result) {
   if (b == 0) {
      return ERR_DIV_ZERO;
   }
   if (a == INT_MIN && b == -1) {
      return ERR_DIV_OVERFLOW;
   }
   *result = a / b;
   return ERR_OK;
}

T_Error dofunction(T_token token, Terminal *term) {
   int val1 = 0;
   int val2 = 0;
   int result = 0;
   T_Error err = ERR_OK;
   char buffer[BUFFER_SIZE];

   if (IS_OPERATOR(token.tok)) { // +, -, *, /, SWAP
      if (size() < 2) {
         return ERR_STACK_EMPTY;
      }
      pop(&val1);
      pop(&val2);
   }

   switch (token.tok) {
   case PLUS:
      err = safeAdd(val2, val1, &result);
      break;
   case MINUS:
      err = safeSub(val2, val1, &result);
      break;
   case MULT:
      err = safeMul(val2, val1, &result);
      break;
   case DIV:
      err = safeDiv(val2, val1, &result);
      break;
   case PRT:
      if (isEmpty()) {
         return ERR_STACK_EMPTY;
      }
      clearTerm(term);
      snprintf(buffer, sizeof(buffer), "%d", getTop(0));
      printTerm(buffer, term);
      return ERR_OK;
   case PRT_ALL:
      if (size() < 3) {
         return ERR_STACK_EMPTY;
      }
      clearTerm(term);
      snprintf(buffer, sizeof(buffer), "%d\n%d\n%d", getTop(0), getTop(1),
               getTop(2));
      printTerm(buffer, term);
      return ERR_OK;
   case CLEAR:
      clearTerm(term);
      clear();
      return ERR_OK;
   case DOUBLE:
      if (isEmpty()) {
         return ERR_STACK_EMPTY;
      }
      return push(getTop(0));
   case SWAP:
      push(val1);
      return push(val2);
   case ENTER:
      return ERR_OK;
   case NUMBER:
      return push(token.val);
   case OVERFLOW:
      return ERR_INT_OVERFLOW;
   default:
      return ERR_OK;
   }

   if (err != ERR_OK) {
      push(val2);
      push(val1);
      return err;
   }
   return push(result);
}