/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define 	PWM_TIM 							TIM14
#define 	ENC_TIM 							TIM3

#define		TIM_DELAY							TIM16
#define 	TIM_DELAY_APB1_BIT 		LL_APB1_GRP2_PERIPH_TIM16

#define 	TIM_BOUNCE_PEDAL	 							TIM17
#define 	TIM_BOUNCE_PEDAL_APB1_BIT 			LL_APB1_GRP2_PERIPH_TIM17
#define  	TIM_BOUNCE_PEDAL_IRQn           TIM17_IRQn
#define		TIM_BOUNCE_PEDAL_IRQHandler			TIM17_IRQHandler

/* USER CODE END Private defines */

void MX_TIM3_Init(void);
void MX_TIM14_Init(void);

/* USER CODE BEGIN Prototypes */
void drive_PWM_start (uint16_t ) ;
void drive_PWM_mod (uint16_t ) ;
void drive_PWM_stop (void) ;
void delay_us(uint16_t);
void repeat_time (uint16_t );
void TIM_Bounce_DirToogle_init (uint16_t );
void Bounce_DirToogle_Callback (void);
void timers_ini (void);

//Variables -----------------------------------------------------------------------//
extern volatile uint8_t end_bounce_pedal;
extern volatile uint8_t end_bounce_toogle;
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

