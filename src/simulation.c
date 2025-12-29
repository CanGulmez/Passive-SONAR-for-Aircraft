/**
 ******************************************************************************
 * @file 	simulation.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Simulation integration of AeroSONAR.
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

/* Shared widgets and variables */

GtkWidget *simBallLayers[MAX_BALLISTIC_LAYER];

guint simLayerNumber = 1;
SimLayerMaterial simLayerMaterial = SIM_LAYER_MATERIAL_B4C;
gdouble simLayerThickness = 1.0f;
guint simCurrentLayer = 1;
SimButton simButton;

/**
 * Simulation Page Ballistic Layer Group
 */
void simulation_group_layer(guint currentRow, gpointer data)
{
	int i;
	GtkWidget *layerGroup;
	GtkWidget *thicknessRow, *materialRow;
	GtkStringList *materialList;
	GtkAdjustment *thicknessAdj;
	char materialBuf[32];
	char thicknessBuf[32];

	/* Store the current material and thickness row names. */
	snprintf(materialBuf, 32, "%d. Layer Material", currentRow + 1);
	snprintf(thicknessBuf, 32, "%d. Layer Thickness (mm)", currentRow + 1);

	/* Put the layer group. */
	layerGroup = __generic_group_new(NULL, NULL);

	/* Put the layer material selection row here. */
	materialRow = __generic_combo_row_new(
		materialBuf, (const char *[]){"Boron Carbide (B4C)", "Alumina (Al2O3)", 
		"Silicon Carbide (SiC)", "Titanium Diboride (TiB2)", "Silicon Nitride (Si3N4)",
		"Zirconium Diboride (ZrB2)", "Tungsten Carbide (WC)", "Steel (Fe)", NULL}, 0
	);
	__generic_group_add(layerGroup, materialRow);

	combo_row_signal_with_data(materialRow, on_layer_material_selected, 
		GUINT_TO_POINTER(currentRow));

	/* Put the layer thickness setting row here. */
	thicknessRow = __generic_spin_row_new(
		thicknessBuf, 1, 1, MAX_LAYER_THICKNESS, 0.01, 2 
	);
	__generic_group_add(layerGroup, thicknessRow);
	
	spin_row_signal_with_data(thicknessRow, on_layer_thickness_changed, 
		GUINT_TO_POINTER(currentRow));

	/* Store the created layer group in the global array. */
	simBallLayers[currentRow] = layerGroup;
}

/**
 * Simulation Page
 */
void simulation(GtkBox *simBox, gpointer data)
{
	GtkWidget *leftBox, *sep, *rightBox, *btnBox, *layerBox;
	GtkWidget *ballGroup, *layerGroup;
	GtkWidget *numRow;
	GtkWidget *setBtn, *renderBtn, *simulateBtn;
	GtkWidget *GLArea;
	GtkWidget *scrolledWin;

	GtkAdjustment *numAdj, *thicknessAdj;
	GtkGesture *gesture;
	GtkEventController *motion;

	leftBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
	sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	rightBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	layerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	btnBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

	scrolledWin = gtk_scrolled_window_new();
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWin),
		GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledWin), layerBox);

	gtk_widget_set_hexpand(leftBox, TRUE);
	gtk_widget_set_vexpand(leftBox, TRUE);
	gtk_widget_set_hexpand(rightBox, TRUE);
	gtk_widget_set_vexpand(rightBox, TRUE);
	gtk_widget_set_hexpand(scrolledWin, TRUE);
	gtk_widget_set_vexpand(scrolledWin, TRUE);
	gtk_widget_set_hexpand(btnBox, TRUE);
	// gtk_widget_set_vexpand(btnBox, TRUE);

	gtk_widget_set_margin_end(leftBox, BOX_INNER_MARGIN);
	gtk_widget_set_margin_start(rightBox, BOX_INNER_MARGIN);

	/* Put the simulation header groups and controls here. */
	ballGroup = __generic_group_new("Ballistic Simulation", 
		"Settings for ballistic simulation model");
	
	numRow = __generic_spin_row_new(
		"Layer Number", 1, 1, MAX_BALLISTIC_LAYER, 1, 0
	);
	__generic_group_add(ballGroup, numRow);
	spin_row_signal(numRow, on_ballistic_layer_changed);

	/* Put the simulation control buttons here. */
	setBtn = __generic_button_new("Set", "suggested-action");
	renderBtn = __generic_button_new("Render", "suggested-action");
	simulateBtn = __generic_button_new("Simulate", "suggested-action");

	GtkWidget *buttons[3] = {setBtn, renderBtn, simulateBtn};
	for (int i = 0; i < 3; i++) 
	{
		button_signal_with_data(buttons[i], on_sim_button_clicked, layerBox);
		gtk_box_append(GTK_BOX(btnBox), buttons[i]);
	}
	gtk_widget_set_halign(btnBox, GTK_ALIGN_CENTER);
	
	/* Put the OpenGL drawing area here. */
	GLArea = gtk_gl_area_new();
	gtk_gl_area_set_allowed_apis(GTK_GL_AREA(GLArea), GDK_GL_API_GL);
	gtk_widget_set_hexpand(GLArea, TRUE);
	gtk_widget_set_vexpand(GLArea, TRUE);
	gtk_widget_set_size_request(GLArea, 900, -1);

	realize_signal(GLArea, on_ballistic_sim_realize);
	render_signal(GLArea, on_ballistic_sim_render);

	gesture = gtk_gesture_click_new();
	gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture), GDK_BUTTON_PRIMARY);
	gtk_widget_add_controller(GLArea, GTK_EVENT_CONTROLLER(gesture));

	pressed_signal(gesture, on_gesture_click_press);
	released_signal(gesture, on_gesture_click_release);

	motion = gtk_event_controller_motion_new();
	gtk_widget_add_controller(GLArea, motion);

	motion_signal(motion, on_event_controller_motion);

	/* Assemble the simulation page layout. */
	gtk_box_append(GTK_BOX(leftBox), ballGroup);
	gtk_box_append(GTK_BOX(leftBox), scrolledWin);
	gtk_box_append(GTK_BOX(leftBox), btnBox);
	gtk_box_append(GTK_BOX(rightBox), GLArea);

	gtk_box_append(simBox, leftBox);
	gtk_box_append(simBox, sep);
	gtk_box_append(simBox, rightBox);
}
