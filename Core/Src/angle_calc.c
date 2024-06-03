
// Includes ---------------------------------------------------------------------------------------//
#include "angle_calc.h"

// Functions --------------------------------------------------------------------------------------//

//--------------------------------сохранение данных в буффер EEPROM--------------------------------//
void turns_to_EEPROMbuf (turn_data_t * handle, uint8_t * EEPROM_buffer)
{	
	*(EEPROM_buffer+0) = (uint8_t)(handle->TurnInMinute >> 8); 
	*(EEPROM_buffer+1) = (uint8_t)(handle->TurnInMinute >> 0); 
}

//-----------------------------------получение данных  из буффера EEPROM-----------------------------------//
void turns_from_EEPROMbuf (turn_data_t * handle, uint8_t * EEPROM_buffer)
{
	handle->TurnInMinute = (uint32_t)(((*(EEPROM_buffer+0))<<8) | ((*(EEPROM_buffer+1))<<0)); //старший байт числа - 1 элемент массива
}

//------------------------перевод угла шага хода вала из формата гр/мин/с в секунды------------------------//
void SetAngle_in_Seconds (angular_data_t * handle)
{
	handle->StepAngleInSec=0;
	handle->StepAngleInSec += handle->set_second;
	handle->StepAngleInSec += handle->set_minute*SECOND_PER_MINUTE ;
	handle->StepAngleInSec += handle->set_degree*SECOND_PER_DEGREE;
}

//-------------------------перевод угла шага хода вала из секунд в формат гр/мин/с-------------------------//
void GetSetAngle_from_Seconds (angular_data_t * handle)
{
	uint32_t tmp = 0;
	handle->set_degree = handle->StepAngleInSec/ SECOND_PER_DEGREE; //количество градусов
	tmp = handle->StepAngleInSec % SECOND_PER_DEGREE; //остаток с минутами и секундами
	handle->set_minute = tmp/SECOND_PER_MINUTE;  //количество минут
	handle->set_second = tmp % SECOND_PER_MINUTE; //количество секунд
}

//-------------------перевод угла текущего положения вала из формата гр/мин/с в секунды-------------------//
void ShaftAngle_in_Seconds (angular_data_t * handle)
{
	handle->ShaftAngleInSec = 0;
	handle->ShaftAngleInSec += handle->shaft_second;
	handle->ShaftAngleInSec += handle->shaft_minute*SECOND_PER_MINUTE;
	handle->ShaftAngleInSec += handle->shaft_degree*SECOND_PER_DEGREE;
}

//-------------------перевод угла текущего положения вала из секунд в формат гр/мин/с-------------------//
void GetAngleShaft_from_Seconds (angular_data_t * handle)
{
	uint32_t tmp = 0;
	handle->shaft_degree = handle->ShaftAngleInSec/(SECOND_PER_DEGREE); //количество градусов
	tmp = handle->ShaftAngleInSec % SECOND_PER_DEGREE; //остаток с минутами и секундами
	handle->shaft_minute = tmp/SECOND_PER_MINUTE; //количество минут
	handle->shaft_second = tmp % SECOND_PER_MINUTE; //количество секунд
}

//-------------------перевод угла текущего положения вала из секунд в формат гр/мин/с-------------------//
void GetSetShaftAngle_from_Seconds (angular_data_t * handle)
{
	uint32_t tmp = 0;
	handle->shaft_degree = handle->SetShaftAngleInSec/(SECOND_PER_DEGREE); //количество градусов
	tmp = handle->SetShaftAngleInSec % SECOND_PER_DEGREE; //остаток с минутами и секундами
	handle->shaft_minute = tmp/SECOND_PER_MINUTE; //количество минут
	handle->shaft_second = tmp % SECOND_PER_MINUTE; //количество секунд
}

//-------------------------------обнуление угла текущего положения вала -------------------------------//
void AngleShaftReset (angular_data_t * handle)
{
	handle->shaft_second = 0; 
	handle->shaft_minute = 0; 
	handle->shaft_degree = 0;
	handle->ShaftAngleInSec = 0; //текущее положение вала - нулевое
}

//---------------------------сброс угла шага хода вала на минимальное значение---------------------------//
void SetAngleReset (angular_data_t * handle)
{
	handle->set_second = 0;
	handle->set_minute = 1; 
	handle->set_degree = 0;
	SetAngle_in_Seconds (handle);
}

//------------------------------------------------------------------------------------------------//
