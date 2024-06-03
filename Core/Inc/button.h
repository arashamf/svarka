
#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes --------------------------------------------------------------------------//
#include "main.h"

// Exported types -------------------------------------------------------------------//

//Private defines ------------------------------------------------------------------//

// Prototypes ---------------------------------------------------------------------//
uint16_t scan_keys (void);
void switch_mode (encoder_data_t * , turn_data_t * );

#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_H__ */

