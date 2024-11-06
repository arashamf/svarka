
// Includes --------------------------------------------------------------------//
#include "eeprom.h"
#include "i2c.h"

// Functions -------------------------------------------------------------------//

// Defines ---------------------------------------------------------------------//
#define EEPROM_I2C1_ADDRESS     	0xA0	 // A0 = A1 = A2 = 0                                        
#define I2C_REQUEST_WRITE       	0x00
#define I2C_REQUEST_READ        	0x01
#define EEPROM_I2C_TIMEOUT    		0xFF

// Private typedef ------------------------------------------------------------//
//extern I2C_HandleTypeDef hi2c1;

// Private variables ---------------------------------------------------------//
uint8_t eeprom_tx_buffer[EEPROM_NUMBER_BYTES+1] = {0} ;

//---------------------------------------------------------------------//
void EEPROM_WriteBytes (uint16_t registr, uint8_t *buf, uint16_t bytes_count)
{
	i2c_write_buffer_16bit_registr (EEPROM_I2C1_ADDRESS, registr, buf, bytes_count);
}

//------------------------------------------------------------------------------------------------//
void EEPROM_ReadBytes (uint16_t registr, uint8_t *buf, uint16_t bytes_count)
{  
		i2c_read_array(EEPROM_I2C1_ADDRESS, registr, buf, bytes_count);
}

//------------------------------------------------------------------------------------------------//
void SaveData_In_EEPROM (turn_data_t * handle, uint8_t * tx_buffer, uint8_t number)
{
	//turns_to_EEPROMbuf (handle, tx_buffer); //cохранение в буфере EEPROM текущих данных угла вала
	EEPROM_WriteBytes (EEPROM_MEMORY_PAGE, tx_buffer, number); //запись 8 байт
}
