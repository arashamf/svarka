
// Includes ------------------------------------------------------------------//
#include "button.h"
#include "typedef.h"
#include "tim.h"
#include "drive.h"

// Exported types -------------------------------------------------------------//
static struct KEY_MACHINE_t Key_Machine;

//Private defines -----------------------------------------------------------//
#define KEY_BOUNCE_TIME 			50 				// ����� �������� � ��
#define KEY_AUTOREPEAT_TIME 	100 			// ����� ����������� � ��
#define COUNT_REPEAT_BUTTON 	5

// Prototypes ----------------------------------------------------------------//
static uint8_t scan_pedal_GPIO (void);

// Private variables --------------------------------------------------------//
uint8_t count_autorepeat = 0; //������� ��������� ������
__IO uint16_t key_code = 0;

//---------------------------------------------------------------------------//
uint16_t scan_pedal (void)
{
	static __IO uint8_t pedal_state = KEY_STATE_OFF; // ���������� �� ������� ��������� ��������
	static __IO uint16_t status_pedal = OFF; //��������� ������
	
	if(pedal_state == STATE_PEDAL_OFF) //������ - ������ �� ������
	{
		if(scan_pedal_GPIO() == ON)	//���� ������ ���� ������ 
		{
			pedal_state =  STATE_PEDAL_BOUNCE; //������� � ����� ������� ������
			repeat_time (KEY_BOUNCE_TIME); //������ ������� �������� ��������� ��������
		}
		status_pedal = OFF;
		return status_pedal;
	}
	
	if(pedal_state == STATE_PEDAL_BOUNCE) //����� ��������� ��������
	{
		if (end_bounce == SET) //���� ���� ��������� �������� ����������
		{
			if(scan_pedal_GPIO() == OFF)	 // ���� ������ �������� (������� ����� 50 �� ��� �������)
			{
				pedal_state = STATE_PEDAL_OFF; //������� � ��������� ��������� �������� ������� ������
			}	
			else //���� ������ ���������� ������������
			{	
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
				pedal_state = STATE_PEDAL_ON;   //������� � ����� ������� ������
			}
		}
		status_pedal = OFF;
		return pedal_state; //������� ������� ������ 
	}
	
	if (pedal_state == STATE_PEDAL_ON) //���� ������� ����� ������� ������
	{
		if (end_bounce == SET) //���� ���� ��������� ����� ����������
		{
			if(scan_pedal_GPIO() == OFF)	 // ���� ������ ���� �������� 
			{
				pedal_state = STATE_PEDAL_OFF; //������� � ��������� ��������� �������� ������� ������
				status_pedal = OFF;
			}
			else
			{
				status_pedal = ON;
			}
			return pedal_state;
		}
	}
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
void switch_mode (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData)
{
	if ((key_code = scan_keys()) != NO_KEY) //���� ���� ������ ������
	{
		switch (key_code) //��������� ���� ������� ������
		{		
			case KEY_ENC_SHORT: 
				encoder_reset (HandleEncData);
				setup_enc_data (HandleEncData, HandleTurnData);
				key_code = NO_KEY;
				break;
			
			case KEY_ENC_LONG: 
				encoder_reset (HandleEncData);
				setup_enc_data (HandleEncData, HandleTurnData);				
				key_code = NO_KEY;
				break;
		}
	}
}