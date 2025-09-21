/*
 * aht20.h
 *
 *  Created on: Sep 20, 2025
 *      Author: npero
 */

#ifndef AHT20_H_
#define AHT20_H_

#include "stm32f4xx_hal.h"

typedef enum
{
	AHT20_address = 0x38 << 1, //Address is 0x38 but it is a 7bit address so we shift to the left by 1 to make it 8 bit (The 8th bit is zero so it is in "read" mode)
	AHT20_state_cmd = 0x71,
	AHT20_init_cmd = 0xBE,
	AHT20_init_cmd_parameter1 = 0x08,
	AHT20_init_cmd_parameter2 = 0x00,
	AHT20_trigger_measurement_cmd = 0xAC,
	AHT20_trigger_measurement_cmd_parameter1 = 0x33,
	AHT20_trigger_measurement_cmd_parameter2 = 0x00

} AHT20_cmd_t ;

void AHT20_init(I2C_HandleTypeDef *hi2c1);
HAL_StatusTypeDef AHT20_measure(float *temperature, float *humidity);


#endif /* AHT20_H_ */
