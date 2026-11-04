#include "calc.h"
#include "phase.h"
#include <stdint.h>
static uint32_t last_refresh;
struct Snapshot {
   int counter;
   uint32_t timestamp;
};
static struct Snapshot old;
#define TICKS_PER_MS 90000
#define REFRESHRATE (300*TICKS_PER_MS)
#define DELAY (3000*TICKS_PER_MS)
#define DEGRE_STEP 0.3

bool calc_update(uint32_t current, double *angle, double *angle_speed) {//NOLINT
   // 300ms fenster
   if (current - last_refresh < REFRESHRATE) {
      return false;
   }

   uint32_t counter_ts = get_last_timestamp();

   bool difference = counter_ts != old.timestamp;
   // kein wechsel nach 3sek
   if (!difference && current - last_refresh < DELAY) {
      return false;
   }

   int new_counter = get_counter();
   *angle = new_counter * DEGRE_STEP;

   if (difference) {
      uint32_t ticks = counter_ts - old.timestamp;
      double sec = ticks / (TICKS_PER_MS * 1000.0);//NOLINT
      *angle_speed = (new_counter - old.counter) * DEGRE_STEP / sec;
   }else{
    *angle_speed = 0.0;
   }

   old.timestamp = counter_ts;
   old.counter = new_counter;

   last_refresh = current;

   return true;
}

void calc_init(uint32_t current){
    old.timestamp = current;
    last_refresh = current;
}