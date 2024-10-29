
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
	//���������� ���� ���� ���� � ������� ��/���/���/
	uint16_t 	set_degree; 			//����� �������������� ���� ���� � ��������
	int8_t 		set_minute; 			//����� �������������� ���� ���� � �������
	int8_t 		set_second; 			 //����� �������������� ���� ���� � ��������
	//������� ��������� ���� � ������� ��/���/���/
	uint16_t 	shaft_degree; 		//����� �������� ��������� ���� � ��������
	int8_t 		shaft_minute; 		//����� �������� ���������  ���� � �������
	int8_t 		shaft_second; 		//����� �������� ���������  ���� � ��������
	
	uint32_t 	StepAngleInSec;				//������ ������������� ��� ���� � ��������
	uint32_t 	ShaftAngleInSec; 			//������ ������� ��������� ���� � ��������
	uint32_t 	SetShaftAngleInSec; 	//������������� ��������� ���� � ��������
	float mode1_error;
} angular_data_t;

//------------------------------------------------------------------------------//
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

//��� ������� ������----------------------------------------------------------------//
typedef enum 
{
	NO_KEY 						= 	0x00,			//������ �� ������	
	KEY_PEDAL_SHORT 	= 	0x01,			//�������� ������� ������
	KEY_PEDAL_LONG 		= 	0x02,			//������� ������� ������
	KEY_ENC_SHORT		=			0x03,			//�������� ������� ��������
	KEY_ENC_LONG			=		0x04,			//�������� ������� ��������
} KEY_CODE_t; 						

//-----------------------------------------------------------------------------------//
typedef enum 
{
	KEY_STATE_OFF 				= 0	,			//����� - ������ �� ������
	KEY_STATE_ON							,			//����� - ������ ������
	KEY_STATE_BOUNCE					, 		//����� -  ������� ������
	KEY_STATE_AUTOREPEAT			,	 		//����� - ����� �������� (�����������) ������� ������
	KEY_STATE_WAIT_TURNOFF
} KEY_STATE_t; 										//������ ������������ ����������

//-----------------------------------------------------------------------------------//
typedef enum 
{
	PEDAL_OFF 				= 	0x00,			//������ �� ������	
	PEDAL_ON 					= 	0x01,			//������ ������
} STATUS_PEDAL_t; 	

//-----------------------------------------------------------------------------------//
typedef enum 
{
	STATE_PEDAL_OFF 						= 0	,			//����� - ������ �� ������							
	STATE_PEDAL_BOUNCE							, 		//����� - ������� ������
	STATE_PEDAL_ON									,			//����� - ������ ������ 
} PEDAL_STATE_t; 										//��������� ������������ ������

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

#define CPU_CLOCK									(48000000UL)	// ������� ����������� 
#define TIMER_CLOCK_PRESCALER			(CPU_CLOCK/1000000UL)
#define TIMER_CLOCK								(CPU_CLOCK/TIMER_CLOCK_PRESCALER)
#define TICKS_PER_SECOND					(1000UL) 

#define STEP_TURN_SETUP 		(5UL) //��� ��������� ���������� �������� � ������

#define   REDUCER 					(80UL)			//�������� ���������
#define 	STEPS_IN_REV			(3200UL) 	//���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� ��������
#define 	CIRCLE_IN_STEP		(200UL)		 	//���������� ����� (1,8��) � ����� ������ ������� (360 ��)
#define 	STEP_DIV 					(STEPS_IN_REV/CIRCLE_IN_STEP)		//���������� ���������� (16) � ����� ���� ��������� (1,8��)
#define 	STEP_TOOL					(STEPS_IN_REV*REDUCER) 					//���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� �������� � ��������� 

#define 	STEP18_IN_SEC					(6480UL) 			//���������� ������ � ����� ���� ��������� (1,8��)
#define 	CIRCLE_IN_SEC					(STEP18_IN_SEC*CIRCLE_IN_STEP)	//���������� ������ � ����� ������ ������� ��������� (360 ��)
#define 	SECOND_PER_MINUTE 		(60UL)
#define 	SECOND_PER_DEGREE 		(3600UL)

#define 	PWM_TIM 							TIM14
// Private variables -----------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

