#include "aht20.h"
#include <math.h>

I2C_HandleTypeDef *hi2c_cb;

void AHT20_init(I2C_HandleTypeDef *hi2c)
{
	hi2c_cb = hi2c;

	HAL_Delay(40);
	uint8_t status;
	HAL_I2C_Mem_Read(hi2c_cb, AHT20_address, AHT20_state_cmd, 1, &status, 1, HAL_MAX_DELAY);

	//Check if Calibration
	if((status >> 3 & 0x01) == 0)
	{
		AHT20_cmd_t AHT20_init_cmds[3] = {AHT20_init_cmd, AHT20_init_cmd_parameter1, AHT20_init_cmd_parameter2};
		HAL_I2C_Master_Transmit(hi2c_cb, AHT20_address, AHT20_init_cmds, 3, HAL_MAX_DELAY);
		HAL_Delay(10);
	}
}

HAL_StatusTypeDef AHT20_soft_reset(void)
{
	if(HAL_I2C_Master_Transmit(hi2c_cb, AHT20_address, AHT20_soft_reset_cmd, 1, HAL_MAX_DELAY) != HAL_OK)
	{
		return HAL_ERROR;
	}
	HAL_Delay(20);
	return HAL_OK;
}

HAL_StatusTypeDef AHT20_measure(float *temperature, float *humidity)
{
	AHT20_cmd_t AHT20_trigger_measurement_cmds[3] =
	{
			AHT20_trigger_measurement_cmd,
			AHT20_trigger_measurement_cmd_parameter1,
			AHT20_trigger_measurement_cmd_parameter2
	};
	if(HAL_I2C_Master_Transmit(hi2c_cb, AHT20_address, AHT20_trigger_measurement_cmds, 3, HAL_MAX_DELAY) != HAL_OK)
	{
		return HAL_ERROR;
	}
	HAL_Delay(80);

	uint8_t status;
	do
	{
		if(HAL_I2C_Mem_Read(hi2c_cb, AHT20_address, AHT20_state_cmd, 1, &status, 1, HAL_MAX_DELAY) != HAL_OK)
		{
			return HAL_ERROR;
		}
		HAL_Delay(10);
	}
	while ((status >> 7 & 0x01) == 1);

	uint8_t received_data[7];
	if(HAL_I2C_Master_Receive(hi2c_cb, AHT20_address, received_data, 7, HAL_MAX_DELAY) != HAL_OK)
	{
		return HAL_ERROR;
	}


	uint32_t humidity_data = (received_data[1]<<16 | received_data[2]<<8 | received_data[3]) >> 4;
	*humidity = (float) (humidity_data/pow(2,20)) * 100; 	//formula as per datasheet

	uint32_t temperature_data = (received_data[3]<<16 | received_data[4]<<8 | received_data[5]) & 0xFFFFF;
	*temperature = (float) (((temperature_data/pow(2,20)) * 200) - 50); 	//formula as per datasheet

	return HAL_OK;
}
