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

#define TASK_MIC_NAME_N				"taskMicSensorNorth"
#define TASK_MIC_NAME_E				"taskMicSensorEast"
#define TASK_MIC_NAME_S				"taskMicSensorSouth"
#define TASK_MIC_NAME_W				"taskMicSensorWest"
#define TASK_GPS_NAME				"taskGPSModule"
#define TASK_IMU_NAME				"taskIMUSensor"
#define TASK_SD_NAME					"taskSDCard"
#define TASK_SERVO_NAME				"taskServoMotors"
#define TASK_LORA_NAME				"taskLoRaModule"
#define TASK_SYSTEM_NAME			"taskSystemCheck"
#define TASK_LED_NAME				"taskLEDs"
#define TASK_WATCHDOG_NAME			"taskWatchdog"

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
 