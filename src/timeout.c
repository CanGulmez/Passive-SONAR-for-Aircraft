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
gboolean timeout_mic_device_node(gpointer data)
{
	double max_freq;
	int i, arrival;

	/* Read the device node to update 'payloadData' object. */
	read_mic_device_node(micChannel, micDeviceNode);

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
