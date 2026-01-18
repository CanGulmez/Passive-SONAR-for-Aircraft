/**
 ******************************************************************************
 * @file 	tasks.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	FreeRTOS tasks of the firmware.
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

#include "main.h"

/* Global and static variables. */

volatile DataPackage dataPackage = {0};

/**
 * Read microphone data from the north channel.
 */
void taskMicReadingNorth(void *pvParams)
{
	uint8_t i;
	int32_t samples[SAMPLE_SIZE];
	HAL_StatusTypeDef status;

	printLog("I'm taskMicReadingNorth() task!");
	/* Start the regular DFSDM filter conversion. */
	status = HAL_DFSDM_FilterRegularStart(&hdfsdm1f[0]);
	if (status != HAL_OK)
	{
		printError(status, "Failed to start DFSDM conversion!");
	}

	for (;;)
	{
		/* Poll the regular conversion simultaneously. */
		for (i = 0; i < SAMPLE_SIZE; i++)
		{
			status = HAL_DFSDM_FilterPollForRegConversion(&hdfsdm1f[0],
				HAL_MAX_DELAY);
			if (status != HAL_OK)
			{
				printError(status, "Failed to poll DFSDM conversion!");
			}

			/* Get the digital MEMS mic data and then shift it. */
			samples[i] = HAL_DFSDM_FilterGetRegularValue(&hdfsdm1f[0], 0);
			samples[i] = samples[i] >> 8;	/* 24-bit data */

			printLog("#%i:		%d", i, samples[i]);
		}
	}	
	vTaskDelete(NULL);
}

/**
 * Read microphone data from the east channel.
 */
void taskMicReadingEast(void *pvParams)
{
	for (;;)
	{
		printLog("I'm taskMicReadingEast() task!");
	}
	vTaskDelete(NULL);
}

/**
 * Read microphone data from the south channel.
 */
void taskMicReadingSouth(void *pvParams)
{
	for (;;)
	{
		printLog("I'm taskMicReadingSouth() task!");
	}	
	vTaskDelete(NULL);
}

/**
 * Read microphone data from the west channel.
 */
void taskMicReadingWest(void *pvParams)
{
	for (;;)
	{
		printLog("I'm taskMicReadingWest() task!");
	}	
	vTaskDelete(NULL);
}

/**
 * Read GPS data from the existing module.
 */
void taskGPSReading(void *pvParams)
{
	for (;;)
	{
		printLog("I'm taskGPSReading() task!");
	}	
	vTaskDelete(NULL);
}

/**
 * Read IMU data from the existing sensor.
 */
void taskIMUReading(void *pvParams)
{
	for (;;)
	{
		printLog("I'm taskIMUReading() task!");
	}	
	vTaskDelete(NULL);
}

/**
 * Write the backup data into existing SD card.
 */
void taskSDCardWriting(void *pvParams)
{
	for (;;)
	{
		printLog("I'm taskSDCardWriting() task!");
	}	
	vTaskDelete(NULL);
}

/**
 * Drive the servo motors to vectorize system.
 */
void taskServoDriving(void *pvParams)
{
	for (;;)
	{
		printLog("I'm taskServoDriving() task!");
	}	
	vTaskDelete(NULL);
}

/**
 * Transmit the LoRa package down to ground.
 */
void taskLoRaTransmitting(void *pvParams)
{
	for (;;)
	{
		printLog("I'm taskLoRaTransmitting() task!");
	}	
	vTaskDelete(NULL);
}

/**
 * Check the system healthy with predefined conditions.
 */
void taskSystemChecking(void *pvParams)
{
	for (;;)
	{
		printLog("I'm taskSystemChecking() task!");
	}	
	vTaskDelete(NULL);
}

/**
 * Blink the LEDs to give visual insights.
 */
void taskLEDUpdating(void *pvParams)
{
	for (;;)
	{		
		printLog("I'm taskLEDUpdating() task!");
	}	
	vTaskDelete(NULL);
}

/**
 * Handle the watch dog to maintain system wakeup. 
 */
void taskWatchdogTiming(void *pvParams)
{
	for (;;)
	{		
		printLog("I'm taskWatchDogTiming() task!");
	}	
	vTaskDelete(NULL);	
}

/**
 * Application idle hook
 */
void vApplicationIdleHook(void)
{
	__WFI();		/* enter the deep sleep! */
}
