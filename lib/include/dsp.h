/* Real-Time Digital Signal Processing */

#ifndef DSP_H
#define DSP_H

/* Standard GNU libraries */

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
 
/* User-defined constants */

#define MAX_SAMPLES        1024
#define MAX_MICS           24    
#define MAX_SOURCES        8       
#define SOUND_SPEED        343.0    // m/s

#define DEG(rad)           (rad * 180.0 / M_PI)
#define RAD(deg)           (deg * M_PI / 180.0)

#define assert_signal(signal) do {                                   \
   assert (signal.length > 0 && signal.length <= MAX_SAMPLES);       \
} while (0)

#define assert_beamform(beamform) do {                               \
   assert (beamform.mics > 1 && beamform.mics <= MAX_MICS);          \
   for (int i = 0; i < beamform.mics; i++)                           \
      assert_signal(beamform.signals[i]);                            \
} while (0)
 
// #define assert_doa(arrival) do {                                     \
//    assert (arrival.mics > 0 && arrival.mics  i++)                    \
//       assert_signal(beamform.signals[i]);                            \
// } while (0)
 
#define assert_doa(arrival) do {                                     \
   assert (arrival.mics > 0 && arrival.mics <= MAX_MICS &&           \
      arrival.sources > 0 && arrival.sources <= MAX_SOURCES);        \
   for (int i = 0; i < arrival.mics; i++)                            \
      assert_signal(arrival.signals[i]);                             \
} while (0);

/* User-defined data types */
 
typedef unsigned int len_t; 

/* User-defined enumerations */

typedef enum {
   false = 0, 
   true
} bool_t;
 
/* User-defined data structures */
 
typedef struct {
   len_t length;
   double samples[MAX_SAMPLES];
} dsp_time_t;

typedef struct {
   len_t length;
   double samples[MAX_SAMPLES][2];
} dsp_freq_t;

typedef struct {
   uint8_t mics;
   double fs;                       /* Sampling frequency in Hz */
   double radius;                   /* Radius in meters */
   double theta;                    /* Arrival angle in degrees */
   dsp_time_t signals[MAX_MICS];
} dsp_beamform_t;
 
typedef struct {
   uint8_t mics;
   double radius;                   /* Radius in meters */
   double freq;                     /* Signal frequency in Hz */
   uint8_t sources;
   dsp_time_t signals[MAX_MICS];
} dsp_doa_t; 

/* Time Domain Digital Signal Processing Methods  */

extern dsp_time_t dsp_time_add(dsp_time_t fsignal, dsp_time_t ssignal);
extern dsp_time_t dsp_time_subtract(dsp_time_t fsignal, dsp_time_t ssignal);
extern dsp_time_t dsp_time_scalar_mul(dsp_time_t fsignal, double scalar);
extern dsp_time_t dsp_time_dot_mul(dsp_time_t fsignal, dsp_time_t ssignal);
extern dsp_time_t dsp_time_resize(dsp_time_t signal, len_t length);
extern dsp_time_t dsp_time_insert(dsp_time_t signal, double value, int sample);
extern dsp_time_t dsp_time_remove(dsp_time_t signal, int sample);
extern dsp_time_t dsp_time_append(dsp_time_t signal, double value);
extern dsp_time_t dsp_time_pop(dsp_time_t signal);
extern dsp_time_t dsp_time_shift(dsp_time_t signal, int shift);
extern dsp_time_t dsp_time_delay_linear(dsp_time_t signal, double delay);
extern dsp_time_t dsp_time_delay_lagrange(dsp_time_t signal, double delay);
extern dsp_time_t dsp_time_delay_sinc(dsp_time_t signal, double delay, int taps);
extern dsp_time_t dsp_time_concat(dsp_time_t fsignal, dsp_time_t ssignal);
extern dsp_time_t dsp_time_scalar_div(dsp_time_t signal, double scalar);
extern dsp_time_t dsp_time_abs(dsp_time_t signal);
extern int dsp_time_length(dsp_time_t signal);
extern double dsp_time_value(dsp_time_t signal, int sample);
extern int dsp_time_sample(dsp_time_t signal, double value);
extern double dsp_time_max(dsp_time_t signal);
extern double dsp_time_min(dsp_time_t signal);
extern double dsp_time_abs_max(dsp_time_t signal);
extern double dsp_time_abs_min(dsp_time_t signal);
extern int dsp_time_argmax(dsp_time_t signal);
extern int dsp_time_argmin(dsp_time_t signal);
extern double dsp_time_mean(dsp_time_t signal);
extern double dsp_time_stddev(dsp_time_t signal);
extern double dsp_time_energy(dsp_time_t signal);
extern double dsp_time_rms(dsp_time_t signal);
extern double dsp_time_crest_factor(dsp_time_t signal);
extern double dsp_time_skewness(dsp_time_t signal);
extern double dsp_time_kurtosis(dsp_time_t signal);
extern double dsp_time_variance(dsp_time_t signal);
extern double dsp_time_power(dsp_time_t signal);
extern double dsp_time_snr(dsp_time_t signal, dsp_time_t noise);
extern double dsp_time_entropy(dsp_time_t signal, int resolution);
extern dsp_time_t dsp_time_downsample(dsp_time_t signal, int factor);
extern dsp_time_t dsp_time_upsample(dsp_time_t signal, int factor);
extern dsp_time_t dsp_time_peaks(dsp_time_t signal, double threshold);
extern dsp_time_t dsp_time_clip(dsp_time_t signal, double min_val, double max_val);
extern dsp_time_t dsp_time_convolve(dsp_time_t fsignal, dsp_time_t ssignal);
extern dsp_time_t dsp_time_auto_corr(dsp_time_t signal);
extern dsp_time_t dsp_time_auto_corr_neg(dsp_time_t signal);
extern dsp_time_t dsp_time_auto_corr_pos(dsp_time_t signal);
extern dsp_time_t dsp_time_cross_corr(dsp_time_t fsignal, dsp_time_t ssignal);
extern dsp_time_t dsp_time_cross_corr_neg(dsp_time_t fsignal, dsp_time_t ssignal);
extern dsp_time_t dsp_time_cross_corr_pos(dsp_time_t fsignal, dsp_time_t ssignal); 

/* Frequency Domain Signal Processing Methods */

extern dsp_freq_t dsp_freq_add(dsp_freq_t fsignal, dsp_freq_t ssignal);
extern dsp_freq_t dsp_freq_subtract(dsp_freq_t fsignal, dsp_freq_t ssignal);
extern dsp_freq_t dsp_freq_scalar_mul(dsp_freq_t fsignal, double scalar);
extern dsp_freq_t dsp_freq_dot_mul(dsp_freq_t fsignal, dsp_freq_t ssignal);
extern dsp_freq_t dsp_freq_scalar_div(dsp_freq_t signal, double scalar);
extern dsp_freq_t dsp_freq_resize(dsp_freq_t signal, len_t length);
extern dsp_freq_t dsp_freq_insert(dsp_freq_t signal, double real, double imag, int sample);
extern dsp_freq_t dsp_freq_remove(dsp_freq_t signal, int sample);
extern dsp_freq_t dsp_freq_append(dsp_freq_t signal, double real, double imag);
extern dsp_freq_t dsp_freq_pop(dsp_freq_t signal);
extern dsp_freq_t dsp_freq_concat(dsp_freq_t ssignal, dsp_freq_t fsignal);
extern int dsp_freq_length(dsp_freq_t signal);
extern double dsp_freq_real(dsp_freq_t signal, int sample);
extern dsp_time_t dsp_freq_real_seq(dsp_freq_t signal);
extern dsp_time_t dsp_freq_real_abs_seq(dsp_freq_t signal);
extern double dsp_freq_real_abs(dsp_freq_t signal, int sample);
extern double dsp_freq_real_max(dsp_freq_t signal);
extern double dsp_freq_real_min(dsp_freq_t signal);
extern double dsp_freq_real_abs_max(dsp_freq_t signal);
extern double dsp_freq_real_abs_min(dsp_freq_t signal);
extern double dsp_freq_real_mean(dsp_freq_t signal);
extern double dsp_freq_real_stddev(dsp_freq_t signal);
extern int dsp_freq_real_argmax(dsp_freq_t signal);
extern int dsp_freq_real_argmin(dsp_freq_t signal);
extern double dsp_freq_imag(dsp_freq_t signal, int sample);
extern double dsp_freq_imag_abs(dsp_freq_t signal, int sample);
extern dsp_time_t dsp_freq_imag_seq(dsp_freq_t signal);
extern dsp_time_t dsp_freq_imag_abs_seq(dsp_freq_t signal);
extern double dsp_freq_imag_max(dsp_freq_t signal);
extern double dsp_freq_imag_abs_max(dsp_freq_t signal);
extern double dsp_freq_imag_abs_min(dsp_freq_t signal);
extern double dsp_freq_imag_min(dsp_freq_t signal);
extern double dsp_freq_imag_mean(dsp_freq_t signal);
extern double dsp_freq_imag_stddev(dsp_freq_t signal);
extern int dsp_freq_imag_argmax(dsp_freq_t signal);
extern int dsp_freq_imag_argmin(dsp_freq_t signal);
extern double dsp_freq_magnitude(dsp_freq_t signal, int sample);
extern double dsp_freq_magnitude_abs(dsp_freq_t signal, int sample);
extern dsp_time_t dsp_freq_magnitude_seq(dsp_freq_t signal);
extern dsp_time_t dsp_freq_magnitude_abs_seq(dsp_freq_t signal);
extern double dsp_freq_magnitude_max(dsp_freq_t signal);
extern double dsp_freq_magnitude_min(dsp_freq_t signal);
extern double dsp_freq_magnitude_abs_max(dsp_freq_t signal);
extern double dsp_freq_magnitude_abs_min(dsp_freq_t signal);
extern int dsp_freq_magnitude_argmax(dsp_freq_t signal);
extern int dsp_freq_magnitude_argmin(dsp_freq_t signal);
extern double dsp_freq_magnitude_mean(dsp_freq_t signal);
extern double dsp_freq_magnitude_stddev(dsp_freq_t signal);
extern double dsp_freq_phase(dsp_freq_t signal, int sample);
extern dsp_time_t dsp_freq_phase_seq(dsp_freq_t signal);
extern double dsp_freq_phase_max(dsp_freq_t signal);
extern double dsp_freq_phase_min(dsp_freq_t signal);
extern int dsp_freq_phase_argmax(dsp_freq_t signal);
extern int dsp_freq_phase_argmin(dsp_freq_t signal);
extern double dsp_freq_psd(dsp_freq_t signal, int sample);
extern double dsp_freq_power(dsp_freq_t signal);
extern double dsp_freq_spectral_centroid(dsp_freq_t signal, double fs);
extern double dsp_freq_spectral_flatness(dsp_freq_t signal);
extern double dsp_freq_spectral_rolloff(dsp_freq_t signal, double threshold, double fs);
extern double dsp_freq_thd(dsp_freq_t signal, double fs);

/* Signal Generation Methods */

extern dsp_time_t dsp_signal_normal(double mean, double stddev, len_t length);
extern dsp_time_t dsp_signal_awgn(dsp_time_t signal, double snr);
extern dsp_time_t dsp_signal_sin(double A, double f, double fs, double theta, len_t length);
extern dsp_time_t dsp_signal_sinc(double A, double f, double fs, double theta, len_t length);
extern dsp_time_t dsp_signal_cos(double A, double f, double fs, double theta, len_t length);
extern dsp_time_t dsp_signal_impulse(int sample, len_t length);
extern dsp_time_t dsp_signal_step(double A, int fsample, int ssample, len_t length);
extern dsp_time_t dsp_signal_square(double A, double f, double fs, len_t length);
extern dsp_time_t dsp_signal_sawtooth(double A, double f, double fs, len_t length);
extern dsp_time_t dsp_signal_triangle(double A, double f, double fs, len_t length);

/* Windowing Methods */

extern dsp_time_t dsp_windowing_hamming(dsp_time_t signal);
extern dsp_time_t dsp_windowing_hanning(dsp_time_t signal);
extern dsp_time_t dsp_windowing_blackman(dsp_time_t signal);
extern dsp_time_t dsp_windowing_chebyshev(dsp_time_t signal, int factor);
extern dsp_time_t dsp_windowing_kaiser(dsp_time_t signal, int factor);

/* Transformation Methods */

extern dsp_freq_t dsp_transform_dft(dsp_time_t signal);
extern dsp_freq_t dsp_transform_dft_real(dsp_time_t signal);
extern dsp_time_t dsp_transform_idft(dsp_freq_t signal);

/* Digital FIR-based Filter Methods */

extern dsp_time_t dsp_filter_high_pass(dsp_time_t signal, double fc, double fs, int taps);
extern dsp_time_t dsp_filter_low_pass(dsp_time_t signal, double fc, double fs, int taps);
extern dsp_time_t dsp_filter_band_pass(dsp_time_t signal, double fc1, double fc2, double fs, int taps);
extern dsp_time_t dsp_filter_band_stop(dsp_time_t signal, double fc1, double fc2, double fs, int taps);

/* Beamforming Methods */

extern dsp_time_t dsp_beamforming_delay_sum(dsp_beamform_t beamform); 

/* DoA (Direction of Arrival) Methods */

extern int dsp_doa_music(dsp_doa_t arrival); 

#endif /* DSP */
