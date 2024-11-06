
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
	uint16_t 	PulsePeriod; 			//������ ��������
	uint32_t 	PulseInSecond;		//���������� ��������� � �������
	uint16_t 	TurnInMinute;			//���������� �������� ���� � ������ ���������
} turn_data_t;

//------------------------------------------------------------------------------//
typedef struct 
{
	int32_t 	prevCounter_SetRotation; 			//���������� ��������� ��������
	int32_t 	currCounter_SetRotation; 			//������� ��������� ��������
	int32_t 	delta;
} encoder_data_t;

//----------------------------------------------------------------------------------//
typedef union
{
	struct
	{
		uint8_t pwm_on									: 1;
		uint8_t currStatusDirToogle			: 1; //���������� ��������� �������� ������ ����������� ��������
		uint8_t prevStatusDirToogle			: 1; //������� ��������� �������� ������ ����������� ��������
		uint8_t reserve									:	5;
	};
	uint8_t flag;
}STATUS_FLAG_DRIVE_t;

//��� ������� ������----------------------------------------------------------------//
typedef enum 
{
	NO_KEY 						= 	0x00,			//������ �� ������	
	KEY_PEDAL_SHORT 	= 	0x01,			//�������� ������� ������
	KEY_PEDAL_LONG 		= 	0x02,			//������� ������� ������
	KEY_DIR_SHORT		=			0x03,			//�������� ������� �������� ������ ����������� �������� ����
	KEY_DIR_LONG			=		0x04,			//������� ������� �������� ������ ����������� �������� ����
} KEY_CODE_t; 						


//-----------------------------------------------------------------------------------//
typedef enum 
{
	STATE_PEDAL_OFF 						= 0	,				//����� - ������ �� ������							
	STATE_PEDAL_BOUNCE							, 			//����� - ������� ������
	STATE_PEDAL_ON									,				//����� - ������ ������ 
} STATUS_PEDAL_t; 												//��������� ������������ ������

//-----------------------------------------------------------------------------------//
typedef enum 
{
	ST_DIR_SCAN 					= 	0,		 	//������ - ������������ ��������� ��������			
	ST_DIR_BOUNCE 						,				//������ - �������� ��������� ��������							
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

#define 	CPU_CLOCK								(48000000UL)	// ������� ����������� 
#define 	TIMER_CLOCK_PRESCALER		(CPU_CLOCK/1000000UL)
#define 	TIMER_CLOCK							(CPU_CLOCK/TIMER_CLOCK_PRESCALER)
#define 	TICKS_PER_SECOND				(1000UL) 

#define 	KEY_BOUNCE_TIME 				50 				// ����� �������� � ��
#define 	KEY_AUTOREPEAT_TIME 		100 			// ����� ����������� � ��
#define 	COUNT_REPEAT_BUTTON 		5

#define 	STEP_TURN_SETUP 				(5UL) 		//��� ��������� ���������� �������� � ������

#define   REDUCER 								(80UL)		//�������� ���������
#define 	STEPS_IN_REV						(3200UL) 	//���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� ��������
#define 	CIRCLE_IN_STEP					(200UL)		//���������� ����� (1,8��) � ����� ������ ������� (360 ��)
#define 	STEP_DIV 								(STEPS_IN_REV/CIRCLE_IN_STEP)	//���������� ���������� � ����� ���� ��������� (1,8��)
#define 	STEP_TOOL								(STEPS_IN_REV*REDUCER) 				//���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� �������� � ��������� 

#define 	LOWER_LIMIT_SOFT_START	(260UL)		//������ ������ ���������� ��������, ��� �������  ���������� ������� ����
#define 	LOWER_PERIOD_SOFT_START	(232UL)
#define 	STEP_PERIOD_SOFT_START	(4UL)

#define 	STEP18_IN_SEC						(6480UL) 			//���������� ������ � ����� ���� ��������� (1,8��)
#define 	CIRCLE_IN_SEC						(STEP18_IN_SEC*CIRCLE_IN_STEP)	//���������� ������ � ����� ������ ������� ��������� (360 ��)
#define 	SECOND_PER_MINUTE 			(60UL)
#define 	SECOND_PER_DEGREE 			(3600UL)


// Private variables -----------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

