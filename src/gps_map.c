/**
 ******************************************************************************
 * @file 	gps_map.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	GPS map interation of AeroSONAR.
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


/* GPS map shared widgets and variables */

GPSData gpsData;

/**
 * GPS Map Page Area
 */
void gps_map_area(GtkBox *rightBox, gpointer data)
{
	ShumateSimpleMap *simpleMap;
	ShumateViewport *viewport;
	ShumateMapSourceRegistry *registry;
	ShumateMapSource *mapSource;
	ShumateMap *map;
	ShumateLocation *location;

	simpleMap = shumate_simple_map_new();
	map = shumate_simple_map_get_map(simpleMap);
	viewport = shumate_simple_map_get_viewport(simpleMap);

	registry = shumate_map_source_registry_new_with_defaults();
	mapSource = shumate_map_source_registry_get_by_id(registry,
		SHUMATE_MAP_SOURCE_OSM_MAPNIK);

	shumate_map_center_on(map, 41.008, 28.9784);
	shumate_simple_map_set_map_source(simpleMap, mapSource);

	shumate_viewport_set_reference_map_source(viewport, mapSource);
	shumate_viewport_set_zoom_level(viewport, 10.0);

	gtk_widget_set_size_request(GTK_WIDGET(simpleMap), 
		900, -1);
	gtk_widget_set_hexpand(GTK_WIDGET(simpleMap), TRUE);
	gtk_widget_set_vexpand(GTK_WIDGET(simpleMap), TRUE);

   gtk_box_append(GTK_BOX(rightBox), GTK_WIDGET(simpleMap));
}

/**
 * GPS Map Page
 */
void gps_map(GtkBox *mapBox, gpointer data)
{
	GtkWidget *rightBox, *leftBox, *sep;	
	GtkWidget *GPSModuleGroup, *GPSResGroup;
	GtkWidget *scrolledWin;
	GtkWidget *timeRow, *latitudeRow, *longitudeRow, *qualityRow;
	GtkWidget *numSatRow, *altitudeRow, *statusRow, *speedRow;
	GtkWidget *courseRow, *dateRow, *seriesRow;

	leftBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
	sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	rightBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	scrolledWin = gtk_scrolled_window_new();

	gtk_widget_set_hexpand(leftBox, TRUE);
	gtk_widget_set_vexpand(leftBox, TRUE);
	gtk_widget_set_hexpand(rightBox, TRUE);
	gtk_widget_set_vexpand(rightBox, TRUE);

	gtk_widget_set_margin_end(leftBox, BOX_INNER_MARGIN);
	gtk_widget_set_margin_start(rightBox, BOX_INNER_MARGIN);

	gtk_widget_set_size_request(leftBox, 300, -1);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWin),
		GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWin), leftBox);

	/*	Show the used GPS module infomration */
	GPSModuleGroup = __generic_group_new("GPS Module",
		"Show the used GPS module information");

	seriesRow = __generic_action_row_new("Module Series", "Null");
	__generic_group_add(GPSModuleGroup, seriesRow);

	/* GPS Data (NMEA 0183 sentences)
	
		$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
			+ 123519 		--> UTC time (12:35:19)
			+ 4807.038,N	--> latitude 48^07.038' North
			+ 01131.000,E	--> longitude 11^31.000' East
			+ 1 				--> fix quality (1 = GPS fix)
			+ 08				--> number of sattelite used
			+ 545.4,M		--> altitude 545.4 m

		$GPRCM,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
			+ 123519			--> UTC time
			+ A				--> status (A = valid, V = invalid)
			+ 					--> latitude
			+ 					--> longitude
			+ 022.4			--> speed over ground (knots)
			+ 084.4			--> course over ground (degrees)
			+ 230394			--> date (23 March 1994)
	*/
	GPSResGroup = __generic_group_new("GPS Data",
		"Show the GPS module data simultaneously");

	/* UTC Time */
	timeRow = __generic_action_row_new("UTC Time", "Null");
	__generic_group_add(GPSResGroup, timeRow);
	
	/* Latitude */
	latitudeRow = __generic_action_row_new("Latitude", "Null");
	__generic_group_add(GPSResGroup, latitudeRow);

	/* Longitude */
	longitudeRow = __generic_action_row_new("Longitude", "Null");
	__generic_group_add(GPSResGroup, longitudeRow);

	/* Fix Quality */
	qualityRow = __generic_action_row_new("Fix Quality", "Null");
	__generic_group_add(GPSResGroup, qualityRow);

	/* Number of Sattelites */
	numSatRow = __generic_action_row_new("Number of Sattelites", "Null");
	__generic_group_add(GPSResGroup, numSatRow);

	/* Altitude */
	altitudeRow = __generic_action_row_new("Altitude", "Null");
	__generic_group_add(GPSResGroup, altitudeRow);

	/* Status */
	statusRow = __generic_action_row_new("Status", "Null");
	__generic_group_add(GPSResGroup, statusRow);

	/* Speed over Ground (knots) */
	speedRow = __generic_action_row_new("Speed over Ground (knots)", "Null");
	__generic_group_add(GPSResGroup, speedRow);

	/* Course over Ground (degrees) */
	courseRow = __generic_action_row_new("Course over Ground (degrees)", "Null");
	__generic_group_add(GPSResGroup, courseRow);

	/* Date */
	dateRow = __generic_action_row_new("Date", "Null");
	__generic_group_add(GPSResGroup, dateRow);
	
	/* Map area */
	gps_map_area(GTK_BOX(rightBox), NULL);

	/* Layout */
	gtk_box_append(GTK_BOX(leftBox), GPSModuleGroup);
	gtk_box_append(GTK_BOX(leftBox), GPSResGroup);

	gtk_box_append(mapBox, scrolledWin);
	gtk_box_append(mapBox, sep);
	gtk_box_append(mapBox, rightBox);
}
 