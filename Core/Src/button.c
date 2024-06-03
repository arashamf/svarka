
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
static uint8_t scan_buttons_GPIO (void);

// Private variables --------------------------------------------------------//
uint8_t count_autorepeat = 0; //������� ��������� ������
__IO uint16_t key_code = 0;

//---------------------------------------------------------------------------//
uint16_t scan_keys (void)
{
	static __IO uint8_t key_state = KEY_STATE_OFF; // ��������� ��������� ������ - �� ������
	static __IO uint16_t key_code; //��� ������� ������
	//static __IO uint16_t key_repeat_time; // ������� ������� �������
	
	if(key_state == KEY_STATE_OFF) //������ �������� ������� ������
	{
		if(LL_GPIO_IsInputPinSet(ENCODER_BTN_GPIO_Port, ENCODER_BTN_Pin) == 1)	//���� ������ ���� ������ - ��������� ���� ������� ������
		{
			key_state =  KEY_STATE_ON; //������� � ����� ������� ������
			key_code = KEY_ENC_SHORT;
		}
	}
	
	if (key_state ==  KEY_STATE_ON)  //����� ������� ������
	{
		repeat_time (KEY_BOUNCE_TIME); //������ ������� �������� ��������� ��������
		key_state = KEY_STATE_BOUNCE; // ������� � ����� ��������� ��������
	}
	
	if(key_state == KEY_STATE_BOUNCE) //����� ��������� ��������
	{
		if (end_bounce == SET) //���� ���� ��������� �������� ����������
		{
			end_bounce = RESET;  //����� �����
			if(scan_buttons_GPIO() == 0)	 // ���� ������ �������� (������� ����� 50 �� ��� �������)
			{
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
				return NO_KEY; //������� 0 (������ ��������)
			}	
			else //���� ������ ���������� ������������
			{	
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
				key_state = KEY_STATE_AUTOREPEAT;   //������� � ����� ����������� 
				count_autorepeat = 0;
			}
		}
	}
	
	if (key_state == KEY_STATE_AUTOREPEAT) //���� ������� ����� �����������
	{
		if (end_bounce == SET) //���� ���� ��������� �������� ���������� (��������������� � ���������� �������)
		{
			end_bounce = RESET; //����� �����
			if(scan_buttons_GPIO() == 0)	 // ���� ������ ���� �������� (�������� ������� ������ < 150 ��)
			{
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
				return key_code; //�������� ������ ������
			}
			else //���� ������ ���������� ������������
			{			
				if (count_autorepeat < COUNT_REPEAT_BUTTON) //�������� 500 ��
				{	count_autorepeat++;	}
				else //���� ������ ������������ ����� 650 ��
				{	
					switch (key_code) //�������� �������� �������
					{						
						case KEY_ENC_SHORT:
							key_code = KEY_ENC_LONG;	
							break;
						
						default:
							break;	
					}
					key_state = KEY_STATE_WAIT_TURNOFF; //������ �������� ���������� ������
					repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
					return key_code;
				}
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
			} 	
		}					
	}
	
	if (key_state == KEY_STATE_WAIT_TURNOFF) //�������� ���������� ������
	{	
		if (end_bounce == SET) //���� ���� ��������� �������� ���������� (��������������� � ���������� �������)
		{
			key_code = NO_KEY;
			end_bounce = RESET; //����� �����
			if(scan_buttons_GPIO() == 0)	 // ���� ������ ���� �������� (�������� ������� ������ < 150 ��)
			{
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
			}
			else
			{
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
			}
			return NO_KEY; //�������� ������ ������
		}
	}
	return NO_KEY;
}

//-------------------------------------------------------------------------------------------------//
static uint8_t scan_buttons_GPIO (void)
{
	return ((LL_GPIO_IsInputPinSet(RIGHT_BTN_GPIO_Port, RIGHT_BTN_Pin)) 		|| 
					(LL_GPIO_IsInputPinSet(CENTER_BTN_GPIO_Port, CENTER_BTN_Pin)) 	|| 
					(LL_GPIO_IsInputPinSet(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin)) 			||
					(LL_GPIO_IsInputPinSet(ENCODER_BTN_GPIO_Port, ENCODER_BTN_Pin)) ||
					(LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin))) 			; 																																		
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
				key_code = NO_KEY;
				break;
		}
	}
}