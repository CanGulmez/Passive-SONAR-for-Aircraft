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

/* Global and shared variables. */

MicData micData = {0};
GPSData gpsData = {0};
IMUData imuData = {0};
DataPackage dataPackage = {0};

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
	printLog("I'm taskMicReadingEast() task!");

	for (;;)
	{

	}
	vTaskDelete(NULL);
}

/**
 * Read microphone data from the south channel.
 */
void taskMicReadingSouth(void *pvParams)
{
	printLog("I'm taskMicReadingSouth() task!");

	for (;;)
	{
		
	}	
	vTaskDelete(NULL);
}

/**
 * Read microphone data from the west channel.
 */
void taskMicReadingWest(void *pvParams)
{
	printLog("I'm taskMicReadingWest() task!");

	for (;;)
	{
		
	}	
	vTaskDelete(NULL);
}

/**
 * Read GPS data from the existing module.
 */
void taskGPSReading(void *pvParams)
{
	int i;
	uint8_t buffer[DATA_SIZE];
	HAL_StatusTypeDef status;

	printLog("I'm taskGPSReading() task!");

	for (;;)
	{
		/* Recieve the GPS sentences over serial line. */
		status = HAL_UART_Receive(&huart7, buffer, DATA_SIZE, 
			HAL_MAX_DELAY);
		if (status != HAL_OK)
		{
			printError(status, "Failed to receive GPS sentences!\n");
		}

		/* Parse the sentences. */
		__parse_nmea_sentences(buffer, &gpsData);
	}	
	vTaskDelete(NULL);
}

/**
 * Read IMU data from the existing sensor.
 */
void taskIMUReading(void *pvParams)
{
	uint8_t whoami;

	printLog("I'm taskIMUReading() task!");

	/* Confirm that IMU sensor is registered. */
	whoami = __read_reg_from_imu(IMU_REG_WHO_AM_I);
	if (whoami != 0x6A)
	{
		printLog("Failed to confirm the IMU sensor!");
	}
	printLog("Confirmed the existing of IMU sensor.");

	/* Configure the accelerometer and gyroscope. */
	__write_to_imu_reg(IMU_REG_CTRL1_XL, 0x6C);	/* 416Hz, 16g */
	__write_to_imu_reg(IMU_REG_CTRL2_G, 0x6C);	/* 416Hz, 2000dps */
	__write_to_imu_reg(IMU_REG_CTRL3_C, 0x44);	/* BDU and IF_INC */

	printLog("Configured the accelerometer (416Hz, 16g) "
				"and gyroscope (416Hz, 2000dps).");

	for (;;)
	{
		/* Read the acceleromenter, gyroscope and temperature. */
		__read_accel_from_imu(&imuData);
		__read_gyro_from_imu(&imuData);
		__read_temp_from_imu(&imuData);
	}	
	vTaskDelete(NULL);
}

/**
 * Write the backup data into existing SD card.
 */
void taskSDCardWriting(void *pvParams)
{
	uint8_t buffer[DATA_SIZE]= {0};
	HAL_StatusTypeDef status;
	HAL_SD_CardInfoTypeDef info;
	static uint32_t csector = START_SECTOR;

	printLog("I'm taskSDCardWriting() task!");

	/* Initialize the SD card handler. */
	status = HAL_SD_Init(&hsdmmc1);
	if (status != HAL_OK)
	{
		printError(status, "Failed to initialize SD card!");
	}

	/* Get the SD card information. */
	HAL_SD_GetCardInfo(&hsdmmc1, &info);

	printLog("SD Card info:");
	printLog("	Capacity		: %lu sectors", info.BlockNbr);
	printLog("	Sector Size	: %lu bytes", info.BlockSize);
	printLog("	Totol Size	: %lu MB",
		((info.BlockNbr * info.BlockSize) / (1024 * 1024)));
	printLog("	Card Type	: %s",
		(info.CardType == CARD_SDSC) ? "SDSC" :
		(info.CardType == CARD_SDHC_SDXC) ? "SDHC/SDXC" :
		"UNKNOWN");
	printLog("	Speed			: %.2f Mo/s",
		(info.CardSpeed == CARD_NORMAL_SPEED) ? 12.5 :
		(info.CardSpeed == CARD_HIGH_SPEED) ? 25.0 : 50.0);

	for (;;)
	{
		/* Fill the backup buffer. */
		memcpy(buffer, "will be implemented later", 50);

		/* Write the backup data blocks to SD card. */
		status = HAL_SD_WriteBlocks(
			&hsdmmc1,			/* SD card interface */
			buffer,				/* data that will be written */
			csector,				/* current sector number */
			1,						/* just 1 sector at each op */
			HAL_MAX_DELAY		/* give some time */
		);
		if (status != HAL_OK)
		{
			printError(status, "Failed to write data blocks to "
				"SD Card at sector %lu!", csector);
		}
		csector++;
	}
	vTaskDelete(NULL);
}

/**
 * Drive the servo motors to vectorize system.
 */
void taskServoDriving(void *pvParams)
{
	printLog("I'm taskServoDriving() task!");

	for (;;)
	{
		
	}	
	vTaskDelete(NULL);
}

/**
 * Transmit the LoRa package down to ground.
 */
void taskLoRaTransmitting(void *pvParams)
{
	printLog("I'm taskLoRaTransmitting() task!");

	for (;;)
	{
		
	}	
	vTaskDelete(NULL);
}

/**
 * Check the system healthy with predefined conditions.
 */
void taskSystemChecking(void *pvParams)
{
	printLog("I'm taskSystemChecking() task!");

	for (;;)
	{
		
	}	
	vTaskDelete(NULL);
}

/**
 * Blink the LEDs to give visual insights.
 */
void taskLEDUpdating(void *pvParams)
{
	printLog("I'm taskLEDUpdating() task!");

	for (;;)
	{		
		
	}	
	vTaskDelete(NULL);
}

/**
 * Handle the watch dog to maintain system wakeup. 
 */
void taskWatchdogTiming(void *pvParams)
{
	printLog("I'm taskWatchDogTiming() task!");
	
	for (;;)
	{		
		
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
