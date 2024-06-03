
#ifndef __DRIVE_H__
#define __DRIVE_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ---------------------------------------------------------------------//
#include "main.h"

// Defines ---------------------------------------------------------------------//

// Prototypes ------------------------------------------------------------------//
void setup_enc_data (encoder_data_t * , turn_data_t * );
void encoder_reset (encoder_data_t * ) ;

// Variables ------------------------------------------------------------------//
extern __IO uint32_t need_step;

#ifdef __cplusplus
}
#endif

#endif 
