/**
 ******************************************************************************
 * @file 	dsp.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Main headers of Digital Signal Processing.
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

#ifndef DSP_H
#define DSP_H

/* Standard GNU Libraries */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

/* GNU Scientific Library */

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>

/* GUI and Plot Libraries */

#include <gtk/gtk.h>
#include <cairo/cairo.h>
 
/* User-defined Constants */

#define MAX_DATA        		4096
#define MAX_MICS           	24  
#define MAX_SOURCES        	8       
#define SOUND_SPEED        	343.0    // m/s

#define DEG(rad)           	(rad * 180.0 / M_PI)
#define RAD(deg)           	(deg * M_PI / 180.0)

#define PLOT_ID					"com.example.DSPPlot"
#define PLOT_MARGIN				40		/* pixel */
#define PLOT_GRID					20		/* pixel */
#define PLOT_WIDTH				800	/* pixel */
#define PLOT_HEIGHT				600	/* pixel */

/* User-defined Data Types */
 
typedef unsigned int len_t; 
typedef unsigned char bool_t;

/* User-defined Enumerations */

typedef enum {
	DSP_COLOR_BLACK,
	DSP_COLOR_WHITE,
	DSP_COLOR_RED,
	DSP_COLOR_GREEN,
	DSP_COLOR_BLUE,
	DSP_COLOR_YELLOW,
	DSP_COLOR_MAGENTA,
	DSP_COLOR_CYAN,
	DSP_COLOR_ORANGE,
	DSP_COLOR_PINK,
	DSP_COLOR_BROWN,
	DSP_COLOR_PURPLE,
	DSP_COLOR_SILVER,
	DSP_COLOR_GOLD
} dsp_color_t;

typedef enum {
	DSP_IIR_FILTER_LOW_PASS,
	DSP_IIR_FILTER_HIGH_PASS,
	DSP_IIR_FILTER_BAND_PASS,
	DSP_IIR_FILTER_BAND_STOP
} dsp_iir_filter_t;

/* User-defined Structures */
 
typedef struct {
   len_t length;
   double data[MAX_DATA];
} dsp_time_t;

typedef struct {
	len_t length;
	double *data;
} dsp_t;

typedef struct {
   len_t length;
   double data[MAX_DATA][2];
} dsp_freq_t;
 
typedef struct {
   uint8_t mics;							/* mics in phased array */
   double fs;                       /* sampling frequency in Hz */
   double radius;                   /* radius in meter */
   double theta;                    /* arrival angle in degrees */
   dsp_time_t *samples[MAX_MICS];
} dsp_beamform_t;

typedef struct {
   uint8_t mics;							/* mics in phased array */
   double radius;                   /* radius in meters */
   double freq;                     /* signal frequency in Hz */
   uint8_t sources;						/* sound sources */
   dsp_time_t *samples[MAX_MICS];		
} dsp_arrival_t; 

typedef struct {
	const char *title;					/* title of plot */
	dsp_time_t *sample;					/* sample to be plotted */
	double width;							/* sample data line width */
	dsp_color_t color;					/* sample data line color */
	int factor;								/* factor to widen the x-axis */
} dsp_plot_t;


/**
 * Validate the `length` object. It's passed by value to functions.
 */
#define assert_length(length)														\
{																							\
	assert (length > 0 && length <= MAX_DATA);								\
}

/**
 * Validate the `sample` object. It's passed by reference to functions.
 */
#define assert_sample(sample) 													\
{																							\
	assert (sample != NULL);														\
	assert_length(sample->length);												\
}		

/**
 * Validate the `beamform` object. It's passed by reference to functions.
 */
#define assert_beamform(beamform)												\
{                               													\
   assert (beamform->mics > 1 && beamform->mics <= MAX_MICS);        \
   for (int i = 0; i < beamform->mics; i++)									\
	{																						\
		assert_sample(beamform->samples[i]); 									\
	}                         														\
}

/**
 * Validate the `arrival` object. It's passed by reference to functions.
 */
#define assert_arrival(arrival)													\
{                                 												\
   assert (arrival->mics > 0 && arrival->mics <= MAX_MICS &&         \
      arrival->sources > 0 && arrival->sources <= MAX_SOURCES);      \
   for (int i = 0; i < arrival->mics; i++)									\
	{																						\
		assert_sample(arrival->samples[i]);										\
	}                          													\
} 

/**
 * Validate the `plot` object. It's passed by reference to functions.
 */
#define assert_plot(plot)															\
{																							\
	assert(plot->title != NULL);													\
	assert_sample(plot->sample);													\
	assert(plot->factor >= 1);														\
	assert((plot->color == DSP_COLOR_BLACK) 	|| 							\
			 (plot->color == DSP_COLOR_WHITE)	||								\
			 (plot->color == DSP_COLOR_RED)		||								\
			 (plot->color == DSP_COLOR_GREEN)	||								\
			 (plot->color == DSP_COLOR_BLUE)		||								\
			 (plot->color == DSP_COLOR_YELLOW)	||								\
			 (plot->color == DSP_COLOR_MAGENTA)	||								\
			 (plot->color == DSP_COLOR_CYAN)		||								\
			 (plot->color == DSP_COLOR_ORANGE)	||								\
			 (plot->color == DSP_COLOR_PINK)		||								\
			 (plot->color == DSP_COLOR_BROWN)	||								\
			 (plot->color == DSP_COLOR_PURPLE)	||								\
			 (plot->color == DSP_COLOR_SILVER)	||								\
			 (plot->color == DSP_COLOR_GOLD));									\
	assert(plot->width >= 0.0);													\
}

/* Time Domain Signal Processing Methods  */

extern void dsp_time_add(const dsp_time_t *fsample, const dsp_time_t *ssample, dsp_time_t *result);
extern void dsp_time_subtract(const dsp_time_t *fsample, const dsp_time_t *ssample, dsp_time_t *result);
extern void dsp_time_scalar_mul(const dsp_time_t *sample, double scalar, dsp_time_t *result);
extern void dsp_time_dot_mul(const dsp_time_t *fsample, const dsp_time_t *ssample, dsp_time_t *result);
extern void dsp_time_scalar_div(const dsp_time_t *sample, double scalar, dsp_time_t *result);
extern void dsp_time_resize(const dsp_time_t *sample, len_t length, dsp_time_t *result);
extern void dsp_time_insert(const dsp_time_t *sample, int index, double data, dsp_time_t *result);
extern void dsp_time_remove(const dsp_time_t *sample, int index, dsp_time_t *result);
extern void dsp_time_append(const dsp_time_t *sample, double data, dsp_time_t *result);
extern void dsp_time_pop(const dsp_time_t *sample, dsp_time_t *result);
extern void dsp_time_shift(const dsp_time_t *sample, int shift, dsp_time_t *result);
extern void dsp_time_delay_linear(const dsp_time_t *sample, double delay, dsp_time_t *result);
extern void dsp_time_delay_lagrange(const dsp_time_t *sample, double delay, dsp_time_t *result);
extern void dsp_time_concat(const dsp_time_t *fsample, const dsp_time_t *ssample, dsp_time_t * rsample);
extern void dsp_time_abs(const dsp_time_t *sample, dsp_time_t *result);
extern void dsp_time_rand(len_t length, dsp_time_t *result);
extern void dsp_time_randn(len_t length, dsp_time_t *result); 
extern len_t dsp_time_length(const dsp_time_t *sample);
extern double dsp_time_data(const dsp_time_t *sample, int index);
extern int dsp_time_index(const dsp_time_t *sample, double data);
extern double dsp_time_max(const dsp_time_t *sample);
extern double dsp_time_min(const dsp_time_t *sample);
extern double dsp_time_abs_max(const dsp_time_t *sample);
extern double dsp_time_abs_min(const dsp_time_t *sample);
extern int dsp_time_argmax(const dsp_time_t *sample);
extern int dsp_time_argmin(const dsp_time_t *sample);
extern double dsp_time_mean(const dsp_time_t *sample);
extern double dsp_time_stddev(const dsp_time_t *sample);
extern double dsp_time_energy(const dsp_time_t *sample);
extern double dsp_time_power(const dsp_time_t *sample);
extern double dsp_time_rms(const dsp_time_t *sample);
extern double dsp_time_crest_factor(const dsp_time_t *sample);
extern double dsp_time_skewness(const dsp_time_t *sample);
extern double dsp_time_kurtosis(const dsp_time_t *sample);
extern double dsp_time_variance(const dsp_time_t *sample);
extern double dsp_time_snr(const dsp_time_t *sample, const dsp_time_t* noise);
extern double dsp_time_entropy(const dsp_time_t *sample, int bins);
extern void dsp_time_scale(const dsp_time_t *sample, double scale, dsp_time_t *result);
extern void dsp_time_downsample(const dsp_time_t *sample, int factor, dsp_time_t *result);
extern void dsp_time_upsample(const dsp_time_t *sample, int factor, dsp_time_t *result);
extern void dsp_time_peaks(const dsp_time_t *sample, double threshold, dsp_time_t *result);
extern void dsp_time_clip(const dsp_time_t *sample, double min_val, double max_val, dsp_time_t *result);
extern void dsp_time_convolve(const dsp_time_t *fsample, const dsp_time_t *ssample, dsp_time_t *result);
extern void dsp_time_auto_corr(const dsp_time_t *sample, dsp_time_t *result);
extern void dsp_time_auto_corr_neg(const dsp_time_t *sample, dsp_time_t *result);
extern void dsp_time_auto_corr_pos(const dsp_time_t *sample, dsp_time_t *result);
extern void dsp_time_cross_corr(const dsp_time_t *fsample, const dsp_time_t *ssample, dsp_time_t *result);
extern void dsp_time_cross_corr_neg(const dsp_time_t *fsample, const dsp_time_t *ssample, dsp_time_t *result);
extern void dsp_time_cross_corr_pos(const dsp_time_t *fsample, const dsp_time_t *ssample, dsp_time_t *result); 

/* Frequency Domain Signal Processing Methods */

extern void dsp_freq_add(const dsp_freq_t *fsample, const dsp_freq_t *ssample, dsp_freq_t *result);
extern void dsp_freq_subtract(const dsp_freq_t *fsample, const dsp_freq_t *ssample, dsp_freq_t *result);
extern void dsp_freq_scalar_mul(const dsp_freq_t *sample, double scalar, dsp_freq_t *result);
extern void dsp_freq_dot_mul(const dsp_freq_t *fsample, const dsp_freq_t *ssample, dsp_freq_t *result);
extern void dsp_freq_scalar_div(const dsp_freq_t *sample, double scalar, dsp_freq_t *result);
extern void dsp_freq_resize(const dsp_freq_t *sample, len_t length, dsp_freq_t *result);
extern void dsp_freq_insert(const dsp_freq_t *sample, double real, double imag, int index, dsp_freq_t *result);
extern void dsp_freq_remove(const dsp_freq_t *sample, int index, dsp_freq_t *result);
extern void dsp_freq_append(const dsp_freq_t *sample, double real, double imag, dsp_freq_t *result);
extern void dsp_freq_pop(const dsp_freq_t *sample, dsp_freq_t *result);
extern void dsp_freq_concat(const dsp_freq_t *fsample, const dsp_freq_t *ssample, dsp_freq_t *result);
extern len_t dsp_freq_length(const dsp_freq_t *sample);
extern void dsp_freq_real(const dsp_freq_t *sample, dsp_time_t *result);
extern void dsp_freq_imag(const dsp_freq_t *sample, dsp_time_t *result);
extern void dsp_freq_magnitude(const dsp_freq_t *sample, dsp_time_t *result);
extern void dsp_freq_phase(const dsp_freq_t *sample, dsp_time_t *result);
extern double dsp_freq_psd(const dsp_freq_t *sample, int index);
extern double dsp_freq_power(const dsp_freq_t *sample);
extern double dsp_freq_spectral_centroid(const dsp_freq_t *sample, double fs);
extern double dsp_freq_spectral_flatness(const dsp_freq_t *sample);
extern double dsp_freq_spectral_rolloff(const dsp_freq_t *sample, double threshold, double fs);
extern double dsp_freq_thd(const dsp_freq_t *sample, double fs);

/* Plotting Methods */

extern void __plot_frame(GtkDrawingArea *area, cairo_t *cr, int width, int height);
extern void __plot_grid(GtkDrawingArea *area, cairo_t *cr, int width, int height);
extern void __plot_label_x(GtkDrawingArea *area, cairo_t *cr, len_t length, int width, int height);
extern void __plot_label_y(GtkDrawingArea *area, cairo_t *cr, int width, int height);
extern void __plot_indices(GtkDrawingArea *area, cairo_t *cr, dsp_plot_t *plot, int width, int height);
extern void __plot_color_to_rgb(dsp_color_t color, double rgb[3]);
extern void __plot_sample_data(GtkDrawingArea *area, cairo_t *cr, dsp_plot_t *plot, int width, int height);
extern void __plot_drawing(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data);
extern void __plot_drawing(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data);
extern void __plot_button_clicked(GtkWidget *button, gpointer data);
extern void __plot_activate(GtkApplication *app, gpointer data);

extern int dsp_plot_sample(const dsp_plot_t *plot);

/* Signal Generation Methods */

extern void dsp_signal_normal(double mean, double stddev, len_t length, dsp_time_t *result);
extern void dsp_signal_awgn(const dsp_time_t *sample, double snr, dsp_time_t *result);
extern void dsp_signal_sin(double A, double fc, double fs, double theta, len_t length, dsp_time_t *result);
extern void dsp_signal_sinc(double A, double fc, double fs, double theta, len_t length, dsp_time_t *result);
extern void dsp_signal_cos(double A, double fc, double fs, double theta, len_t length, dsp_time_t *result);
extern void dsp_signal_impulse(int index, len_t length, dsp_time_t *result);
extern void dsp_signal_step(double A, int fsample, int ssample, len_t length, dsp_time_t *result);
extern void dsp_signal_square(double A, double fc, double fs, len_t length, dsp_time_t *result);
extern void dsp_signal_sawtooth(double A, double fc, double fs, len_t length, dsp_time_t *result);
extern void dsp_signal_triangle(double A, double fc, double fs, len_t length, dsp_time_t *result);

/* Windowing Methods */

extern void dsp_window_hamming(const dsp_time_t *sample, dsp_time_t *result);
extern void dsp_window_hanning(const dsp_time_t *sample, dsp_time_t *result);
extern void dsp_window_blackman(const dsp_time_t *sample, dsp_time_t *result);
extern void dsp_window_chebyshev(const dsp_time_t *sample, int factor, dsp_time_t *result);
extern void dsp_window_kaiser(const dsp_time_t *sample, int factor, dsp_time_t *result);

/* Time-Frequency Transformation Methods */

extern void dsp_transform_dft(const dsp_time_t *sample, dsp_freq_t *result);
extern void dsp_transform_dft_real(const dsp_time_t *sample, dsp_freq_t *result);
extern void dsp_transform_idft(const dsp_freq_t *sample, dsp_time_t *result);
// extern void dsp_transform_fft(const dsp_time_t *sample, dsp_freq_t *result);
// extern void dsp_transform_ifft(const dsp_freq_t *sample, dsp_time_t *result);
// extern void dsp_transform_dct(const dsp_time_t *sample, dsp_time_t *result);
// extern void dsp_transform_idct(const dsp_time_t *sample, dsp_time_t *result);
// extern void dsp_transform_hilbert(const dsp_time_t *sample, dsp_time_t *result);

/* FIR-based Filter Methods */

extern void dsp_filter_fir_low_pass(const dsp_time_t *sample, double fc, double fs, int taps, dsp_time_t *result);
extern void dsp_filter_fir_high_pass(const dsp_time_t *sample, double fc, double fs, int taps, dsp_time_t *result);
extern void dsp_filter_fir_band_pass(const dsp_time_t *sample, double fc1, double fc2, double fs, int taps, dsp_time_t *result);
extern void dsp_filter_fir_band_stop(const dsp_time_t *sample, double fc1, double fc2, double fs, int taps, dsp_time_t *result);
extern void dsp_filter_iir(const dsp_time_t *sample, dsp_iir_filter_t filter, double fc, double fs, double Q, dsp_time_t *result);
extern void dsp_filter_iir_low_pass(const dsp_time_t *sample, double fc, double fs, dsp_time_t *result);
extern void dsp_filter_iir_high_pass(const dsp_time_t *sample, double fc, double fs, dsp_time_t *result);
extern void dsp_filter_iir_band_pass(const dsp_time_t *sample, double fc, double fs, double Q, dsp_time_t *result);
extern void dsp_filter_iir_band_stop(const dsp_time_t *sample, double fc, double fs, double Q, dsp_time_t *result);
extern void dsp_filter_dc_block(const dsp_time_t *sample, double fc, double fs, dsp_time_t *result);

/* Beamforming Methods */

extern void dsp_beamform_delay_sum(const dsp_beamform_t *beamform, dsp_time_t *result); 
// extern void dsp_beamform_mvdr(const dsp_beamform_t *beamform, double tetha, dsp_time_t *result);

/* DoA (Direction of Arrival) Methods */

extern int dsp_arrival_music(const dsp_arrival_t *arrival); 

#endif /* DSP */
