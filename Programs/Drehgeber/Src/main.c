/**
 ******************************************************************************
 * @file    main.c
 * @author  Franz Korf
 * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "LCD_general.h"
#include "gpio.h"
#include "init.h"
#include "lcd.h"
#include "phase.h"
#include "timer.h"
#include "main.h"
#include <stdio.h>

#define IDX_LED_ERROR 13
#define IDX_LED_FORWARD 14
#define IDX_LED_BACKWARD 15
#define LCD_FONT_SIZE 20
#define DEG_PER_STEP 0.3F
#define LCD_MIN_DELTA (300 * 1000 * TICKS_PER_US)
#define LCD_MAX_DELTA (3000 * 1000 * TICKS_PER_US)
#define LCD_EL_BUF_SIZE 10

typedef struct {
   Coordinate coord;
   char buf[LCD_EL_BUF_SIZE];
   char visible_buf[LCD_EL_BUF_SIZE];
} lcd_element_t;

static lcd_element_t lcd_elements[2];
static int lcd_el_index = 0;
static int lcd_buf_index = 0;
static uint32_t prev_lcd_update;
static double prev_angle_deg;

static int error_state;
static double angle_deg;
static double angle_speed;

static void update_leds() {
   if (error_state) {
      set_led(IDX_LED_ERROR, true);
      return;
   } 

   set_led(IDX_LED_ERROR, false);
   direction_t direction = get_direction();

   set_led(IDX_LED_FORWARD, direction == FORWARD);
   set_led(IDX_LED_BACKWARD, direction == BACKWARD);

   int counter = get_counter();
   display_binary((uint32_t)counter);
}

static void init_lcd() {
   GUI_init(DEFAULT_BRIGHTNESS); // Initialisierung des LCD Boards mit Touch
   TP_Init(false); // Initialisierung des LCD Boards mit Touch

   lcdSetFont(LCD_FONT_SIZE);

   lcdGotoXY(0, 0);
   lcdPrintS("Winkel: ");
   lcdGotoXY(0, 1);
   lcdPrintS("Geschwindigkeit: ");
   
   lcd_element_t el0 = {0};
   Coordinate coord0 = {17, 0};
   el0.coord = coord0;
   lcd_elements[0] = el0;

   lcd_element_t el1 = {0};
   Coordinate coord1 = {17, 1};
   el1.coord = coord1;
   lcd_elements[1] = el1;
}

static void update_lcd() {
   uint32_t new_timestamp = getTimeStamp();
   uint32_t phase_change_time = get_timestamp();
   uint32_t time_delta = new_timestamp - prev_lcd_update;

   if (((phase_change_time > prev_lcd_update) && (time_delta > LCD_MIN_DELTA))
         || (((new_timestamp - phase_change_time) > LCD_MAX_DELTA)) && (time_delta > LCD_MAX_DELTA)) {
      sprintf(lcd_elements[0].buf, "%.2f", angle_deg);

      double delta_ms = time_delta / (TICKS_PER_US * 1e6F);
      angle_speed = (angle_deg - prev_angle_deg) / delta_ms;
      prev_angle_deg = angle_deg;

      sprintf(lcd_elements[1].buf, "%.3f", angle_speed);

      lcd_el_index = 0;
      lcd_buf_index = 0;
      prev_lcd_update = new_timestamp;
   }

   if (lcd_el_index >= 2) {
      return;
   }

   lcd_element_t* el = &lcd_elements[lcd_el_index];
   char curr_char = el->buf[lcd_buf_index];

   if (curr_char == '\0') {
       curr_char = ' ';

       if ((lcd_buf_index + 1) < LCD_EL_BUF_SIZE) {
         el->buf[lcd_buf_index + 1] = '\0';
       }
   }

   if (curr_char != el->visible_buf[lcd_buf_index]) {
      lcdGotoXY(el->coord.x + lcd_buf_index, el->coord.y);
      lcdPrintC(curr_char);
      el->visible_buf[lcd_buf_index] = curr_char;
   }

   lcd_buf_index++;

   if (lcd_buf_index >= LCD_EL_BUF_SIZE) {
      lcd_el_index++;
      lcd_buf_index = 0;
   }
}

static void calc_angle() {
   int counter = get_counter();
   angle_deg = (double)counter * DEG_PER_STEP;
}

int main(void) {
   initITSboard(); // Initialisierung des ITS Boards

   init_lcd();
   initTimer();

   while (1) {
      run_state();

      error_state = get_error_state();
      update_leds();

      if (!error_state) {
         calc_angle();
         update_lcd();
      }

      if (!read_button(6)) {
         reset_state();
      }
   }
}

// EOF
