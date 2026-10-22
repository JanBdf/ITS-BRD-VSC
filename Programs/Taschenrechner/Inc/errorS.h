#ifndef errorS_h
#define errorS_h
typedef enum {
   ERR_OK = 0,
   ERR_STACK_EMPTY,
   ERR_STACK_FULL,
   ERR_DIV_ZERO,
   ERR_INT_OVERFLOW,
   ERR_ADD_OVERFLOW,
   ERR_SUB_OVERFLOW,
   ERR_MUL_OVERFLOW,
   ERR_DIV_OVERFLOW,
   ERR_COUNT,
} T_Error;

const char *errorText(T_Error err);
#endif