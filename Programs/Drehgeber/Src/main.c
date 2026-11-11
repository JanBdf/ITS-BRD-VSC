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
#define LCD_FONT_SIZE 16
#define DEG_PER_STEP 0.3F
#define LCD_MIN_DELTA_MS 300
#define LCD_MAX_DELTA_MS 3000
#define LCD_EL_BUF_SIZE 20

typedef struct lcd_element_t {
   Coordinate coord;
   char* buf;
   char* visible_buf;
} lcd_element_t;

static lcd_element_t lcd_elements[2];
static int lcd_el_index = 0;
static int lcd_buf_index = 0;
static uint32_t prev_timestamp;

static int error_state;
static double angle_deg;

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

   // Begruessungstext
   lcdSetFont(LCD_FONT_SIZE);
   lcdPrintlnS("Warte auf Drehgeber input...");

   lcdGotoXY(0, 0);
   lcdPrintS("Winkel: ");
   lcdGotoXY(0, 1);
   lcdPrintS("Geschwindigkeit: ");
   
   lcd_element_t el0;
   char buf0[LCD_EL_BUF_SIZE];
   el0.buf = buf0;
   Coordinate coord0;
   coord0.x = 20;
   coord0.y = 0;
   el0.coord = coord0;
   char vis_buf0[LCD_EL_BUF_SIZE];
   el0.visible_buf = vis_buf0;
   lcd_elements[0] = el0;

   lcd_element_t el1;
   char buf1[LCD_EL_BUF_SIZE];
   el1.buf = buf1;
   Coordinate coord1;
   coord1.x = 20;
   coord1.y = 1;
   el1.coord = coord1;
   char vis_buf1[LCD_EL_BUF_SIZE];
   el1.visible_buf = vis_buf1;
   lcd_elements[1] = el1;
}

static void update_lcd() {
   sprintf(lcd_elements[0].buf, "%.1f", angle_deg);
   sprintf(lcd_elements[1].buf, "%.1f", 238.494f); // TODO

   uint32_t new_timestamp = get_timestamp();
   if ((new_timestamp - prev_timestamp) > LCD_MIN_DELTA_MS) {
      lcd_el_index = 0;
      lcd_buf_index = 0;
      prev_timestamp = new_timestamp;
   }

   if (lcd_el_index >= 2) {
      return;
   }

   lcd_element_t* el = &lcd_elements[lcd_el_index];
   if (el->buf[lcd_buf_index] != el->visible_buf[lcd_buf_index]) {
      lcdGotoXY(el->coord.x + lcd_buf_index, el->coord.y);
      lcdPrintC(el->buf[lcd_buf_index]);
      el->visible_buf[lcd_buf_index] = el->buf[lcd_buf_index];
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
         printf("Resetting");
         reset_state();
      }
   }
}

// EOF
