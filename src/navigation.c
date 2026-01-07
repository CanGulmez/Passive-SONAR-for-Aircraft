/**
 ******************************************************************************
 * @file 	navigation.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Navigation system of AeroSONAR.
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

GtkWidget *navigation_accel_group(gpointer data)
{
	GtkWidget *accelGroup, *status, *output;

	accelGroup = __generic_group_new(
		"Acceloremeter", "Show the acceloremeter status and data"
	);
	status = __generic_action_row_new("Status", "Not Running");
	__generic_group_add(accelGroup, status);
	output = __generic_action_row_new("Output", "[Null, Null, Null]");
	__generic_group_add(accelGroup, output);

	return accelGroup;
}

GtkWidget *navigation_gyro_group(gpointer data)
{
	GtkWidget *gyroGroup, *status, *output;

	gyroGroup = __generic_group_new(
		"Gyroscope", "Show the gyroscope status and data"
	);
	status = __generic_action_row_new("Status", "Not Running");
	__generic_group_add(gyroGroup, status);
	output = __generic_action_row_new("Output", "[Null, Null, Null]");
	__generic_group_add(gyroGroup, output);

	return gyroGroup;
}

GtkWidget *navigation_magnet_group(gpointer data)
{
	GtkWidget *magnetGroup, *status, *output;

	magnetGroup = __generic_group_new(
		"Magnetometer", "Show the magnetometer status and data"
	);
	status = __generic_action_row_new("Status", "Not Running");
	__generic_group_add(magnetGroup, status);
	output = __generic_action_row_new("Output", "[Null, Null, Null]");
	__generic_group_add(magnetGroup, output);

	return magnetGroup;
}

void navigation(GtkBox *imuBox, gpointer data)
{
	GtkWidget *leftBox, *separator, *rightBox;
	GtkWidget *accelGroup, *gyroGroup, *magnetGroup;	
	GtkWidget *navPlot, *propertyBox;
	GtkWidget *navGroup, *navRow;
	GtkWidget *scrolledWin, *startBtn;

	/* Put the required boxes. */
	leftBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	rightBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	propertyBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	scrolledWin = gtk_scrolled_window_new();

	gtk_widget_set_hexpand(rightBox, TRUE);
	gtk_widget_set_vexpand(rightBox, TRUE);
	gtk_widget_set_vexpand(propertyBox, TRUE);

	gtk_widget_set_size_request(leftBox, 300, -1);

	gtk_widget_set_margin_end(leftBox, BOX_INNER_MARGIN);
	gtk_widget_set_margin_start(rightBox, BOX_INNER_MARGIN);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWin), 
		GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWin),
		propertyBox);

	/* Put the navigation series module group. */
	navGroup = __generic_group_new(
		"IMU Module", "Show the used IMU sensor information"
	);
	navRow = __generic_action_row_new("IMU series", "Null");
	__generic_group_add(navGroup, navRow);

	/* Put the accelerometer, gyroscope and gyroscope groups. */
	accelGroup = navigation_accel_group(NULL);
	gyroGroup = navigation_gyro_group(NULL);
	magnetGroup = navigation_magnet_group(NULL);

	/* Put the required buttons. */
	startBtn = __generic_button_new("Start", "suggested-action");
	button_signal(startBtn, on_nav_button_clicked);

	/* Put the plot area. */
	navPlot = gtk_drawing_area_new();
	gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(navPlot), 
		nav_plot_area, NULL, NULL);

	gtk_widget_set_hexpand(navPlot, TRUE);
	gtk_widget_set_vexpand(navPlot, TRUE);
	gtk_widget_set_size_request(navPlot, 900, 600);

	/* Append the all groups and widgets to main box. */
	gtk_box_append(GTK_BOX(propertyBox), navGroup);
	gtk_box_append(GTK_BOX(propertyBox), accelGroup);
	gtk_box_append(GTK_BOX(propertyBox), gyroGroup);
	gtk_box_append(GTK_BOX(propertyBox), magnetGroup);

	gtk_box_append(GTK_BOX(leftBox), scrolledWin);
	gtk_box_append(GTK_BOX(leftBox), startBtn);
	gtk_box_append(GTK_BOX(rightBox), navPlot);

	gtk_box_append(imuBox, leftBox);
	gtk_box_append(imuBox, separator);
	gtk_box_append(imuBox, rightBox);
}
