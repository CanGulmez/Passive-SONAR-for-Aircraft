/**
 * Passive SONAR Firmware for Aircrafts
 * 
 * Maintainer: Ahmet Can GULMEZ
 * Version: 1.0.0
 * License: MIT License
 */

/* Standard GLIBC API */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

/* FreeRTOS API */

#include <FreeRTOS.h>
#include <task.h>
#include <list.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>

/* Hardware Abstraction Layer (HAL) API */

#include <stm32f4xx_hal.h>

/* Global definations */

#define FALSE              0
#define TRUE               (!FALSE)

#define CHANNEL_COUNT      3
#define MIC_COUNT          (CHANNEL_COUNT * 2)

#define SAMPLE_SIZE        1024
#define DATA_SIZE          (SAMPLE_SIZE * 8)
#define MIC_SIZE           (DATA_SIZE / 2)
#define COM_SIZE           (DATA_SIZE / 256)
#define BUFFER_SIZE        64

#define PIN_DATA12_WS      GPIO_PIN_4     /* PA4, I2S1_WS */
#define PIN_DATA12_CK      GPIO_PIN_5     /* PA5, I2S1_CK */
#define PIN_DATA12_SD      GPIO_PIN_5     /* PB5, I2S1_SD */
#define PIN_DATA34_WS      GPIO_PIN_12    /* PB12, I2S2_WS */
#define PIN_DATA34_CK      GPIO_PIN_13    /* PB13, I2S2_CK */
#define PIN_DATA34_SD      GPIO_PIN_15    /* PB15, I2S2_SD */
#define PIN_DATA56_WS      GPIO_PIN_15    /* PA15, I2S3_WS */
#define PIN_DATA56_CK      GPIO_PIN_10    /* PC10, I2S3_CK */
#define PIN_DATA56_SD      GPIO_PIN_12    /* PC12, I2S3_SD */
#define PIN_DEBUG_TX       GPIO_PIN_10    /* PB10, USART3_TX */
#define PIN_DEBUG_RX       GPIO_PIN_11    /* PB11, USART3_RX */

#define DATA12_STREAM      DMA2_Stream0
#define DATA12_CHANNEL     DMA_CHANNEL_3
#define DATA12_IRQn        DMA2_Stream0_IRQn
#define DATA34_STREAM      DMA1_Stream3
#define DATA34_CHANNEL     DMA_CHANNEL_0
#define DATA34_IRQn        DMA1_Stream3_IRQn
#define DATA56_STREAM      DMA1_Stream0
#define DATA56_CHANNEL     DMA_CHANNEL_0
#define DATA56_IRQn        DMA1_Stream0_IRQn


#define SYS_PRINT(handle, buffer) do {                                     \
   HAL_UART_Transmit(handle, buffer, strlen(buffer), HAL_MAX_DELAY);       \
} while (0)     

#define SYS_ERROR(handle, status) do {                                     \
   char buffer[BUFFER_SIZE];                                               \
   snprintf(buffer, BUFFER_SIZE, "\r*** HAL Status: %s (%s::%d) ****\n\r", \
      (status == HAL_OK) ? "OK" :                                          \
      (status == HAL_ERROR) ? "ERROR" :                                    \
      (status == HAL_BUSY) ? "BUSY" :                                      \
      (status == HAL_TIMEOUT) ? "TIMEOUT" : "UNDEFINED",                   \
      __FILE__, __LINE__);                                                 \
   HAL_UART_Transmit(handle, buffer, strlen(buffer), HAL_MAX_DELAY);       \
} while (0)

/* Peripheral configuration structures */

RCC_OscInitTypeDef iosc = {0};
RCC_ClkInitTypeDef iclk = {0};
GPIO_InitTypeDef igpio = {0};
I2S_HandleTypeDef hi2s1 = {0};
I2S_HandleTypeDef hi2s2 = {0};
I2S_HandleTypeDef hi2s3 = {0};
UART_HandleTypeDef huart3 = {0};
RCC_PeriphCLKInitTypeDef hperip = {0};
DMA_HandleTypeDef hdma2_i2s1 = {0};
DMA_HandleTypeDef hdma1_i2s2 = {0};
DMA_HandleTypeDef hdma1_i2s3 = {0};

/* Initial data buffers to hold captured data */

volatile int16_t data[CHANNEL_COUNT][DATA_SIZE] = {FALSE};
volatile int16_t extracted[MIC_COUNT][MIC_SIZE] = {FALSE};
volatile int16_t compressed[MIC_COUNT * COM_SIZE] = {FALSE};
volatile char flags[CHANNEL_COUNT] = {FALSE};

/* System peripheral configurations */

void SystemOscConfig(void);
void SystemClockConfig(void);
void SystemData12Config(void);
void SystemData34Config(void);
void SystemData56Config(void);
void SystemDebugConfig(void);
 