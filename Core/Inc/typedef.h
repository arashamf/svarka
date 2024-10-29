
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ---------------------------------------------------------------------//
#include "main.h"

// Exported types ---------------------------------------------------------------//
typedef struct 
{
	//сохранение шага хода вала в формате гр/мин/сек/
	uint16_t 	set_degree; 			//часть установленного шага вала в градусах
	int8_t 		set_minute; 			//часть установленного шага вала в минутах
	int8_t 		set_second; 			 //часть установленного шага вала в секундах
	//текущее положение вала в формате гр/мин/сек/
	uint16_t 	shaft_degree; 		//часть текущего положения вала в градусах
	int8_t 		shaft_minute; 		//часть текущего положения  вала в минутах
	int8_t 		shaft_second; 		//часть текущего положения  вала в секундах
	
	uint32_t 	StepAngleInSec;				//полный установленный шаг вала в секундах
	uint32_t 	ShaftAngleInSec; 			//полное текущее положение вала в секундах
	uint32_t 	SetShaftAngleInSec; 	//установленное положение вала в секундах
	float mode1_error;
} angular_data_t;

//------------------------------------------------------------------------------//
typedef struct 
{
	uint16_t 	PulsePeriod; 			//период импульса
	uint32_t 	PulseInSecond;		//количество импульсов в секунду
	uint16_t 	TurnInMinute;			//количество оборотов вала с учётом редуктора
} turn_data_t;

//------------------------------------------------------------------------------//
typedef struct 
{
	int32_t 	prevCounter_SetRotation; 			//сохранённое показание энкодера
	int32_t 	currCounter_SetRotation; 			//текущее показание энкодера
	int32_t delta;
} encoder_data_t;

//----------------------------------------------------------------------------------//
typedef union
{
	struct
	{
		uint8_t tool_mode			: 1;
		uint8_t left_flag			: 1;
		uint8_t right_flag		: 1;
		uint8_t raw						:	5;
	};
	uint8_t flag;
}STATUS_FLAG_t;

//код нажатой кнопки----------------------------------------------------------------//
typedef enum 
{
	NO_KEY 						= 	0x00,			//кнопка не нажата	
	KEY_PEDAL_SHORT 	= 	0x01,			//короткое нажатие педали
	KEY_PEDAL_LONG 		= 	0x02,			//длинное нажатие педали
	KEY_ENC_SHORT		=			0x03,			//короткое нажатие энкодера
	KEY_ENC_LONG			=		0x04,			//короткое нажатие энкодера
} KEY_CODE_t; 						

//-----------------------------------------------------------------------------------//
typedef enum 
{
	KEY_STATE_OFF 				= 0	,			//режим - кнопка не нажата
	KEY_STATE_ON							,			//режим - кнопка нажата
	KEY_STATE_BOUNCE					, 		//режим -  дребезг кнопки
	KEY_STATE_AUTOREPEAT			,	 		//режим - режим ожидания (автоповтора) отжатия кнопки
	KEY_STATE_WAIT_TURNOFF
} KEY_STATE_t; 										//статус сканирования клавиатуры

//-----------------------------------------------------------------------------------//
typedef enum 
{
	PEDAL_OFF 				= 	0x00,			//педаль не нажата	
	PEDAL_ON 					= 	0x01,			//педаль нажата
} STATUS_PEDAL_t; 	

//-----------------------------------------------------------------------------------//
typedef enum 
{
	STATE_PEDAL_OFF 						= 0	,			//режим - педаль не нажата							
	STATE_PEDAL_BOUNCE							, 		//режим - дребезг педали
	STATE_PEDAL_ON									,			//режим - педаль нажата 
} PEDAL_STATE_t; 										//состояние сканирования педали

//-----------------------------------------------------------------------------------//
struct KEY_MACHINE_t
{
	KEY_CODE_t 		key_code;
	KEY_STATE_t 	key_state;
};

// Defines ----------------------------------------------------------------------//
#define ON 												1
#define OFF 											0
#define FORWARD 									1
#define BACKWARD 									0
#define DISP_CLEAR 								1
#define DISP_NOT_CLEAR 						0
#define EEPROM_NUMBER_BYTES 			2

#define CPU_CLOCK									(48000000UL)	// Частота контроллера 
#define TIMER_CLOCK_PRESCALER			(CPU_CLOCK/1000000UL)
#define TIMER_CLOCK								(CPU_CLOCK/TIMER_CLOCK_PRESCALER)
#define TICKS_PER_SECOND					(1000UL) 

#define STEP_TURN_SETUP 		(5UL) //шаг настройки количества оборотов в минуту

#define   REDUCER 					(80UL)			//делитель редуктора
#define 	STEPS_IN_REV			(3200UL) 	//количество микрошагов в одном полном обороте (360 гр) с учётом делителя драйвера
#define 	CIRCLE_IN_STEP		(200UL)		 	//количество шагов (1,8гр) в одном полном обороте (360 гр)
#define 	STEP_DIV 					(STEPS_IN_REV/CIRCLE_IN_STEP)		//количество микрошагов (16) в одном шаге двигателя (1,8гр)
#define 	STEP_TOOL					(STEPS_IN_REV*REDUCER) 					//количество микрошагов в одном полном обороте (360 гр) с учётом делителя драйвера и редуктора 

#define 	STEP18_IN_SEC					(6480UL) 			//количество секунд в одном шаге двигателя (1,8гр)
#define 	CIRCLE_IN_SEC					(STEP18_IN_SEC*CIRCLE_IN_STEP)	//количество секунд в одном полном обороте двигателя (360 гр)
#define 	SECOND_PER_MINUTE 		(60UL)
#define 	SECOND_PER_DEGREE 		(3600UL)

#define 	PWM_TIM 							TIM14
// Private variables -----------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

