
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
uint8_t count_autorepeat = 0; //подсчёт удержания кнопки

//-----------------------------------------------------------------------------------------------//
uint16_t scan_pedal (void)
{
	static __IO uint8_t pedal_state = STATE_PEDAL_OFF; // переменная со стадией конечного автомата
	static __IO uint8_t status_pedal = OFF; //состояние педали
	
	if(pedal_state == STATE_PEDAL_OFF) //стадия - педаль не нажата
	{
		if(scan_pedal_GPIO() == ON)	//если педаль была нажата 
		{
			pedal_state =  STATE_PEDAL_BOUNCE; //переход в режим нажатия кнопки
			repeat_time (KEY_BOUNCE_TIME); 		//запуск таймера ожидания окончания дребезга
		}
		status_pedal = OFF;
		return status_pedal;
	}
	
	if(pedal_state == STATE_PEDAL_BOUNCE) //режим окончания дребезга
	{
		if (end_bounce_pedal == SET) //если флаг окончания дребезга установлен
		{
			if(scan_pedal_GPIO() == OFF)	 // если кнопка отпущена (нажатие менее 50 мс это дребезг)
			{
				pedal_state = STATE_PEDAL_OFF; //переход в начальное состояние ожидания нажатия педали
				status_pedal = OFF;
			}	
			else //если педаль продолжает удерживаться
			{	
				repeat_time (KEY_AUTOREPEAT_TIME); //установка таймера ожидания отключения кнопки
				pedal_state = STATE_PEDAL_ON;   //переход в режим нажатия педали
				status_pedal = ON;
			}
		}
		return status_pedal; //возврат статуса педали 
	}
	
	if (pedal_state == STATE_PEDAL_ON) //если активен режим нажатия педали
	{
		if (end_bounce_pedal == SET) //если флаг окончания паузы установлен
		{
			if(scan_pedal_GPIO() == OFF)	 // если педаль была отпущена 
			{
				pedal_state = STATE_PEDAL_OFF; //переход в начальное состояние ожидания нажатия педали
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
	uint8_t pin_status = OFF; //статус кнопки - кнопка не нажата
	
	if ((LL_GPIO_IsInputPinSet(PEDAL_GPIO_Port, PEDAL_Pin))	== OFF) //инверсная логика - если кнопка нажата, функция LL_GPIO_IsInputPinSet возвращает 0
	{	pin_status = ON; }//статус кнопки - кнопка нажата
	
	return 	pin_status; 																																		
}

//-------------------------------------------------------------------------------------------------//
uint8_t scan_direction (STATUS_FLAG_DRIVE_t * HandleFlagData)
{
	static __IO uint8_t toggle_state = ST_DIR_SCAN; // переменная со стадией конечного автомата
		
	if(toggle_state == ST_DIR_SCAN) //стадия сканирования состояния тумблера
	{
		if(( HandleFlagData->currStatusDirToogle = scan_dir_GPIO()) !=  HandleFlagData->prevStatusDirToogle)	//если текущее состояние тумблера изменилось
		{
			toggle_state =  STATE_PEDAL_BOUNCE; //переход в режим ожидания окончания дребезга
			TIM_Bounce_DirToogle_init (KEY_BOUNCE_TIME); //запуск таймера ожидания окончания дребезга
		}
	}
	
	if(toggle_state == ST_DIR_BOUNCE) //стадия ожидания окончания дребезга
	{
		if (end_bounce_toogle == SET) //если флаг окончания дребезга установлен
		{
			if(( HandleFlagData->currStatusDirToogle = scan_dir_GPIO()) !=  HandleFlagData->prevStatusDirToogle)	//если текущее состояние тумблера изменилось(нажатие менее 50 мс это дребезг)
			{
				 HandleFlagData->prevStatusDirToogle =  HandleFlagData->currStatusDirToogle; //сохранение текущего показания тумблера
			}	
			toggle_state = ST_DIR_SCAN;   //переход в стадию сканирования состояния тумблера
			end_bounce_toogle = RESET;
			return ON;
		}
	}
	
	return OFF;
}

//-------------------------------------------------------------------------------------------------//
uint8_t scan_dir_GPIO (void)
{
	uint8_t pin_status = OFF; //статус кнопки - кнопка не нажата
	
	if ((LL_GPIO_IsInputPinSet(IN_DIR_GPIO_Port, IN_DIR_Pin))	== OFF) //инверсная логика - если тумблер нажат, функция LL_GPIO_IsInputPinSet возвращает 0
	{	pin_status = ON; }//статус - тумблер нажат
	
	return 	pin_status; 																																		
}

