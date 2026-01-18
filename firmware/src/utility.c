/**
 ******************************************************************************
 * @file 	utility.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Common utilities of the firmware.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2025 Ahmet Can GULMEZ.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 *****************************************************************************
 */

#include "main.h"

/* Global and shared variables */

/**
 * Parse the NMEA sentences into sentences.
 */
// void __parse_nmea_sentences(uint8_t *sentences, GPSData *result)
// {
// 	uint8_t *sentence, *token;

// 	/* Tokenize the each sentence. */
// 	sentence = strtok(sentences, "\n");
// 	while (sentence != NULL)
// 	{
// 		if (strncmp(sentence, "$GPGGA", 6) == 0)
// 		{
// 			/* Get the remaining string and then parse. */
// 			token = strchr(sentence, sentence[7]);

// 			/* Get the UTC time (123519 --> 12:35:19). */
// 			result->UTCTime = strchr(token, ',');
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);

// 			/* Get the latitude (4807.038 --> 48^ 07.038' N). */
// 			result->latitude = strchr(token, ',');
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);

// 			/* Get the longitude (01131.000 --> 11^ 31.000' East). */
// 			result->longitude = strchr(token, ',');
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);

// 			/* Get the fix quality (1 --> GPS fix). */
// 			result->quality = strchr(token, ',');
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);

// 			/* Get the number of satellites used. */
// 			result->numSat = strchr(token, ',');
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);

// 			/* Get the altitude (545.4 --> 545.4 meters). */
// 			result->altitude = strchr(token, ',');
// 		}
// 		else if (strncmp(sentence, "$GPRCM", 6))
// 		{
// 			/* Get the remaining string and then parse. */
// 			token = strchr(sentence, sentence[7]);
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);

// 			/* Get the status (A --> valid, V --> invalid). */
// 			result->status = strchr(token, ',');
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);

// 			/* Get the speed over ground (knots). */
// 			result->speed = strchr(token, ',');
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);

// 			/* Get the course over ground (degrees). */
// 			result->course = strchr(token, ',');
// 			token = strchr(token, ',');
// 			token = strchr(token, token[1]);

// 			/* Get the date (230394 --> 23 March 1994). */
// 			result->course = strchr(token, ',');
// 		}
// 		/* Proceed to next sentence in buffer. */
// 		sentence = strtok(NULL, "\n");
// 	}
// }

/**
 * Parse the NMEA sentences coming from GPS module.
 */
void __parse_nmea_sentences(uint8_t *buffer, GPSData *gpsData)
{
	
}

/**
 * Write the data to IMU sensor registers.
 */
void __write_to_imu_reg(uint8_t reg, uint8_t data)
{
	HAL_StatusTypeDef status;
	uint8_t tx_data[2] = {reg | 0x00, data};	/* address then data */

	IMU_NSS_LOW();
	status = HAL_SPI_Transmit(&hspi1, tx_data, 2, HAL_MAX_DELAY);
	if (status != HAL_OK)
	{
		printError(status, "Failed to write the data to IMU register!");
	}
	IMU_NSS_HIGH();
}

/**
 * Read the IMU sensor registers.
 */
uint8_t __read_reg_from_imu(uint8_t reg)
{
	HAL_StatusTypeDef status;
	uint8_t tx_data[2] = {reg | 0x80, 0x00};	/* address then data */
	uint8_t rx_data[2] = {0};

	IMU_NSS_LOW();
	status = HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2,
		HAL_MAX_DELAY);
	if (status != HAL_OK)
	{
		printError(status, "Failed to read the IMU register!");
	}
	IMU_NSS_HIGH();

	return rx_data[1];
}

/**
 * Read the acceleromenter from IMU sensor.
 */
void __read_accel_from_imu(IMUData *imuData)
{
	int i;
	uint8_t data[6];
	
	/* Read the associated registers in order. */
	for (i = 0; i < 6; i++)
	{
		data[i] = __read_reg_from_imu(IMU_REG_OUTX_L_XL + i);
	}

	/* Combine bytes (little endian) then convert to mg. */
	imuData->accelX = ((int16_t)((data[1] << 8) | data[0])) * 0.488;
	imuData->accelY = ((int16_t)((data[3] << 8) | data[2])) * 0.488;
	imuData->accelZ = ((int16_t)((data[5] << 8) | data[4])) * 0.488;
}

/**
 * Read the gyroscope from IMU sensor.
 */
void __read_gyro_from_imu(IMUData *imuData)
{
	int i;
	uint8_t data[6];
	
	/* Read the associated registers in order. */
	for (i = 0; i < 6; i++)
	{
		data[i] = __read_reg_from_imu(IMU_REG_OUTX_L_G + i);
	}

	/* Combine bytes (little endian) then convert to dps. */
	imuData->gyroX = ((int16_t)((data[1] << 8) | data[0])) * 0.07;
	imuData->gyroY = ((int16_t)((data[3] << 8) | data[2])) * 0.07;
	imuData->gyroZ = ((int16_t)((data[5] << 8) | data[4])) * 0.07;
}

/**
 * Read the temperature from IMU sensor.
 */
void __read_temp_from_imu(IMUData *imuData)
{
	uint8_t temp_l, temp_h;

	temp_l = __read_reg_from_imu(IMU_REG_OUT_TEMP_L);
	temp_h = __read_reg_from_imu(IMU_REG_OUT_TEMP_H);

	imuData->temp = (((temp_h << 8) | temp_l) / 256.0) + 25.0;
}
