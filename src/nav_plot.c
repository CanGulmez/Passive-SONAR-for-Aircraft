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

// void nav_plot_area_axes(cairo_t *cr, int width, int height)
// {
// 	int i, centerX, centerY, ind;
// 	double radius;

// 	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
// 	cairo_set_line_width(cr, 5.0);

// 	centerX = (width - 2 * NAV_PLOT_MARGIN) / 2 + NAV_PLOT_MARGIN;
// 	centerY = (height - 2 * NAV_PLOT_MARGIN) / 2 + NAV_PLOT_MARGIN;
// 	radius = (double) centerY - NAV_PLOT_MARGIN;

// 	/* Put the two full circles. */
// 	cairo_move_to(cr, centerX + radius, centerY);
// 	cairo_arc(cr, centerX, centerY, radius, 0.0, 2 * M_PI);
// 	cairo_move_to(cr, centerX + radius - 50, centerY);
// 	cairo_arc(cr, centerX, centerY, radius - 50, 0.0, 2 * M_PI);
// 	cairo_stroke(cr);

// 	/* Put the direction indicators. */
// 	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
// 	cairo_set_line_width(cr, 4.0);

// 	cairo_move_to(cr, centerX + radius - 50 - 15, centerY);
// 	cairo_line_to(cr, centerX + radius - 50 + 15, centerY);
// 	cairo_move_to(cr, centerX, centerY - radius + 50 - 15);
// 	cairo_line_to(cr, centerX, centerY - radius + 50 + 15);
// 	cairo_move_to(cr, centerX - radius + 50 - 15, centerY);
// 	cairo_line_to(cr, centerX - radius + 50 + 15, centerY);
// 	cairo_move_to(cr, centerX, centerY + radius - 50 - 15);
// 	cairo_line_to(cr, centerX, centerY + radius - 50 + 15);
// 	cairo_stroke(cr);

// 	cairo_move_to(cr, centerX + radius - 15, centerY);
// 	cairo_line_to(cr, centerX + radius + 15, centerY);
// 	cairo_move_to(cr, centerX, centerY - radius - 15);
// 	cairo_line_to(cr, centerX, centerY - radius + 15);
// 	cairo_move_to(cr, centerX - radius - 15, centerY);
// 	cairo_line_to(cr, centerX - radius + 15, centerY);
// 	cairo_move_to(cr, centerX, centerY + radius - 15);
// 	cairo_line_to(cr, centerX, centerY + radius + 15);
// 	cairo_stroke(cr);
// }

// void nav_plot_area_grid(cairo_t *cr, int width, int height)
// {
// 	int i;

// 	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);	/* gray grid */
// 	cairo_set_line_width(cr, 0.5);

// 	/* Put the vertical grid lines */
// 	for (i = NAV_PLOT_MARGIN + NAV_PLOT_GRID;
// 		  i < width - (NAV_PLOT_MARGIN); i += NAV_PLOT_GRID) 
// 	{
// 		cairo_move_to(cr, i, NAV_PLOT_MARGIN);
// 		cairo_line_to(cr, i, height - NAV_PLOT_MARGIN);
// 	}
// 	cairo_stroke(cr);

// 	/* Put the horizontal grid lines */
// 	for (i = height - NAV_PLOT_MARGIN - NAV_PLOT_GRID;
// 		  i >= NAV_PLOT_MARGIN; i -= NAV_PLOT_GRID) 
// 	{
// 		cairo_move_to(cr, NAV_PLOT_MARGIN, i);
// 		cairo_line_to(cr, width - NAV_PLOT_MARGIN, i);
// 	}
// 	cairo_stroke(cr);
// }

// void nav_plot_area_device(cairo_t *cr, int width, int height)
// {
// 	int middle, centerX, centerY;

// 	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);

// 	/* Get the center points to put the board device. */
// 	centerX = (width - (NAV_PLOT_MARGIN * 2)) / 2 + NAV_PLOT_MARGIN;
// 	centerY = (height - (NAV_PLOT_MARGIN * 2)) / 2 + NAV_PLOT_MARGIN;

// 	cairo_move_to(cr, centerX - 30, centerY); 
// 	cairo_line_to(cr, centerX + 30, centerY);
// 	cairo_line_to(cr, centerX, centerY - 120);
// 	cairo_close_path(cr);
// 	cairo_fill(cr);
// }

// void nav_plot_area_label(cairo_t *cr, int width, int height)
// {
// 	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
// 	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, 
// 		CAIRO_FONT_WEIGHT_NORMAL);
//    cairo_set_font_size(cr, 20.0);

// 	cairo_move_to(cr, width - NAV_PLOT_MARGIN - 200, 
// 							NAV_PLOT_MARGIN + 75);
// 	cairo_show_text(cr, "North-Eastean");

// 	cairo_move_to(cr, width - NAV_PLOT_MARGIN - 200, 
// 							height - NAV_PLOT_MARGIN - 70);
// 	cairo_show_text(cr, "South-Eastean");

// 	cairo_move_to(cr, NAV_PLOT_MARGIN + 50, 
// 							NAV_PLOT_MARGIN + 75);
// 	cairo_show_text(cr, "North-Westean");

// 	cairo_move_to(cr, NAV_PLOT_MARGIN + 50, 
// 							height - NAV_PLOT_MARGIN - 70);
// 	cairo_show_text(cr, "South-Weastean");
// }

// void nav_plot_area(GtkDrawingArea *area, cairo_t *cr, int width, 
// 						 int height, gpointer data)
// {
// 	/* Set the background of navigation plot area. */
// 	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
// 	cairo_paint(cr);

// 	nav_plot_area_axes(cr, width, height);		/* draw the axes */
// 	nav_plot_area_grid(cr, width, height);		/* draw the grid */
// 	nav_plot_area_device(cr, width, height);	/* draw the device */
// 	nav_plot_area_label(cr, width, height);	/* show the text */
// }

// void nav_plot_top_lines(cairo_t *cr, int width, int height)
// {
// 	int i;
// 	double center_x, center_y, range_x, range_y;

// 	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
// 	cairo_set_line_width(cr, 1.0);

// 	center_x = (width - (2 * NAV_PLOT_MARGIN)) / 2;
// 	center_y = NAV_PLOT_MARGIN + 2 * NAV_PLOT_GRID;
// 	range_x = center_x / 3;
// 	range_y = (height - center_y) / 3;

// 	cairo_move_to(cr, NAV_PLOT_MARGIN, center_y);
// 	cairo_line_to(cr, (width - NAV_PLOT_MARGIN), center_y);
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, center_x, (height - NAV_PLOT_MARGIN));
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, NAV_PLOT_MARGIN, (height - NAV_PLOT_MARGIN));
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, (width - NAV_PLOT_MARGIN), (height - NAV_PLOT_MARGIN));
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, (NAV_PLOT_MARGIN + range_x), (height - NAV_PLOT_MARGIN));
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, (NAV_PLOT_MARGIN + 2 * range_x), (height - NAV_PLOT_MARGIN));
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, (center_x + NAV_PLOT_MARGIN + range_x), (height - NAV_PLOT_MARGIN));
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, (center_x + NAV_PLOT_MARGIN + 2 * range_x), (height - NAV_PLOT_MARGIN));

// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, NAV_PLOT_MARGIN, (height - NAV_PLOT_MARGIN - range_y));
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, NAV_PLOT_MARGIN, (height - NAV_PLOT_MARGIN - 2 * range_y));
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, (width - NAV_PLOT_MARGIN), (height - NAV_PLOT_MARGIN - range_y));
// 	cairo_move_to(cr, center_x, center_y);
// 	cairo_line_to(cr, (width - NAV_PLOT_MARGIN), (height - NAV_PLOT_MARGIN - 2 * range_y));

// 	cairo_stroke(cr);
// }

// void nav_plot_top_dev(cairo_t *cr, int width, int height)
// {
// 	int i;
// 	double center_x, center_y;

// 	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
// 	cairo_set_line_width(cr, 3.0);

// 	center_x = (width - (2 * NAV_PLOT_MARGIN)) / 2;
// 	center_y = NAV_PLOT_MARGIN + 2 * NAV_PLOT_GRID;

// 	cairo_move_to(cr, center_x, (center_y + NAV_PLOT_GRID));
// 	cairo_line_to(cr, (center_x + 4 * NAV_PLOT_GRID), center_y);
// 	cairo_line_to(cr, center_x, (center_y - NAV_PLOT_GRID));
// 	cairo_line_to(cr, (center_x - 4 * NAV_PLOT_GRID), center_y);
// 	cairo_line_to(cr, center_x, (center_y + NAV_PLOT_GRID));
// 	// cairo_arc(cr, center_x, center_y, NAV_PLOT_GRID, 0, 2 * M_PI);
// 	// cairo_fill(cr);
// 	cairo_stroke(cr);
// }

// void nav_plot_bottom_lines(cairo_t *cr, int width, int height)
// {
// 	int i;
// 	double center_x;

// 	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
// 	cairo_set_line_width(cr, 1.0);

// 	center_x = (width - (2 * NAV_PLOT_MARGIN)) / 2;

// 	/* Put the vertical center line. */
// 	cairo_move_to(cr, center_x, NAV_PLOT_MARGIN);
// 	cairo_line_to(cr, center_x, height - NAV_PLOT_MARGIN);
// 	cairo_stroke(cr);
// }

// void nav_plot_top(GtkDrawingArea *area, cairo_t *cr, int width,
// 						int height, gpointer data)
// {
// 	/* Set the background of top plot area. */
// 	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
// 	cairo_paint(cr);

// 	nav_plot_grid(cr, width, height);		/* draw grids */
// 	nav_plot_top_lines(cr, width, height);	/* draw lines */
// 	nav_plot_top_dev(cr, width, height);	/* put device */
// }

// void nav_plot_bottom(GtkDrawingArea *area, cairo_t *cr, int width,
// 						 	int height, gpointer data)
// {
// 	/* Set the background of bottom plot area. */
// 	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
// 	cairo_paint(cr);

// 	nav_plot_grid(cr, width, height);			/* draw grids */
// 	nav_plot_bottom_lines(cr, width, height);	/* put lines */
// }

void nav_plot_area_grid(cairo_t *cr, int width, int height)
{
	int i;

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);	/* gray grid */
	cairo_set_line_width(cr, 0.3);

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
