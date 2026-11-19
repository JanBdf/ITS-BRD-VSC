/**
 * @file phase.h
 * @author Lasse Boll & Jan Bittendorf
 * @brief fsm for phases, and getter
 * @version 0.1
 * @date 2026-11-04
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef phase_h
#define phase_h
#include <stdbool.h>
#include <stdint.h>

struct Phaset {
   uint8_t phase;
   uint32_t timestamp;
};

typedef enum { FORWARD, BACKWARD, UNKNOWN } direction_t;
/**
 * @brief Get the direction object
 *
 * @return direction_t
 */
direction_t get_direction(void);
/**
 * @brief returns if fsm is in error state
 *
 * @return true
 * @return false
 */
bool has_error(void);
/**
 * @brief polled phase via readpin
 *
 * @return struct Phaset
 */
struct Phaset poll_phase(void);
/**
 * @brief call specific state functions
 *
 * @param current
 * @return int
 */
void run_state(struct Phaset current);
/**
 * @brief Get the counter object
 *
 * @return int
 */
int get_counter(void);
/**
 * @brief Get the last timestamp object
 *
 * @return uint32_t
 */
uint32_t get_last_timestamp(void);
/**
 * @brief init phase struct
 *
 * @param init
 */
void phase_init(struct Phaset init);
#endif