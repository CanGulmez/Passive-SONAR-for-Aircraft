/**
 ******************************************************************************
 * @file 	model.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	AI model integration of AeroSONAR.
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

/* Shared widgets and variables */

char *modelDatasets[MAX_MODEL_DATASET];
guint modelFitPid = 0;
char *modelDataset = NULL;
char *modelKerasLog = NULL;
GtkWidget *modelTextView;
GtkTextBuffer *modelTextBuffer;
guint modelTimeout = 0;
ModelLayerType modelLayerType = MODEL_LAYER_TYPE_LSTM;
guint modelUnits = 1;
guint modelEpochs = 1;
gdouble modelDropout = 0.1;
ModelBatchSize modelBatchSize = MODEL_BATCH_SIZE_16;
ModelEarlyStopping modelEarlyStopping = MODEL_EARLY_STOPPING_FALSE;
char *modelOutputName = NULL;
ModelButton modelButton;

/**
 * Dataset Selection Group
 */
void model_group_dataset(GtkWidget *datasetGroup)
{
	int i, numDataset;
	GtkWidget *datasetRow;
	GtkStringList *datasetList;

	/* Dataset */
	numDataset = get_model_datasets();	/* datasets from host */

	datasetRow = __generic_combo_row_new(
		"Dataset", (const char **)modelDatasets, -1
	);
	__generic_group_add(datasetGroup, datasetRow);
	combo_row_signal(datasetRow, on_dataset_selected);
}

/**
 * Model Parameter Group
 */
void model_group_model(GtkWidget *modelGroup)
{
	int i;
	GtkWidget *layerRow, *unitsRow, *useBiasRow, *epochsRow, 
		*batchSizeRow, *earlyStoppingRow, *dropoutRow, 
		*outputModelRow;

	/* Output Model Name */
	outputModelRow = __generic_entry_row_new("Output Model");
	__generic_group_add(modelGroup, outputModelRow);
	entry_row_signal(outputModelRow, on_output_model_texted);
	
	/* Layer */
	layerRow = __generic_combo_row_new(
		"Layer Type", (const char *[]) {"LSTM", "GRU", NULL}, 0
	);
	__generic_group_add(modelGroup, layerRow);
	combo_row_signal(layerRow, on_layer_type_selected);

	/* Units */  
	unitsRow = __generic_spin_row_new(
		"Units", 1, 1, MAX_MODEL_UNITS, 1, 0
	);
	__generic_group_add(modelGroup, unitsRow);
	spin_row_signal(unitsRow, on_units_changed);

	/* Epochs */
	epochsRow = __generic_spin_row_new(
		"Epochs", 1, 1, MAX_MODEL_EPOCHS, 1, 0
	);
	__generic_group_add(modelGroup, epochsRow);
	spin_row_signal(epochsRow, on_epochs_changed);

	/* Batch Size */
	batchSizeRow = __generic_combo_row_new(
		"Batch Size", (const char *[]) {"16", "32", "64", "128", "256", "512", NULL}, 0
	);
	__generic_group_add(modelGroup, batchSizeRow);
	combo_row_signal(batchSizeRow, on_batch_size_selected);

	/* Early Stopping */ 
	earlyStoppingRow = __generic_switch_row_new("Early Stopping");
	__generic_group_add(modelGroup, earlyStoppingRow);
	switch_row_signal(earlyStoppingRow, on_early_stopping_switched);

	/* Recurrent Dropout */
	dropoutRow = __generic_spin_row_new(
		"Dropout", 0.1, 0.1, 1.0, 0.01, 2
	);
	__generic_group_add(modelGroup, dropoutRow);
	spin_row_signal(dropoutRow, on_recurrent_dropout_changed);
}

/**
 * AI Model Integration Page
 */
void model(GtkBox *modelBox, gpointer data)
{
	int i;
	GtkWidget *rightBox, *separator, *leftBox;
	GtkWidget *scrolledWin, *settingsBox, *btnBox;
	GtkWidget *datasetGroup, *modelGroup;
	GtkWidget *fitBtn, *abortBtn, *evaluateBtn, *predictBtn;
	GtkWidget *scrolledText;

	leftBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
	separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	rightBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	settingsBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
	btnBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	scrolledWin = gtk_scrolled_window_new();
	scrolledText = gtk_scrolled_window_new();

	gtk_widget_set_hexpand(settingsBox, TRUE);
	gtk_widget_set_vexpand(settingsBox, TRUE);
	gtk_widget_set_hexpand(scrolledText, TRUE);
	gtk_widget_set_vexpand(scrolledText, TRUE);

	gtk_widget_set_size_request(leftBox, 300, -1);
	gtk_widget_set_size_request(rightBox, 900, -1);

	gtk_widget_set_margin_end(leftBox, BOX_INNER_MARGIN);
	gtk_widget_set_margin_start(rightBox, BOX_INNER_MARGIN);

	/* Put the dataset selection group. */
	datasetGroup = __generic_group_new(
		"Dataset Selection", "Select the appropriate dataset (.csv)"
	);
	model_group_dataset(datasetGroup);
	gtk_box_append(GTK_BOX(settingsBox), datasetGroup);
	
	/* Put the model parameter rows. */
	modelGroup = __generic_group_new(
		"Model Parameters", "Set the model parameters"
	);
	model_group_model(modelGroup);
	gtk_box_append(GTK_BOX(settingsBox), modelGroup);
	
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWin), 
		GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWin),
		settingsBox);

	/* Put the text view area with Keras log outputs. */
	modelTextView = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(modelTextView), FALSE);
	gtk_text_view_set_monospace(GTK_TEXT_VIEW(modelTextView), TRUE);
	
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(modelTextView), 10);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(modelTextView), 20);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(modelTextView), 10);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(modelTextView), 20);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledText), 
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledText),
		modelTextView);	
	
	/* Put the fit, evaluate, and predict buttons. */
	fitBtn = __generic_button_new("Fit", "suggested-action");
	abortBtn = __generic_button_new("Abort", "destructive-action");
	evaluateBtn = __generic_button_new("Evaluate", "suggested-action");
	predictBtn = __generic_button_new("Predict", "suggested-action");
	GtkWidget *buttons[3] = {fitBtn, predictBtn, abortBtn};

	for (i = 0; i < 3; i++) 
	{
		button_signal(buttons[i], on_model_button_clicked);
		gtk_box_append(GTK_BOX(btnBox), buttons[i]);
	}
	gtk_widget_set_halign(btnBox, GTK_ALIGN_CENTER);

	/* Put the component layouts into page. */
	gtk_box_append(GTK_BOX(leftBox), scrolledWin);
	gtk_box_append(GTK_BOX(leftBox), btnBox);
	gtk_box_append(GTK_BOX(rightBox), scrolledText);
	gtk_box_append(modelBox, leftBox);
	gtk_box_append(modelBox, separator);
	gtk_box_append(modelBox, rightBox);
}
