/**
 ******************************************************************************
 * @file    errorS.c
 * @author  Lasse Boll
 * @brief   errors
 ******************************************************************************
 */
#include "errorS.h"
static const char *texts[ERR_COUNT] = {
    [ERR_OK] = "OK",
    [ERR_STACK_EMPTY] = "Stack leer",
    [ERR_STACK_FULL] = "Stack voll",
    [ERR_DIV_ZERO] = "Div durch 0",
    [ERR_INT_OVERFLOW] = "Int overflow",
    [ERR_ADD_OVERFLOW] = "overflow bei add",
    [ERR_SUB_OVERFLOW] = "overflow bei sub",
    [ERR_MUL_OVERFLOW] = "overflow bei mul",
    [ERR_DIV_OVERFLOW] = "overflow bei div",
};

const char *errorText(T_Error err) {
   if (err < 0 || err >= ERR_COUNT) {
      return "Unbekannter Fehler";
   }
   return texts[err];
}