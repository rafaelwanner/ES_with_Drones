/**
 ******************************************************************************
 * @file    steval_fcu001_v1_pressure.h
 * @author  MEMS Application Team, Competence Center Japan
 * @version V3.0.0 (MEMS library version)
 * @date    12-August-2016
 * @brief   This file contains definitions for x_nucleo_iks01a2_pressure.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STEVAL_FCU001_V1_ToF_H
#define __STEVAL_FCU001_V1_ToF_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/
#include "steval_fcu001_v1.h"

typedef enum {
	LONG_RANGE 		= 0, /*!< Long range mode */
	HIGH_SPEED 		= 1, /*!< High speed mode */
	HIGH_ACCURACY	= 2, /*!< High accuracy mode */
} RangingConfig_e;

/**
 * 53L0X Device selector
 *
 * @note Most functions are using a device selector as input. ASCII 'c', 'l' or 'r' are also accepted.
 */
enum XNUCLEO53L0A1_dev_e{
    X_53L0A1_DEV =  0,    //!< left satellite device P21 header : 'l'
    XNUCLEO53L0A1_DEV_CENTER  =  1, //!< center (built-in) vl053 device : 'c"
    XNUCLEO53L0A1_DEV_RIGHT=  2     //!< Right satellite device P22 header : 'r'
};


int DetectSensors( void );
uint16_t BSP_ToF_Get_Distance( void );
void SetupSingleShot(RangingConfig_e rangingConfig);


#ifdef __cplusplus
}
#endif

#endif /* __STEVAL_FCU001_V1_PRESSURE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
