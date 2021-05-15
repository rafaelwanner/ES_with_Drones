#ifndef __VL53L0X_H
#define __VL53L0X_H
#include "stm32f4xx_hal.h"

#define VL_CS1_Enable HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, SET); //PB2 pull high enable of VL53L0X on the left
#define VL_CS1_Disable HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, SET, RESET); //PB2 of VL53L0X on the left is pulled down and disabled

#define VL_CS2_Enable HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, SET, SET); //The PB1 of the VL53L0X on the right side is enabled
#define VL_CS2_Disable HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, SET, RESET); //PB1 of VL53L0X on the right side is pulled down and disabled


 //Register operation function
#define VL53L0x_add 0x52

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14


struct VL53L0xData {
	uint8_t vtemp[12];
	uint16_t acnt; //Environmental statistics, laser light intensity
	uint16_t scnt; //Number of signals
	uint16_t dist; //distance, the unit mm is the most primitive data
	uint8_t DeviceRangeStatusInternal;

	uint16_t dist_last; //historical effective value, used to judge

	uint16_t distValid; //The effective value after the original distance value is extracted
	uint16_t dist_buff[15]; //Slide window buffer for ranging

	uint8_t Flag_OverRange; //Overrange flag bit
};


void VL53L0x_init(I2C_HandleTypeDef *hi2c);

 //Check if VL53L0x is normal
uint8_t VL53L0x_Check(I2C_HandleTypeDef *hi2c);

 //VL53L0x convert once
void VL53L0x_StartConversion(I2C_HandleTypeDef *hi2c);

 //VL53L0x reading distance and other data feedback information
void VL53L0x_ReadDistance(I2C_HandleTypeDef *hi2c, struct VL53L0xData *VL_temp);

void VL53L0x_DataFitting(void);

#endif
