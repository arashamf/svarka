
// Includes ------------------------------------------------------------------//
#include "button.h"
#include "typedef.h"
#include "tim.h"
#include "drive.h"

// Exported types -------------------------------------------------------------------------------//

//Private defines -------------------------------------------------------------------------------//

// Prototypes -----------------------------------------------------------------------------------//
static uint8_t scan_pedal_GPIO (void);

// Private variables ----------------------------------------------------------------------------//
uint8_t count_autorepeat = 0; //������� ��������� ������

//-----------------------------------------------------------------------------------------------//
uint16_t scan_pedal (void)
{
	static __IO uint8_t pedal_state = STATE_PEDAL_OFF; // ���������� �� ������� ��������� ��������
	static __IO uint8_t status_pedal = OFF; //��������� ������
	
	if(pedal_state == STATE_PEDAL_OFF) //������ - ������ �� ������
	{
		if(scan_pedal_GPIO() == ON)	//���� ������ ���� ������ 
		{
			pedal_state =  STATE_PEDAL_BOUNCE; //������� � ����� ������� ������
			repeat_time (KEY_BOUNCE_TIME); 		//������ ������� �������� ��������� ��������
		}
		status_pedal = OFF;
		return status_pedal;
	}
	
	if(pedal_state == STATE_PEDAL_BOUNCE) //����� ��������� ��������
	{
		if (end_bounce_pedal == SET) //���� ���� ��������� �������� ����������
		{
			if(scan_pedal_GPIO() == OFF)	 // ���� ������ �������� (������� ����� 50 �� ��� �������)
			{
				pedal_state = STATE_PEDAL_OFF; //������� � ��������� ��������� �������� ������� ������
				status_pedal = OFF;
			}	
			else //���� ������ ���������� ������������
			{	
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
				pedal_state = STATE_PEDAL_ON;   //������� � ����� ������� ������
				status_pedal = ON;
			}
		}
		return status_pedal; //������� ������� ������ 
	}
	
	if (pedal_state == STATE_PEDAL_ON) //���� ������� ����� ������� ������
	{
		if (end_bounce_pedal == SET) //���� ���� ��������� ����� ����������
		{
			if(scan_pedal_GPIO() == OFF)	 // ���� ������ ���� �������� 
			{
				pedal_state = STATE_PEDAL_OFF; //������� � ��������� ��������� �������� ������� ������
				status_pedal = OFF;
			}
			else
			{	status_pedal = ON;	}
			return status_pedal;
		}
	}
	return status_pedal;
}

//-------------------------------------------------------------------------------------------------//
static uint8_t scan_pedal_GPIO (void)
{
	uint8_t pin_status = OFF; //������ ������ - ������ �� ������
	
	if ((LL_GPIO_IsInputPinSet(PEDAL_GPIO_Port, PEDAL_Pin))	== OFF) //��������� ������ - ���� ������ ������, ������� LL_GPIO_IsInputPinSet ���������� 0
	{	pin_status = ON; }//������ ������ - ������ ������
	
	return 	pin_status; 																																		
}

//-------------------------------------------------------------------------------------------------//
uint8_t scan_direction (STATUS_FLAG_DRIVE_t * HandleFlagData)
{
	static __IO uint8_t toggle_state = ST_DIR_SCAN; // ���������� �� ������� ��������� ��������
		
	if(toggle_state == ST_DIR_SCAN) //������ ������������ ��������� ��������
	{
		if(( HandleFlagData->currStatusDirToogle = scan_dir_GPIO()) !=  HandleFlagData->prevStatusDirToogle)	//���� ������� ��������� �������� ����������
		{
			toggle_state =  STATE_PEDAL_BOUNCE; //������� � ����� �������� ��������� ��������
			TIM_Bounce_DirToogle_init (KEY_BOUNCE_TIME); //������ ������� �������� ��������� ��������
		}
	}
	
	if(toggle_state == ST_DIR_BOUNCE) //������ �������� ��������� ��������
	{
		if (end_bounce_toogle == SET) //���� ���� ��������� �������� ����������
		{
			if(( HandleFlagData->currStatusDirToogle = scan_dir_GPIO()) !=  HandleFlagData->prevStatusDirToogle)	//���� ������� ��������� �������� ����������(������� ����� 50 �� ��� �������)
			{
				 HandleFlagData->prevStatusDirToogle =  HandleFlagData->currStatusDirToogle; //���������� �������� ��������� ��������
			}	
			toggle_state = ST_DIR_SCAN;   //������� � ������ ������������ ��������� ��������
			end_bounce_toogle = RESET;
			return ON;
		}
	}
	
	return OFF;
}

//-------------------------------------------------------------------------------------------------//
uint8_t scan_dir_GPIO (void)
{
	uint8_t pin_status = OFF; //������ ������ - ������ �� ������
	
	if ((LL_GPIO_IsInputPinSet(IN_DIR_GPIO_Port, IN_DIR_Pin))	== OFF) //��������� ������ - ���� ������� �����, ������� LL_GPIO_IsInputPinSet ���������� 0
	{	pin_status = ON; }//������ - ������� �����
	
	return 	pin_status; 																																		
}

