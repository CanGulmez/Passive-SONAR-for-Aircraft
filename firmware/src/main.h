/**
 ******************************************************************************
 * @file 	main.h
 * @author 	Ahmet Can GULMEZ
 * @brief 	Main headers of the firmware.
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

/* Libraries */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include <FreeRTOS.h>
#include <task.h>
#include <list.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

#include <stm32h7xx_hal.h>

#include "./kernel.h"
#include "./peripheral.h"

/* Global and General Definitions */

#define BUFFER_SIZE			64
#define DATA_SIZE				512
#define SAMPLE_SIZE			(DATA_SIZE * 2)	
#define CHANNEL_COUNT		4
#define MIC_COUNT				(CHANNEL_COUNT * 2)
#define START_SECTOR			0x1000	/* 4096 bytes, after 2MB of FAT/reserved */

#define FILE					__FILE__
#define LINE					__LINE__
#define PACKED					__attribute__((packed))
#define NORETURN				__attribute__((noreturn))
#define DEPRECATED			__attribute__((deprecated))
#define ALIGNED(n)			__attribute__((aligned(n)))
#define SECTION(s)			__attribute__((section(s)))

#define STATUS(status) 		((status == HAL_ERROR) 		? "ERROR" 	: 	\
									 (status == HAL_BUSY) 		? "BUSY"		:	\
									 (status == HAL_TIMEOUT)	? "TIMEOUT" :	\
									  "UNDEFINED")

/* Some Useful Shorthands */

#define IMU_NSS_LOW()		(HAL_GPIO_WritePin(IMU_PORTA, IMU_PIN_NSS, RESET))
#define IMU_NSS_HIGH()		(HAL_GPIO_WritePin(IMU_PORTA, IMU_PIN_NSS, SET))

#define TASK_NUM()			(uxTaskGetNumberOfTasks())
#define FREE_HEAP()			(xPortGetFreeHeapSize())

/*****************************************************************************/
/*****************************************************************************/

/* Peripheral Handlers */

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

/*****************************************************************************/
/*****************************************************************************/

/* Data Structures and Enumerations */

PACKED typedef struct _MicData 
{
	int32_t north[DATA_SIZE];
	int32_t northEast[DATA_SIZE];
	int32_t east[DATA_SIZE];
	int32_t southEast[DATA_SIZE];
	int32_t south[DATA_SIZE];
	int32_t southWest[DATA_SIZE];
	int32_t west[DATA_SIZE];
	int32_t northWest[DATA_SIZE];
} MicData;

PACKED typedef struct _GPSData 
{
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

PACKED typedef struct _IMUData 
{
	double accelX;		/* mg */
	double accelY;		/* mg */
	double accelZ;		/* mg */
	double gyroX;		/* dps */
	double gyroY;		/* dps */
	double gyroZ;		/* dps */
	double temp;		/* C */ 
} IMUData;

PACKED typedef struct _PayloadData
{
	MicData MicData;
	GPSData GPSData;
	IMUData IMUData;
} PayloadData;

extern MicData micData;
extern GPSData gpsData;
extern IMUData imuData;
extern PayloadData payloadData;

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
 * Initialize DFSDM channel with given parameters.
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

/**
 * Initialize DFSDM filter with given parameters.
 */
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

/* Function Prototypes */

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

extern void __parse_nmea_sentences(uint8_t *buffer, GPSData *gpsData);
extern void __write_to_imu_reg(uint8_t reg, uint8_t data);
extern uint8_t __read_reg_from_imu(uint8_t reg);
extern void __read_accel_from_imu(IMUData *IMUData);
extern void __read_gyro_from_imu(IMUData *IMUData);
extern void __read_temp_from_imu(IMUData *IMUData);
extern void __get_sd_card_info(void);

#endif /* FIRMWARE_H */
