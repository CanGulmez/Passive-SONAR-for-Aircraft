/**
 ******************************************************************************
 * @file 	mic_plot.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Microphone plot area integrations of AeroSONAR. 
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

/**
 * Draw the cartesian plot frame.
 */
void mic_plot_car_area_frame(cairo_t *cr, int width, int height)
{
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_set_line_width(cr, 3.0);

	cairo_move_to(cr, MIC_PLOT_MARGIN, MIC_PLOT_MARGIN / 2);
	cairo_line_to(cr, MIC_PLOT_MARGIN, height - MIC_PLOT_MARGIN);

	cairo_move_to(cr, MIC_PLOT_MARGIN, height - MIC_PLOT_MARGIN);
	cairo_line_to(cr, width - MIC_PLOT_MARGIN / 2, height - MIC_PLOT_MARGIN);

	cairo_move_to(cr, width - MIC_PLOT_MARGIN / 2, height - MIC_PLOT_MARGIN);
	cairo_line_to(cr, width - MIC_PLOT_MARGIN / 2, MIC_PLOT_MARGIN / 2);

	cairo_move_to(cr, width - MIC_PLOT_MARGIN / 2, MIC_PLOT_MARGIN / 2);
	cairo_line_to(cr, MIC_PLOT_MARGIN, MIC_PLOT_MARGIN / 2);

	cairo_stroke(cr);
}

/**
 * Draw the cartesian plot grid.
 */
void mic_plot_car_area_grid(cairo_t *cr, int width, int height)
{
	int i;

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);	/* gray grid */
	cairo_set_line_width(cr, 0.5);

	/* Put the vertical grid lines */
	for (i = MIC_PLOT_MARGIN + MIC_PLOT_GRID;
		  i < width - (MIC_PLOT_MARGIN / 2); i += MIC_PLOT_GRID) 
	{
		cairo_move_to(cr, i, MIC_PLOT_MARGIN / 2);
		cairo_line_to(cr, i, height - MIC_PLOT_MARGIN);
	}
	cairo_stroke(cr);

	/* Put the horizontal grid lines */
	for (i = (MIC_PLOT_MARGIN / 2) + MIC_PLOT_GRID;
		  i < height - MIC_PLOT_MARGIN; i += MIC_PLOT_GRID) 
	{
		cairo_move_to(cr, MIC_PLOT_MARGIN, i);
		cairo_line_to(cr, width - (MIC_PLOT_MARGIN / 2), i);
	}
	cairo_stroke(cr);
}

/**
 * Draw the cartesian plot x-axis label.
 */
void mic_plot_car_area_label_x(cairo_t *cr, int width, int height)
{
	char buffer[64];

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);	/* black indices */
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, 
		CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size(cr, 15.0);

	cairo_move_to(cr, 250, height - MIC_PLOT_MARGIN + 20);
	
	snprintf(buffer, 64, "Sensor Data (%d-points)", DATA_SIZE);
	cairo_show_text(cr, buffer);	
}

/**
 * Draw the cartesian plot y-axis label.
 */
void mic_plot_car_area_label_y(cairo_t *cr, int width, int height)
{
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);	/* black indices */
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, 
		CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size(cr, 15.0);

	cairo_save(cr);
	cairo_move_to(cr, (MIC_PLOT_MARGIN - 10), 250);
	cairo_rotate(cr, -M_PI / 2);
	cairo_show_text(cr, "Amplitude (Normalized)");	
	cairo_restore(cr);
}

/**
 * Draw the cartesian plot read data.
 */
void mic_plot_car_area_data(cairo_t *cr, int width, int height)
{
	int i;
	double step, middle;

	cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);	
	cairo_set_line_width(cr, 2.0);

	middle = (MIC_PLOT_MARGIN / 2) + 
		((height - (MIC_PLOT_MARGIN * 3/2)) / 2);
	step = (width - (MIC_PLOT_MARGIN * 3/2)) / (DATA_SIZE - 1);
	
	cairo_move_to(cr, MIC_PLOT_MARGIN + 2, middle);
	for (i = 1; i <= DATA_SIZE; i++) 
	{
		cairo_line_to(cr, MIC_PLOT_MARGIN + (step * i) + 2, 
			middle - micSensorData.data[i - 1]);
	}
	cairo_stroke(cr);
}

/**
 * Draw the cartesian plot area.
 */
void mic_plot_car(GtkDrawingArea *area, cairo_t *cr, 
	int width, int height, gpointer data)
{
	/* Set the background of plot area */
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);			/* white background */
	cairo_paint(cr);

	mic_plot_car_area_frame(cr, width, height);		/* frame */
	mic_plot_car_area_grid(cr, width, height);		/* grid */
	mic_plot_car_area_label_x(cr, width, height);	/* x-Axis Label */
	mic_plot_car_area_label_y(cr, width, height);	/* y-Axis Label */
	mic_plot_car_area_data(cr, width, height);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/**
 * Draw the polar plot area frame.
 */
void mic_plot_polar_frame(cairo_t *cr, int width, int height)
{
	int i, center_x, center_y, radius;

	center_x = width / 2;
	center_y = height / 2;
	radius = (height - 2 * MIC_PLOT_MARGIN) / 2;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);		/* outer arch circular */
	cairo_set_line_width(cr, 3.0);	

	cairo_move_to(cr, center_x - radius, center_y);
	cairo_arc(cr, center_x, center_y, radius, -M_PI, M_PI);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);		/* inner arch circulars */
	cairo_set_line_width(cr, 3.0);	

	for (i = 1; i < 5; i++) 
	{
		cairo_move_to(cr, center_x - radius + i * MIC_PLOT_GRID, center_y);
		cairo_arc(cr, center_x, center_y, radius - i * MIC_PLOT_GRID, -M_PI, M_PI);	
	}
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);		/* sector lines */
	cairo_set_line_width(cr, 3.0);

	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, center_x + radius, center_y);
	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, center_x - radius, center_y);
	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, center_x, center_y - radius);
	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, center_x, center_y + radius);
	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, center_x + radius - MIC_PLOT_MARGIN, 
					  center_y - radius + MIC_PLOT_MARGIN);
	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, center_x + radius - MIC_PLOT_MARGIN, 
					  center_y + radius - MIC_PLOT_MARGIN);
	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, center_x - radius + MIC_PLOT_MARGIN, 
					  center_y - radius + MIC_PLOT_MARGIN);
	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, center_x - radius + MIC_PLOT_MARGIN, 
					  center_y + radius - MIC_PLOT_MARGIN);
	cairo_stroke(cr);
}

/**
 * Draw the polar plot area degrees and directions.
 */
void mic_plot_polar_label(cairo_t *cr, int width, int height)
{
	int i, center_x, center_y, radius;

	center_x = width / 2;
	center_y = height / 2;
	radius = (height - 2 * MIC_PLOT_MARGIN) / 2;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);	/* black indices */
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, 
		CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size(cr, 18.0);

	cairo_move_to(cr, center_x - 4, center_y - radius - 4);
	cairo_show_text(cr, "0");
	// cairo_move_to(cr, center_x + radius - MIC_PLOT_MARGIN + 4, 
	// 				  center_y - radius + MIC_PLOT_MARGIN - 4);
	// cairo_show_text(cr, "45");
	cairo_line_to(cr, center_x + radius + 4, center_y + 4);
	cairo_show_text(cr, "90");
	// cairo_move_to(cr, center_x + radius - MIC_PLOT_MARGIN + 2, 
	// 				  center_y + radius - MIC_PLOT_MARGIN + 16);
	// cairo_show_text(cr, "135");
	cairo_move_to(cr, center_x - 16, center_y + radius + 20);
	cairo_show_text(cr, "180");
	// cairo_move_to(cr, center_x - radius + MIC_PLOT_MARGIN - 30, 
	// 				  center_y + radius - MIC_PLOT_MARGIN + 20);
	// cairo_show_text(cr, "225");
	cairo_move_to(cr, center_x - radius - 35, center_y + 5);
	cairo_show_text(cr, "270");
	// cairo_move_to(cr, center_x - radius + MIC_PLOT_MARGIN - 30, 
	// 				  center_y - radius + MIC_PLOT_MARGIN - 10);
	// cairo_show_text(cr, "315");
}

/**
 * Draw the polar plot area.
 */
extern void mic_plot_polar(GtkDrawingArea *area, cairo_t *cr, 
	int width, int height, gpointer data)
{
	/* Set the background of plot area */
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);	/* white background */
	cairo_paint(cr);

	mic_plot_polar_frame(cr, width, height);	
	mic_plot_polar_label(cr, width, height);
}

/**
 * Redraw the plot areas.
 */
gboolean mic_plot_timeout(gpointer data)
{
	GtkWidget *carPlot;
	
	carPlot = GTK_WIDGET(data);
	if (micTimeout)
		gtk_widget_queue_draw(carPlot); /* request redraw */

	return G_SOURCE_CONTINUE;
}

