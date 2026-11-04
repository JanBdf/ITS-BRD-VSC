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
#include "calc.h"
#include "general.h"
#include "gpio.h"
#include "init.h"
#include "phase.h"
#include "timer.h"
void init_all(struct Phaset current) {
   initTimer();
   phase_init(current);
   calc_init(current.timestamp);
   set_led(D21, false);
   set_led(D22, false);
   set_led(D23, false);
   set_counter(0);
   print_angle_speed(0.0, 0.0);
}
int main(void) {
   initITSboard(); // Initialisierung des ITS Boards

   GUI_init(DEFAULT_BRIGHTNESS); // Initialisierung des LCD Boards mit Touch
   TP_Init(false);               // Initialisierung des LCD Boards mit Touch
   
   print_init();
   init_all(poll_phase());
   while (1) {
      struct Phaset current = poll_phase();
      if (read_button(SE)) {
         init_all(current);
      }
      run_state(current);
   
      double angle;
      double angle_speed;

      if (!has_error()) {
         if (calc_update(current.timestamp, &angle, &angle_speed)) {
            // print(angle,speed);
            print_angle_speed(angle, angle_speed);
         }
         
         // setleds d22-23(get_direction)
         int direction = get_direction();
         set_led(D23, direction == FORWARD);
         set_led(D22, direction == BACKWARD);
         // setcounter binary(get_counter())
         set_counter(get_counter());
      } else {
         set_led(D21, true);
      }
   }
}

// EOF
