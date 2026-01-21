/**
 ******************************************************************************
 * @file 	main.h
 * @author 	Ahmet Can GULMEZ
 * @brief 	Passive acoustic surveillance system for aircraft.
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

#ifndef AERO_SONAR_H
#define AERO_SONAR_H

#ifdef __cplusplus
extern "C" {	
#endif

/* Standard GNU libaries */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <signal.h>
#include <termios.h>
#include <locale.h>
#include <limits.h>
#include <sys/types.h>
#include <stdint.h>
#include <endian.h>

/* GUI libraries */

#include <cairo/cairo.h>
#include <adwaita.h>

/* GPS map libraries */

#include <shumate/shumate.h>

/* Database libraries */

#include <sqlite3.h>

/* Custom libraries */

// #include "../lib/include/alat.h"
#include "../lib/include/dsp.h"

/* Compile-specific definitions */

#pragma GCC optimize("O3")

/* Global macro definitions */

#define TIME_FORMAT							"%F %T"
#define BUFFER_SIZE							512
#define DATA_SIZE								BUFFER_SIZE 
#define SQL_SIZE								20480
#define INTERPRETER							"/bin/python3"
#define SYSTEM_LOG_PATH						"./logs/system.log"

#define DB_SENSOR_DATA_PATH				"./db/sensor_data.db"
#define DB_SENSOR_DATA_TABLE				"SensorData"

#define MAX_COMM_CHANNEL					3
#define MAX_BUFFER_SIZE						( BUFFER_SIZE * 200 )
#define MAX_ANALYSIS							11
#define MAX_DEVICE_NODE						64
#define MAX_BALLISTIC_LAYER				10
#define MAX_LAYER_THICKNESS				200
#define MAX_MODEL_DATASET					128
#define MAX_MODEL_UNITS						1024
#define MAX_MODEL_EPOCHS					100
#define MAX_MODEL_LAYER_NUMBER			48
#define MAX_CAMERA_FILE						16	

#define BUTTON_WIDTH							100 	/* pixel */	
#define BUTTON_HEIGHT						40  	/* pixel */	
#define PAGE_BOX_MARGIN_WIDTH				20  	/* pixel */	
#define PAGE_BOX_MARGIN_HEIGHT			10  	/* pixel */	
#define BOX_INNER_MARGIN					10  	/* pixel */	
#define BOX_SPACING							25  	/* pixel */		

#define MIC_SERIAL_PATH						"/dev/"
#define MIC_WIFI_PATH						"/sys/class/net/"
#define MIC_UART_PREFIX						"ttyS"
#define MIC_USB_PREFIX						"ttyUSB"
#define MIC_WIFI_PREFIX						"wl"
#define MIC_PLOT_MARGIN						40		/* pixel */
#define MIC_PLOT_GRID						20 	/* pixel */		

#define MODEL_DATASET_PATH					"/home/can/Datasets/"
#define MODEL_DATASET_SUFFIX				".csv"
#define MODEL_FIT_SCRIPT					"scripts/acoustic_model.py"
#define MODEL_EVALUATE_SCRIPT				"scripts/evaluating_acoustic_model.py"
#define MODEL_PREDICT_SCRIPT				"scripts/predicting_acoustic_model.py"
#define MODEL_LOG_PATH						"./logs/keras-output.log"

#define NAV_PLOT_MARGIN						0		/* pixel */
#define NAV_PLOT_GRID						20		/* pixel */
#define NAV_PLOT_CAXIS						250	/* pixel */
#define NAV_PLOT_VAXIS						300	/* pixel */

#define TIMEOUT_DEVICE_READ				1000	/* ms */
#define TIMEOUT_PLOT_REDRAW				1000	/* ms */
#define TIMEOUT_MODEL_LOG					5000	/* ms */
#define TIMEOUT_DATA_RECORD				4000	/* ms */

/* Attribute and built-in macro definitions  */

#define __file									__FILE__
#define __line									__LINE__
#define __func									__func__
#define __packed								__attribute__((packed))
#define __noreturn							__attribute__((noreturn))
#define __deprecated							__attribute__((deprecated))
#define __aligned(n)							__attribute__((aligned(n)))
#define __section(s)							__attribute__((section(s)))
#define __address0							__builtin_return_address(0)
#define __unreachable						__builtin_unreachable()

/* Maro function definitions */

#define print_log(msg, ...)																\
{																									\
	char buffer[BUFFER_SIZE];																\
																									\
	memset(buffer, 0, BUFFER_SIZE);														\
	sprintf(buffer, "[PID=%d][%s] " msg "\n", getpid(), 							\
			  get_current_time(TIME_FORMAT), ##__VA_ARGS__);						\
																									\
	logging(buffer, strlen(buffer));		/* write the logs */						\
	printf("%s", buffer);		/* print the log buffer to "stdout" */			\
}

#define syscall_error()																		\
{                                      												\
	fprintf(stderr, "\n*** %s (%s::%d in %s()) ***\n", strerror(errno),		\
			  __file, __line, __func);		   	 										\
	exit(EXIT_FAILURE);	/* exit with failure status */							\
} 

#define custom_error(errmsg, ...) 														\
{                            																\
	fprintf(stderr, "\n*** " errmsg " (%s::%d in %s()) ***\n",					\
			  ##__VA_ARGS__, __file, __line, __func);									\
	exit(EXIT_FAILURE);	/* exit with failure status */							\
} 

#define database_error(db)																	\
{																									\
	fprintf(stderr, "\n*** %s (%s::%d in %s()) ***\n",								\
			  sqlite3_errmsg(db), __file, __line, __func);							\
	exit(EXIT_FAILURE);	/* exit with failure status */							\
}

/* Shorthands for GTK objects */

#define visible_page_signal(page, callback) (g_signal_connect(page, "notify::visible-child", G_CALLBACK(callback), NULL))

#define combo_row_signal(row, callback) (g_signal_connect(row, "notify::selected", G_CALLBACK(callback), NULL))
#define spin_row_signal(row, callback) (g_signal_connect(row, "notify::value", G_CALLBACK(callback), NULL))
#define switch_row_signal(row, callback) (g_signal_connect(row, "notify::active", G_CALLBACK(callback), NULL))
#define entry_row_signal(row, callback) (g_signal_connect(row, "notify::text", G_CALLBACK(callback), NULL))
#define button_signal(button, callback) (g_signal_connect(button, "clicked", G_CALLBACK(callback), NULL))

#define combo_row_signal_with_data(row, callback, data) (g_signal_connect(row, "notify::selected", G_CALLBACK(callback), data))
#define spin_row_signal_with_data(row, callback, data) (g_signal_connect(row, "notify::value", G_CALLBACK(callback), data))
#define button_signal_with_data(button, callback, data) (g_signal_connect(button, "clicked", G_CALLBACK(callback), data))
 
#define realize_signal(widget, callback) (g_signal_connect(widget, "realize", G_CALLBACK(callback), NULL))
#define render_signal(widget, callback) (g_signal_connect(widget, "render", G_CALLBACK(callback), NULL))
#define pressed_signal(widget, callback) (g_signal_connect(widget, "pressed", G_CALLBACK(callback), NULL))
#define released_signal(widget, callback) (g_signal_connect(widget, "released", G_CALLBACK(callback), NULL))
#define motion_signal(widget, callback) (g_signal_connect(widget, "motion", G_CALLBACK(callback), NULL))

#define cmp(fstring, sstring)	(strcmp(fstring, sstring) == 0)

/*****************************************************************************/
/*****************************************************************************/

/* General enumerations and structures */

typedef enum _CurrentPage 
{
	PAGE_MICROPHONE,
	PAGE_AI_MODEL,
	PAGE_NAVIGATION,
	PAGE_GPS_MAP
} CurrentPage;

typedef enum _HeaderButton 
{
	HEADER_BUTTON_NEW,
	HEADER_BUTTON_SAVE_AS,
	HEADER_BUTTON_TRASH,
	HEADER_BUTTON_INFO,
	HEADER_BUTTON_PREFS,
	HEADER_BUTTON_AVATAR
} HeaderButton;

typedef enum _Database
{
	DATABASE_SENSOR_DATA,
} Database;

/* Microphone enumerations and structures */

typedef enum _MicChannel
{
	MIC_CHANNEL_UART,
	MIC_CHANNEL_USB,
	MIC_CHANNEL_WIFI
} MicChannel;

typedef enum _MicBaudRate
{
	MIC_BAUD_RATE_9600,
	MIC_BAUD_RATE_14400,
	MIC_BAUD_RATE_19200,
	MIC_BAUD_RATE_28800,
	MIC_BAUD_RATE_38400,
	MIC_BAUD_RATE_57600,
	MIC_BAUD_RATE_115200
} MicBaudRate;

typedef enum _MicDataBits
{
	MIC_DATA_BITS_5,
	MIC_DATA_BITS_6,
	MIC_DATA_BITS_7,
	MIC_DATA_BITS_8
} MicDataBits;

typedef enum _MicParityBit
{
	MIC_PARITY_BIT_NONE,
	MIC_PARITY_BIT_EVEN,
	MIC_PARITY_BIT_ODD,
	MIC_PARITY_BIT_MARK,
	MIC_PARITY_BIT_SPACE
} MicParityBit;

typedef enum _MicStopBits
{
	MIC_STOP_BITS_1,
	MIC_STOP_BITS_2
} MicStopBits;

typedef enum _MicFlowControl
{
	MIC_FLOW_CONTROL_NONE,
	MIC_FLOW_CONTROL_HARDWARE,
	MIC_FLOW_CONTROL_SOFTWARE
} MicFlowControl;

typedef enum _MicButton
{
	MIC_BUTTON_START,
	MIC_BUTTON_ANALYZE,
	MIC_BUTTON_STOP
} MicButton;

typedef struct __packed _MicSensorData 
{
	int8_t ID;
	int8_t data[DATA_SIZE];
} MicSensorData;

/* AI model enumerations and structures */

typedef enum _ModelLayerType
{
	MODEL_LAYER_TYPE_LSTM,
	MODEL_LAYER_TYPE_GRU
} ModelLayerType;

typedef enum _ModelBatchSize
{
	MODEL_BATCH_SIZE_16,
	MODEL_BATCH_SIZE_32,
	MODEL_BATCH_SIZE_64,
	MODEL_BATCH_SIZE_128,
	MODEL_BATCH_SIZE_256,
	MODEL_BATCH_SIZE_512
} ModelBatchSize;

typedef enum _ModelEarlyStop
{
	MODEL_EARLY_STOP_TRUE,
	MODEL_EARLY_STOP_FALSE
} ModelEarlyStop;

typedef enum _ModelButton
{
	MODEL_BUTTON_FIT,
	MODEL_BUTTON_ABORT,
	MODEL_BUTTON_EVALUATE,
	MODEL_BUTTON_PREDICT
} ModelButton;

typedef struct _ModelParams
{
	const char *dataset;
	const char *outputModel;
	const char *layerType;
	const char *layerNumber;
	const char *units;
	const char *epochs;
	const char *batchSize;
	const char *earlyStop;
	const char *dropout;
} ModelParams;

/* GPS map enumerations and structures */
typedef struct _GPSData
{
	const char *UTCTime;
	const char *latitude;
	const char *longitude;
	const char *quality;
	const char *numSat;
	const char *altitude;
	const char *status;
	const char *speed;		/* knots */
	const char *course;		/* degrees */
	const char *date;
} GPSData;

/* Navigation enumerations and structures */

typedef enum _NavAccel
{
	NAV_ACCEL_X_PLUS,
	NAV_ACCEL_X_MINUS,
	NAV_ACCEL_Y_PLUS,
	NAV_ACCEL_Y_MINUS,
	NAV_ACCEL_Z_PLUS,
	NAV_ACCEL_Z_MINUS
} NavAccel;

typedef enum _NavGyro
{
	NAV_GYRO_X_PLUS,
	NAV_GYRO_X_MINUS,
	NAV_GYRO_Y_PLUS,
	NAV_GYRO_Y_MINUS,
	NAV_GYRO_Z_PLUS,
	NAV_GYRO_Z_MINUS
} NavGyro;

typedef struct _NavIMUData
{
	double accelX;
	double accelY;
	double accelZ;
	double gyroX;
	double gyroY;
	double gyroZ;
} NavIMUData;

/*****************************************************************************/
/*****************************************************************************/

/* General shared widgets and variables */

extern HeaderButton headerButton;
extern CurrentPage currentPage;

/* Microphone shared widgets and variables */

extern char *micDeviceNodes[MAX_DEVICE_NODE];
extern GtkWidget *micUARTGroup;
extern GtkWidget *micUSBGroup;
extern GtkWidget *micWiFiGroup;
extern GtkWidget *micAnalysisLabels[MAX_ANALYSIS];
extern GtkWidget *micAnalysisRows[MAX_ANALYSIS];
extern MicChannel micChannel;
extern char *micDeviceNode;
extern MicBaudRate micBaudRate;
extern MicDataBits micDataBits;
extern MicParityBit micParityBit;
extern MicStopBits micStopBits;
extern MicFlowControl micFlowControl;
extern guint micTimeout;
extern guint recordTimeout;
extern MicButton micButton;
extern MicSensorData micSensorData;

/* AI model shared widgets and variables */

extern char *modelDatasets[MAX_MODEL_DATASET];
extern guint modelFitPid;
extern char *modelDataset;
extern char *modelKerasLog;
extern guint modelTimeout;
extern GtkWidget *modelTextView;
extern GtkTextBuffer *modelTextBuffer;
extern ModelLayerType modelLayerType;
extern guint modelLayerNumber;
extern guint modelUnits;
extern guint modelEpochs;
extern gdouble modelDropout;
extern ModelBatchSize modelBatchSize;
extern ModelEarlyStop modelEarlyStop;
extern char *modelOutputName;
extern ModelButton modelButton;

/* GPS map shared widgets and variables */

extern ShumateMarkerLayer *gpsMarkerLayer;
extern GPSData gpsData;

/* Nagivation shared widgets and variables */

extern NavAccel navAccel;
extern NavGyro navGyro;
extern NavIMUData navIMUData;

/*****************************************************************************/
/*****************************************************************************/

/* Microphone function prototypes */

extern void microphone(GtkBox *, gpointer);
extern void microphone_row_device_node(GtkWidget *);						
extern void microphone_row_baud_rate(GtkWidget *);	
extern void microphone_row_data_bits(GtkWidget *);	
extern void microphone_row_parity_bit(GtkWidget *);
extern void microphone_row_stop_bits(GtkWidget *);	
extern void microphone_row_flow_control(GtkWidget *);
extern void microphone_group_UART(gpointer);
extern void microphone_group_USB(gpointer);
extern void microphone_group_WiFi(gpointer);
extern void mic_plot_car(GtkDrawingArea *, cairo_t *, int, int, gpointer);
extern void mic_plot_car_frame(cairo_t *, int, int);	
extern void mic_plot_car_grid(cairo_t *, int, int);	
extern void mic_plot_car_label_x(cairo_t *, int, int);		
extern void mic_plot_car_label_y(cairo_t *, int, int);	
extern void mic_plot_car_data(cairo_t *, int, int);
extern void mic_plot_polar(GtkDrawingArea *, cairo_t *, int, int, gpointer);
extern void mic_plot_polar_frame(cairo_t *, int, int);
extern void mic_plot_polar_label(cairo_t *, int, int);
extern void mic_plot_polar_fill(cairo_t *, int, int, double, double);

/* AI model function prototypes */

extern void model(GtkBox *, gpointer);
extern void model_group_dataset(GtkWidget *);
extern void model_group_model(GtkWidget *);

/* Navigation function prototypes */

extern void navigation(GtkBox *, gpointer);
extern GtkWidget *navigation_accel_group(gpointer);
extern GtkWidget *navigation_gyro_group(gpointer);
extern GtkWidget *navigation_magnet_group(gpointer);
extern void nav_plot_area_grid(cairo_t *, int, int);
extern void nav_plot_area_axes(cairo_t *, int, int);
extern void nav_plot_area_labels(cairo_t *, int, int);
extern void nav_plot_area_accel(cairo_t *, int, int, NavAccel);
extern void nav_plot_area_gyro(cairo_t *, int, int, NavGyro);
extern void nav_plot_area(GtkDrawingArea *, cairo_t *, int, int, gpointer);

/* GPS map function prototypes */

extern void gps_map(GtkBox *, gpointer);
extern void gps_map_area(GtkBox *, gpointer);
extern void gps_map_area_markers(ShumateMarkerLayer *, double, double);

/* Database function prototypes */

extern struct sqlite3 *database_open(const char *);
extern void database_create_table(struct sqlite3 *, Database);
extern void database_bind_data(struct sqlite3 *, Database);
extern void database_query_data(struct sqlite3 *, Database);
extern void database_close(struct sqlite3 *);

/* Common utility function prototypes */

extern char *get_host_name(void);
extern void logging(const char *, size_t);
extern char *get_current_time(const char *);
extern int get_mic_device_nodes(MicChannel);
extern int get_camera_USB_nodes(void);
extern int get_camera_host_videos(void);
extern int get_model_datasets(void);
extern void read_mic_device_node(MicChannel, const char *);
extern void set_serial_attributes(int, struct termios *);
extern int run_keras_script(const char *);
extern void abort_keras_script(int);
extern int is_keras_script_running(int);
extern char *get_keras_script_logs(const char *);

/* Timeout utility function prototypes */

extern gboolean timeout_mic_device_node(gpointer);
extern gboolean timeout_model_keras_log(gpointer);
extern gboolean timeout_database_record(gpointer);
extern gboolean timeout_mic_plot(gpointer);

/* Generic component function prototypes */

extern GtkWidget *__generic_page_box_new(void);
extern GtkWidget *__generic_header_button_new(const char *, const char *);
extern guint __generic_row_selected(GObject *, GParamSpec *, gpointer, const char *);
extern guint __generic_row_changed(GObject *, GParamSpec *, gpointer, const char *);
extern gboolean __generic_row_switched(GObject *, GParamSpec *, gpointer, const char *);
extern char *__generic_row_texted(GObject *, GParamSpec *, gpointer, const char *);
extern GtkWidget *__generic_action_row_new(const char *, const char *);
extern GtkWidget *__generic_combo_row_new(const char *, const char **, guint);
extern GtkWidget *__generic_spin_row_new(const char *, double, double, double, double, guint);
extern GtkWidget *__generic_switch_row_new(const char *);
extern GtkWidget *__generic_entry_row_new(const char *);
extern GtkWidget *__generic_group_new(const char *, const char *);
extern void __generic_group_add(GtkWidget *, GtkWidget *);
extern GtkWidget *__generic_button_new(const char *, const char *);

/*****************************************************************************/
/*****************************************************************************/

/* General signal handler prototypes */

extern void on_visible_page_changed(GObject *, GParamSpec *, gpointer);
extern void on_header_button_clicked(GtkButton *, gpointer);

/* Microphone signal handler prototypes */

extern void on_comm_channel_selected(GObject *, GParamSpec *, gpointer);
extern void on_device_node_selected(GObject *, GParamSpec *, gpointer);
extern void on_baud_rate_selected(GObject *, GParamSpec *, gpointer);
extern void on_data_bits_selected(GObject *, GParamSpec *, gpointer);
extern void on_parity_bit_selected(GObject *, GParamSpec *, gpointer);
extern void on_stop_bits_selected(GObject *, GParamSpec *, gpointer);
extern void on_flow_control_selected(GObject *, GParamSpec *, gpointer);
extern void on_mic_button_clicked(GtkButton *, gpointer);

/* AI model signal handler prototypes */

extern void on_dataset_selected(GObject *, GParamSpec *, gpointer);
extern void on_layer_type_selected(GObject *, GParamSpec *, gpointer);
extern void on_recurrent_dropout_changed(GObject *, GParamSpec *, gpointer);
extern void on_units_changed(GObject *, GParamSpec *, gpointer);
extern void on_batch_size_selected(GObject *, GParamSpec *, gpointer);
extern void on_epochs_changed(GObject *, GParamSpec *, gpointer);
extern void on_layer_number_changed(GObject *, GParamSpec *, gpointer);
extern void on_early_stop_switched(GObject *, GParamSpec *, gpointer);
extern void on_output_model_texted(GObject *, GParamSpec *, gpointer);
extern void on_model_button_clicked(GtkButton *, gpointer);

/* Navigation signal handler prototypes */

extern void on_nav_button_clicked(GtkButton *, gpointer);

/* GPS map signal handler prototypes */

extern void on_gps_button_clicked(GtkButton *, gpointer);

#ifdef __cplusplus
}
#endif

#endif /* AERO_SONAR_H */
