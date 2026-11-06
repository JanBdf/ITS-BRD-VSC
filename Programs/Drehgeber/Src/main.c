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
#include "gpio.h"
#include "init.h"
#include "lcd.h"
#include "phase.h"
#include "timer.h"
#include "main.h"
#include <stdio.h>

#define IDX_LED_ERROR 15
#define IDX_LED_FORWARD 14
#define IDX_LED_BACKWARD 13
#define LCD_FONT_SIZE 16

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
}

static void update_lcd() {
   char buf[20];

   lcdGotoXY(20, 0);
   sprintf(buf, "%.1f", angle_deg);
   lcdPrintS(buf);
   lcdGotoXY(20, 1);
   sprintf(buf, "%.1f", 238.494f);
   lcdPrintS(buf);
}

static void calc_angle() {
   int counter = get_counter();
   angle_deg = (double)counter * 0.3F;
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
