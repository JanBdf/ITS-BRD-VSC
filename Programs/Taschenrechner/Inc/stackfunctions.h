#ifndef stackfunctions_h
#define stackfunctions_h
#include "errorS.h"
#include "terminal.h"
#include "token.h"
T_Error safeAdd(int a, int b, int *result);
T_Error safeSub(int a, int b, int *result);
T_Error safeMul(int a, int b, int *result);
T_Error safeDiv(int a, int b, int *result);
T_Error dofunction(T_token token, Terminal *term);
#endif