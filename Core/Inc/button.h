
#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes --------------------------------------------------------------------------//
#include "main.h"
#include "typedef.h"

// Exported types -------------------------------------------------------------------//

//Private defines ------------------------------------------------------------------//

// Prototypes ---------------------------------------------------------------------//
uint16_t scan_pedal (void);
uint8_t scan_direction (STATUS_FLAG_DRIVE_t * );
uint8_t scan_dir_GPIO (void);
#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_H__ */

