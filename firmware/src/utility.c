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

/**
 * Parse the NMEA sentences into sentences.
 */
void __parse_nmea_sentences(uint8_t *sentences, GPSData *result)
{
	uint8_t *sentence, *token;

	/* Tokenize the each sentence. */
	sentence = strtok(sentences, "\n");
	while (sentence != NULL)
	{
		if (strncmp(sentence, "$GPGGA", 6) == 0)
		{
			/* Get the remaining string and then parse. */
			token = strchr(sentence, sentence[7]);

			/* Get the UTC time (123519 --> 12:35:19). */
			result->UTCTime = strchr(token, ',');
			token = strchr(token, ',');
			token = strchr(token, token[1]);

			/* Get the latitude (4807.038 --> 48^ 07.038' N). */
			result->latitude = strchr(token, ',');
			token = strchr(token, ',');
			token = strchr(token, token[1]);
			token = strchr(token, ',');
			token = strchr(token, token[1]);

			/* Get the longitude (01131.000 --> 11^ 31.000' East). */
			result->longitude = strchr(token, ',');
			token = strchr(token, ',');
			token = strchr(token, token[1]);
			token = strchr(token, ',');
			token = strchr(token, token[1]);

			/* Get the fix quality (1 --> GPS fix). */
			result->quality = strchr(token, ',');
			token = strchr(token, ',');
			token = strchr(token, token[1]);

			/* Get the number of satellites used. */
			result->numSat = strchr(token, ',');
			token = strchr(token, ',');
			token = strchr(token, token[1]);
			token = strchr(token, ',');
			token = strchr(token, token[1]);

			/* Get the altitude (545.4 --> 545.4 meters). */
			result->altitude = strchr(token, ',');
		}
		else if (strncmp(sentence, "$GPRCM", 6))
		{
			/* Get the remaining string and then parse. */
			token = strchr(sentence, sentence[7]);
			token = strchr(token, ',');
			token = strchr(token, token[1]);

			/* Get the status (A --> valid, V --> invalid). */
			result->status = strchr(token, ',');
			token = strchr(token, ',');
			token = strchr(token, token[1]);
			token = strchr(token, ',');
			token = strchr(token, token[1]);
			token = strchr(token, ',');
			token = strchr(token, token[1]);
			token = strchr(token, ',');
			token = strchr(token, token[1]);
			token = strchr(token, ',');
			token = strchr(token, token[1]);

			/* Get the speed over ground (knots). */
			result->speed = strchr(token, ',');
			token = strchr(token, ',');
			token = strchr(token, token[1]);

			/* Get the course over ground (degrees). */
			result->course = strchr(token, ',');
			token = strchr(token, ',');
			token = strchr(token, token[1]);

			/* Get the date (230394 --> 23 March 1994). */
			result->course = strchr(token, ',');
		}
		/* Proceed to next sentence in buffer. */
		sentence = strtok(NULL, "\n");
	}
}

/**
 * Write the data into IMU sensor registers.
 */
void __write_to_imu_reg(uint8_t reg, uint8_t data)
{
	HAL_StatusTypeDef status;
	uint8_t tx_data[2] = {reg, data};	/* address then data */

	IMU_NSS_LOW();
	status = HAL_SPI_Transmit(&hspi1, tx_data, 2, HAL_MAX_DELAY);
	if (status != HAL_OK)
	{
		printError(status, "Failed to write data into IMU register!");
	}
	IMU_NSS_HIGH();
}

/**
 * Read the data from IMU sensor registers.
 */
uint8_t __read_from_imu_reg(uint8_t reg)
{
	HAL_StatusTypeDef status;
	uint8_t tx_data[2] = {reg | 0x80, 0x00};
	uint8_t rx_data[2] = {0};

	IMU_NSS_LOW();
	status = HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2,
		HAL_MAX_DELAY);
	if (status != HAL_OK)
	{
		printError(status, "Failed to read data from IMU register!");
	}
	IMU_NSS_HIGH();

	return rx_data[1];
}
