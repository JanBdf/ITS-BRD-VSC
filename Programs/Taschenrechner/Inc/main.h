#ifndef MAIN_H
#define MAIN_H

int main(void);
void token_number(int value);
void token_op(int (*func)(int, int, int*));
int div(int left, int right, int* result);
int mul(int left, int right, int* result);
int sub(int left, int right, int* result);
int add(int left, int right, int* result);
void token_swap(int value);
void token_prt(int value);
void token_prt_all(int value);
void token_clear(int value);
void token_double(int value);
void token_enter(int value);
void token_unexpected(int value);
void token_overflow(int value);
void token_unknown(int value);
void handle_stack_overflow();
void print_err(char* message);

#endif // ifndef MAIN_H
