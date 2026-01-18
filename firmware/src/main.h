/**
 ******************************************************************************
 * @file 	main.h
 * @author 	Ahmet Can GULMEZ
 * @brief 	Main header file of the firmware.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2025 Ahmet Can GULMEZ.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 ******************************************************************************
 */

#ifndef FIRMWARE_H
#define FIRMWARE_H

/* Standard C Libraries */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

/* FreeRTOS Libraries */

#include <FreeRTOS.h>
#include <task.h>
#include <list.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

/* STM32H7 Cube Libraries */

#include <stm32h7xx_hal.h>

/* Global and General Definitions */

#define BUFFER_SIZE							64
#define DATA_SIZE								512
#define SAMPLE_SIZE							1024				
#define CHANNEL_COUNT						4
#define MIC_COUNT								(CHANNEL_COUNT * 2)

#define SENSOR_MIC_ID						0x01
#define SENSOR_GPS_ID						0x02
#define SENSOR_LORA_ID						0x03
#define SENSOR_IMU_ID						0x04

#define FILE									__FILE__
#define LINE									__LINE__

#define STATUS(status) 						((status == HAL_ERROR) 		? "ERROR" 	: 	\
													 (status == HAL_BUSY) 		? "BUSY"		:	\
													 (status == HAL_TIMEOUT) 	? "TIMEOUT" :	\
													 "UNDEFINED")

/* FreeRTOS-specific Definitions */

#define TASK_MIC_NAME_N						"MicReadingNorth"
#define TASK_MIC_NAME_E						"MicReadingEast"
#define TASK_MIC_NAME_S						"MicReadingSouth"
#define TASK_MIC_NAME_W						"MicReadingWest"
#define TASK_GPS_NAME						"GPSReading"
#define TASK_IMU_NAME						"IMUReading"
#define TASK_SD_NAME							"SDCardWriting"
#define TASK_SERVO_NAME						"ServoDriving"
#define TASK_LORA_NAME						"LoRaTransmitting"
#define TASK_SYSTEM_NAME					"SystemChecking"
#define TASK_LED_NAME						"LEDUpdating"
#define TASK_WATCHDOG_NAME					"WatchDogTiming"

#define TASK_MIC_STACK						8192
#define TASK_GPS_STACK						8192
#define TASK_IMU_STACK						8192
#define TASK_SD_STACK						8192
#define TASK_SERVO_STACK					8192
#define TASK_LORA_STACK						8192
#define TASK_SYSTEM_STACK					8192
#define TASK_LED_STACK						8192
#define TASK_WATCHDOG_STACK				8192

#define TASK_MIC_PRIORITY					1
#define TASK_GPS_PRIORITY					1
#define TASK_IMU_PRIORITY					1
#define TASK_SD_PRIORITY					1
#define TASK_SERVO_PRIORITY				2
#define TASK_LORA_PRIORITY					2
#define TASK_SYSTEM_PRIORITY				3
#define TASK_LED_PRIORITY					3
#define TASK_WATCHDOG_PRIORITY			3

/*****************************************************************************/
/*****************************************************************************/

/**
 * PLL1_M: 5					(25MHz / 5 = 5MHz)
 * PLL1_N: 80					(5MHz × 80 = 400MHz)
 * PLL1_P: 4					(400MHz / 4 = 100MHz) → SYSCLK
 * PLL1_Q: 8					(400MHz / 8 = 50MHz)  → For SDMMC, USB, RNG
 * PLL1_R: 8					(400MHz / 8 = 50MHz)  → For SPI, I2S
 * 
 * AHB Prescaler: 1			HCLK = 100MHz
 * APB1 Prescaler: 2			PCLK1 = 50MHz
 * APB2 Prescaler: 2			PCLK2 = 50MHz
 * APB3 Prescaler: 2			PCLK3 = 50MHz
 * APB4 Prescaler: 2			PCLK4 = 50MHz
 */

#define OSC_TYPE								RCC_OSCILLATORTYPE_HSE
#define OSC_HSE_STATE						RCC_HSE_ON
#define OSC_PLL_STATE						RCC_PLL_ON
#define OSC_PLL_SRC							RCC_PLLSOURCE_HSE
#define OSC_PLLM								5
#define OSC_PLLN								80
#define OSC_PLLP								4
#define OSC_PLLQ								8
#define OSC_PLLR								8
#define OSC_PLLRGE							RCC_PLL1VCIRANGE_2
#define OSC_PLLVCOSEL						RCC_PLL1VCOWIDE
#define OSC_PLLFRACN							0

#define CLOCK_TYPE							RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | \
													RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | \
													RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_D3PCLK1
#define CLOCK_SOURCE							RCC_SYSCLKSOURCE_PLLCLK
#define CLOCK_SYS_DIV						RCC_SYSCLK_DIV1
#define CLOCK_AHB_DIV						RCC_HCLK_DIV1
#define CLOCK_APB1_DIV						RCC_APB1_DIV2
#define CLOCK_APB2_DIV						RCC_APB2_DIV2
#define CLOCK_APB3_DIV						RCC_APB3_DIV2
#define CLOCK_APB4_DIV						RCC_APB4_DIV2

/* Debug Port Definitions */

#define DEBUG_PIN_TX							GPIO_PIN_0	/* GPIOA - UART4 */
#define DEBUG_PIN_RX							GPIO_PIN_1	/* GPIOA - UART4 */
#define DEBUG_PORT							GPIOA
#define DEBUG_PULL							GPIO_NOPULL
#define DEBUG_GPIO_MODE						GPIO_MODE_AF_PP
#define DEBUG_ALTERNATE						GPIO_AF6_UART4
#define DEBUG_INSTANCE						UART4
#define DEBUG_UART_MODE						UART_MODE_RX | UART_MODE_TX								
#define DEBUG_BAUDRATE						115200
#define DEBUG_WORDLEN						UART_WORDLENGTH_8B
#define DEBUG_STOPBITS						UART_STOPBITS_1
#define DEBUG_PARITY							UART_PARITY_NONE
#define DEBUG_HWCONTROL						UART_HWCONTROL_NONE
#define DEBUG_SAMPLING						UART_OVERSAMPLING_16

/* LoRa Module Definitions */

#define LORA_PIN_TX							GPIO_PIN_13	/* GPIOB - UART5 */
#define LORA_PIN_RX							GPIO_PIN_12	/* GPIOB - UART5 */
#define LORA_PIN_AUX							GPIO_PIN_3	/* GPIOD - Input */
#define LORA_PIN_M0							GPIO_PIN_5	/* GPIOD - Output */
#define LORA_PIN_M1							GPIO_PIN_4	/* GPIOD - Output */
#define LORA_PORTB							GPIOB
#define LORA_PORTD							GPIOD
#define LORA_GPIO_MODE_O					GPIO_MODE_OUTPUT_PP
#define LORA_GPIO_MODE_I					GPIO_MODE_INPUT
#define LORA_GPIO_MODE_AF					GPIO_MODE_AF_PP
#define LORA_PULL								GPIO_NOPULL
#define LORA_ALTERNATE						GPIO_AF8_UART5
#define LORA_INSTANCE						UART5
#define LORA_MODE								UART_MODE_RX | UART_MODE_TX
#define LORA_BAUDRATE						115200
#define LORA_WORDLEN							UART_WORDLENGTH_8B
#define LORA_STOPBITS						UART_STOPBITS_1
#define LORA_PARITY							UART_PARITY_NONE
#define LORA_HWCONTROL						UART_HWCONTROL_NONE
#define LORA_SAMPLING						UART_OVERSAMPLING_16

/* GPS Module Definitions */

#define GPS_PIN_RX							GPIO_PIN_7	/* GPIOE - UART7 */
#define GPS_PIN_TX							GPIO_PIN_8	/* GPIOE - UART7 */
#define GPS_PORT								GPIOE
#define GPS_GPIO_MODE						GPIO_MODE_AF_PP
#define GPS_PULL								GPIO_NOPULL
#define GPS_ALTERNATE						GPIO_AF7_UART7
#define GPS_INSTANCE							UART7
#define GPS_UART_MODE						UART_MODE_RX | UART_MODE_TX							
#define GPS_BAUDRATE							9600
#define GPS_WORDLEN							UART_WORDLENGTH_8B
#define GPS_STOPBITS							UART_STOPBITS_1
#define GPS_PARITY							UART_PARITY_NONE
#define GPS_HWCONTROL						UART_HWCONTROL_NONE
#define GPS_SAMPLING							UART_OVERSAMPLING_16

/* SD Card Definitions */

#define SD_PIN_CD								GPIO_PIN_8	/* GPIOA - Output */
#define SD_PIN_DAT0							GPIO_PIN_8	/* GPIOC - SDMMC1 */
#define SD_PIN_DAT1							GPIO_PIN_9	/* GPIOC - SDMMC1 */
#define SD_PIN_DAT2							GPIO_PIN_10	/* GPIOC - SDMMC1 */
#define SD_PIN_DAT3							GPIO_PIN_11	/* GPIOC - SDMMC1 */
#define SD_PIN_CMD							GPIO_PIN_2	/* GPIOD - SDMMC1 */
#define SD_PIN_CLK							GPIO_PIN_12	/* GPIOC - SDMMC1 */
#define SD_PORTA								GPIOA
#define SD_PORTC								GPIOC
#define SD_PORTD								GPIOD
#define SD_GPIO_MODE_AF						GPIO_MODE_AF_PP
#define SD_GPIO_MODE_O						GPIO_MODE_OUTPUT_PP
#define SD_PULL								GPIO_NOPULL
#define SD_ALTERNATE							GPIO_AF12_SDMMC1
#define SD_INSTANCE							SDMMC1
#define SD_BUS_WIDE							SDMMC_BUS_WIDE_4B
#define SD_CLOCK_EDGE						SDMMC_CLOCK_EDGE_RISING
#define SD_POWER_SAVE						SDMMC_CLOCK_POWER_SAVE_DISABLE
#define SD_CLOCKDIV							1
#define SD_HWCONTROL							SDMMC_HARDWARE_FLOW_CONTROL_DISABLE

/* IMU Sensor Definitions */

#define IMU_PIN_NSS							GPIO_PIN_4	/* GPIOA - Output */
#define IMU_PIN_SCK							GPIO_PIN_5	/* GPIOA - SPI1 */
#define IMU_PIN_MISO							GPIO_PIN_6	/* GPIOA - SPI1 */
#define IMU_PIN_MOSI							GPIO_PIN_7	/* GPIOA - SPI1 */
#define IMU_PIN_INT1							GPIO_PIN_2	/* GPIOB - Input */
#define IMU_PIN_INT2							GPIO_PIN_3	/* GPIOA - Input */
#define IMU_PORTA								GPIOA
#define IMU_PORTB								GPIOB
#define IMU_GPIO_MODE_I						GPIO_MODE_INPUT
#define IMU_GPIO_MODE_O						GPIO_MODE_OUTPUT_PP
#define IMU_GPIO_MODE_AF					GPIO_MODE_AF_PP
#define IMU_PULL								GPIO_NOPULL
#define IMU_ALTERNATE						GPIO_AF5_SPI1
#define IMU_INSTANCE							SPI1
#define IMU_SPI_MODE							SPI_MODE_MASTER
#define IMU_DIRECTION						SPI_DIRECTION_2LINES
#define IMU_DATASIZE							SPI_DATASIZE_8BIT
#define IMU_POLARITY							SPI_POLARITY_LOW
#define IMU_PHASE								SPI_PHASE_1EDGE
#define IMU_NSS								SPI_NSS_SOFT
#define IMU_PRESCALER						SPI_BAUDRATEPRESCALER_8
#define IMU_FIRSTBIT							SPI_FIRSTBIT_MSB
#define IMU_TIMODE							SPI_TIMODE_DISABLE

#define IMU_REG_WHO_AM_I					0x0F

#define IMU_NSS_LOW()						(HAL_GPIO_WritePin(IMU_PORTA, IMU_PIN_NSS, RESET))
#define IMU_NSS_HIGH()						(HAL_GPIO_WritePin(IMU_PORTA, IMU_PIN_NSS, SET))

/* Microhone Sensor Definitions */

/**
 * System Clock:								100 MHz
 * DFSDM Clock:								100 MHz / DIVIDER = 3.125 MHz
 * Filter Output Rate:						3.125 MHz / OVERSAMPLING = 48.828 kHz
 * Actual Audio Rate:						48.828 kHz / (ORDER + 1) = 12.207 kHz
 */

#define MIC_PIN_DATAIN0						GPIO_PIN_1	/* GPIOC - DFSDM1 */ 
#define MIC_PIN_DATAIN1						GPIO_PIN_1	/* GPIOB - DFSDM1 */
#define MIC_PIN_DATAIN2						GPIO_PIN_5	/* GPIOC - DFSDM1 */
#define MIC_PIN_DATAIN3						GPIO_PIN_7	/* GPIOC - DFSDM1 */
#define MIC_PIN_CKOUT						GPIO_PIN_0	/* GPIOB - DFSDM1 */
#define MIC_PORTB								GPIOB
#define MIC_PORTC								GPIOC
#define MIC_GPIO_MODE						GPIO_MODE_AF_PP
#define MIC_PULL								GPIO_NOPULL
#define MIC_ALTERNATE						GPIO_AF3_DFSDM1
#define MIC_DIVIDER							32
#define MIC_FILTER_TYPE						DFSDM_CHANNEL_FASTSINC_ORDER
#define MIC_OFFSET							0
#define MIC_RIGHT_SHIFT						0
#define MIC_SINC_ORDER						DFSDM_FILTER_SINC3_ORDER
#define MIC_OVERSAMPLING					64

/* LED Definitions */

#define LED_PIN_HEARTBEAT					GPIO_PIN_0	/* GPIOE - Output */
#define LED_PIN_ERROR						GPIO_PIN_1	/* GPIOE - Output */
#define LED_PIN_DETECT						GPIO_PIN_9	/* GPIOE - Output */
#define LED_PORT								GPIOA
#define LED_MODE								GPIO_MODE_OUTPUT_PP
#define LED_PULL								GPIO_NOPULL

/* Servo Motors Definitions */

#define SERVO_PIN_1							GPIO_PIN_6	/* GPIOB - TIM4 */
#define SERVO_PIN_2							GPIO_PIN_7	/* GPIOB - TIM4 */
#define SERVO_PIN_3							GPIO_PIN_8	/* GPIOB - TIM4 */

/*****************************************************************************/
/*****************************************************************************/

/**
 * Transmit the logs from MCU to PC over serial UART line.
 */
#define printLog(format, ...)																	\
{																										\
	char buffer[BUFFER_SIZE];																	\
																										\
	snprintf(buffer, BUFFER_SIZE, format "\r\n", ##__VA_ARGS__);					\
	HAL_UART_Transmit(&huart4, buffer, strlen(buffer), HAL_MAX_DELAY);			\
}

/**
 * Transmit the HAL error from MCU to PC over serial UART line.
 */
#define printError(status, format, ...)													\
{																										\
	char buffer[BUFFER_SIZE];																	\
																										\
	snprintf(buffer, BUFFER_SIZE, "*** " format 	" (STATUS = %s) " 				\
		"(%s::%d) ***\r\n", ##__VA_ARGS__, STATUS(status), FILE, LINE);			\
	HAL_UART_Transmit(&huart4, buffer, strlen(buffer), HAL_MAX_DELAY);			\
}

/**
 * Transmit the kernel error from MCU to PC over serial UART line.
 */
#define printKernel(format, ...)																\
{																										\
	char buffer[BUFFER_SIZE];																	\
																										\
	snprintf(buffer, BUFFER_SIZE, "*** " format 	" (%s::%d) ***\r\n", 			\
		##__VA_ARGS__, FILE, LINE);															\
	HAL_UART_Transmit(&huart4, buffer, strlen(buffer), HAL_MAX_DELAY);			\
}

/*****************************************************************************/
/*****************************************************************************/

/**
 * Initialize the system oscillator with given parameters.
 */
#define initOscillator(handle, type, hsestate, pllstate, pllsrc, pllm, plln,	\
							  pllp, pllq, pllr, pllrge, pllvcosel, pllfracn)			\
{																										\
	handle.OscillatorType = type;																\
	handle.HSEState = hsestate;																\
	handle.PLL.PLLState = pllstate;															\
	handle.PLL.PLLSource = pllsrc;															\
	handle.PLL.PLLM = pllm;																		\
	handle.PLL.PLLN = plln;																		\
	handle.PLL.PLLP = pllp;																		\
	handle.PLL.PLLQ = pllq;																		\
	handle.PLL.PLLR = pllr;																		\
	handle.PLL.PLLRGE = pllrge;																\
	handle.PLL.PLLVCOSEL = pllvcosel;														\
	handle.PLL.PLLFRACN = pllfracn;															\
}

/**
 * Initialize the system clock with given parameters.
 */
#define initClock(handle, type, source, sysdiv, ahbdiv, apb1div, apb2div,		\
						apb3div, apb4div)															\
{																										\
	handle.ClockType = type;																	\
	handle.SYSCLKSource = source;																\
	handle.SYSCLKDivider = sysdiv;															\
	handle.AHBCLKDivider = ahbdiv;															\
	handle.APB1CLKDivider = apb1div;															\
	handle.APB2CLKDivider = apb2div;															\
	handle.APB3CLKDivider = apb3div;															\
	handle.APB4CLKDivider = apb4div;															\
}

/**
 * Initialize GPIO peripheral with given parameters.
 */
#define initGPIO(handle, pin, mode, pull, alternate)									\
{																										\
	handle.Pin = pin;																				\
	handle.Mode = mode;																			\
	handle.Pull = pull;																			\
	handle.Speed = GPIO_SPEED_FREQ_HIGH;													\
	if (alternate != NULL)																		\
	{																									\
		handle.Alternate = alternate;															\
	}																									\
}

/**
 * Initialize UART peripheral with given parameters.
 */
#define initUART(handle, instance, baudrate, mode, wordlen,	stopbits, 			\
					  parity, hwcontrol, sampling)											\
{																										\
	handle.Instance = instance;																\
	handle.Init.BaudRate = baudrate;															\
	handle.Init.Mode = mode;																	\
	handle.Init.WordLength = wordlen;														\
	handle.Init.StopBits = stopbits;															\
	handle.Init.Parity = parity;																\
	handle.Init.HwFlowCtl = hwcontrol;														\
	handle.Init.OverSampling = sampling;													\
}

/**
 * Initialize base TIM peripheral with given parameters.
 */
#define initBaseTIM(handle, instance, prescaler, period, clockdiv, 				\
						  countermode, repitation, autoreload)								\
{																										\
	handle.Instance = instance;																\
	handle.Init.Prescaler = prescaler;														\
	handle.Init.CounterMode = countermode;													\
	handle.Init.Period = period;																\
	handle.Init.ClockDivision = clockdiv;													\
	handle.Init.RepetitionCounter = repitation;											\
	handle.Init.AutoReloadPreload = autoreload;											\
}

/**
 * Initialize DMA peripheral with given parameters.
 */
#define initDMA(handle, instance, stream, direction, periphinc, meminc, 		\
					 periphdataalign, memdataalign, mode, priority)						\
{																										\
	handle.Instance = instance;																\
	handle.Init.Request = stream;																\
	handle.Init.Direction = direction;														\
	handle.Init.PeriphInc = periphinc;														\
	handle.Init.MemInc = meminc;																\
	handle.Init.PeriphDataAlignment = periphdataalign;									\
	handle.Init.MemDataAlignment = memdataalign;											\
	handle.Init.Mode = mode;																	\
	handle.Init.Priority = priority;															\
}

/**
 * Initialize SPI peripheral with given parameters.
 */
#define initSPI(handle, instance, mode, direction, datasize, polarity, 			\
					 phase, nss, prescaler, firstbit, timode)								\
{																										\
	handle.Instance = instance;																\
	handle.Init.Mode = mode;																	\
	handle.Init.Direction = direction;														\
	handle.Init.DataSize = datasize;															\
	handle.Init.CLKPolarity = polarity;														\
	handle.Init.CLKPhase = phase;																\
	handle.Init.NSS = nss;																		\
	handle.Init.BaudRatePrescaler = prescaler;											\
	handle.Init.FirstBit = firstbit;															\
	handle.Init.TIMode = timode;																\
}

/**
 * Initialize SDMMC peripheral with given parameters.
 */
#define initSDMMC(handle, instance, buswide, clockedge, powersave, 				\
						clockdiv, hwcontrol)														\
{																										\
	handle.Instance = instance;																\
	handle.Init.BusWide = buswide;															\
	handle.Init.ClockEdge = clockedge;														\
	handle.Init.ClockPowerSave = powersave;												\
	handle.Init.ClockDiv = clockdiv;															\
	handle.Init.HardwareFlowControl = hwcontrol;											\
}

/**
 * Initialize DFSDM peripheral with given parameters
 */
#define initDFSDMChannel(handle, i, divider, filtertype, offset, rightshift)	\
{																										\
	handle[i].Instance = DFSDM1_Channel0 + i;												\
	handle[i].Init.OutputClock.Activation = ENABLE;										\
	handle[i].Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;	\
	handle[i].Init.OutputClock.Divider = divider;										\
	handle[i].Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;				\
	handle[i].Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;				\
	handle[i].Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;				\
	handle[i].Init.Awd.FilterOrder = filtertype;											\
	handle[i].Init.Offset = offset;															\
	handle[i].Init.RightBitShift = rightshift;											\
}

#define initDFSDMFilter(handle, i, sincorder, oversampling)							\
{																										\
	handle[i].Instance = DFSDM1_Filter0 + i;												\
	handle[i].Init.RegularParam.Trigger = DFSDM_FILTER_SW_TRIGGER;					\
	handle[i].Init.RegularParam.FastMode = ENABLE;										\
	handle[i].Init.RegularParam.DmaMode = DISABLE;										\
	handle[i].Init.FilterParam.SincOrder = sincorder;									\
	handle[i].Init.FilterParam.Oversampling = oversampling;							\
	handle[i].Init.FilterParam.IntOversampling = 1;										\
}

/*****************************************************************************/
/*****************************************************************************/

/* Data structures and enumerations */

typedef enum _IMUAccelScale {
	IMU_ACCEL_SCALE_2G = 0,
	IMU_ACCEL_SCALE_4G = 2,
	IMU_ACCEL_SCALE_8G = 3,
	IMU_ACCEL_SCALE_16G = 1
} IMUAccelScale;

typedef enum _IMUGyroScale {
	IMU_GYRO_SCALE_250DPS = 0,
	IMU_GYRO_SCALE_500DPS = 1,
	IMU_GYRO_SCALE_1000DPS = 2,
	IMU_GYRO_SCALE_2000DPS = 3
} IMUGyroScale;
typedef struct _MicData {
	int32_t north[DATA_SIZE];
	int32_t northEast[DATA_SIZE];
	int32_t east[DATA_SIZE];
	int32_t southEast[DATA_SIZE];
	int32_t south[DATA_SIZE];
	int32_t southWest[DATA_SIZE];
	int32_t west[DATA_SIZE];
	int32_t northWest[DATA_SIZE];
} MicData;

typedef struct _GPSData {
	uint8_t UTCTime[BUFFER_SIZE];
	uint8_t latitude[BUFFER_SIZE];
	uint8_t longitude[BUFFER_SIZE];
	uint8_t quality[BUFFER_SIZE];
	uint8_t numSat[BUFFER_SIZE];
	uint8_t altitude[BUFFER_SIZE];
	uint8_t status[BUFFER_SIZE];
	uint8_t speed[BUFFER_SIZE];		/* knots */
	uint8_t course[BUFFER_SIZE];		/* degrees */
	uint8_t date[BUFFER_SIZE];
} GPSData;

typedef struct _IMUData {
	double accelX;			/* mg */
	double accelY;			/* mg */
	double accelZ;			/* mg */
	double gyroX;			/* dps */
	double gyroY;			/* dps */
	double gyroZ;			/* dps */
} IMUData;

typedef struct _DataPackage {
	MicData MicData;
	GPSData GPSData;
	IMUData IMUData;
} DataPackage;

/*****************************************************************************/
/*****************************************************************************/

/* Global static definitions */

extern RCC_OscInitTypeDef iosc;										/* Oscillator */
extern RCC_ClkInitTypeDef iclk;										/* Clock */
extern GPIO_InitTypeDef	igpio;										/* Generic IO */
extern UART_HandleTypeDef huart4;									/* Debug Port */
extern UART_HandleTypeDef huart5;									/* LoRa Module */
extern UART_HandleTypeDef huart7;									/* GPS Module */
extern SPI_HandleTypeDef hspi1;										/* IMU Sensor */
extern SD_HandleTypeDef	hsdmmc1;										/* SD Card */
extern DFSDM_Channel_HandleTypeDef hdfsdm1c[CHANNEL_COUNT];	/* Mic Sensor */
extern DFSDM_Filter_HandleTypeDef hdfsdm1f[CHANNEL_COUNT];	/* Mic Sensor */

extern volatile DataPackage dataPackage;

/*****************************************************************************/
/*****************************************************************************/

/* Function prototypes */

extern void configOscClk(void);
extern void configDebugPort(void);
extern void configIMUSensor(void);
extern void configMicSensors(void);
extern void configGPSModule(void);
extern void configSDCard(void);
extern void configLoRaModule(void);
extern void configServoMotors(void);
extern void configLEDs(void);
extern void configWatchdog(void);

extern void taskMicReadingNorth(void *pvParams);
extern void taskMicReadingEast(void *pvParams);
extern void taskMicReadingSouth(void *pvParams);
extern void taskMicReadingWest(void *pvParams);
extern void taskGPSReading(void *pvParams);
extern void taskIMUReading(void *pvParams);
extern void taskSDCardWriting(void *pvParams);
extern void taskLoRaTransmitting(void *pvParams);
extern void taskSystemChecking(void *pvParams);
extern void taskServoDriving(void *pvParams);
extern void taskLEDUpdating(void *pvParams);
extern void taskWatchdogTiming(void *pvParams);

extern void SysTick_Handler(void);
extern void xPortSysTickHandler(void);
extern void vApplicationIdleHook(void);

extern void __parse_nmea_sentences(uint8_t *sentences, GPSData *result);
extern void __write_to_imu_reg(uint8_t reg, uint8_t data);
extern uint8_t __read_from_imu_reg(uint8_t reg);

#endif /* FIRMWARE_H */
