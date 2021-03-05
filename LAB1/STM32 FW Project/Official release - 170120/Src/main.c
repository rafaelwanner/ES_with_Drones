/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 * COPYRIGHT(c) 2017 STMicroelectronics
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
#include "stm32f4xx_hal.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "main.h"
#include "debug.h"
#include "config_drone.h"
#include "timer.h"
#include "rc.h"
#include "steval_fcu001_v1.h"
#include "steval_fcu001_v1_accelero.h"
#include "steval_fcu001_v1_gyro.h"
#include "steval_fcu001_v1_magneto.h"
#include "steval_fcu001_v1_pressure.h"
#include "steval_fcu001_v1_temperature.h"
#include "sensor_data.h"
#include "quaternion.h"
#include "ahrs.h"
#include "flight_control.h"
#include "motor.h"
#include "ble_status.h"
#include "SPBTLE_RF.h"
#include "bluenrg_gatt_server.h"
#include "sensor_service.h"
#include "bluenrg_utils.h"
#include "TargetFeatures.h"

#include "bluenrg_l2cap_aci.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
volatile uint32_t HCI_ProcessEvent=0;
uint8_t joydata[8] = {0,0,0,0,0,0,0,0};

uint32_t uhCCR4_Val = 500;
uint32_t uhCCR1_Val = 5000;


ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart1;

static void *LSM6DSL_X_0_handle = NULL;
static void *LSM6DSL_G_0_handle = NULL;
static void *LIS2MDL_M_0_handle = NULL;
static void *LPS22HB_P_0_handle = NULL;
static void *LPS22HB_T_0_handle = NULL; 

//extern Queue_TypeDef que;
extern volatile tUserTimer tim;
extern char rc_connection_flag;
extern int16_t gAIL, gELE, gTHR, gRUD;
int16_t gJoystick_status;
int32_t rc_cal_flag = 0;
int32_t rc_enable_motor = 0;
int32_t rc_cal_cnt = 0;
int32_t fly_ready = 0;
unsigned char ch, ch_flag;

uint32_t tim9_event_flag = 0, tim9_cnt = 0, tim9_cnt2 = 0;
float tmp_euler_z = 0;


/* BLE module */
DrvStatusTypeDef testStatus = COMPONENT_OK;
uint8_t test_res_global = 0;
uint8_t testEvent = 0;
uint8_t bdaddr[6];
//static uint16_t ConfigServW2STHandle;
//static uint16_t ConfigCharHandle;
//static uint16_t ConsoleW2STHandle;
//static uint16_t TermCharHandle;
//static uint16_t StdErrCharHandle;
extern int connected;
//int8_t RSSI_level;



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
//static void MX_SPI1_Init(void);
//static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM9_Init(void);
static void MX_USART1_UART_Init(void);

static void initializeAllSensors( void );
void enableAllSensors( void );

void BlueNRG_Init(void);
//tBleStatus Add_ConsoleW2ST_Service(void);
//tBleStatus Add_ConfigW2ST_Service(void);
static void Init_BlueNRG_Custom_Services(void);
static void SendMotionData(void);
static void SendBattEnvData(void);
static void SendArmingData(void);



/* USER CODE BEGIN 0 */
P_PI_PIDControlTypeDef pid;
EulerAngleTypeDef euler_rc, euler_ahrs, euler_rc_fil, euler_rc_y_pre[4], euler_rc_x_pre[4];
AxesRaw_TypeDef acc, gyro, mag, acc_fil_int, gyro_fil_int, mag_fil_int;
AxesRaw_TypeDef_Float acc_fil, acc_y_pre[4], acc_x_pre[4], acc_ahrs_FIFO[FIFO_Order], acc_FIFO[FIFO_Order], acc_ahrs;
AxesRaw_TypeDef_Float gyro_fil, gyro_y_pre[4], gyro_x_pre[4], gyro_ahrs_FIFO[FIFO_Order], gyro_FIFO[FIFO_Order], gyro_ahrs;
AxesRaw_TypeDef_Float mag_fil;
AxesRaw_TypeDef acc_off_calc, gyro_off_calc, acc_offset, gyro_offset;
EulerAngleTypeDef euler_ahrs_offset;
int sensor_init_cali = 0, sensor_init_cali_count = 0;
int gyro_cali_count = 0;

typedef struct
{
	int16_t X_Degree;
	int16_t Y_Degree;
	int16_t Z_Degree;
}Attitude_Degree;

typedef struct
{
	float a1, a2, b0, b1, b2;
}IIR_Coeff;

//sensor filter
//7hz, 800hz
//IIR_Coeff gyro_fil_coeff = {1.922286512869545,  -0.92519529534950118, 0.00072719561998898304, 0.0014543912399779661, 0.00072719561998898304};

//15hz, 800hz
//IIR_Coeff gyro_fil_coeff = {1.8337326589246479,  -0.84653197479202391, 0.003199828966843966, 0.0063996579336879321, 0.003199828966843966};

//30hz, 800hz
//IIR_Coeff gyro_fil_coeff = {1.66920314293119312,  -0.71663387350415764, 0.011857682643241156, 0.023715365286482312, 0.011857682643241156};

//60hz, 800hz
//IIR_Coeff gyro_fil_coeff = {1.3489677452527946 ,  -0.51398189421967566, 0.041253537241720303, 0.082507074483440607, 0.041253537241720303};

//100hz, 800hz
IIR_Coeff gyro_fil_coeff = {0.94280904158206336,  -0.33333333333333343, 0.09763107293781749 , 0.19526214587563498 , 0.09763107293781749 };

Attitude_Degree  Fly, Fly_offset, Fly_origin;
Gyro_Rad gyro_rad, gyro_degree, gyro_cali_degree;
MotorControlTypeDef motor_pwm;
int count1 = 0, count2 = 0;
AHRS_State_TypeDef ahrs;
float press, press_zero_level;
float temperature;

uint32_t VBAT_Sense;
float VBAT = 0;

uint8_t tmp_lis2mdl;
SensorAxes_t tmp_mag;

/* BLE */
extern uint8_t set_connectable;
uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
uint32_t ConnectionBleStatus=0;
uint8_t BufferToWrite[256];
int32_t BytesToWrite;

/* USER CODE END 0 */

int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_TIM2_Init();
	MX_TIM4_Init();
	MX_TIM9_Init();
	MX_USART1_UART_Init();
	//MX_USB_DEVICE_Init();

	/* USER CODE BEGIN 2 */

	PRINTF("STEVAL-FCU001V1 FW rev.1.0 - Sep 2017\n\r\n\r");

	//  Initialize Onboard LED
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Off(LED1);
	BSP_LED_Off(LED2);

	/* Configure and disable all the Chip Select pins for sensors on SPI*/
	Sensor_IO_SPI_CS_Init_All();

	/* Initialize and Enable the available sensors on SPI*/
	initializeAllSensors();
	enableAllSensors();

	/* BLE communication */
	PRINTF("BLE communication initialization...\n\n");

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		//This function provides accurate delay (in milliseconds)
		HAL_Delay(1000);
		BSP_LED_Toggle(LED1);
		BSP_LED_Toggle(LED2);
		PRINTF("LOOP");

	}
	/* USER CODE END 3 */

}

/** System Clock Configuration
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/**Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;

	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);


}



/* **************************************************************** */
/* **************************************************************** */
/* **************** FROM LAB2 TO LAB7 - NO LAB1 ! ***************** */
/* **************************************************************** */
/* **************************************************************** */



/* ADC1 init function */
void MX_ADC1_Init(void)
{

	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION12b;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = EOC_SINGLE_CONV;
	HAL_ADC_Init(&hadc1);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}

/* TIM2 init function */
void MX_TIM2_Init(void)
{

	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_IC_InitTypeDef sConfigIC;

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 20;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 32767;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim2);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

	HAL_TIM_IC_Init(&htim2);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1);

	HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2);

	HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3);

	HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_4);

}

/* TIM4 init function */
void MX_TIM4_Init(void)
{

	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	htim4.Instance = TIM4;
#ifdef MOTOR_DC
	htim4.Init.Prescaler = 84;                                    /* DC motor configuration - Freq 494Hz*/
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 1999;
#endif
#ifdef MOTOR_ESC
	htim4.Init.Prescaler = 100;                                    /* ESC motor configuration - Freq 400Hz*/
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 2075;
#endif

	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim4);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

	HAL_TIM_PWM_Init(&htim4);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);

	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2);

	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3);

	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);

}

/* TIM9 init function */
void MX_TIM9_Init(void)
{

	TIM_ClockConfigTypeDef sClockSourceConfig;

	htim9.Instance = TIM9;
	htim9.Init.Prescaler = 51;
	htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim9.Init.Period = 1999;
	htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim9);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig);

}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);

}

/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 * Free pins are configured automatically as Analog (this feature is enabled through
 * the Code Generation settings)
 */
void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__GPIOC_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

	/*Configure GPIO pins : PB4 PB5 */
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/*
 *  Handle Timer9 interrupt @ 800Hz
 *  Set the event flag and increase time index
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(sensor_init_cali == 0)
	{
		sensor_init_cali_count++;

		if(sensor_init_cali_count > 800)
		{
			// Read sensor data and prepare for specific coodinate system
			ReadSensorRawData(LSM6DSL_X_0_handle, LSM6DSL_G_0_handle, LIS2MDL_M_0_handle, LPS22HB_P_0_handle, &acc, &gyro, &mag, &press);

			acc_off_calc.AXIS_X += acc.AXIS_X;
			acc_off_calc.AXIS_Y += acc.AXIS_Y;
			acc_off_calc.AXIS_Z += acc.AXIS_Z;

			gyro_off_calc.AXIS_X += gyro.AXIS_X;
			gyro_off_calc.AXIS_Y += gyro.AXIS_Y;
			gyro_off_calc.AXIS_Z += gyro.AXIS_Z;

			if (sensor_init_cali_count >= 1600)
			{
				acc_offset.AXIS_X = (int32_t) (acc_off_calc.AXIS_X * 0.00125f);
				acc_offset.AXIS_Y = (int32_t) (acc_off_calc.AXIS_Y * 0.00125f);
				acc_offset.AXIS_Z = (int32_t) (acc_off_calc.AXIS_Z * 0.00125f);

				gyro_offset.AXIS_X = (int32_t) (gyro_off_calc.AXIS_X * 0.00125f);
				gyro_offset.AXIS_Y = (int32_t) (gyro_off_calc.AXIS_Y * 0.00125f);
				gyro_offset.AXIS_Z = (int32_t) (gyro_off_calc.AXIS_Z * 0.00125f);

				acc_off_calc.AXIS_X = 0;
				acc_off_calc.AXIS_Y = 0;
				acc_off_calc.AXIS_Z = 0;
				gyro_off_calc.AXIS_X = 0;
				gyro_off_calc.AXIS_Y = 0;
				gyro_off_calc.AXIS_Z = 0;

				sensor_init_cali_count = 0;
				sensor_init_cali = 1;
			}
		}
	}

	if(sensor_init_cali == 1)
	{
		tim9_cnt++;
		tim9_cnt2++;

		// Read sensor data and prepare for specific coodinate system
		ReadSensorRawData(LSM6DSL_X_0_handle, LSM6DSL_G_0_handle, LIS2MDL_M_0_handle, LPS22HB_P_0_handle, &acc, &gyro, &mag, &press);

		if (rc_cal_flag == 1)
		{
			acc_off_calc.AXIS_X += acc.AXIS_X;
			acc_off_calc.AXIS_Y += acc.AXIS_Y;
			acc_off_calc.AXIS_Z += acc.AXIS_Z;

			gyro_off_calc.AXIS_X += gyro.AXIS_X;
			gyro_off_calc.AXIS_Y += gyro.AXIS_Y;
			gyro_off_calc.AXIS_Z += gyro.AXIS_Z;

			rc_cal_cnt++;

			if (rc_cal_cnt >= 800)
			{
				acc_offset.AXIS_X = (int32_t) (acc_off_calc.AXIS_X * 0.00125f);
				acc_offset.AXIS_Y = (int32_t) (acc_off_calc.AXIS_Y * 0.00125f);
				acc_offset.AXIS_Z = (int32_t) (acc_off_calc.AXIS_Z * 0.00125f);

				gyro_offset.AXIS_X = (int32_t) (gyro_off_calc.AXIS_X * 0.00125f);
				gyro_offset.AXIS_Y = (int32_t) (gyro_off_calc.AXIS_Y * 0.00125f);
				gyro_offset.AXIS_Z = (int32_t) (gyro_off_calc.AXIS_Z * 0.00125f);

				acc_off_calc.AXIS_X = 0;
				acc_off_calc.AXIS_Y = 0;
				acc_off_calc.AXIS_Z = 0;
				gyro_off_calc.AXIS_X = 0;
				gyro_off_calc.AXIS_Y = 0;
				gyro_off_calc.AXIS_Z = 0;

				rc_cal_cnt = 0;
				rc_cal_flag = 0;
			}
		}

		acc.AXIS_X -= acc_offset.AXIS_X;
		acc.AXIS_Y -= acc_offset.AXIS_Y;
		acc.AXIS_Z -= (acc_offset.AXIS_Z - 1000);
		gyro.AXIS_X -= gyro_offset.AXIS_X;
		gyro.AXIS_Y -= gyro_offset.AXIS_Y;
		gyro.AXIS_Z -= gyro_offset.AXIS_Z;

		// Save filtered data to acc_FIFO
		acc_FIFO[tim9_cnt2-1].AXIS_X = acc.AXIS_X;
		acc_FIFO[tim9_cnt2-1].AXIS_Y = acc.AXIS_Y;
		acc_FIFO[tim9_cnt2-1].AXIS_Z = acc.AXIS_Z;

		// IIR Filtering on gyro
		gyro_fil.AXIS_X = gyro_fil_coeff.b0*gyro.AXIS_X + gyro_fil_coeff.b1*gyro_x_pre[0].AXIS_X + gyro_fil_coeff.b2*gyro_x_pre[1].AXIS_X
				+ gyro_fil_coeff.a1*gyro_y_pre[0].AXIS_X + gyro_fil_coeff.a2*gyro_y_pre[1].AXIS_X;
		gyro_fil.AXIS_Y = gyro_fil_coeff.b0*gyro.AXIS_Y + gyro_fil_coeff.b1*gyro_x_pre[0].AXIS_Y + gyro_fil_coeff.b2*gyro_x_pre[1].AXIS_Y
				+ gyro_fil_coeff.a1*gyro_y_pre[0].AXIS_Y + gyro_fil_coeff.a2*gyro_y_pre[1].AXIS_Y;
		gyro_fil.AXIS_Z = gyro_fil_coeff.b0*gyro.AXIS_Z + gyro_fil_coeff.b1*gyro_x_pre[0].AXIS_Z + gyro_fil_coeff.b2*gyro_x_pre[1].AXIS_Z
				+ gyro_fil_coeff.a1*gyro_y_pre[0].AXIS_Z + gyro_fil_coeff.a2*gyro_y_pre[1].AXIS_Z;
		// Shift IIR filter state
		for(int i=1;i>0;i--)
		{
			gyro_x_pre[i].AXIS_X = gyro_x_pre[i-1].AXIS_X;
			gyro_x_pre[i].AXIS_Y = gyro_x_pre[i-1].AXIS_Y;
			gyro_x_pre[i].AXIS_Z = gyro_x_pre[i-1].AXIS_Z;
			gyro_y_pre[i].AXIS_X = gyro_y_pre[i-1].AXIS_X;
			gyro_y_pre[i].AXIS_Y = gyro_y_pre[i-1].AXIS_Y;
			gyro_y_pre[i].AXIS_Z = gyro_y_pre[i-1].AXIS_Z;
		}
		gyro_x_pre[0].AXIS_X = gyro.AXIS_X;
		gyro_x_pre[0].AXIS_Y = gyro.AXIS_Y;
		gyro_x_pre[0].AXIS_Z = gyro.AXIS_Z;
		gyro_y_pre[0].AXIS_X = gyro_fil.AXIS_X;
		gyro_y_pre[0].AXIS_Y = gyro_fil.AXIS_Y;
		gyro_y_pre[0].AXIS_Z = gyro_fil.AXIS_Z;

		//  Save filtered data to gyro_FIFO
		gyro_FIFO[tim9_cnt2-1].AXIS_X = gyro_fil.AXIS_X;
		gyro_FIFO[tim9_cnt2-1].AXIS_Y = gyro_fil.AXIS_Y;
		gyro_FIFO[tim9_cnt2-1].AXIS_Z = gyro_fil.AXIS_Z;


		if(tim9_cnt2 == FIFO_Order)
		{
			tim9_cnt2 = 0;
			tim9_event_flag = 1;
			for(int i=0;i<FIFO_Order;i++)
			{
				acc_ahrs_FIFO[i].AXIS_X = acc_FIFO[i].AXIS_X;
				acc_ahrs_FIFO[i].AXIS_Y = acc_FIFO[i].AXIS_Y;
				acc_ahrs_FIFO[i].AXIS_Z = acc_FIFO[i].AXIS_Z;
				gyro_ahrs_FIFO[i].AXIS_X = gyro_FIFO[i].AXIS_X;
				gyro_ahrs_FIFO[i].AXIS_Y = gyro_FIFO[i].AXIS_Y;
				gyro_ahrs_FIFO[i].AXIS_Z = gyro_FIFO[i].AXIS_Z;
			}
		}


		gyro_rad.gx = ((float)gyro_fil.AXIS_X)*((float)COE_MDPS_TO_RADPS);
		gyro_rad.gy = ((float)gyro_fil.AXIS_Y)*((float)COE_MDPS_TO_RADPS);
		gyro_rad.gz = ((float)gyro_fil.AXIS_Z)*((float)COE_MDPS_TO_RADPS);

		euler_ahrs.thz += gyro_rad.gz*PID_SAMPLING_TIME;

		if(gTHR<MIN_THR)
		{
			euler_rc.thz = 0;
			euler_ahrs.thz = 0;
		}

		if (rc_connection_flag && rc_enable_motor)
		{   // Do PID Control
			FlightControlPID_innerLoop(&euler_rc_fil, &gyro_rad, &ahrs, &pid, &motor_pwm);
		}
		else
		{
			// set motor output zero
			set_motor_pwm_zero(&motor_pwm);
		}

		if(gTHR<MIN_THR)
		{
			set_motor_pwm_zero(&motor_pwm);
		}

		set_motor_pwm(&motor_pwm);      /* To comment if want to debug remocon calibration switching off the motors */
	}

}


/**
 * @brief  Initialize all sensors
 * @param  None
 * @retval None
 */
static void initializeAllSensors( void )
{
	if (BSP_ACCELERO_Init( LSM6DSL_X_0, &LSM6DSL_X_0_handle ) != COMPONENT_OK)
	{
		while(1);
	}

	if (BSP_GYRO_Init( LSM6DSL_G_0, &LSM6DSL_G_0_handle ) != COMPONENT_OK)
	{
		while(1);
	}

	if (BSP_MAGNETO_Init( LIS2MDL_M_0, &LIS2MDL_M_0_handle ) != COMPONENT_OK)
	{
		while(1);
	}


	if (BSP_PRESSURE_Init( LPS22HB_P_0, &LPS22HB_P_0_handle ) != COMPONENT_OK)
	{
		while(1);
	}

	if (BSP_TEMPERATURE_Init( LPS22HB_T_0, &LPS22HB_T_0_handle ) != COMPONENT_OK)
	{
		while(1);
	}


}

/**
 * @brief  Enable all sensors
 * @param  None
 * @retval None
 */
void enableAllSensors( void )
{
	BSP_ACCELERO_Sensor_Enable( LSM6DSL_X_0_handle );
	PRINTF("LSM6DSL MEMS Accelerometer initialized and enabled\n\r");
	BSP_GYRO_Sensor_Enable( LSM6DSL_G_0_handle );
	PRINTF("LSM6DSL MEMS Gyroscope initialized and enabled\n\r");
	BSP_MAGNETO_Sensor_Enable( LIS2MDL_M_0_handle );
	PRINTF("LIS2MDL Magnetometer initialized and enabled\n\r");
	BSP_PRESSURE_Sensor_Enable( LPS22HB_P_0_handle );
	PRINTF("LPS22HB Pressure sensor initialized and enabled\n\r");
	BSP_TEMPERATURE_Sensor_Enable( LPS22HB_T_0_handle );
	PRINTF("LPS22HB Temperature sensor initialized and enabled\n\r");
}



void BlueNRG_Init(void)
{

	int ret = 1;
	uint8_t  hwVersion=0;
	uint16_t fwVersion=0;

	PRINTF("****** START BLE TESTS ******\r\n");
	BNRG_SPI_Init();

	/* Commented on Jan 15, 2020 */
	// uint8_t tmp_bdaddr[6]= {MAC_BLUEMS};
	// int32_t i;
	/* Commented on Jan 15, 2020 */
	//for(i=0;i<6;i++)
	//  bdaddr[i] = tmp_bdaddr[i];

	/* Added on Jan 15, 2020 */
	bdaddr[0] = (STM32_UUID[1]>>24)&0xFF;
	bdaddr[1] = (STM32_UUID[0]    )&0xFF;
	bdaddr[2] = (STM32_UUID[2] >>8)&0xFF;
	bdaddr[3] = (STM32_UUID[0]>>16)&0xFF;
	bdaddr[4] = (hwVersion > 0x30) ?
			((((0x34-48)*10) + (0x30-48)+100)&0xFF) :
			((((0x34-48)*10) + (0x30-48)    )&0xFF) ;
	bdaddr[5] = 0xC0; /* for a Legal BLE Random MAC */

	/* Initialize the BlueNRG HCI */
	HCI_Init();

	/* Reset BlueNRG hardware */
	BlueNRG_RST();

	/* get the BlueNRG HW and FW versions */
	PRINTF("\r\nReading BlueNRG version ...\r\n");
	if (getBlueNRGVersion(&hwVersion, &fwVersion)== BLE_STATUS_SUCCESS)
	{

		/*
		 * Reset BlueNRG again otherwise it will fail.
		 */
		BlueNRG_RST();

		//PRINTF("Setting Pubblic Address...\r\n");
		/* Commented on Jan 15, 2020 */
		//ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
		//                                CONFIG_DATA_PUBADDR_LEN,
		//                                bdaddr);
		//if(ret){
		//  testStatus = COMPONENT_ERROR;
		//  PRINTF("\r\nSetting Pubblic BD_ADDR failed *****\r\n");
		//  goto fail;
		//}

		PRINTF("GATT Initializzation...\r\n");
		ret = aci_gatt_init();
		if(ret){
			testStatus = COMPONENT_ERROR;
			PRINTF("\r\nGATT_Init failed ****\r\n");
			goto fail;
		}

		//    ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
		//                                     7/*strlen(BoardName)*/, (uint8_t *)BoardName);
		//
		//    if(ret){
		//       PRINTF("\r\naci_gatt_update_char_value failed\r\n");
		//      while(1);
		//    }

		/* Set the GAP INIT like X-NUCLEO-IDB05A1 eval board  since using same SPBTLE_RF module*/
		ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);

		if(ret != BLE_STATUS_SUCCESS){
			PRINTF("\r\nGAP_Init failed\r\n");
			goto fail;
		}

		// Added Jan 10th
		ret = hci_le_set_random_address(bdaddr);
		// Added Jan 10th
		const char BoardName[7] = {NAME_BLUEMS};
		// Added Jan 10th
		ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
				7/*strlen(BoardName)*/, (uint8_t *)BoardName);

		PRINTF("GAP setting Authentication ....\r\n");
		ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
				OOB_AUTH_DATA_ABSENT,
				NULL, 7, 16,
				USE_FIXED_PIN_FOR_PAIRING, 123456,
				BONDING);
		if (ret != BLE_STATUS_SUCCESS) {
			testStatus = COMPONENT_ERROR;
			PRINTF("\r\nGAP setting Authentication failed ******\r\n");
			goto fail;
		}

		PRINTF("SERVER: BLE Stack Initialized \r\n"
				"Board HWver=%d, FWver=%d.%d.%c\r\n"
				"BoardMAC = %x:%x:%x:%x:%x:%x\r\n",
				hwVersion,
				fwVersion>>8,
				(fwVersion>>4)&0xF,
				(hwVersion > 0x30) ? ('a'+(fwVersion&0xF)-1) : 'a',
						bdaddr[5],bdaddr[4],bdaddr[3],bdaddr[2],bdaddr[1],bdaddr[0]);

		/* Set output power level */
		aci_hal_set_tx_power_level(1,4);    /* -2.1dBm */

		ret = Add_ConsoleW2ST_Service();
		if(ret == BLE_STATUS_SUCCESS)
			PRINTF("Console Service W2ST added successfully\r\n");
		else{
			testStatus = COMPONENT_ERROR;
			PRINTF("\r\nError while adding Console Service W2ST\r\n");
		}

		ret = Add_ConfigW2ST_Service();
		if(ret == BLE_STATUS_SUCCESS)
			PRINTF("Config  Service W2ST added successfully\r\n");
		else{
			testStatus = COMPONENT_ERROR;
			PRINTF("\r\nError while adding Config Service W2ST\r\n");
		}

		PRINTF("\r\nAll test passed!\r\n");
	}
	else {
		testStatus = COMPONENT_ERROR;
		PRINTF("\r\nError in BlueNRG tests. ******\r\n");
	}
	PRINTF("****** END BLE TESTS ******\r\n");
	return;

	fail:
	testStatus = COMPONENT_ERROR;
	return;
}

/** @brief Initialize all the Custom BlueNRG services
 * @param None
 * @retval None
 */
static void Init_BlueNRG_Custom_Services(void)
{
	int ret;

	ret = Add_HWServW2ST_Service();
	if(ret == BLE_STATUS_SUCCESS) {
		PRINTF("HW      Service W2ST added successfully\r\n");
	} else {
		PRINTF("\r\nError while adding HW Service W2ST\r\n");
	}

	ret = Add_ConsoleW2ST_Service();
	if(ret == BLE_STATUS_SUCCESS) {
		PRINTF("Console Service W2ST added successfully\r\n");
	} else {
		PRINTF("\r\nError while adding Console Service W2ST\r\n");
	}

	ret = Add_ConfigW2ST_Service();
	if(ret == BLE_STATUS_SUCCESS) {
		PRINTF("Config  Service W2ST added successfully\r\n");
	} else {
		PRINTF("\r\nError while adding Config Service W2ST\r\n");
	}
}

/**
 * @brief  Send Motion Data Acc/Mag/Gyro to BLE
 * @param  None
 * @retval None
 */
static void SendMotionData(void)
{
	SensorAxes_t ACC_Value;
	SensorAxes_t GYR_Value;
	SensorAxes_t MAG_Value;



	ACC_Value.AXIS_X = acc.AXIS_X;
	ACC_Value.AXIS_Y = acc.AXIS_Y;
	ACC_Value.AXIS_Z = acc.AXIS_Z;
	GYR_Value.AXIS_X = gyro.AXIS_X;
	GYR_Value.AXIS_Y = gyro.AXIS_Y;
	GYR_Value.AXIS_Z = gyro.AXIS_Z;
	MAG_Value.AXIS_X = mag.AXIS_X;
	MAG_Value.AXIS_Y = mag.AXIS_Y;
	MAG_Value.AXIS_Z = mag.AXIS_Z;



	/*Debug */
	//PRINTF("ACC[X, Y, Z]: %d\t%d\t%d\t\n", ACC_Value.AXIS_X, ACC_Value.AXIS_Y, ACC_Value.AXIS_Z);
	//PRINTF("GYRO[X, Y, Z]: %d\t%d\t%d\t\n", GYR_Value.AXIS_X, GYR_Value.AXIS_Y, GYR_Value.AXIS_Z);
	//PRINTF("MAG[X, Y, Z]: %d\t%d\t%d\t\n", MAG_Value.AXIS_X, MAG_Value.AXIS_Y, MAG_Value.AXIS_Z);

	AccGyroMag_Update(&ACC_Value,&GYR_Value,&MAG_Value);

}

static void SendBattEnvData(void)
{
	int32_t decPart, intPart;
	int32_t PressToSend=0;
	uint16_t BattToSend=0;
	int16_t RSSIToSend=0, TempToSend=0;
	int8_t rssi;
	uint16_t conn_handle;

	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK)
	{
		VBAT_Sense = HAL_ADC_GetValue(&hadc1);
		VBAT = (((VBAT_Sense*3.3)/4095)*(BAT_RUP+BAT_RDW))/BAT_RDW;
		//PRINTF("Battery voltage = %fV\n\n", VBAT);
	}
	HAL_ADC_Stop(&hadc1);

	MCR_BLUEMS_F2I_2D(press, intPart, decPart);
	PressToSend=intPart*100+decPart;
	MCR_BLUEMS_F2I_1D(((int32_t)((float)VBAT*100.0f)/4.2f), intPart, decPart);
	BattToSend = intPart*10+decPart;
	if (BattToSend > 1000){
		BattToSend =1000;
	}
	MCR_BLUEMS_F2I_1D(temperature, intPart, decPart);
	TempToSend = intPart*10+decPart;

	hci_read_rssi(&conn_handle, &rssi);
	RSSIToSend = (int16_t)rssi*10;

	Batt_Env_RSSI_Update(PressToSend,BattToSend,(int16_t) TempToSend,RSSIToSend );

}


static void SendArmingData(void)
{
	ARMING_Update(rc_enable_motor);
}



/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
