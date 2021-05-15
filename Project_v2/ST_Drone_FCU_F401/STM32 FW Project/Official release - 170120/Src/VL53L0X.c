#include "VL53L0X.h"
#include <math.h>


void VL53L0x_init(I2C_HandleTypeDef *hi2c)
{
	uint8_t VL53L0x_SendData[2] = {0x01};
	uint8_t VL53L0x_RecData[5] ;

	HAL_I2C_Mem_Write(hi2c, VL53L0x_add, VL53L0X_REG_SYSRANGE_START, I2C_MEMADD_SIZE_8BIT, VL53L0x_SendData, 1, 1);

	VL53L0x_SendData[1] = 100;
	while(VL53L0x_SendData[1]--)
	{
		HAL_Delay(1);
		HAL_I2C_Mem_Read(hi2c, VL53L0x_add, VL53L0X_REG_RESULT_RANGE_STATUS, I2C_MEMADD_SIZE_8BIT, VL53L0x_RecData+4, 1, 1);
			if (VL53L0x_RecData[4] & 0x01) break;
	}
}

 //Check if VL53L0x is normal
 //Return 0 if check is successful
 //Return 1 if the check fails
uint8_t VL53L0x_Check(I2C_HandleTypeDef *hi2c)
{
	uint8_t VL53L0x_SendData[2] = {0x01};
	uint8_t VL53L0x_RecData[5];

	//Revision ID:
	HAL_I2C_Mem_Read(hi2c, VL53L0x_add, VL53L0X_REG_IDENTIFICATION_REVISION_ID, I2C_MEMADD_SIZE_8BIT, VL53L0x_RecData, 1, 1);
	//Device ID:
	HAL_I2C_Mem_Read(hi2c, VL53L0x_add, VL53L0X_REG_IDENTIFICATION_MODEL_ID, I2C_MEMADD_SIZE_8BIT, VL53L0x_RecData+1, 1, 1);
	//PRE_RANGE_CONFIG_VCSEL_PERIOD =
	HAL_I2C_Mem_Read(hi2c, VL53L0x_add, VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD, I2C_MEMADD_SIZE_8BIT, VL53L0x_RecData+2, 1, 1);
	//FINAL_RANGE_CONFIG_VCSEL_PERIOD=
	HAL_I2C_Mem_Read(hi2c, VL53L0x_add, VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD, I2C_MEMADD_SIZE_8BIT, VL53L0x_RecData+3, 1, 1);

	HAL_I2C_Mem_Write(hi2c, VL53L0x_add, VL53L0X_REG_SYSRANGE_START, I2C_MEMADD_SIZE_8BIT, VL53L0x_SendData, 1, 1);
	HAL_Delay(500);

	VL53L0x_SendData[1] = 100;
	while(VL53L0x_SendData[1]--)
	{
		HAL_Delay(10);
		HAL_I2C_Mem_Read(hi2c, VL53L0x_add, VL53L0X_REG_RESULT_RANGE_STATUS, I2C_MEMADD_SIZE_8BIT, VL53L0x_RecData+4, 1, 1);
			if (VL53L0x_RecData[4] & 0x01) break;
	}

	if (VL53L0x_RecData[4] & 0x01)
	 return 0; //return 0 if check is successful
	 else return 1; //return 1 if check fails
}

 //VL53L0x convert once
void VL53L0x_StartConversion(I2C_HandleTypeDef *hi2c)
{
	uint8_t VL53L0x_SendData[1] = {0x01};
	HAL_I2C_Mem_Write(hi2c, VL53L0x_add, VL53L0X_REG_SYSRANGE_START, I2C_MEMADD_SIZE_8BIT, VL53L0x_SendData, 1, 1);
}

uint16_t makeuint16(int lsb, int msb)
{
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}


 //VL53L0x reading distance
void VL53L0x_ReadDistance(I2C_HandleTypeDef *hi2c, struct VL53L0xData *VL_temp)
{
	 //Record historical valid values
	VL_temp->dist_last = VL_temp->distValid;

	HAL_I2C_Mem_Read(hi2c, VL53L0x_add, VL53L0X_REG_RESULT_RANGE_STATUS, I2C_MEMADD_SIZE_8BIT, VL_temp->vtemp, 12, 1);

	VL_temp->dist = makeuint16(VL_temp->vtemp[11], VL_temp->vtemp[10]);
	VL_temp->DeviceRangeStatusInternal = ((VL_temp->vtemp[0] & 0x78) >> 3);


	 //Take last reading if read is invalid
	 if(VL_temp->dist == 8190 || VL_temp->dist == 20 ) //Distance data is invalid
		VL_temp->distValid = VL_temp->dist_last;
	 else //valid
		VL_temp->distValid = (VL_temp->dist + VL_temp->dist_last)/2;

}
