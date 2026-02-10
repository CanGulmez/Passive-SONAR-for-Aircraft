/**
 ******************************************************************************
 * @file 	analysis.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Signal analysis operations of AeroSONAR.
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

/* Global and Shared Variables */

DspTime micSamples[MIC_COUNT] = {0};
DspTime micBeamformed = {0};
guint micVolumest = 1;

/**
 * Convert the payload mic data to 'DspTime' objects.
 */
void convert_payload_to_sample(void)
{
	int i;

	/* Convert the payload data to 'DspTime' objects. */
	for (i = 0; i < MIC_COUNT; i++)
	{
		micSamples[i].length = DATA_SIZE;
	}
	for (i = 0; i < DATA_SIZE; i++)
	{
		micSamples[0].data[i] = (double) payloadData.micNorth[i];
		micSamples[1].data[i] = (double) payloadData.micNorthEast[i];
		micSamples[2].data[i] = (double) payloadData.micEast[i];
		micSamples[3].data[i] = (double) payloadData.micSouthEast[i];
		micSamples[4].data[i] = (double) payloadData.micSouth[i];
		micSamples[5].data[i] = (double) payloadData.micSouthWest[i];
		micSamples[6].data[i] = (double) payloadData.micWest[i];
		micSamples[7].data[i] = (double) payloadData.micNorthWest[i];
	}
}

/**
 * Find the dominant frequency around the mic outputs.
 */
double find_dominant_freq(void)
{
	int i, j;
	DspFreq outputs[MIC_COUNT];
	DspTime magnitudes[MIC_COUNT];
	int indexes[MIC_COUNT];
	double frequencies[MIC_COUNT];
	double max_freq = 0;

	/* Convert the time domain signals into frequency domain. */
	for (i = 0; i < MIC_COUNT; i++)
	{
		dsp_transform_dft(&micSamples[i], &outputs[i]);
		outputs[i].length = (int) (outputs[i].length / 2);
	}
	/* Find the maximum frequencies and corresponding bins. */
	for (i = 0; i < MIC_COUNT; i++)
	{
		dsp_freq_magnitude(&outputs[i], &magnitudes[i]);
		magnitudes[i].data[0] = 0.0;	/* pass the DC bias */
		indexes[i] = dsp_time_argmax(&magnitudes[i]);
		frequencies[i] = (SAMPLING_FREQ / DATA_SIZE) * indexes[i];
	}
	/* Compare the frequencies to find the maximum. */
	for (i = 0; i < MIC_COUNT; i++)
	{
		if (frequencies[i] > max_freq)
		{
			max_freq = frequencies[i];
		}
	}
	return max_freq;
}

/**
 * Calculate the arrival of angle from coming signals.
 */
int calculate_arrival(double freq)
{
	int i;
	DspArrival arrival;

	arrival.mics = MIC_COUNT;
	arrival.freq = freq;
	arrival.radius = MIC_RADIUS;
	arrival.sources = 1;
	for (i = 0; i < MIC_COUNT; i++)
	{
		arrival.samples[i] = &micSamples[i];	/* samples itself */
	}
	return dsp_arrival_music(&arrival);
}

/**
 * Make the delay-and-sum beamforming.
 */
DspTime make_beamforming(double freq, double arrival)
{
	int i;
	DspBeamform beamform;
	DspTime sample;

	beamform.mics = MIC_COUNT;	
	beamform.freq = freq;	
	beamform.radius = MIC_RADIUS;	
	beamform.theta = arrival;
	for (i = 0; i < MIC_COUNT; i++)	
	{
		beamform.samples[i] = &micSamples[i];	/* samples itself */
	}
	dsp_beamform_delay_sum(&beamform, &sample);

	return sample;
}

/**
 * Make the other signal analysis to update `MicSignal` struct.
 */
void make_signal_analysis(DspTime *beamformed, int arrival)
{
	int i;
	char buffer[MIC_SIGNAL_NUM][BUFFER_SIZE];

	/* Make the signal analysis one by one. */
	snprintf(buffer[0], BUFFER_SIZE, "%.4f", dsp_time_max(beamformed));	/* maximum */
	snprintf(buffer[1], BUFFER_SIZE, "%.4f", dsp_time_min(beamformed));	/* minimum */
	snprintf(buffer[2], BUFFER_SIZE, "%.4f", dsp_time_mean(beamformed));	/* mean */
	snprintf(buffer[3], BUFFER_SIZE, "%.4f", dsp_time_stddev(beamformed)); /* standard deviation */
	snprintf(buffer[4], BUFFER_SIZE, "%.4f", dsp_time_energy(beamformed));	/* energy */
	snprintf(buffer[5], BUFFER_SIZE, "%.4f", dsp_time_rms(beamformed));	/* RMS */
	snprintf(buffer[6], BUFFER_SIZE, "%.4f", dsp_time_power(beamformed));	/* power */
	snprintf(buffer[7], BUFFER_SIZE, "%.4f", dsp_time_crest_factor(beamformed));	/* crest factor */
	snprintf(buffer[8], BUFFER_SIZE, "%.4f", dsp_time_skewness(beamformed));	/* skewness */
	snprintf(buffer[9], BUFFER_SIZE, "%.4f", dsp_time_kurtosis(beamformed));	/* kurtosis */
	snprintf(buffer[10], BUFFER_SIZE, "%.4f", dsp_time_variance(beamformed));	/* variance */
	snprintf(buffer[11], BUFFER_SIZE, "%d", arrival);	/* arrival of angle */

	/* Update the signal analysis rows. */
	for (i = 0; i < 12; i++)
	{
		__generic_action_row_update(micSignalRows[i], buffer[i]);
	}
}

/**
 * Return the sector that has much more intensity.
 */
int select_sector(void)
{
	int i;
	double means[MIC_COUNT], biggest;

	/* Get the mean of sensor signals. */
	for (i = 0; i < MIC_COUNT; i++)
	{
		means[i] = dsp_time_mean(&micSamples[i]);
	}
	/* Find the biggest mean. */
	biggest = means[i];
	for (i = 0; i < MIC_COUNT; i++)
	{
		if (means[i] > biggest)
		{
			biggest = means[i];
		}
	}
	for (i = 0; i < MIC_COUNT; i++)
	{
		if (means[i] == biggest)
		{
			return i + 1;
		}
	}
}