#include "gpio.h"
#include "general.h"
#include "lcd.h"
#include "stdio.h"
#include "stm32f429xx.h"
#include <stdint.h>

#define COL_VALUE 23

bool read_pin(int index){
    return (GPIOF->IDR >> index) & 1U;
}

bool read_button(int index) {
   return !read_pin(index);
}

int set_led(int index, bool onn) {
   GPIOE->BSRR = 0x01U << (index + (!onn * BSRR_RESET_OFFSET));

   return 0;
}

void print_angle_speed(double angle, double angle_speed) { // NOLINT
   char buffer[20];
   snprintf(buffer, sizeof(buffer), "%10.1f", angle);
   lcdGotoXY(COL_VALUE, 0);
   lcdPrintlnS(buffer);

   snprintf(buffer, sizeof(buffer), "%10.1f", angle_speed);
   lcdGotoXY(COL_VALUE, 1);
   lcdPrintlnS(buffer);
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