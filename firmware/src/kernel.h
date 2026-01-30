/**
 ******************************************************************************
 * @file 	kernel.h
 * @author 	Ahmet Can GULMEZ
 * @brief 	Kernel headers of the firmware.
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

/* FreeRTOS-specific Definitions */

#define TASK_MIC_NAME_N				"MicReadingNorth"
#define TASK_MIC_NAME_E				"MicReadingEast"
#define TASK_MIC_NAME_S				"MicReadingSouth"
#define TASK_MIC_NAME_W				"MicReadingWest"
#define TASK_GPS_NAME				"GPSReading"
#define TASK_IMU_NAME				"IMUReading"
#define TASK_SD_NAME					"SDCardWriting"
#define TASK_SERVO_NAME				"ServoDriving"
#define TASK_LORA_NAME				"LoRaTransmitting"
#define TASK_SYSTEM_NAME			"SystemChecking"
#define TASK_LED_NAME				"LEDUpdating"
#define TASK_WATCHDOG_NAME			"WatchDogTiming"

#define TASK_MIC_STACK				10240
#define TASK_GPS_STACK				10240
#define TASK_IMU_STACK				10240
#define TASK_SD_STACK				10240
#define TASK_SERVO_STACK			10240
#define TASK_LORA_STACK				10240
#define TASK_SYSTEM_STACK			10240
#define TASK_LED_STACK				10240
#define TASK_WATCHDOG_STACK		10240

#define TASK_MIC_PRIORITY			3
#define TASK_GPS_PRIORITY			3
#define TASK_IMU_PRIORITY			3
#define TASK_SD_PRIORITY			3
#define TASK_SERVO_PRIORITY		2
#define TASK_LORA_PRIORITY			2
#define TASK_SYSTEM_PRIORITY		1
#define TASK_LED_PRIORITY			1
#define TASK_WATCHDOG_PRIORITY	1
