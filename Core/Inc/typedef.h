
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
	uint16_t 	PulsePeriod; 			//период импульса
	uint32_t 	PulseInSecond;		//количество импульсов в секунду
	uint16_t 	TurnInMinute;			//количество оборотов вала с учётом редуктора
} turn_data_t;

//------------------------------------------------------------------------------//
typedef struct 
{
	int32_t 	prevCounter_SetRotation; 			//сохранённое показание энкодера
	int32_t 	currCounter_SetRotation; 			//текущее показание энкодера
	int32_t 	delta;
} encoder_data_t;

//----------------------------------------------------------------------------------//
typedef union
{
	struct
	{
		uint8_t pwm_on									: 1;
		uint8_t currStatusDirToogle			: 1; //сохранённое показание тумблера выбора направления вращения
		uint8_t prevStatusDirToogle			: 1; //текущее показание тумблера выбора направления вращения
		uint8_t reserve									:	5;
	};
	uint8_t flag;
}STATUS_FLAG_DRIVE_t;

//код нажатой кнопки----------------------------------------------------------------//
typedef enum 
{
	NO_KEY 						= 	0x00,			//кнопка не нажата	
	KEY_PEDAL_SHORT 	= 	0x01,			//короткое нажатие педали
	KEY_PEDAL_LONG 		= 	0x02,			//длинное нажатие педали
	KEY_DIR_SHORT		=			0x03,			//короткое нажатие тумблера выбора направления вращения вала
	KEY_DIR_LONG			=		0x04,			//длинное нажатие тумблера выбора направления вращения вала
} KEY_CODE_t; 						


//-----------------------------------------------------------------------------------//
typedef enum 
{
	STATE_PEDAL_OFF 						= 0	,				//режим - педаль не нажата							
	STATE_PEDAL_BOUNCE							, 			//режим - дребезг педали
	STATE_PEDAL_ON									,				//режим - педаль нажата 
} STATUS_PEDAL_t; 												//состояние сканирования педали

//-----------------------------------------------------------------------------------//
typedef enum 
{
	ST_DIR_SCAN 					= 	0,		 	//стадия - сканирование состояние тумблера			
	ST_DIR_BOUNCE 						,				//стадия - ожидания окончания дребезга							
} STATUS_DIR_t;

//-----------------------------------------------------------------------------------//
typedef enum 
{
	PWM_OFF				=				0,
	PWM_ON								,
	PWM_CONTINUE					,
} DRIVE_INSTR_t;

//-----------------------------------------------------------------------------------//
typedef enum 
{
	DIR_REVERSE				=		0,
	DIR_CONTINUE					,
} DIR_INSTR_t;

// Defines ----------------------------------------------------------------------//
#define 	ON 																1
#define 	OFF 															0
#define 	PEDAL_ON 													1
#define 	PEDAL_OFF 												0
#define 	CLOCKWISE 												1
#define 	COUNTERCLOCKWISE 									0
#define 	DISP_CLEAR 												1
#define 	DISP_NO_CLEAR 										0
#define 	EEPROM_NUMBER_BYTES								2

#define 	CPU_CLOCK								(48000000UL)	// Частота контроллера 
#define 	TIMER_CLOCK_PRESCALER		(CPU_CLOCK/1000000UL)
#define 	TIMER_CLOCK							(CPU_CLOCK/TIMER_CLOCK_PRESCALER)
#define 	TICKS_PER_SECOND				(1000UL) 

#define 	KEY_BOUNCE_TIME 				50 				// время дребезга в мс
#define 	KEY_AUTOREPEAT_TIME 		100 			// время автоповтора в мс
#define 	COUNT_REPEAT_BUTTON 		5

#define 	STEP_TURN_SETUP 				(5UL) 		//шаг настройки количества оборотов в минуту

#define   REDUCER 								(80UL)		//делитель редуктора
#define 	STEPS_IN_REV						(3200UL) 	//количество микрошагов в одном полном обороте (360 гр) с учётом делителя драйвера
#define 	CIRCLE_IN_STEP					(200UL)		//количество шагов (1,8гр) в одном полном обороте (360 гр)
#define 	STEP_DIV 								(STEPS_IN_REV/CIRCLE_IN_STEP)	//количество микрошагов в одном шаге двигателя (1,8гр)
#define 	STEP_TOOL								(STEPS_IN_REV*REDUCER) 				//количество микрошагов в одном полном обороте (360 гр) с учётом делителя драйвера и редуктора 

#define 	LOWER_LIMIT_SOFT_START	(260UL)		//нижний предел количества оборотов, при котором  включается плавный пуск
#define 	LOWER_PERIOD_SOFT_START	(232UL)
#define 	STEP_PERIOD_SOFT_START	(4UL)

#define 	STEP18_IN_SEC						(6480UL) 			//количество секунд в одном шаге двигателя (1,8гр)
#define 	CIRCLE_IN_SEC						(STEP18_IN_SEC*CIRCLE_IN_STEP)	//количество секунд в одном полном обороте двигателя (360 гр)
#define 	SECOND_PER_MINUTE 			(60UL)
#define 	SECOND_PER_DEGREE 			(3600UL)


// Private variables -----------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

