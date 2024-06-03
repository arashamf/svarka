
// Includes ---------------------------------------------------------------------------------------//
#include "angle_calc.h"

// Functions --------------------------------------------------------------------------------------//

//--------------------------------���������� ������ � ������ EEPROM--------------------------------//
void turns_to_EEPROMbuf (turn_data_t * handle, uint8_t * EEPROM_buffer)
{	
	*(EEPROM_buffer+0) = (uint8_t)(handle->TurnInMinute >> 8); 
	*(EEPROM_buffer+1) = (uint8_t)(handle->TurnInMinute >> 0); 
}

//-----------------------------------��������� ������  �� ������� EEPROM-----------------------------------//
void turns_from_EEPROMbuf (turn_data_t * handle, uint8_t * EEPROM_buffer)
{
	handle->TurnInMinute = (uint32_t)(((*(EEPROM_buffer+0))<<8) | ((*(EEPROM_buffer+1))<<0)); //������� ���� ����� - 1 ������� �������
}

//------------------------������� ���� ���� ���� ���� �� ������� ��/���/� � �������------------------------//
void SetAngle_in_Seconds (angular_data_t * handle)
{
	handle->StepAngleInSec=0;
	handle->StepAngleInSec += handle->set_second;
	handle->StepAngleInSec += handle->set_minute*SECOND_PER_MINUTE ;
	handle->StepAngleInSec += handle->set_degree*SECOND_PER_DEGREE;
}

//-------------------------������� ���� ���� ���� ���� �� ������ � ������ ��/���/�-------------------------//
void GetSetAngle_from_Seconds (angular_data_t * handle)
{
	uint32_t tmp = 0;
	handle->set_degree = handle->StepAngleInSec/ SECOND_PER_DEGREE; //���������� ��������
	tmp = handle->StepAngleInSec % SECOND_PER_DEGREE; //������� � �������� � ���������
	handle->set_minute = tmp/SECOND_PER_MINUTE;  //���������� �����
	handle->set_second = tmp % SECOND_PER_MINUTE; //���������� ������
}

//-------------------������� ���� �������� ��������� ���� �� ������� ��/���/� � �������-------------------//
void ShaftAngle_in_Seconds (angular_data_t * handle)
{
	handle->ShaftAngleInSec = 0;
	handle->ShaftAngleInSec += handle->shaft_second;
	handle->ShaftAngleInSec += handle->shaft_minute*SECOND_PER_MINUTE;
	handle->ShaftAngleInSec += handle->shaft_degree*SECOND_PER_DEGREE;
}

//-------------------������� ���� �������� ��������� ���� �� ������ � ������ ��/���/�-------------------//
void GetAngleShaft_from_Seconds (angular_data_t * handle)
{
	uint32_t tmp = 0;
	handle->shaft_degree = handle->ShaftAngleInSec/(SECOND_PER_DEGREE); //���������� ��������
	tmp = handle->ShaftAngleInSec % SECOND_PER_DEGREE; //������� � �������� � ���������
	handle->shaft_minute = tmp/SECOND_PER_MINUTE; //���������� �����
	handle->shaft_second = tmp % SECOND_PER_MINUTE; //���������� ������
}

//-------------------������� ���� �������� ��������� ���� �� ������ � ������ ��/���/�-------------------//
void GetSetShaftAngle_from_Seconds (angular_data_t * handle)
{
	uint32_t tmp = 0;
	handle->shaft_degree = handle->SetShaftAngleInSec/(SECOND_PER_DEGREE); //���������� ��������
	tmp = handle->SetShaftAngleInSec % SECOND_PER_DEGREE; //������� � �������� � ���������
	handle->shaft_minute = tmp/SECOND_PER_MINUTE; //���������� �����
	handle->shaft_second = tmp % SECOND_PER_MINUTE; //���������� ������
}

//-------------------------------��������� ���� �������� ��������� ���� -------------------------------//
void AngleShaftReset (angular_data_t * handle)
{
	handle->shaft_second = 0; 
	handle->shaft_minute = 0; 
	handle->shaft_degree = 0;
	handle->ShaftAngleInSec = 0; //������� ��������� ���� - �������
}

//---------------------------����� ���� ���� ���� ���� �� ����������� ��������---------------------------//
void SetAngleReset (angular_data_t * handle)
{
	handle->set_second = 0;
	handle->set_minute = 1; 
	handle->set_degree = 0;
	SetAngle_in_Seconds (handle);
}

//------------------------------------------------------------------------------------------------//
