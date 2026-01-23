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
	read_mic_device_node(micChannel, micDeviceNode);

	return G_SOURCE_CONTINUE;
}

/**
 * Redraw the plot areas.
 */
gboolean timeout_mic_plot_car(gpointer data)
{
	GtkWidget *carPlot;
	
	carPlot = GTK_WIDGET(data);
	if (micTimeout)
	{
		gtk_widget_queue_draw(carPlot); /* request redraw */
	}
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
gboolean timeout_database_record(gpointer data)
{
	struct sqlite3 *db;

	db = (struct sqlite3 *) data;
	/* Bind the last sensor data. */
	database_bind_data(db, DATABASE_SENSOR_DATA);

	return G_SOURCE_CONTINUE;
}
