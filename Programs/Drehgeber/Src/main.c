/**
 * @file main.c
 * @author Lasse Boll & Jan Bittendorf
 * @brief
 * @version 0.1
 * @date 2026-11-04
 *
 * @copyright Copyright (c) 2026
 *
 */
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
   update_buffer(0.0, 0.0);
}

int main(void) {
   initITSboard(); // Initialisierung des ITS Boards

   GUI_init(DEFAULT_BRIGHTNESS); // Initialisierung des LCD Boards mit Touch
   TP_Init(false);               // Initialisierung des LCD Boards mit Touch

   print_init();
   init_all(poll_phase());

   while (1) {
      set_led(0, 1);
      struct Phaset current = poll_phase();
      if (read_button(SE)) {
         init_all(current);
      }
      run_state(current);

      double angle;
      double angle_speed;
      if (!has_error()) {
         bool calcup = calc_update(current.timestamp, &angle, &angle_speed);
         if (calcup) {
            // update print buffer(angle,speed);
            update_buffer(angle, angle_speed);
         }
         set_led(1, 1);
         print_c();
         set_led(1, 0);
         // setleds d22-23(get_direction)
         int direction = get_direction();
         set_led(D23, direction == FORWARD);
         set_led(D22, direction == BACKWARD);
         // setcounter binary(get_counter())
         set_counter(get_counter());
         set_led(0, 0);
      } else {
         set_led(D21, true);
      }
   }
}

// EOF

// TESTZEITEN: mit drawrectangle maximum bei 360 sind 252/sec und ohne bei 360
// sind 502/sec with release 549/sec auf 360