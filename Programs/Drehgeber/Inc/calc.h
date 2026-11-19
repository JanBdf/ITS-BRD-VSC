/**
 * @file calc.h
 * @author Lasse Boll
 * @brief 
 * @version 0.1
 * @date 2026-11-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef calc_h
#define calc_h
#include <stdbool.h>
#include <stdint.h>
/**
 * @brief 
 * 
 * @param current 
 * @param angle 
 * @param angle_speed 
 * @return true 
 * @return false 
 */
bool calc_update(uint32_t current, double *angle, double *angle_speed);
/**
 * @brief 
 * 
 * @param current 
 */
void calc_init(uint32_t current);
#endif