
// Includes ------------------------------------------------------------------//
#include "drive.h"
#include "gpio.h"
#include "tim.h"
#include "typedef.h"
#include "button.h"
#include "ssd1306.h"
#include "stanok_math.h"
#include "encoder.h"

// Exported types -------------------------------------------------------------//

//Private defines ------------------------------------------------------------//

// Prototypes ----------------------------------------------------------------//
void turn_drive_start (turn_data_t * );
void turn_drive_soft_start (turn_data_t * );
void turn_drive_stop (void);
static uint8_t check_status_pedal (void);

// Variables -----------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //���������� ������ � ����� ���������(1,8��/100=6480/100=64,8)
STATUS_FLAG_DRIVE_t status = {0};

//-------------------------------------------------------------------------------------------------------//
void drive_init ( turn_data_t * HandleTurnData ) 
{
	status.currStatusDirToogle=scan_dir_GPIO ();
	status.prevStatusDirToogle = status.currStatusDirToogle;
	SET_DIR(status.prevStatusDirToogle);
	
	HandleTurnData->TurnInMinute = 10;
	calc_period_pulse (HandleTurnData); //������ ������� ��������
	default_screen (HandleTurnData, &Font_16x26);
}


//-------------------------------------------------------------------------------------------------------//
uint8_t setup_turn (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	if ((read_enc_data (HandleEncData, HandleTurnData)) != 0)		//�������� ��������� ��������
	{	
		default_screen (HandleTurnData, &Font_16x26);	
		calc_period_pulse (HandleTurnData); //������ ������� ��������
		return ON;
	}
	else
	{	return OFF;	}
}

//-------------------------------------------------------------------------------------------------------//
void turn_drive_start (turn_data_t * HandleTurnData)
{
	drive_PWM_start(HandleTurnData->PulsePeriod); //������ ���������� ���� �� ���������
}

//-------------------------------------------------------------------------------------------------------//
void turn_drive_soft_start (turn_data_t * HandleTurnData)
{
	uint16_t period_PWM = LOWER_PERIOD_SOFT_START;
	drive_PWM_start(period_PWM); //������ ���������� ���� �� ���������
	
	while (period_PWM > HandleTurnData->PulsePeriod)
	{
		delay_us (1000);
		period_PWM -= STEP_PERIOD_SOFT_START;
		drive_PWM_mod(period_PWM);
	}
	delay_us (1000);
	drive_PWM_mod((HandleTurnData->PulsePeriod));
}

//-------------------------------------------------------------------------------------------------------//
void turn_drive_stop (void)
{
	drive_PWM_stop(); //��������� ��������� ����
}

//-------------------------------------------------------------------------------------------------------//
static uint8_t check_status_pedal (void)
{
	static uint8_t status_pedal = PEDAL_OFF;	
	status_pedal = scan_pedal(); //������������ ������
	
	if (status_pedal == PEDAL_ON) //���� ������ ������
	{
		if (status.pwm_on == OFF)  //���� ��� �� �������
		{
			return PWM_ON;
		}
	}
	else
	{
		if (status_pedal == PEDAL_OFF) //���� ������ �� ������
		{
			if (status.pwm_on == ON)  //���� ��� �������
			{
				return PWM_OFF;
			}
		}
	}
	return PWM_CONTINUE;
}

//-------------------------------------------------------------------------------------------------------//
void main_loop (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData)
{
	uint8_t control_PWM;
	if ((scan_direction(&status)) == ON) //���� ������� ������ ����������� �������� ��� ����������
	{	
		if (HandleTurnData->TurnInMinute < LOWER_LIMIT_SOFT_START)
		{
			SET_DIR(status.prevStatusDirToogle); //��������� ����������� ��������
		}
		else
		{
			SET_DIR(status.prevStatusDirToogle); //��������� ����������� ��������
			if (status.pwm_on == ON)
			{	turn_drive_soft_start (HandleTurnData);	}  //������ ������� ��������� ����
		}
	}	
	
	if (setup_turn (HandleEncData, HandleTurnData) == ON) //���� ������� ��� �������
	{	drive_PWM_mod (HandleTurnData->PulsePeriod);	} //��������� �������� ����
	
	control_PWM = check_status_pedal (); //�������� ��������� ������
	switch (control_PWM)
	{
		case PWM_ON:
			if (HandleTurnData->TurnInMinute < LOWER_LIMIT_SOFT_START)
			{	turn_drive_start (HandleTurnData); }	//������ ��������� ����
			else
			{	turn_drive_soft_start  (HandleTurnData); }	//������ ������� ��������� ����
			status.pwm_on = ON; //��������� ����� ��������� ����
			break;
		
		case PWM_OFF:
			turn_drive_stop (); //��������� ��������� ����
			status.pwm_on = OFF; //����� ����� ��������� ����
			break;
		
		case PWM_CONTINUE:
			break;
	}
}

//-------------------------------------------------------------------------------------------------------//

