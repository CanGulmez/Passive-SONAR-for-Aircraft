/**
 ******************************************************************************
 * @file 	timeouts.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Timeout utilities of AeroSONAR.
 * 
 ******************************************************************************
 * @attention 
 * 
 * Copyright (c) 2026 Ahmet Can GULMEZ.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 ******************************************************************************
 */

#include "main.h"

/**
 * Set the timeout to get the device data simultenously.
 */
gboolean timeout_device_node(gpointer data)
{
	double max_freq;
	int i, arrival;
	int deviceFd;

	/* Read the device node to update 'payloadData' object. */
	deviceFd = GPOINTER_TO_INT(data);
	read_device_node(deviceFd);

	/* Prepare the collected data for signal analysis. */
	convert_payload_to_sample();

	/* Extract the required calculations in here. */
	max_freq = find_dominant_freq();
	arrival = calculate_arrival(max_freq);
	micBeamformed = make_beamforming(max_freq, arrival);
	micVolumest = select_sector();

	/* Make sure the amplitude of signal fits into the frame. */
	dsp_time_scale(&micBeamformed, 128.0, &micBeamformed);

	/* Make the signal analysis. */
	make_signal_analysis(&micBeamformed, arrival);
	printLog("completed the signal analysis operations");

	/* Lastly, redraw the cartesian and polar plots. */
	gtk_widget_queue_draw(micCarPlot);
	gtk_widget_queue_draw(micPolarPlot);
	printLog("requested the plot redraws");

	return G_SOURCE_CONTINUE;
}

/**
 * Set the timeout to get the Keras logs into text view.
 */
gboolean timeout_model_keras_log(gpointer data)
{
	const char *kerasLog;

	/* Get the current text view and then update it. */
	modelTextBuffer = gtk_text_view_get_buffer(
									GTK_TEXT_VIEW(modelTextView));
	kerasLog = get_keras_script_logs(MODEL_LOG_PATH);

	gtk_text_buffer_set_text(modelTextBuffer, kerasLog, -1);
	if ( is_keras_script_running(modelFitPid) )
	{
		return G_SOURCE_CONTINUE;
	}
	else
	{
		return G_SOURCE_REMOVE;
	}
}

/**
 * Set the timeout to record the sensor data into database.
 */
gboolean timeout_db_record(gpointer data)
{
	struct sqlite3 *db;

	db = (struct sqlite3 *) data;
	/* Bind the last sensor data. */
	db_bind_data(db, DATABASE_SENSOR_DATA);

	return G_SOURCE_CONTINUE;
}

/**
 * Set the timeout for navigation updates.
 */
gboolean timeout_nav_update(gpointer data)
{
	char buffer[BUFFER_SIZE];

	/* Update the sensor series and information. */
	// __generic_action_row_update(navSensorRows[0], USED_IMU_SENSOR);
	// printf("%.2f, %.2f, %.2f\n", payloadData.imuAccelX, payloadData.imuAccelY, payloadData.imuAccelZ);
	/* Update the acceloremeter output. */
	// snprintf(
	// 	buffer, BUFFER_SIZE, "[%.2f, %.2f, %.2f]", 
	// 	payloadData.imuAccelX, payloadData.imuAccelY, payloadData.imuAccelZ
	// );
	// __generic_action_row_update(navSensorRows[1], "Running");
	// __generic_action_row_update(navSensorRows[2], buffer);
	printf("%s\n", payloadData.gpsUTCTime);
	/* Update the gyroscope output. */
	// snprintf(
	// 	buffer, BUFFER_SIZE, "[%.2f, %.2f, %.2f]", 
	// 	payloadData.imuGyroX, payloadData.imuGyroY, payloadData.imuGyroZ
	// );	
	// __generic_action_row_update(navSensorRows[3], "Running");
	// __generic_action_row_update(navSensorRows[4], buffer);

	/* Update the temperature output. */
	// snprintf(
	// 	buffer, BUFFER_SIZE, "%.3f", payloadData.imuTemp
	// );	
	// __generic_action_row_update(navSensorRows[7], buffer);

	return G_SOURCE_CONTINUE;
}

/**
 * Set the timeout for GPS map updates.
 */
gboolean timeout_gps_update(gpointer)
{
	static int i = 0;
	char buffer[BUFFER_SIZE];

	/* Put the markers into map area. */
	shumate_map_center_on(gpsMap, 41.008, 28.9784 + i * 0.01);
	gps_map_area_markers(gpsMarkerLayer, 41.008, 28.9784 + i * 0.01);

	// printf("%s\n", payloadData.gpsUTCTime);

	// __generic_action_row_update(gpsModuleRows[0], USED_GPS_MODULE);
	// __generic_action_row_update(gpsModuleRows[1], (const char *) payloadData.gpsLatitude);
	// __generic_action_row_update(gpsModuleRows[2], (const char *) payloadData.gpsLongitude);

	// shumate_map_center_on(gpsMap, 41.008, 29.0500);
	// gps_map_area_markers(gpsMarkerLayer, 41.008, 29.0500);
	i++;

	return G_SOURCE_CONTINUE;
}
 