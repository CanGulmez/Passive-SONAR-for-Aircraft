/**
 ******************************************************************************
 * @file 	nav_plot.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Navigation plotting of AeroSONAR.
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

#include "./main.h"

void nav_plot_area_axes(cairo_t *cr, int width, int height)
{
	int i, centerX, centerY, ind;
	double radius;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_set_line_width(cr, 5.0);

	centerX = (width - 2 * NAV_PLOT_MARGIN) / 2 + NAV_PLOT_MARGIN;
	centerY = (height - 2 * NAV_PLOT_MARGIN) / 2 + NAV_PLOT_MARGIN;
	radius = (double) centerY - NAV_PLOT_MARGIN;

	/* Put the two full circles. */
	cairo_move_to(cr, centerX + radius, centerY);
	cairo_arc(cr, centerX, centerY, radius, 0.0, 2 * M_PI);
	cairo_move_to(cr, centerX + radius - 50, centerY);
	cairo_arc(cr, centerX, centerY, radius - 50, 0.0, 2 * M_PI);
	cairo_stroke(cr);

	/* Put the direction indicators. */
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_set_line_width(cr, 4.0);

	cairo_move_to(cr, centerX + radius - 50 - 15, centerY);
	cairo_line_to(cr, centerX + radius - 50 + 15, centerY);
	cairo_move_to(cr, centerX, centerY - radius + 50 - 15);
	cairo_line_to(cr, centerX, centerY - radius + 50 + 15);
	cairo_move_to(cr, centerX - radius + 50 - 15, centerY);
	cairo_line_to(cr, centerX - radius + 50 + 15, centerY);
	cairo_move_to(cr, centerX, centerY + radius - 50 - 15);
	cairo_line_to(cr, centerX, centerY + radius - 50 + 15);
	cairo_stroke(cr);

	cairo_move_to(cr, centerX + radius - 15, centerY);
	cairo_line_to(cr, centerX + radius + 15, centerY);
	cairo_move_to(cr, centerX, centerY - radius - 15);
	cairo_line_to(cr, centerX, centerY - radius + 15);
	cairo_move_to(cr, centerX - radius - 15, centerY);
	cairo_line_to(cr, centerX - radius + 15, centerY);
	cairo_move_to(cr, centerX, centerY + radius - 15);
	cairo_line_to(cr, centerX, centerY + radius + 15);
	cairo_stroke(cr);
}

void nav_plot_area_grid(cairo_t *cr, int width, int height)
{
	int i;

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);	/* gray grid */
	cairo_set_line_width(cr, 0.5);

	/* Put the vertical grid lines */
	for (i = NAV_PLOT_MARGIN + NAV_PLOT_GRID;
		  i < width - (NAV_PLOT_MARGIN); i += NAV_PLOT_GRID) 
	{
		cairo_move_to(cr, i, NAV_PLOT_MARGIN);
		cairo_line_to(cr, i, height - NAV_PLOT_MARGIN);
	}
	cairo_stroke(cr);

	/* Put the horizontal grid lines */
	for (i = height - NAV_PLOT_MARGIN - NAV_PLOT_GRID;
		  i >= NAV_PLOT_MARGIN; i -= NAV_PLOT_GRID) 
	{
		cairo_move_to(cr, NAV_PLOT_MARGIN, i);
		cairo_line_to(cr, width - NAV_PLOT_MARGIN, i);
	}
	cairo_stroke(cr);
}

void nav_plot_area_device(cairo_t *cr, int width, int height)
{
	int middle, centerX, centerY;

	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);

	/* Get the center points to put the board device. */
	centerX = (width - (NAV_PLOT_MARGIN * 2)) / 2 + NAV_PLOT_MARGIN;
	centerY = (height - (NAV_PLOT_MARGIN * 2)) / 2 + NAV_PLOT_MARGIN;

	cairo_move_to(cr, centerX - 40, centerY); 
	cairo_line_to(cr, centerX + 40, centerY);
	cairo_line_to(cr, centerX, centerY - 100);
	cairo_close_path(cr);
	cairo_fill(cr);
}

void nav_plot_area_label(cairo_t *cr, int width, int height)
{
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, 
		CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size(cr, 20.0);

	cairo_move_to(cr, width - NAV_PLOT_MARGIN - 200, 
							NAV_PLOT_MARGIN + 75);
	cairo_show_text(cr, "Noth-Eastean");

	cairo_move_to(cr, width - NAV_PLOT_MARGIN - 200, 
							height - NAV_PLOT_MARGIN - 70);
	cairo_show_text(cr, "South-Eastean");

	cairo_move_to(cr, NAV_PLOT_MARGIN + 50, 
							NAV_PLOT_MARGIN + 75);
	cairo_show_text(cr, "Noth-Westean");

	cairo_move_to(cr, NAV_PLOT_MARGIN + 50, 
							height - NAV_PLOT_MARGIN - 70);
	cairo_show_text(cr, "South-Weastean");
}

void nav_plot_area(GtkDrawingArea *area, cairo_t *cr, int width, 
						 int height, gpointer data)
{
	/* Set the background of navigation plot area. */
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_paint(cr);

	nav_plot_area_axes(cr, width, height);		/* draw the axes */
	nav_plot_area_grid(cr, width, height);		/* draw the grid */
	nav_plot_area_device(cr, width, height);	/* draw the device */
	nav_plot_area_label(cr, width, height);	/* show the text */
}
