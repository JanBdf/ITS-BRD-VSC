/**
 * @file gpio.c
 * @author Lasse Boll
 * @brief 
 * @version 0.1
 * @date 2026-11-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "gpio.h"
#include "general.h"
#include "lcd.h"
#include "stdio.h"
#include "stm32f429xx.h"
#include <stdint.h>

#define COL_VALUE 25
#define VALUE_WIDTH 10
#define ROWS 2

static char buf[ROWS][VALUE_WIDTH + 1];
static uint8_t buf_pos = ROWS * VALUE_WIDTH;

bool read_pin(int index) { return (GPIOF->IDR >> index) & 1U; }

bool read_button(int index) { return !read_pin(index); }

int set_led(int index, bool onn) {
   GPIOE->BSRR = 0x01U << (index + (!onn * BSRR_RESET_OFFSET));

   return 0;
}

void set_counter(int count) {
   uint32_t val = (uint8_t)count;
   GPIOD->BSRR = val | ((~val & 0xFFU) << BSRR_RESET_OFFSET);
}

void print_init(void) {
   lcdGotoXY(0, 0);
   lcdPrintlnS("Winkel:");

   lcdGotoXY(0, 1);
   lcdPrintlnS("Winkelgeschwindigkeit:");
}

void print_all(double angle, double angle_speed) { // NOLINT
   snprintf(buf[0], sizeof buf[0], "%*.1f", VALUE_WIDTH, angle);
   snprintf(buf[1], sizeof buf[1], "%*.1f", VALUE_WIDTH, angle_speed);
   buf_pos = 0;
}

void print_c(void) {
   if (buf_pos >= ROWS * VALUE_WIDTH) {
      return;
   }
   uint8_t field = buf_pos / VALUE_WIDTH;
   uint8_t column = buf_pos % VALUE_WIDTH;

   lcdGotoXY(COL_VALUE + column, field);
   lcdPrintC(buf[field][column]);
   buf_pos++;
}
