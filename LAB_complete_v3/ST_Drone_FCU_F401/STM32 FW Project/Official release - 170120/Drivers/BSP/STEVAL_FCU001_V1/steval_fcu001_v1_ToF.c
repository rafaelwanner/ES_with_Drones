/**
 ******************************************************************************
 * @file    steval_fcu001_v1_pressure.c
 * @author  MEMS Application Team, Competence Center Japan
 * @version V3.0.0 (MEMS library version)
 * @date    12-August-2016
 * @brief   This file provides a set of functions needed to manage the pressure sensor
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "steval_fcu001_v1_ToF.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_api.h"

typedef enum {
	RANGE_VALUE 	= 0, /*!< Range displayed in cm */
	BAR_GRAPH 		= 1, /*!< Range displayed as a bar graph : one bar per sensor */
} DemoMode_e;

/**
 * Global ranging struct
 */
VL53L0X_RangingMeasurementData_t RangingMeasurementData;

/** leaky factor for filtered range
 *
 * r(n) = averaged_r(n-1)*leaky +r(n)(1-leaky)
 *
 * */
int LeakyFactorFix8 = (int)( 0.6 *256);
/** How many device detect set by @a DetectSensors()*/
int nDevPresent=0;
/** bit is index in VL53L0XDevs that is not necessary the dev id of the BSP */
int nDevMask;

extern I2C_HandleTypeDef hi2c2;
VL53L0X_Dev_t VL53L0XDevs[]={
		{.Id=X_53L0A1_DEV, .DevLetter=0, .I2cHandle=&hi2c2, .I2cDevAddr=0x52},
};

/**
 * Reset all sensor then do presence detection
 *
 * All present devices are data initiated and assigned to their final I2C address
 * @return
 */
int DetectSensors( void ) {
	uint16_t Id;
	int status = VL53L0X_ERROR_NONE;
	int FinalAddress;

	/* Reset all */
	nDevPresent = 0;

	/* detect all sensors (even on-board)*/
	VL53L0X_Dev_t *pDev;
	pDev = &VL53L0XDevs[0];
	pDev->I2cDevAddr = 0x52;
	pDev->Present = 0;
	HAL_Delay(2);
	FinalAddress=pDev->I2cDevAddr;

	/* Set I2C standard mode (400 KHz) before doing the first register access */
	if (status == VL53L0X_ERROR_NONE)
		status = VL53L0X_WrByte(pDev, 0x88, 0x00);

	/* Try to read one register using default 0x52 address */
	status = VL53L0X_RdWord(pDev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
	if (status) {
		return 0;
	}
	if (Id == 0xEEAA) {

		pDev->I2cDevAddr = FinalAddress;
		/* Check all is OK with the new I2C address and initialize the sensor */
		status = VL53L0X_RdWord(pDev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
		if (status != 0) {
			return 0;
		}

		status = VL53L0X_DataInit(pDev);
		if( status == 0 ){
			pDev->Present = 1;
		}
		else{
			return 0;
		}
		nDevPresent++;
		nDevMask |= 1;
		pDev->Present = 1;
	}else {
		return 0;
		status = 1;
	}

	return nDevPresent;
}

/**
 *  Setup all detected sensors for single shot mode and setup ranging configuration
 */
void SetupSingleShot(RangingConfig_e rangingConfig){
	int status;
	uint8_t VhvSettings;
	uint8_t PhaseCal;
	uint32_t refSpadCount;
	uint8_t isApertureSpads;
	FixPoint1616_t signalLimit = (FixPoint1616_t)(0.25*65536);
	FixPoint1616_t sigmaLimit = (FixPoint1616_t)(18*65536);
	uint32_t timingBudget = 33000;
	uint8_t preRangeVcselPeriod = 14;
	uint8_t finalRangeVcselPeriod = 10;

	if( VL53L0XDevs[0].Present){
		status=VL53L0X_StaticInit(&VL53L0XDevs[0]);
		if( status ){
			return;
		}

		status = VL53L0X_PerformRefCalibration(&VL53L0XDevs[0], &VhvSettings, &PhaseCal);
		if( status ){
			return;
		}

		status = VL53L0X_PerformRefSpadManagement(&VL53L0XDevs[0], &refSpadCount, &isApertureSpads);
		if( status ){
			return;
		}

		status = VL53L0X_SetDeviceMode(&VL53L0XDevs[0], VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
		if( status ){
			return;
		}

		status = VL53L0X_SetLimitCheckEnable(&VL53L0XDevs[0], VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1); // Enable Sigma limit
		if( status ){
			return;
		}

		status = VL53L0X_SetLimitCheckEnable(&VL53L0XDevs[0], VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1); // Enable Signa limit
		if( status ){
			return;
		}
		/* Ranging configuration */
		switch(rangingConfig) {
		case LONG_RANGE:
			signalLimit = (FixPoint1616_t)(0.1*65536);
			sigmaLimit = (FixPoint1616_t)(60*65536);
			timingBudget = 33000;
			preRangeVcselPeriod = 18;
			finalRangeVcselPeriod = 14;
			break;
		case HIGH_ACCURACY:
			signalLimit = (FixPoint1616_t)(0.25*65536);
			sigmaLimit = (FixPoint1616_t)(18*65536);
			timingBudget = 200000;
			preRangeVcselPeriod = 14;
			finalRangeVcselPeriod = 10;
			break;
		case HIGH_SPEED:
			signalLimit = (FixPoint1616_t)(0.25*65536);
			sigmaLimit = (FixPoint1616_t)(32*65536);
			timingBudget = 20000;
			preRangeVcselPeriod = 14;
			finalRangeVcselPeriod = 10;
			break;
		default:
			//debug_printf("Not Supported");
			return;
		}

		status = VL53L0X_SetLimitCheckValue(&VL53L0XDevs[0],  VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, signalLimit);
		if( status ){
			//debug_printf("VL53L0X_SetLimitCheckValue failed\n");
			return;
		}

		status = VL53L0X_SetLimitCheckValue(&VL53L0XDevs[0],  VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, sigmaLimit);
		if( status ){
			//debug_printf("VL53L0X_SetLimitCheckValue failed\n");
			return;
		}

		status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(&VL53L0XDevs[0],  timingBudget);
		if( status ){
			//debug_printf("VL53L0X_SetMeasurementTimingBudgetMicroSeconds failed\n");
			return;
		}

		status = VL53L0X_SetVcselPulsePeriod(&VL53L0XDevs[0],  VL53L0X_VCSEL_PERIOD_PRE_RANGE, preRangeVcselPeriod);
		if( status ){
			//debug_printf("VL53L0X_SetVcselPulsePeriod failed\n");
			return;
		}

		status = VL53L0X_SetVcselPulsePeriod(&VL53L0XDevs[0],  VL53L0X_VCSEL_PERIOD_FINAL_RANGE, finalRangeVcselPeriod);
		if( status ){
			//debug_printf("VL53L0X_SetVcselPulsePeriod failed\n");
			return;
		}

		status = VL53L0X_PerformRefCalibration(&VL53L0XDevs[0], &VhvSettings, &PhaseCal);
		if( status ){
			//debug_printf("VL53L0X_PerformRefCalibration failed\n");
			return;
		}

		VL53L0XDevs[0].LeakyFirst=1;
	}

}


/* Store new ranging data into the device structure, apply leaky integrator if needed */
void Sensor_SetNewRange(VL53L0X_Dev_t *pDev, VL53L0X_RangingMeasurementData_t *pRange){
	if( pRange->RangeStatus == 0 ){
		if( pDev->LeakyFirst ){
			pDev->LeakyFirst = 0;
			pDev->LeakyRange = pRange->RangeMilliMeter;
		}
		else{
			pDev->LeakyRange = (pDev->LeakyRange*LeakyFactorFix8 + (256-LeakyFactorFix8)*pRange->RangeMilliMeter)>>8;
		}
	}
	else{
		pDev->LeakyFirst = 1;
	}
}


/**
 * Return the distance in cm
 */
uint16_t BSP_ToF_Get_Distance( void ){

	int status;
	int SingleSensorNo=0;

	if( VL53L0XDevs[0].Present){

		/* only one sensor */
		/* Call All-In-One blocking API function */
		status = VL53L0X_PerformSingleRangingMeasurement(&VL53L0XDevs[SingleSensorNo],&RangingMeasurementData);
		if( status == 0 ){
			/* Display distance in cm */
			if( RangingMeasurementData.RangeStatus == 0 ){
				//return RangingMeasurementData.RangeMilliMeter;
				Sensor_SetNewRange(&VL53L0XDevs[SingleSensorNo],&RangingMeasurementData);
				return (uint16_t)(VL53L0XDevs[SingleSensorNo].LeakyRange/10);
			}
			else{
				return (uint16_t)(VL53L0XDevs[SingleSensorNo].LeakyRange/10);
			}
		}
		else{
			return 0;
		}

	}

	return 0;

}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
