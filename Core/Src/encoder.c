
// Includes ------------------------------------------------------------------//
#include "encoder.h"
#include "tim.h"
#include "typedef.h"

// Functions --------------------------------------------------------------------------------------//

// Variables --------------------------------------------------------------------------------------//

//--------------------------------------����� ��������� ��������--------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //���������� ��� �������� ������ ��������
	encCounter = LL_TIM_GetCounter(ENC_TIM ); //���������� �������� ��������� ��������
	HandleEncData->currCounter_SetRotation = (32767 - ((encCounter-1) & 0xFFFF))/2; //�������������� ����������� ��������� �������� � ������ �� -10 �� 10
	HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //���������� ���������������� �������� ��������� �������� � ��������� ��������� ���� ��������	
	HandleEncData->delta = 0; //��������� �� �������� ����������
}

//-------------��������� ��������� �������� � ������ ��������� ���������� �������� ���������-------------//
int32_t read_enc_data (encoder_data_t * HandleEncData, turn_data_t * HandleTurnData) 
{
	int32_t currCounter=0; //������� ��������� ��������
	int32_t delta = 0; //������� ����� ������� � ���������� ���������� ��������
	currCounter = LL_TIM_GetCounter(ENC_TIM); //������� ��������� ��������
	HandleEncData->currCounter_SetRotation = (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_SetRotation != HandleEncData->prevCounter_SetRotation) //���� ������� �������� �������� �� ����� �����������
	{
		delta = (HandleEncData->currCounter_SetRotation - HandleEncData->prevCounter_SetRotation); //������� ����� ������� � ���������� ���������� ��������
    HandleEncData->prevCounter_SetRotation = HandleEncData->currCounter_SetRotation; //���������� �������� ���������� ��������    
    if((delta > -10) && (delta < 10)) // ������ �� �������� ��������� � ������������ �������� 
		{
			if (delta != 0) //���� ���������� ��������� ��������
			{  
				HandleTurnData->TurnInMinute += delta*STEP_TURN_SETUP;	
				if (HandleTurnData->TurnInMinute < 10)
				{	HandleTurnData->TurnInMinute = 10;	}
				else
				{
					if (HandleTurnData->TurnInMinute > 900)
					{	HandleTurnData->TurnInMinute = 900;	}
				}
			}
		}
		else
		{	
			//delta = 0; 
			//HandleEncData->currCounter_SetRotation = 0;
			//HandleEncData->prevCounter_SetRotation = 0;			
		}
	}	
	return delta;
}


