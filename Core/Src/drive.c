
// Includes ------------------------------------------------------------------//
#include "drive.h"
#include "gpio.h"
#include "tim.h"
#include "typedef.h"
#include "button.h"
#include "ssd1306.h"
#include "eeprom.h"
#include "angle_calc.h"
#include "stanok_math.h"

// Exported types -------------------------------------------------------------//

//Private defines ------------------------------------------------------------//

// Prototypes ----------------------------------------------------------------//
static void rotate_step (uint8_t , uint16_t );
int32_t  read_enc_data (encoder_data_t * , turn_data_t *	);

// Variables -----------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //���������� ������ � ����� ���������(1,8��/100=6480/100=64,8)

//---------------------------------------������� ���� �� ���� ��������---------------------------------------//
static void rotate_step (uint8_t micro_step, uint16_t period)
{
	for (uint8_t count = 0; count < micro_step; count++) //���������� ���������� (���������)
	{
			//STEP(ON);
			delay_us (period/2); 
			//STEP(OFF);
			delay_us (period/2); 
	}
}

//--------------��������� ��������� �������� � ������ ��������� ���������� �������� ���������--------------//
int32_t read_enc_data (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	int32_t currCounter=0; //������� ��������� ��������
	int32_t delta = 0; //������� ����� ������� � ���������� ���������� ��������
	currCounter = LL_TIM_GetCounter(TIM3); //������� ��������� ��������
	HandleEncData->currCounter_SetRotation = (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_SetRotation != HandleEncData->prevCounter_SetRotation) //���� ������� �������� �������� �� ����� �����������
	{
		delta = (HandleEncData->currCounter_SetRotation - HandleEncData->prevCounter_SetRotation); //������� ����� ������� � ���������� ���������� ��������
    HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //���������� �������� ���������� ��������    
    if((delta > -20) && (delta < 20)) // ������ �� �������� ��������� � ������������ �������� (������������ ����� ��������� ����� �����)
		{
			if (delta != 0) //���� ���������� ��������� ��������
			{  
				HandleTurnData->TurnInMinute += delta*STEP_TURN_SETUP;	
				if (HandleTurnData->TurnInMinute < 10)
				{	HandleTurnData->TurnInMinute = 10;	}
				else
				{
					if (HandleTurnData->TurnInMinute > 900)
					{	HandleTurnData->TurnInMinute = 900;	}
				}
			}
		}
		else
		{	delta = 0; }
	}	
	return delta;
}

//--------------------------------------------------------------------------------------------------------//
void setup_enc_data (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	turn_setup_screen (HandleTurnData, HandleEncData, &Font_16x26);
	while(1) 
	{
		if ((scan_keys()) == KEY_ENC_SHORT) //����� �� ������ ��������� ��������
		{
			default_screen (HandleTurnData, &Font_16x26);
			break;
		}
		else
		{
			if ((read_enc_data (HandleEncData, HandleTurnData)) != 0)		//�������� ��������� ��������
			{	
				calc_period_pulse (HandleTurnData); //������ ������� ��������
				drive_PWM_start (HandleTurnData);
				turn_setup_screen (HandleTurnData, HandleEncData, &Font_16x26); 			
			}
			continue;
		}
	}
}

//--------------------------------------------------------------------------------------------------------//
uint8_t setup_turn (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	if ((read_enc_data (HandleEncData, HandleTurnData)) != 0)		//�������� ��������� ��������
	{	
		calc_period_pulse (HandleTurnData); //������ ������� ��������
		//DRIVE_ENABLE(OFF);
		//drive_PWM_start(HandleTurnData); //������ ���������� ����
		//DRIVE_ENABLE(ON);
		default_screen (HandleTurnData, &Font_16x26);	
		return ON;
	}
	else
	{
		return OFF;
	}
}

//--------------------------------------------------------------------------------------------------------//
void turn_drive_start (turn_data_t * HandleTurnData)
{
	drive_PWM_start(HandleTurnData); //������ ���������� ���� �� ���������
	DRIVE_ENABLE(ON); //���������� �� ������
}

//--------------------------------------------------------------------------------------------------------//
void turn_drive_stop (void)
{
	DRIVE_ENABLE(OFF); //������ �� ������ ���������
	drive_PWM_stop(); //��������� ��������� ����
}

//--------------------------------------����� ��������� ��������--------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //���������� ��� �������� ������ ��������
	encCounter = LL_TIM_GetCounter(TIM3); //���������� �������� ��������� ��������
	HandleEncData->currCounter_SetRotation = (32767 - ((encCounter-1) & 0xFFFF))/2; //�������������� ����������� ��������� �������� � ������ �� -10 �� 10
	HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //���������� ���������������� �������� ��������� �������� � ��������� ��������� ���� ��������	
	HandleEncData->delta = 0; //��������� �� �������� ����������
}
