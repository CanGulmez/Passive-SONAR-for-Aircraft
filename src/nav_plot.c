/**
 ******************************************************************************
 * @file 	nav_plot.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Navigation plotting of AeroSONAR.
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

#include "./main.h"

void nav_plot_area_grid(cairo_t *cr, int width, int height)
{
	int i;

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);	/* gray grid */
	cairo_set_line_width(cr, 0.2);

	/* Put the vertical grid lines */
	for (i = (NAV_PLOT_MARGIN + NAV_PLOT_GRID);
		  i < (width - NAV_PLOT_MARGIN); i += NAV_PLOT_GRID) 
	{
		cairo_move_to(cr, i, NAV_PLOT_MARGIN);
		cairo_line_to(cr, i, height - NAV_PLOT_MARGIN);
	}
	cairo_stroke(cr);

	/* Put the horizontal grid lines */
	for (i = (NAV_PLOT_MARGIN + NAV_PLOT_GRID);
		  i <= (height - NAV_PLOT_MARGIN); i += NAV_PLOT_GRID) 
	{
		cairo_move_to(cr, NAV_PLOT_MARGIN, i);
		cairo_line_to(cr, width - NAV_PLOT_MARGIN, i);
	}
	cairo_stroke(cr);
}

void nav_plot_area_axes(cairo_t *cr, int width, int height)
{
	int i;
	double center_x, center_y, margin;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);	/* gray grid */
	cairo_set_line_width(cr, 2.0);

	center_x = (width - (2 * NAV_PLOT_MARGIN)) / 2 - 100;
	center_y = (height - (2 * NAV_PLOT_MARGIN)) / 2 + 100;
	margin = 2 * NAV_PLOT_GRID;

	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, center_x, margin);
	cairo_move_to(cr, center_x - 6, margin + 6);
	cairo_line_to(cr, center_x, margin);
	cairo_line_to(cr, center_x + 6, margin + 6);
	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, width - margin, center_y);
	cairo_move_to(cr, width - margin - 6, center_y - 6);
	cairo_line_to(cr, width - margin, center_y);
	cairo_line_to(cr, width - margin - 6, center_y + 6);
	cairo_move_to(cr, center_x, center_y);
	cairo_line_to(cr, margin, height - margin);
	cairo_move_to(cr, margin, height - margin - 10);
	cairo_line_to(cr, margin, height - margin);
	cairo_line_to(cr, margin + 10, height - margin);
	cairo_stroke(cr);
}

void nav_plot_area_labels(cairo_t *cr, int width, int height)
{
	int i;
	double center_x, center_y, margin;

	center_x = (width - (2 * NAV_PLOT_MARGIN)) / 2 - 100;
	center_y = (height - (2 * NAV_PLOT_MARGIN)) / 2 + 100;
	margin = 2 * NAV_PLOT_GRID;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);	/* black labels */
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, 
		CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size(cr, 18.0);

	cairo_move_to(cr, center_x - 5, margin - 5);
	cairo_show_text(cr, "Z");
	cairo_move_to(cr, width - margin + 2, center_y + 5);
	cairo_show_text(cr, "X");
	cairo_move_to(cr, margin - 14, height - margin + 10);
	cairo_show_text(cr, "Y");
}

void nav_plot_area_shadow(cairo_t *cr, int width, int height)
{
	int i;

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);	/* gray grid */
	cairo_set_line_width(cr, 1.0);
}

void nav_plot_area_device(cairo_t *cr, int width, int height)
{

}

void nav_plot_area_gyro(cairo_t *cr, int width, int height,
								NavGyro navGyro)
{
		
}

void nav_plot_area_accel(cairo_t *cr, int width, int height,
								 NavAccel navAccel)
{

}

void nav_plot_area(GtkDrawingArea *area, cairo_t *cr, int width,
						 int height, gpointer data)
{
	/* Set the background of bottom plot area. */
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_paint(cr);

	nav_plot_area_grid(cr, width, height);		/* draw grids */
	nav_plot_area_axes(cr, width, height);		/* put axes */
	nav_plot_area_labels(cr, width, height);	/* put labels */
}
 