
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
void read_enc_data (encoder_data_t * , turn_data_t *	);

// Variables -----------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //количество секунд в одном микрошаге(1,8гр/100=6480/100=64,8)

//---------------------------------------поворот вала на один микрошаг---------------------------------------//
static void rotate_step (uint8_t micro_step, uint16_t period)
{
	for (uint8_t count = 0; count < micro_step; count++) //количество микрошагов (импульсов)
	{
			STEP(ON);
			delay_us (period/2); 
			STEP(OFF);
			delay_us (period/2); 
	}
}

//---------------------------------------поворот вала на один микрошаг---------------------------------------//
void drive_rotate (uint16_t period)
{
//	for (uint8_t count = 0; count < micro_step; count++) //количество микрошагов (импульсов)
	{

	}
}

//--------------обработка показаний энкодера в режиме установки количества оборотов двигателя--------------//
void read_enc_data (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	int32_t currCounter=0;
	int32_t delta = 0;
	currCounter = LL_TIM_GetCounter(TIM3); //текущее показание энкодера
	HandleEncData->currCounter_SetRotation = (32767 - ((currCounter-1) & 0xFFFF))/2; //деление на 2, счёт щелчков (щелчок = 2 импульса)
	
	if(HandleEncData->currCounter_SetRotation != HandleEncData->prevCounter_SetRotation) //если текущее значение энкодера на равно предыдущему
	{
		delta = (HandleEncData->currCounter_SetRotation - HandleEncData->prevCounter_SetRotation); //разница между текущим и предыдущим показанием энкодера
    HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //сохранение текущего показанаия энкодера    
    if((delta > -20) && (delta < 20)) // защита от дребезга контактов и переполнения счетчика (переполнение будет случаться очень редко)
		{
			if (delta != 0) //если изменилось положение энкодера
			{  
				HandleTurnData->TurnInMinute += delta*STEP_TURN_SETUP;	
				if (HandleTurnData->TurnInMinute < 10)
				{	HandleTurnData->TurnInMinute = 900;	}
				else
				{
					if (HandleTurnData->TurnInMinute > 900)
					{	HandleTurnData->TurnInMinute = 10;	}
				}
			}
		}
		else
		{	delta = 0; }
	}				
}

//--------------------------------------------------------------------------------------------------------//
void setup_enc_data (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	
	while(1) 
	{
		if ((scan_keys()) == KEY_ENC_SHORT) //выход из режима установки оборотов
		{
			calc_period_pulse (HandleTurnData); //расчёт периода импульса
			//SaveData_In_EEPROM (HandleTurnData, eeprom_tx_buffer, EEPROM_NUMBER_BYTES);
			default_screen (HandleTurnData, &Font_16x26);
			break;
		}
		else
		{
			read_enc_data (HandleEncData, HandleTurnData); //обработка показаний энкодера
			turn_setup_screen (HandleTurnData, HandleEncData, &Font_16x26);
			LL_TIM_CC_DisableChannel(TIM14, LL_TIM_CHANNEL_CH1); 
			LL_TIM_DisableCounter(TIM14);	//включение таймера 14 для генерации ШИМ*/
			continue;
		}
	}
}

//--------------------------------------сброс показаний энкодера--------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //переменная для хранения данных энкодера
	encCounter = LL_TIM_GetCounter(TIM3); //сохранение текущего показания энкодера
	HandleEncData->currCounter_SetRotation = (32767 - ((encCounter-1) & 0xFFFF))/2; //преобразование полученного показания энкодера в формат от -10 до 10
	HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //сохранение преобразованного текущего показания энкодера в структуру установки шага поворота	
	HandleEncData->delta = 0; //показания от энкодера обнуляются
}
