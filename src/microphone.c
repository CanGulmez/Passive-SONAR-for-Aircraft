/**
 ******************************************************************************
 * @file 	microphone.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Microphone data integration of AeroSONAR.
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

/* Shared widgets and variables */

char *micDeviceNodes[MAX_DEVICE_NODE];
GtkWidget *micUARTGroup;
GtkWidget *micUSBGroup;
GtkWidget *micWiFiGroup;
GtkWidget *analysisGroup;
MicChannel micChannel = MIC_CHANNEL_UART;
char *micDeviceNode = NULL;
MicBaudRate micBaudRate = MIC_BAUD_RATE_115200;
MicDataBits micDataBits = MIC_DATA_BITS_8;
MicParityBit micParityBit = MIC_PARITY_BIT_NONE;
MicStopBits micStopBits = MIC_STOP_BITS_1;
MicFlowControl micFlowControl = MIC_FLOW_CONTROL_NONE;
guint micTimeout = 0;
guint recordTimeout = 0;
MicButton micButton;

MicSensorData micSensorData = {0};
MicSignal micSignal = {"Null", "Null", "Null", "Null", "Null", 
	"Null", "Null", "Null", "Null", "Null", "Null", "Null", 
	"Null", "Null", "Null"};

/**
 * Initialize the device node row.
 */
void microphone_row_device_node(GtkWidget *propertyGroup)
{
	int i;
	GtkWidget *deviceNodeRow;
	GtkStringList *deviceNodeList;

	deviceNodeRow = __generic_combo_row_new(
		"Device Node", (const char **)micDeviceNodes, 0
	);
	__generic_group_add(propertyGroup, deviceNodeRow);
	realize_signal(deviceNodeRow, on_device_node_selected);
	combo_row_signal(deviceNodeRow, on_device_node_selected);
}

/**
 * Initialize the baud rate row.
 */
void microphone_row_baud_rate(GtkWidget *propertyGroup)
{
	GtkWidget *baudRateRow;

	baudRateRow = __generic_combo_row_new(
		"Baud Rate", (const char *[]) {"9600", "14400", "19200", "28800", 
		"38400", "57600", "115200", NULL}, 6
	);
	__generic_group_add(propertyGroup, baudRateRow);
	combo_row_signal(baudRateRow, on_baud_rate_selected);
}

/**
 * Initialize the data bits row.
 */
void microphone_row_data_bits(GtkWidget *propertyGroup)
{
	GtkWidget *dataBitsRow;

	dataBitsRow = __generic_combo_row_new(
		"Data Bits", (const char *[]) {"5", "6", "7", "8", NULL}, 3
	);
	__generic_group_add(propertyGroup, dataBitsRow);
	combo_row_signal(dataBitsRow, on_data_bits_selected);
}

/**
 * Initialize the parity bit row.
 */
void microphone_row_parity_bit(GtkWidget *propertyGroup)
{
	GtkWidget *parityBitRow;

	parityBitRow = __generic_combo_row_new(
		"Parity Bit", (const char *[]) {"None", "Even", "Odd", 
		"Mark", "Space", NULL}, 0
	);
	__generic_group_add(propertyGroup, parityBitRow);
	combo_row_signal(parityBitRow, on_parity_bit_selected);
}

/**
 * Initialize the stop bits row.
 */
void microphone_row_stop_bits(GtkWidget *propertyGroup)
{
	GtkWidget *stopBitsRow;

	stopBitsRow = __generic_combo_row_new(
		"Stop Bits", (const char *[]) {"1", "2", NULL}, 0
	);
	__generic_group_add(propertyGroup, stopBitsRow);
	combo_row_signal(stopBitsRow, on_stop_bits_selected);
}

/**
 * Initialize the flow control row.
 */
void microphone_row_flow_control(GtkWidget *propertyGroup)
{
	GtkWidget *flowControlRow;

	flowControlRow = __generic_combo_row_new(
		"Flow Control", (const char *[]) {"None", "Hardware (RTS/CTS)", 
		"Software (XON/XOFF)", NULL}, 0
	);
	__generic_group_add(propertyGroup, flowControlRow);
	combo_row_signal(flowControlRow, on_flow_control_selected);
}

/**
 * Initialize the UART property group.
 */
void microphone_group_UART(gpointer data)
{
	/* Set the UART property group with default settings. */
	micUARTGroup = __generic_group_new("UART Properties",
		"Select the UART channel properties");

	microphone_row_device_node(micUARTGroup);			/* Device Node */
	microphone_row_baud_rate(micUARTGroup);			/* Baud Rate */
	microphone_row_data_bits(micUARTGroup);			/* Data Bits */
	microphone_row_parity_bit(micUARTGroup);			/* Parity Bit */
	microphone_row_stop_bits(micUARTGroup);			/* Stop Bits */
	microphone_row_flow_control(micUARTGroup);		/* Flow Control */
}

/**
 * Initialize the USB property group.
 */
void microphone_group_USB(gpointer data)
{
	/* Set the USB property group with default settings. */
	micUSBGroup = __generic_group_new("USB Properties",
		"Select the USB channel properties");

	microphone_row_device_node(micUSBGroup);			/* Device Node */
	microphone_row_baud_rate(micUSBGroup);				/* Baud Rate */
	microphone_row_data_bits(micUSBGroup);				/* Data Bits */
	microphone_row_parity_bit(micUSBGroup);			/* Parity Bit */
	microphone_row_stop_bits(micUSBGroup);				/* Stop Bits */
	microphone_row_flow_control(micUSBGroup);			/* Flow Control */
}

/**
 * Initialize the Wi-Fi property group.
 */
void microphone_group_WiFi(gpointer data)
{
	/* Set the Wi-Fi property group with default settings. */
	micWiFiGroup = __generic_group_new("Wi-Fi Properties",
		"Select the Wi-Fi channel properties");

	microphone_row_device_node(micWiFiGroup);		/* Device Node */
}

/**
 * Update the signal analysis group.
 */
void microphone_signal_analysis(GtkWidget *analysisGroup, MicSignal *micSignal)
{
	GtkWidget *maxRow, *minRow, *meanRow, *stddevRow, *energyRow, *rmsRow,
				 *powerRow, *crestRow, *skewnessRow, *kurtosisRow, *varianceRow,
				 *doaRow, *distanceRow, *coordinateRow, *targetRow;

	maxRow = __generic_action_row_new("Maximum", micSignal->max);
	minRow = __generic_action_row_new("Minimum", micSignal->min);
	meanRow = __generic_action_row_new("Mean", micSignal->mean);
	stddevRow = __generic_action_row_new("Standard Deviation", micSignal->stddev);
	energyRow = __generic_action_row_new("Energy", micSignal->energy);
	rmsRow = __generic_action_row_new("RMS", micSignal->rms);
	powerRow = __generic_action_row_new("Power (dB)", micSignal->power);
	crestRow = __generic_action_row_new("Crest Factor", micSignal->crest);
	skewnessRow = __generic_action_row_new("Skewness", micSignal->skewness);
	kurtosisRow = __generic_action_row_new("Kurtosis", micSignal->kurtosis);
	varianceRow = __generic_action_row_new("Variance", micSignal->variance);
	doaRow = __generic_action_row_new("Direction of Angle", micSignal->doa);
	distanceRow = __generic_action_row_new("Distance (m)", micSignal->distance);
	coordinateRow = __generic_action_row_new("Coordinate", micSignal->coordinate);
	targetRow = __generic_action_row_new("Estimated Target", micSignal->target);

	__generic_group_add(analysisGroup, maxRow);
	__generic_group_add(analysisGroup, minRow);
	__generic_group_add(analysisGroup, meanRow);
	__generic_group_add(analysisGroup, stddevRow);
	__generic_group_add(analysisGroup, energyRow);
	__generic_group_add(analysisGroup, rmsRow);
	__generic_group_add(analysisGroup, powerRow);
	__generic_group_add(analysisGroup, crestRow);
	__generic_group_add(analysisGroup, skewnessRow);
	__generic_group_add(analysisGroup, kurtosisRow);
	__generic_group_add(analysisGroup, varianceRow);
	__generic_group_add(analysisGroup, doaRow);
	__generic_group_add(analysisGroup, distanceRow);
	__generic_group_add(analysisGroup, coordinateRow);
	__generic_group_add(analysisGroup, targetRow);
}

/**
 * Microphone Data Page
 */
void microphone(GtkBox *micBox, gpointer data)
{
	int i, numDev;
	GtkWidget *rightBox, *rightSep, *centerBox, *leftSep, *leftBox;
	GtkWidget *scrolledWin, *propertyBox, *btnBox;
	GtkWidget *commGroup, *propertyGroup, *analysisGroup;
	GtkWidget *commRow; 
	GtkStringList *commList;
	GtkWidget *carPlot, *polarPlot;
	GtkWidget *startBtn, *stopBtn;

	leftBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	leftSep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	centerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	rightSep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	rightBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	propertyBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	btnBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	scrolledWin = gtk_scrolled_window_new();

	gtk_widget_set_hexpand(rightBox, TRUE);
	gtk_widget_set_vexpand(rightBox, TRUE);
	gtk_widget_set_hexpand(propertyBox, TRUE);
	gtk_widget_set_vexpand(propertyBox, TRUE);

	gtk_widget_set_size_request(leftBox, 300, -1);
	gtk_widget_set_size_request(centerBox, 300, -1);

	gtk_widget_set_margin_end(leftBox, BOX_INNER_MARGIN);
	gtk_widget_set_margin_start(centerBox, BOX_INNER_MARGIN);
	gtk_widget_set_margin_end(centerBox, BOX_INNER_MARGIN);
	gtk_widget_set_margin_start(rightBox, BOX_INNER_MARGIN);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWin), 
		GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWin),
		centerBox);

	/**
	 * Put the communication group to select the channel. According 
	 * to selected communication channel, related property group will 
	 * be set as visible so that the settings can be selected.
	 */
	commGroup = __generic_group_new("Communication",
		"Set communication channel for data transmission");

	commRow = __generic_combo_row_new(
		"Channel", (const char *[]){"UART", "USB", "Wi-Fi", NULL}, 0
	);
	__generic_group_add(commGroup, commRow);
	combo_row_signal_with_data(commRow, on_comm_channel_selected, 
		propertyBox);

	/* Put the initial UART property box. */ 
	numDev = get_mic_device_nodes(micChannel);
	microphone_group_UART(NULL);

	gtk_box_append(GTK_BOX(propertyBox), micUARTGroup);

	/* Put the signal analysis of the captured data at center. */
	analysisGroup = __generic_group_new("Signal Analysis",
		"Display the static signal analysis results");

	microphone_signal_analysis(analysisGroup, &micSignal);
	
	/* Put the buttons to manage the upper and lower plots. */
	startBtn = __generic_button_new("Start", "suggested-action");
	stopBtn = __generic_button_new("Stop", "destructive-action");
	GtkWidget *buttons[2] = {startBtn, stopBtn};

	for (i = 0; i < 2; i++) 
	{
		button_signal(buttons[i], on_mic_button_clicked);	
		gtk_box_append(GTK_BOX(btnBox), buttons[i]);
	}

	/* Put the required plots to see the microphone data in different forms. */
	carPlot = gtk_drawing_area_new();
	polarPlot = gtk_drawing_area_new();

	gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(carPlot),
		mic_plot_car, NULL, NULL);
	gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(polarPlot),
		mic_plot_polar, NULL, NULL);
		
	g_timeout_add(TIMEOUT_PLOT_REDRAW, timeout_mic_plot_car, carPlot);
	// g_timeout_add(PLOT_REDRAW_TIMEOUT, page_mic_data_plot_timeout, polarPlot);

	gtk_widget_set_hexpand(carPlot, TRUE);
	gtk_widget_set_vexpand(carPlot, TRUE);
	gtk_widget_set_hexpand(polarPlot, TRUE);
	gtk_widget_set_vexpand(polarPlot, TRUE);

	gtk_widget_set_size_request(carPlot, 550, -1);
	gtk_widget_set_size_request(polarPlot, 550, -1);

	/* Append the all defined widgets in the microphone data page. */
	gtk_box_append(GTK_BOX(leftBox), commGroup);
	gtk_box_append(GTK_BOX(leftBox), propertyBox);
	gtk_box_append(GTK_BOX(leftBox), btnBox);
	gtk_box_append(GTK_BOX(centerBox), analysisGroup);
	gtk_box_append(GTK_BOX(rightBox), carPlot);
	gtk_box_append(GTK_BOX(rightBox), polarPlot);

	gtk_box_append(micBox, leftBox);
	gtk_box_append(micBox, leftSep);
	gtk_box_append(micBox, scrolledWin);
	gtk_box_append(micBox, rightSep);
	gtk_box_append(micBox, rightBox);
}
 
