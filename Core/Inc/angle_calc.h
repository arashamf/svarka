
#ifndef __ANGLE_CALC_H__
#define __ANGLE_CALC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "typedef.h" 

/* Defines ------------------------------------------------------------------*/

/* Prototypes ------------------------------------------------------------------*/
void turns_to_EEPROMbuf (turn_data_t * , uint8_t *);
void turns_from_EEPROMbuf (turn_data_t * , uint8_t * );
void SetAngle_in_Seconds (angular_data_t *);
void GetSetAngle_from_Seconds (angular_data_t * );
void ShaftAngle_in_Seconds (angular_data_t * );
void GetAngleShaft_from_Seconds (angular_data_t * );
void GetSetShaftAngle_from_Seconds (angular_data_t * );
void AngleShaftReset (angular_data_t * );
void SetAngleReset (angular_data_t * );
#ifdef __cplusplus
}
#endif

#endif 

