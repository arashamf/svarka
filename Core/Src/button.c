
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
static uint8_t scan_pedal_GPIO (void);

// Private variables --------------------------------------------------------//
uint8_t count_autorepeat = 0; //подсчёт удержания кнопки
__IO uint16_t key_code = 0;

//---------------------------------------------------------------------------//
uint16_t scan_pedal (void)
{
	static __IO uint8_t pedal_state = KEY_STATE_OFF; // переменная со стадией конечного автомата
	static __IO uint16_t status_pedal = OFF; //состояние педали
	
	if(pedal_state == STATE_PEDAL_OFF) //стадия - педаль не нажата
	{
		if(scan_pedal_GPIO() == ON)	//если педаль была нажата 
		{
			pedal_state =  STATE_PEDAL_BOUNCE; //переход в режим нажатия кнопки
			repeat_time (KEY_BOUNCE_TIME); //запуск таймера ожидания окончания дребезга
		}
		status_pedal = OFF;
		return status_pedal;
	}
	
	if(pedal_state == STATE_PEDAL_BOUNCE) //режим окончания дребезга
	{
		if (end_bounce == SET) //если флаг окончания дребезга установлен
		{
			if(scan_pedal_GPIO() == OFF)	 // если кнопка отпущена (нажатие менее 50 мс это дребезг)
			{
				pedal_state = STATE_PEDAL_OFF; //переход в начальное состояние ожидания нажатия педали
			}	
			else //если педаль продолжает удерживаться
			{	
				repeat_time (KEY_AUTOREPEAT_TIME); //установка таймера ожидания отключения кнопки
				pedal_state = STATE_PEDAL_ON;   //переход в режим нажатия педали
			}
		}
		status_pedal = OFF;
		return pedal_state; //возврат статуса педали 
	}
	
	if (pedal_state == STATE_PEDAL_ON) //если активен режим нажатия педали
	{
		if (end_bounce == SET) //если флаг окончания паузы установлен
		{
			if(scan_pedal_GPIO() == OFF)	 // если педаль была отпущена 
			{
				pedal_state = STATE_PEDAL_OFF; //переход в начальное состояние ожидания нажатия педали
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
	uint8_t pin_status = OFF; //статус кнопки - кнопка не нажата
	
	if ((LL_GPIO_IsInputPinSet(PEDAL_GPIO_Port, PEDAL_Pin))	== OFF) //инверсная логика - если кнопка нажата, функция LL_GPIO_IsInputPinSet возвращает 0
	{	pin_status = ON; }//статус кнопки - кнопка нажата
	
	return 	pin_status; 																																		
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
				encoder_reset (HandleEncData);
				setup_enc_data (HandleEncData, HandleTurnData);				
				key_code = NO_KEY;
				break;
		}
	}
}