""" 
Acoustic Sensor Data Deep Learning Model 
----------------------------------------

This model is designed for TUBITAK 2209-A as a part of AeroSONAR
project. This project contains any sub-modules and each modules 
has massive functionalities. In "AI Model" submodule, this Keras
model is being fitted, evaluated, tested. 

The model parameters that come from ground station:

+ Dataset
+ Output Model
+ Layer Type
+ Units
+ Epochs
+ Batch Size
+ Early Stopping
+ Dropout

The ground station will run this command so that parse the command-
line arguments in this format:

$ python3 acoustic_model.py [-h] dataset output_model layer units \
	epochs batch_size early_stopping dropout

"""

# Import the libraries.

import os , sys, time, argparse
import numpy as np
import pandas as pd
import tensorflow.keras as keras

# Parse the command-line aeguments.

parser = argparse.ArgumentParser(
	prog = sys.argv[0],
	description = "Acoustic sensor data deep learning model",
	epilog="The model will be used for AeroSONAR project's "
 			 "AI integration."
)

parser.add_argument("dataset", type=str)
parser.add_argument("output_model", type=str)
parser.add_argument("layer", type=str)
parser.add_argument("units", type=str)
parser.add_argument("epochs", type=str)
parser.add_argument("batch_size", type=str)
parser.add_argument("early_stopping", type=str)
parser.add_argument("dropout", type=str)
args = parser.parse_args()

DATASET			= args.dataset
OUTPUT_MODEL	= args.output_model
LAYER				= args.layer
UNITS				= int(args.units)
EPOCHS			= int(args.epochs)
BATCH_SIZE		= int(args.batch_size)
EARLY_STOPPING	= int(args.early_stopping)
DROPOUT			= float(args.dropout.replace(",", "."))

print(
   f"\nProcessing '{DATASET}' dataset with {LAYER} layer type, {UNITS}"
   f" units per layer, {EPOCHS} epochs, and {BATCH_SIZE} batch size as"
   f" '{OUTPUT_MODEL}.keras' in the script directory."
)

# After the parsed the command-line arguments, get the dataset file
# and open it in "r+" mode. Extract the CVS data into NumPy structure.

with open(DATASET) as dataset:
   data = dataset.read()
   
lines = data.split("\n")
header = lines[0].split(",")
lines = lines[1:]

print(f"\nFound {len(lines)} samples and {len(header)} data in each sample.")

temperature = np.zeros((len(lines),))
raw_data = np.zeros((len(lines), len(header) - 1))
for i, line in enumerate(lines):
   values = [float(x) for x in line.split(",")[1:]]
   temperature[i] = values[1]
   raw_data[i, :] = values[:]
   
print(f"Raw data shape : ({raw_data.shape})")
print(f"Target shape   : ({temperature.shape})")

# Split the raw data into train, validation and test groups.

num_train_samples = int(0.5 * len(raw_data))
num_val_samples = int(0.25 * len(raw_data))
num_test_samples = len(raw_data) - num_train_samples - num_val_samples

print(f"\nSplitted the raw data into train ({num_train_samples} samples), validation "
      f"({num_val_samples} samples) and test ({num_test_samples} samples) groups.\n")

# Prepare the data to fit. Firstly, normalize the row data so that each 
# raw data will be between [-1.0, 1.0]. This is default behaviour for 
# deep learning model. The characterisitic of raw data is timeseries.
# That means, Each data is bound and directly correlated with previous
# and next raw data.

mean = raw_data[:num_train_samples].mean(axis=0)
raw_data -= mean
std = raw_data[:num_train_samples].std(axis=0)
raw_data /= std

sampling_rate = 6
sequence_length = 120
delay = sampling_rate * (sequence_length + 24 - 1)
batch_size = 256

train_dataset = keras.utils.timeseries_dataset_from_array(
	data=raw_data[:-delay],
	targets=temperature[delay:],
	sampling_rate=sampling_rate,
	sequence_length=sequence_length,
	shuffle=True,
	batch_size=batch_size,
	start_index=0,
	end_index=num_train_samples
)

val_dataset = keras.utils.timeseries_dataset_from_array(
	data=raw_data[:-delay],
	targets=temperature[delay:],
	sampling_rate=sampling_rate,
	sequence_length=sequence_length,
	shuffle=True,
	batch_size=batch_size,
	start_index=num_train_samples,
	end_index=num_train_samples + num_val_samples
)

test_dataset = keras.utils.timeseries_dataset_from_array(
	data=raw_data[:-delay],
	targets=temperature[delay:],
	sequence_length=sequence_length,
	shuffle=True,
	batch_size=batch_size,
	start_index=num_train_samples + num_val_samples,
)

# Build the model itself. And customize it with given command-line
# arguments.

def build_model() -> keras.Model:
	# Set the parameters.
	inputs = keras.Input(shape=(sequence_length, raw_data.shape[-1]))
	if LAYER == "LSTM":
		x = keras.layers.LSTM(units=UNITS, recurrent_dropout=DROPOUT,
         	return_sequences=True)(inputs)
		x = keras.layers.LSTM(units=UNITS, recurrent_dropout=DROPOUT)(x)
	elif LAYER == "GRU":
		x = keras.layers.LSTM(units=UNITS, recurrent_dropout=DROPOUT,
         	return_sequences=True)(inputs)
		x = keras.layers.LSTM(units=UNITS, recurrent_dropout=DROPOUT)(x)
	else:
		pass
	x = keras.layers.Dropout(rate=DROPOUT)(x)
	outputs = keras.layers.Dense(units=1)(x)
	model = keras.Model(inputs, outputs)
 
	return model


model = build_model()

# Set the callback functions.

callbacks = [
	keras.callbacks.ModelCheckpoint(filepath=f"./scripts/{OUTPUT_MODEL}.keras", 
      save_best_only=True, monitor="val_loss")
]
if EARLY_STOPPING:
	callbacks.append(keras.callbacks.EarlyStopping(monitor="val_loss", patience=3))
 
# Compile the model.
model.compile(optimizer="rmsprop", loss="mse", metrics=["mae"])
model.summary()

# Fit the built and compiled model.

print("\nFitting the model with common datasets:\n")
model.fit(train_dataset, epochs=EPOCHS, validation_data=val_dataset,
          verbose=2, callbacks=callbacks, batch_size=BATCH_SIZE)

# Evaluate the fitted model.

print("\nEvaluating the fitted model with test dataset:\n")
model = keras.models.load_model(f"./scripts/{OUTPUT_MODEL}.keras")
print(f"\nTest MAE: {model.evaluate(test_dataset, verbose=2)[1]:.4f}")
