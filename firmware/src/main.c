/**
 ******************************************************************************
 * @file 	main.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Main source file of the firmware.
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

int main(void)
{
	int i;
	BaseType_t status;

	HAL_Init();
	
	/* Enable the peripheral clocks. */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_UART4_CLK_ENABLE();
	__HAL_RCC_UART5_CLK_ENABLE();
	__HAL_RCC_UART7_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_SDMMC1_CLK_ENABLE();
	__HAL_RCC_DFSDM1_CLK_ENABLE();

	/* Initialize and configure the peripherals. */
	configOscClk();
	configDebugPort();
	configIMUSensor();
	configMicSensors();
	configGPSModule();
	configSDCard();
	configLoRaModule();
	configLEDs();
	configWatchdog();

	printLog("Firmware started successfully...");

	/* Create the taskMicReadingNorth() task. */
	status = xTaskCreate(taskMicReadingNorth, TASK_MIC_NAME_N, 
		TASK_MIC_STACK, NULL, TASK_MIC_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_MIC_NAME_N);
	}

	/* Create the taskMicReadingEast() task. */
	status = xTaskCreate(taskMicReadingEast, TASK_MIC_NAME_E, 
		TASK_MIC_STACK, NULL, TASK_MIC_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_MIC_NAME_E);
	}

	/* Create the taskMicReadingSouth() task. */
	status = xTaskCreate(taskMicReadingSouth, TASK_MIC_NAME_S, 
		TASK_MIC_STACK, NULL, TASK_MIC_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_MIC_NAME_S);
	}

	/* Create the taskMicReadingWest() task. */
	status = xTaskCreate(taskMicReadingWest, TASK_MIC_NAME_W, 
		TASK_MIC_STACK, NULL, TASK_MIC_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_MIC_NAME_W);
	}

	/* Create the taskGPSReading() task. */
	status = xTaskCreate(taskGPSReading, TASK_GPS_NAME, 
		TASK_GPS_STACK, NULL, TASK_GPS_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_GPS_NAME);
	}

	/* Create the taskIMUReading() task. */
	status = xTaskCreate(taskIMUReading, TASK_IMU_NAME, 
		TASK_IMU_STACK, NULL, TASK_IMU_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_IMU_NAME);
	}

	/* Create the taskSDCardWriting() task. */
	status = xTaskCreate(taskSDCardWriting, TASK_SD_NAME, 
		TASK_SD_STACK, NULL, TASK_SD_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_SD_NAME);
	}

	/* Create the taskServoDriving() task. */
	status = xTaskCreate(taskServoDriving, TASK_SERVO_NAME, 
		TASK_SERVO_STACK, NULL, TASK_SERVO_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_SERVO_NAME);
	}

	/* Create the taskLoRaTransmitting() task. */
	status = xTaskCreate(taskLoRaTransmitting, TASK_LORA_NAME, 
		TASK_LORA_STACK, NULL, TASK_LORA_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_LORA_NAME);
	}

	/* Create the taskSystemChecking() task. */
	status = xTaskCreate(taskSystemChecking, TASK_SYSTEM_NAME, 
		TASK_SYSTEM_STACK, NULL, TASK_SYSTEM_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_SYSTEM_NAME);
	}

	/* Create the taskLEDUpdating() task. */
	status = xTaskCreate(taskLEDUpdating, TASK_LED_NAME, 
		TASK_LED_STACK, NULL, TASK_LED_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_LED_NAME);
	}

	/* Create the taskWatchDogTiming() task. */
	status = xTaskCreate(taskWatchdogTiming, TASK_WATCHDOG_NAME, 
		TASK_WATCHDOG_STACK, NULL, TASK_WATCHDOG_PRIORITY, NULL);
	if (status != pdPASS)
	{
		printKernel("Failed to create '%s' task!", TASK_WATCHDOG_NAME);
	}

	/* Start the task schedular. */
	vTaskStartScheduler();

	while (1);
}
