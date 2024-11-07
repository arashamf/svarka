
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
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //количество секунд в одном микрошаге(1,8гр/100=6480/100=64,8)
STATUS_FLAG_DRIVE_t status = {0};

//-------------------------------------------------------------------------------------------------------//
void drive_init ( turn_data_t * HandleTurnData ) 
{
	status.currStatusDirToogle=scan_dir_GPIO ();
	status.prevStatusDirToogle = status.currStatusDirToogle;
	SET_DIR(status.prevStatusDirToogle);
	
	HandleTurnData->TurnInMinute = 10;
	calc_period_pulse (HandleTurnData); //расчёт периода импульса
	default_screen (HandleTurnData, &Font_16x26);
}


//-------------------------------------------------------------------------------------------------------//
uint8_t setup_turn (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	if ((read_enc_data (HandleEncData, HandleTurnData)) != 0)		//проверка показаний энкодера
	{	
		default_screen (HandleTurnData, &Font_16x26);	
		calc_period_pulse (HandleTurnData); //расчёт периода импульса
		return ON;
	}
	else
	{	return OFF;	}
}

//-------------------------------------------------------------------------------------------------------//
void turn_drive_start (turn_data_t * HandleTurnData)
{
	drive_PWM_start(HandleTurnData->PulsePeriod); //подача изменнёного ШИМа на двигатель
}

//-------------------------------------------------------------------------------------------------------//
void turn_drive_soft_start (turn_data_t * HandleTurnData)
{
	uint16_t period_PWM = LOWER_PERIOD_SOFT_START;
	drive_PWM_start(period_PWM); //подача изменнёного ШИМа на двигатель
	
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
	drive_PWM_stop(); //остановка генерации ШИМа
}

//-------------------------------------------------------------------------------------------------------//
static uint8_t check_status_pedal (void)
{
	static uint8_t status_pedal = PEDAL_OFF;	
	status_pedal = scan_pedal(); //сканирование педали
	
	if (status_pedal == PEDAL_ON) //если педаль нажата
	{
		if (status.pwm_on == OFF)  //если ШИМ не выдаётся
		{
			return PWM_ON;
		}
	}
	else
	{
		if (status_pedal == PEDAL_OFF) //если педаль не нажата
		{
			if (status.pwm_on == ON)  //если ШИМ выдаётся
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
	if ((scan_direction(&status)) == ON) //если тумблер выбора направления вращения был переключен
	{	
		if (HandleTurnData->TurnInMinute < LOWER_LIMIT_SOFT_START)
		{
			SET_DIR(status.prevStatusDirToogle); //установка направления вращения
		}
		else
		{
			SET_DIR(status.prevStatusDirToogle); //установка направления вращения
			if (status.pwm_on == ON)
			{	turn_drive_soft_start (HandleTurnData);	}  //начало плавной генерации ШИМа
		}
	}	
	
	if (setup_turn (HandleEncData, HandleTurnData) == ON) //если энкодер был повёрнут
	{	drive_PWM_mod (HandleTurnData->PulsePeriod);	} //изменение настроек ШИМа
	
	control_PWM = check_status_pedal (); //проверка состояния педали
	switch (control_PWM)
	{
		case PWM_ON:
			if (HandleTurnData->TurnInMinute < LOWER_LIMIT_SOFT_START)
			{	turn_drive_start (HandleTurnData); }	//начало генерации ШИМа
			else
			{	turn_drive_soft_start  (HandleTurnData); }	//начало плавной генерации ШИМа
			status.pwm_on = ON; //установка флага генерации ШИМа
			break;
		
		case PWM_OFF:
			turn_drive_stop (); //остановка генерации ШИМа
			status.pwm_on = OFF; //сброс флага генерации ШИМа
			break;
		
		case PWM_CONTINUE:
			break;
	}
}

//-------------------------------------------------------------------------------------------------------//

