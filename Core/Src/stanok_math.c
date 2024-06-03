
// Includes ----------------------------------------------------------------------------------//
#include "stanok_math.h"
#include "typedef.h"
#include <math.h>

// Exported types ----------------------------------------------------------------------------//

//Private defines ----------------------------------------------------------------------------//

// Functions ---------------------------------------------------------------------------------//

// Variables ---------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------//
void calc_period_pulse (turn_data_t * handle)
{
	uint32_t timer_period_reload = 0;
	handle->PulseInSecond = ((handle->TurnInMinute)*STEPS_IN_REV*REDUCER)/(SECOND_PER_MINUTE*100); //������� �������
	handle->PulsePeriod = 1000000/handle->PulseInSecond;	//������ �������
	LL_TIM_DisableCounter(TIM14);
	LL_TIM_OC_SetCompareCH1(TIM14, handle->PulsePeriod/2);
	LL_TIM_SetAutoReload (TIM14, handle->PulsePeriod);
	LL_TIM_SetCounter(TIM14, 0);
	LL_TIM_CC_EnableChannel(TIM14, LL_TIM_CHANNEL_CH1); 
  LL_TIM_EnableCounter(TIM14);	//��������� ������� 14 ��� ��������� ���*/
}

//--------------------------------------------------------------------------------------------//
uint32_t calc_steps_mode1 (angular_data_t * handle, float step_unit)
{
		uint32_t need_step = 0;
		float buffer = 0;
	
		handle->mode1_error += ((remainderf(handle->StepAngleInSec, step_unit))/step_unit); //������ �������
		buffer = rintf((handle->StepAngleInSec)/step_unit); //���������� �� ������ ����������� �������� ���� float
		need_step = (uint32_t)(REDUCER*buffer);	
		if (handle->mode1_error > 1.0) //���� ���������� ����������� ������ 1
		{	
			handle->mode1_error -= 1.0;	
			need_step += REDUCER;
		}
		else
		{
			if (handle->mode1_error < (-1.0))  //���� ���������� ����������� ������ 1
			{	
				handle->mode1_error += 1.0;	
				need_step -= REDUCER;
			}
		}
		return need_step;
}

//--------------------------------------------------------------------------------------------//
uint32_t steps_for_back_to_zero (uint32_t Second, float step_unit)
{
		uint32_t need_step = 0;

		need_step = (uint32_t)(REDUCER*(Second/step_unit));	
		return need_step;
}

//--------------------------------------------------------------------------------------------//
uint32_t calc_passed_angle  (uint32_t receive_step, float step_unit)
{
		float buffer;
		uint32_t second = 0;
	
		buffer = (uint32_t )(rintf((receive_step/REDUCER) * step_unit));	
		second = (uint32_t)buffer;
		return second;
}

//--------------------------------------------------------------------------------------------//

