/**
 ******************************************************************************
 * @file 	utility.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Common utilities of AeroSONAR.
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

#include "main.h"

/**
 * Get the host name to show the who runs this program.
 */
char *get_host_name(void)
{
	static char buffer[32];

	if (gethostname(buffer, 32) == -1)
		syscall_error();

	return buffer;
}

/**
 * Write the system logs into required file.
 */
void write_system_log(const char* buffer, size_t size)
{
	int fd;

	fd = open(SYSTEM_LOG_PATH, O_WRONLY | O_APPEND);
	if (fd == -1)
		syscall_error();

	if (write(fd, buffer, size) == -1)	/* write the logs. */
		syscall_error();
	
	/* Make sure that each log is explicitly written. */
	if (fsync(fd) == -1)
		syscall_error();

	if (close(fd) == -1)
		syscall_error();
}

/**
 * Get the current time to show with signal handlers.
 */
char *get_current_time(const char* format)
{
	static char buffer[64];
	time_t t;
	size_t s;
	struct tm *tm;
	
	t = time(NULL);			/* get the time in seconds */
	tm = localtime(&t);		/* convert it into broken-down */
	if (tm == NULL)
		syscall_error();

	s = strftime(buffer, 64, (format != NULL) ? format : "%c", tm);

	return buffer;
}

/**
 * Set the 'termios' structure according to selected attributes.
 */
void set_serial_attributes(int fd, struct termios *tty)
{
	if (tcgetattr(fd, tty) == -1)
		syscall_error();

	cfsetispeed(tty, B115200);			/* Baud Rate - 115200 (most common) */
	cfsetospeed(tty, B115200);
	tty->c_cflag &= ~CSIZE;				/* Data Bits - 8 bits (most common) */
	tty->c_cflag |= CS8;
	tty->c_cflag &= ~PARENB;			/* Parity Bit - None (most common) */
	tty->c_cflag &= ~CSTOPB;			/* Stop bits - 1 bit (most common) */
   tty->c_cflag &= ~CRTSCTS;			/* Flow Control - None (most common) */
   tty->c_iflag &= ~(IXON | IXOFF | IXANY);
   tty->c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG); 	/* Raw mode */
   tty->c_oflag &= ~OPOST;
   tty->c_cc[VTIME] = 0;  				/* No inter-character timer */
   tty->c_cc[VMIN] = 0;   				/* Non-blocking read */
   tty->c_cflag |= CREAD | CLOCAL;	/* Enable receiver */
    
   if (tcsetattr(fd, TCSANOW, tty) == -1)
		syscall_error();
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/**
 * Get the available microphone device nodes.
 */
int get_mic_device_nodes(MicChannel channel)
{
	int i, index = 0;
	struct dirent *entry;
	const char *prefix;
	DIR *dir;

	/* Initialize with NULLs. */
	memset(micDeviceNodes, 0, MAX_DEVICE_NODE);

	if (channel == MIC_CHANNEL_WIFI)
	{
		dir = opendir(MIC_WIFI_PATH);		/* open the Wi-Fi device folder */
	}
	else
	{
		dir = opendir(MIC_SERIAL_PATH);	/* open the serial device folder */
	}
	if ( dir == NULL )
		syscall_error();

	if (channel == MIC_CHANNEL_UART)
	{
		prefix = MIC_UART_PREFIX;			/* get the UART prefix */
	}
	else if (channel == MIC_CHANNEL_USB)
	{
		prefix = MIC_USB_PREFIX;			/* get the USB prefix */
	}
	else
	{
		prefix = MIC_WIFI_PREFIX;			/* get the Wi-Fi prefix */
	}

	for (;;) 
	{
		entry = readdir(dir);				/* read each entity */
		if (entry == NULL)
		{
			break;
		}
		/* Continueing the look for ".", ".." */
		if (cmp(entry->d_name, ".") || cmp(entry->d_name, ".."))
		{
			continue;
		}
		if (strstr(entry->d_name, prefix) != NULL) 
		{
			micDeviceNodes[index] = strdup(entry->d_name);
			index++;
		}
	}
	if (closedir(dir) == -1)
		syscall_error();
	
	return index;
}

/**
 * Read the selected channel's device node and return the bytes.
 */
void read_mic_device_node(MicChannel channel, const char* node)
{
	int fd, pathSize = 0;
	char devicePath[32];
	ssize_t numRead;
	struct termios tty;
	
	/* Initialize with NULLs. */
	memset(&micSensorData, 0, sizeof(micSensorData));

	/* Create the device node path name correctly. */
	if (channel == MIC_CHANNEL_UART || channel == MIC_CHANNEL_USB) 
	{
		strcpy(devicePath, MIC_SERIAL_PATH);
		pathSize = strlen(MIC_SERIAL_PATH);
	} 
	else if (channel == MIC_CHANNEL_WIFI) 
	{
		strcpy(devicePath, MIC_WIFI_PATH);
		pathSize = strlen(MIC_WIFI_PATH);
	}
	strcat(devicePath, node);
	pathSize += strlen(node);
	devicePath[pathSize] = '\0';
	
	fd = open(devicePath, 
		O_RDONLY 	| 						/* read-only channel */
		O_NOCTTY 	| 						/* no controlling terminal */
		O_NONBLOCK 	| 						/* non-blocking I/O */
		O_SYNC								/* wait for completion */
	);	
	if (fd == -1)
		syscall_error();
	
	/* Set serial terminal attributes. */
	set_serial_attributes(fd, &tty);
	
	/* Read the device node. */
	numRead = read(fd, &micSensorData, sizeof(micSensorData));
	if (numRead > 0) 
	{
		print_log("read %ld bytes from '%s'", numRead, devicePath);
		if (close(fd) == -1)
			syscall_error(); 
	} 
	else if (numRead == -1) 
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			syscall_error(); 
			/* EAGAIN is normal (no data at this cycle) */
	}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/**
 * Read the appropriate model dataset.
 */
int get_model_datasets(void)
{
	int i, index = 0, num;
	struct dirent *entry;
	DIR *dir;
	char *filename;

	/* Initialize with NULLs */
	memset(modelDatasets, 0, MAX_MODEL_DATASET);

	dir = opendir(MODEL_DATASET_PATH);	/* open the directory */
	if (dir == NULL)
		syscall_error();

	for (;;) 
	{
		entry = readdir(dir);				/* read its contents */
		if (entry == NULL)
		{
			break;
		}
		if (cmp(entry->d_name, ".") || cmp(entry->d_name, ".."))
		{
			continue;
		}
		/* Check the suffix of the datasets. */
		filename = entry->d_name;
		for (i = 0; i < strlen(entry->d_name)-4; i++)
		{
			*filename++;
		}
		if (strstr(filename, MODEL_DATASET_SUFFIX)) 
		{
			modelDatasets[index] = strdup(entry->d_name);
			index++;
		}
	}
	if (closedir(dir) == -1)
		syscall_error();

	return index;
}

/**
 * Set the acoustic sensor data model parameters.
 */
void set_keras_script_params(ModelParams *modelParams)
{
	char dataset[BUFFER_SIZE];
	char units[4], epochs[4], dropout[8];

	/* Build the dataset path. */
	strcpy(dataset, MODEL_DATASET_PATH);
	strcat(dataset, modelDataset);
	dataset[strlen(dataset)] = '\0';
	
	modelParams->dataset = dataset;					/* dataset */
	modelParams->outputModel = modelOutputName;	/* output model name */
	snprintf(units, 4, "%d", modelUnits);
	modelParams->units = units;						/* units per layer */
	snprintf(epochs, 4, "%d", modelEpochs);
	modelParams->epochs = epochs;						/* epochs */
	snprintf(dropout, 8, "%.2f", modelDropout);
	modelParams->dropout = dropout;					/* recurrent dropout */
	switch (modelLayerType)								/* layer type */
	{
		case MODEL_LAYER_TYPE_LSTM: modelParams->layer = "LSTM"; break;
		case MODEL_LAYER_TYPE_GRU:  modelParams->layer = "GRU";  break;
	}
	switch (modelBatchSize)								/* batch size */
	{
		case MODEL_BATCH_SIZE_16:	modelParams->batchSize = "16";  break;
		case MODEL_BATCH_SIZE_32:	modelParams->batchSize = "32";  break;
		case MODEL_BATCH_SIZE_64:	modelParams->batchSize = "64";  break;
		case MODEL_BATCH_SIZE_128:	modelParams->batchSize = "128"; break;
		case MODEL_BATCH_SIZE_256:	modelParams->batchSize = "256"; break;
		case MODEL_BATCH_SIZE_512:	modelParams->batchSize = "512"; break;
	}
	switch (modelEarlyStopping) 						/* early stopping */
	{
		case MODEL_EARLY_STOPPING_FALSE: modelParams->earlyStopping = "0"; break;
		case MODEL_EARLY_STOPPING_TRUE:  modelParams->earlyStopping = "1"; break;
	}	
}

/**
 * Run the Keras script and return the child process ID.
 */
int run_keras_script(const char *script)
{
	int i, childPid, logFd;
	char scriptFile[BUFFER_SIZE], logFile[BUFFER_SIZE];
	ModelParams modelParams;

	/* Build the script file. */
	strncpy(scriptFile, script, strlen(script));
	scriptFile[strlen(script)] = '\0';

	/* Build the log file. */
	strncpy(logFile, MODEL_LOG_PATH, strlen(MODEL_LOG_PATH));
	logFile[strlen(MODEL_LOG_PATH)] = '\0';

	/* Set the model parameters. */
	set_keras_script_params(&modelParams);
	
	switch (childPid = fork()) 
	{
		case -1: 	/* child-process couldn't be forked */
			syscall_error();
		
		case 0: 
			logFd = open(logFile, O_RDWR | O_TRUNC);
			if (logFd == -1)
				syscall_error();

			if (dup2(logFd, STDOUT_FILENO) == -1)	/* duplicate the stdout */
				syscall_error();
			if (dup2(logFd, STDERR_FILENO) == -1)	/* duplicate the stdin */
				syscall_error();

			if (close(logFd) == -1)
				syscall_error();
			
			execl(
				INTERPRETER, 						/* python 3 interpreter */
				INTERPRETER, scriptFile, modelParams.dataset,
				modelParams.outputModel, modelParams.layer,
				modelParams.units, modelParams.epochs, 
				modelParams.batchSize, modelParams.earlyStopping, 
				modelParams.dropout, NULL		/* command-line options */
			);	
			_exit(EXIT_SUCCESS);					/* NEVER should come here */
		
		default:		/* let the parent-process runs the main program */
			break;
	}
	return childPid;
}
 
/**
 * Abort the sub-process that runs Keras model.
 */
void abort_keras_script(int childPid)
{
	if (kill(childPid, SIGKILL) == -1) 
		syscall_error();
}

/**
 * Check the whether Keras model fitting finished.
 */
int is_keras_script_running(int childPid)
{
	int status;
	pid_t res;

	res = waitpid(childPid, &status, WNOHANG);
	if (res == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * Get the Keras script outputs from the its log file.
 */
char *get_keras_script_logs(const char *logFile)
{
	int fd;
	struct stat st;
	static char buffer[MAX_BUFFER_SIZE];

	fd = open(logFile, O_RDONLY);	/* open for read-only */
	if (fd == -1)
		syscall_error();

	/* Bother the "stat" structure to get size of logs. */
	if (fstat(fd, &st) == -1)
		syscall_error();

	if (read(fd, buffer, st.st_size) == -1)
		syscall_error();
	/* Make sure that the log results terminate. */
	buffer[st.st_size] = '\0';

	if (close(fd) == -1)
		syscall_error();

	return buffer;
}
