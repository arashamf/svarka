
#ifndef __STANOK_MATH_H__
#define __STANOK_MATH_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ---------------------------------------------------------------------//
#include "main.h"

// Defines ---------------------------------------------------------------------//

// Prototypes ------------------------------------------------------------------//
void calc_period_pulse (turn_data_t * );
uint32_t calc_steps_mode1 (angular_data_t *, float t);
uint32_t calc_passed_angle  (uint32_t , float );
uint32_t steps_for_back_to_zero (uint32_t, float );
// Variables ------------------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

