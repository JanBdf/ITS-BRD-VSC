/**
 * @file gpio.h
 * @author Lasse Boll
 * @brief
 * @version 0.1
 * @date 2026-11-04
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef gpio_h
#define gpio_h
#include <stdbool.h>
#include <stdint.h>
/**
 * @brief returns if the bit is set by index
 * for GPIOF
 * @param index
 * @return true
 * @return false
 */
bool read_pin(uint8_t index);
/**
 * @brief returns if the button is pressed calls !readpin
 *
 * @param index
 * @return true
 * @return false
 */
bool read_button(uint8_t index);
/**
 * @brief Set the bit for led by index to the bool
 * for GPIOE
 * @param index
 * @return int
 */
int set_led(uint8_t index, bool onn);
/**
 * @brief Set the int counter in binary to the GPIOD
 *
 * @param count
 */
void set_counter(int count);
/**
 * @brief print "winkel, winkelgeschwindigkeit" fixed to the display
 *
 */
void print_init(void);
/**
 * @brief sets to the string buffer angle and speed for printing only one c
 *
 * @param angle
 * @param angle_speed
 */
void print_all(double angle, double angle_speed);
/**
 * @brief prints one char at the time from the string buffer set in print_all
 *
 */
void print_c(void);
#endif