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
	for (;;)
	{
		taskENTER_CRITICAL();	/* enter into critical code section */

		printLog("I'm taskMicReadingNorth() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */
		
		printLog("I'm taskMicReadingEast() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */

		printLog("I'm taskMicReadingSouth() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */

		printLog("I'm taskMicReadingWest() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */

		printLog("I'm taskGPSReading() task!");
		
		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */

		printLog("I'm taskIMUReading() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */

		printLog("I'm taskSDCardWriting() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */

		printLog("I'm taskServoDriving() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */

		printLog("I'm taskLoRaTransmitting() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */

		printLog("I'm taskSystemChecking() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */
		
		printLog("I'm taskLEDUpdating() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
		taskENTER_CRITICAL();	/* enter into critical code section */
		
		printLog("I'm taskWatchDogTiming() task!");

		taskEXIT_CRITICAL();		/* exit from critical code section */
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
