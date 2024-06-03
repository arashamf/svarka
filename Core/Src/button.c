
// Includes ------------------------------------------------------------------//
#include "button.h"
#include "typedef.h"
#include "tim.h"
#include "drive.h"

// Exported types -------------------------------------------------------------//
static struct KEY_MACHINE_t Key_Machine;

//Private defines -----------------------------------------------------------//
#define KEY_BOUNCE_TIME 			50 				// время дребезга в мс
#define KEY_AUTOREPEAT_TIME 	100 			// время автоповтора в мс
#define COUNT_REPEAT_BUTTON 	5

// Prototypes ----------------------------------------------------------------//
static uint8_t scan_buttons_GPIO (void);

// Private variables --------------------------------------------------------//
uint8_t count_autorepeat = 0; //подсчёт удержания кнопки
__IO uint16_t key_code = 0;

//---------------------------------------------------------------------------//
uint16_t scan_keys (void)
{
	static __IO uint8_t key_state = KEY_STATE_OFF; // начальное состояние кнопки - не нажата
	static __IO uint16_t key_code; //код нажатой кнопки
	//static __IO uint16_t key_repeat_time; // счетчик времени повтора
	
	if(key_state == KEY_STATE_OFF) //стадия ожидания нажатия кнопки
	{
		if(LL_GPIO_IsInputPinSet(ENCODER_BTN_GPIO_Port, ENCODER_BTN_Pin) == 1)	//если кнопка была нажата - получение кода нажатой кнопки
		{
			key_state =  KEY_STATE_ON; //переход в режим нажатия кнопки
			key_code = KEY_ENC_SHORT;
		}
	}
	
	if (key_state ==  KEY_STATE_ON)  //режим нажатия кнопки
	{
		repeat_time (KEY_BOUNCE_TIME); //запуск таймера ожидания окончания дребезга
		key_state = KEY_STATE_BOUNCE; // переход в режим окончания дребезга
	}
	
	if(key_state == KEY_STATE_BOUNCE) //режим окончания дребезга
	{
		if (end_bounce == SET) //если флаг окончания дребезга установлен
		{
			end_bounce = RESET;  //сброс флага
			if(scan_buttons_GPIO() == 0)	 // если кнопка отпущена (нажатие менее 50 мс это дребезг)
			{
				key_state = KEY_STATE_OFF; //переход в начальное состояние ожидания нажатия кнопки
				return NO_KEY; //возврат 0 (фильтр дребезга)
			}	
			else //если кнопка продолжает удерживаться
			{	
				repeat_time (KEY_AUTOREPEAT_TIME); //установка таймера ожидания отключения кнопки
				key_state = KEY_STATE_AUTOREPEAT;   //переход в режим автоповтора 
				count_autorepeat = 0;
			}
		}
	}
	
	if (key_state == KEY_STATE_AUTOREPEAT) //если активен режим автоповтора
	{
		if (end_bounce == SET) //если флаг окончания дребезга установлен (устанавливается в прерывании таймера)
		{
			end_bounce = RESET; //сброс флага
			if(scan_buttons_GPIO() == 0)	 // если кнопка была отпущена (короткое нажатие кнопки < 150 мс)
			{
				key_state = KEY_STATE_OFF; //переход в начальное состояние ожидания нажатия кнопки
				return key_code; //возврата номера кнопки
			}
			else //если кнопка продолжает удерживаться
			{			
				if (count_autorepeat < COUNT_REPEAT_BUTTON) //ожидание 500 мс
				{	count_autorepeat++;	}
				else //если кнопка удерживалась более 650 мс
				{	
					switch (key_code) //фиксация длинного нажатия
					{						
						case KEY_ENC_SHORT:
							key_code = KEY_ENC_LONG;	
							break;
						
						default:
							break;	
					}
					key_state = KEY_STATE_WAIT_TURNOFF; //стадия ожидания отпускания кнопки
					repeat_time (KEY_AUTOREPEAT_TIME); //установка таймера ожидания отключения кнопки
					return key_code;
				}
				repeat_time (KEY_AUTOREPEAT_TIME); //установка таймера ожидания отключения кнопки
			} 	
		}					
	}
	
	if (key_state == KEY_STATE_WAIT_TURNOFF) //ожидание отпускания кнопки
	{	
		if (end_bounce == SET) //если флаг окончания дребезга установлен (устанавливается в прерывании таймера)
		{
			key_code = NO_KEY;
			end_bounce = RESET; //сброс флага
			if(scan_buttons_GPIO() == 0)	 // если кнопка была отпущена (короткое нажатие кнопки < 150 мс)
			{
				key_state = KEY_STATE_OFF; //переход в начальное состояние ожидания нажатия кнопки
			}
			else
			{
				repeat_time (KEY_AUTOREPEAT_TIME); //установка таймера ожидания отключения кнопки
			}
			return NO_KEY; //возврата номера кнопки
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
	if ((key_code = scan_keys()) != NO_KEY) //если была нажата кнопка
	{
		switch (key_code) //обработка кода нажатой кнопки
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