/**
 * Passive SONAR for Aircrafts Firmware Development
 * 
 * Maintainer:    Ahmet Can GULMEZ
 * License:       MIT License
 * Version:       1.0.0  
 */

/* Standard GNU API */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Hardware Abstraction Layer (HAL) API */

#include <stm32h7xx_hal.h>

/* FreeRTOS API */

#include <FreeRTOS.h>
#include <task.h>
#include <list.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>

/* Global Definations */

#define CHANNEL_COUNT               4
#define MIC_COUNT                   (CHANNEL_COUNT * 2)
#define SAMPLE_SIZE                 4096
#define MIC_SIZE                    (SAMPLE_SIZE / 2)
#define LORA_SIZE                   100 /* bits */
#define GPS_SIZE                    6   

#define PIN_GPS_RX                  GPIO_PIN_1  /* PA1 */
#define PIN_GPS_TX                  GPIO_PIN_2  /* PA2 */
#define PIN_SERVO1                  GPIO_PIN_11 /* PE11 */
#define PIN_SERVO2                  GPIO_PIN_13 /* PE13 */
#define PIN_SERVO3                  GPIO_PIN_14 /* PE14 */
#define PIN_DATAIN0                 GPIO_PIN_1  /* PC1 */
#define PIN_DATAIN1                 GPIO_PIN_12 /* PB12 */
#define PIN_DATAIN2                 GPIO_PIN_5  /* PC5 */
#define PIN_DATAIN3                 GPIO_PIN_4  /* PE4 */
#define PIN_CKOUT0                  GPIO_PIN_9  /* PE9 */
#define PIN_CKOUT1                  GPIO_PIN_10 /* PD10 */
#define PIN_CKOUT2                  GPIO_PIN_0  /* PB0 */
#define PIN_CKOUT3                  GPIO_PIN_3  /* PD3 */
#define PIN_LED1                    GPIO_PIN_7  /* PC7 */
#define PIN_LED2                    GPIO_PIN_6  /* PC6 */
#define PIN_LORA_RX                 GPIO_PIN_7  /* PE7 */
#define PIN_LORA_TX                 GPIO_PIN_8  /* PE8 */
#define PIN_LORA_M0                 GPIO_PIN_14 /* PB14 */
#define PIN_LORA_M1                 GPIO_PIN_13 /* PB13 */
#define PIN_DEBUG_TX                GPIO_PIN_12 /* PC12 */
#define PIN_DEBUG_RX                GPIO_PIN_2  /* PD2 */

#define SONAR_MIC1                  0x01
#define SONAR_MIC2                  0x02
#define SONAR_MIC3                  0x03 
#define SONAR_MIC4                  0x04
#define SONAR_MIC5                  0x05
#define SONAR_MIC6                  0x06
#define SONAR_MIC7                  0x07
#define SONAR_MIC8                  0x08

/* Global Variables */

int32_t buffer[CHANNEL_COUNT][SAMPLE_SIZE] = {0};
int32_t all[MIC_COUNT][MIC_SIZE] = {0};
int16_t compressed[MIC_COUNT][LORA_SIZE] = {0};
int32_t coordinate[GPS_SIZE] = {0};
uint8_t flags[CHANNEL_COUNT] = {0};

/* Peripheral Configuration Structures */

RCC_OscInitTypeDef iosc = {0};
RCC_ClkInitTypeDef iclk = {0};
GPIO_InitTypeDef igpio = {0};
UART_HandleTypeDef huart4 = {0};
UART_HandleTypeDef huart7 = {0};
UART_HandleTypeDef huart5 = {0};
TIM_HandleTypeDef htim1 = {0};
TIM_OC_InitTypeDef sConfigOC = {0};
DFSDM_Channel_HandleTypeDef hdfsdm1_channel[CHANNEL_COUNT] = {0};
DFSDM_Filter_HandleTypeDef hdfsdm1_filter[CHANNEL_COUNT] = {0};

/* User-defined Enumerations and Structures */

typedef enum {
   FALSE = 0, 
   TRUE
} bool_t;

typedef struct {

} gps_data_t;

typedef struct {
   
} lora_data_t;

typedef struct {

} data_t;

/**
 * System Oscillator and Clock Configuration
 * 
 * Oscillator:               HSI (64 mhZ)
 * System Clock Source:      HSI
 * System Clock:             64 MHz
 * AHB Clock:                64 MHz
 * APB1 Clock:               64 MHz
 * APB2 Clock:               64 MHz
 * APB3 Clock:               64 MHz
 * APB4 Clock:               64 MHz
 * Flash Latency:            4
 * Voltage Scaling:          Scale 1
 */
void SystemClockConfig(void)
{
   HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

   iosc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
   iosc.HSIState = RCC_HSI_ON;
   iosc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
   iosc.PLL.PLLState = RCC_PLL_NONE;
   HAL_RCC_OscConfig(&iosc);

   iclk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | 
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | 
                    RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_D3PCLK1;
   iclk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
   iclk.SYSCLKDivider = RCC_SYSCLK_DIV1;
   iclk.AHBCLKDivider = RCC_HCLK_DIV1;
   iclk.APB1CLKDivider = RCC_APB1_DIV1;
   iclk.APB2CLKDivider = RCC_APB2_DIV1;
   iclk.APB3CLKDivider = RCC_APB3_DIV1;
   iclk.APB4CLKDivider = RCC_APB4_DIV1;
   HAL_RCC_ClockConfig(&iclk, FLASH_LATENCY_4);
}

/**
 * GPS (Global Positioning System) Configuration
 * 
 * Peripheral:    UART4
 * GPIO Port:     GPIOA
 * GPIO Pin:      PA1 (UART4_RX)
 */
void SystemGPSConfig(void)
{
   igpio.Pin = PIN_GPS_RX;
   igpio.Mode = GPIO_MODE_AF_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_HIGH;
   igpio.Alternate = GPIO_AF8_UART4;
   HAL_GPIO_Init(GPIOA, &igpio);

   huart4.Instance = UART4;
   huart4.Init.BaudRate = 115200;
   huart4.Init.WordLength = UART_WORDLENGTH_8B;
   huart4.Init.StopBits = UART_STOPBITS_1;
   huart4.Init.Parity = UART_PARITY_NONE;
   huart4.Init.Mode = UART_MODE_RX;
   huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart4.Init.OverSampling = UART_OVERSAMPLING_16;
   huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
   huart4.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
   HAL_UART_Init(&huart4);
}

/**
 * Servo Motor Configurations
 * 
 * Peripherals:   TIM1
 * GPIO Ports:    GPIOE
 * GPIO Pins:     PE11 (TIM1_CH2), PE13 (TIM1_CH3), 
 *                PE14 (TIM1_CH4)
 */
void SystemServoConfig(void)
{
   igpio.Pin = PIN_SERVO1 | PIN_SERVO2 | PIN_SERVO3;
   igpio.Mode = GPIO_MODE_AF_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   igpio.Alternate = GPIO_AF1_TIM1;
   HAL_GPIO_Init(GPIOE, &igpio);

   htim1.Instance = TIM1;
   htim1.Init.Prescaler = 63;  /* 1MHz tick */
   htim1.Init.Period = 20000;  /* 50Hz */
   htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
   htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim1.Init.RepetitionCounter = 0;
   htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
   HAL_TIM_PWM_Init(&htim1);

   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   // sConfigOC.Pulse = 1000;     /* 1.5ms = 0 deg  */
   sConfigOC.Pulse = 1500;     /* 1.5ms = 90 deg */
   // sConfigOC.Pulse = 2000;     /* 2.0ms = 180 deg */
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   
   HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);
   HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3);
   HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4);
}

/**
 * INMP441 Microphone Configurations
 * 
 * Peripheral:    DFSDM1, 
 * GPIO Ports:    GPIOB, GPIOC, GPIOD, GPIOD
 * GPIO Pins:     PB0 (DFSDM1_CKOUT2), PB12 (DFSDM1_DATIN1),
 *                PC1 (DFSDM1_DATIN0), PC5 (DFSDM1_DATIN2),
 *                PE4 (DFSDM1_DATIN3), PE9 (DFSDM1_CKOUT0),
 *                PD3 (DFSDM1_CKOUT3), PD10 (DFSDM1_CKOUT1)
 */
void SystemDataConfig(void)
{
   int i;

   igpio.Pin = PIN_CKOUT2 | PIN_DATAIN1;
   igpio.Mode = GPIO_MODE_AF_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   igpio.Alternate = GPIO_AF6_DFSDM1;
   HAL_GPIO_Init(GPIOB, &igpio);

   igpio.Pin = PIN_DATAIN0 | PIN_DATAIN2;
   HAL_GPIO_Init(GPIOC, &igpio);

   igpio.Pin = PIN_DATAIN3 | PIN_CKOUT0;
   HAL_GPIO_Init(GPIOE, &igpio);

   igpio.Pin = PIN_CKOUT1 | PIN_CKOUT3;
   HAL_GPIO_Init(GPIOD, &igpio);

   for (i = 0; i < CHANNEL_COUNT; i++) {
      hdfsdm1_channel[i].Instance = DFSDM1_Channel0 + i;
      hdfsdm1_channel[i].Init.OutputClock.Activation = (i == 0) ? ENABLE : DISABLE;
      hdfsdm1_channel[i].Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
      hdfsdm1_channel[i].Init.OutputClock.Divider = 25;  /* 2.56 MHz */
      hdfsdm1_channel[i].Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
      hdfsdm1_channel[i].Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
      hdfsdm1_channel[i].Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
      hdfsdm1_channel[i].Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_FALLING;
      hdfsdm1_channel[i].Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
      hdfsdm1_channel[i].Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
      hdfsdm1_channel[i].Init.Awd.Oversampling = 10; 
      hdfsdm1_channel[i].Init.Offset = 0;
      hdfsdm1_channel[i].Init.RightBitShift = 0;
      HAL_DFSDM_ChannelInit(&hdfsdm1_channel[i]);
   }
   for (i = 0; i < CHANNEL_COUNT; i++) {
      hdfsdm1_filter[i].Instance = DFSDM1_Filter0 + i;
      hdfsdm1_filter[i].Init.RegularParam.Trigger = DFSDM_FILTER_SW_TRIGGER;
      hdfsdm1_filter[i].Init.RegularParam.FastMode = ENABLE;
      hdfsdm1_filter[i].Init.RegularParam.DmaMode = ENABLE;

      hdfsdm1_filter[i].Init.InjectedParam.Trigger = DFSDM_FILTER_SW_TRIGGER;
      hdfsdm1_filter[i].Init.InjectedParam.ScanMode = DISABLE;
      hdfsdm1_filter[i].Init.InjectedParam.DmaMode = DISABLE;
      hdfsdm1_filter[i].Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_RISING_EDGE;
      hdfsdm1_filter[i].Init.InjectedParam.ExtTrigger = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO;

      hdfsdm1_filter[i].Init.FilterParam.SincOrder = DFSDM_FILTER_SINC3_ORDER;
      hdfsdm1_filter[i].Init.FilterParam.Oversampling = 160; /* 2.56 MHz / 160 = 16 kHz */
      HAL_DFSDM_FilterInit(&hdfsdm1_filter[i]);
      HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter[i], DFSDM_CHANNEL_0 + i, DFSDM_CONTINUOUS_CONV_ON);
   }
}

/**
 * LED Configurations
 * 
 * Peripherals:   GPIOC
 * GPIO Pins:     PC7, PC6
 */
void SystemLEDConfig(void)
{
   igpio.Pin = PIN_LED1 | PIN_LED2;
   igpio.Mode = GPIO_MODE_OUTPUT_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   HAL_GPIO_Init(GPIOC, &igpio);
}

/**
 * LoRa (Long Range) Configuration
 * 
 * Peripherals:   UART7
 * GPIO Ports:    GPIOE, GPIOB
 * GPIO Pins:     PE7 (UART7_RX), PB14,
 *                PE8 (UART7_TX), PB13
 */
void SystemLoRaConfig(void)
{
   igpio.Pin = PIN_LORA_M0 | PIN_LORA_M1;
   igpio.Mode = GPIO_MODE_OUTPUT_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   HAL_GPIO_Init(GPIOB, &igpio);

   igpio.Pin = PIN_LORA_RX | PIN_LORA_TX;
   igpio.Mode = GPIO_MODE_AF_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_HIGH;
   igpio.Alternate = GPIO_AF7_UART7;
   HAL_GPIO_Init(GPIOE, &igpio);

   huart7.Instance = UART7;
   huart7.Init.BaudRate = 115200;
   huart7.Init.WordLength = UART_WORDLENGTH_8B;
   huart7.Init.StopBits = UART_STOPBITS_1;
   huart7.Init.Parity = UART_PARITY_NONE;
   huart7.Init.Mode = UART_MODE_TX_RX;
   huart7.Init.ClockPrescaler = UART_PRESCALER_DIV1;
   huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart7.Init.OverSampling = UART_OVERSAMPLING_16;
   HAL_UART_Init(&huart7);
}

/**
 * Debug Configuration
 * 
 * Peripherals:   UART5
 * GPIO Ports:    GPIOC, GPIOD
 * GPIO Pins:     PC12 (UART4_TX), PD2 (UART4_RX)
 */
void SystemDebugConfig(void)
{
   igpio.Pin = PIN_DEBUG_TX;
   igpio.Mode = GPIO_MODE_AF_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   igpio.Alternate = GPIO_AF8_UART4;
   HAL_GPIO_Init(GPIOC, &igpio);

   igpio.Pin = PIN_DEBUG_RX;
   HAL_GPIO_Init(GPIOD, &igpio);

   huart5.Instance = UART5;
   huart5.Init.BaudRate = 115200;
   huart5.Init.WordLength = UART_WORDLENGTH_8B;
   huart5.Init.StopBits = UART_STOPBITS_1;
   huart5.Init.Parity = UART_PARITY_NONE;
   huart5.Init.Mode = UART_MODE_TX_RX;
   huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart5.Init.ClockPrescaler = UART_PRESCALER_DIV1;
   huart5.Init.OverSampling = UART_OVERSAMPLING_16;
   HAL_UART_Init(&huart5);
}
 
void main(int argc, char *argv[])
{
   int i, j, index = 0, mic = 0;
   gps_data_t gps_data;
   lora_data_t lora_data;
   data_t data;

   HAL_Init();

   /* Enable the peripheral clocks */
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOD_CLK_ENABLE();
   __HAL_RCC_GPIOE_CLK_ENABLE();
   __HAL_RCC_UART4_CLK_ENABLE();
   __HAL_RCC_UART5_CLK_ENABLE();
   __HAL_RCC_UART7_CLK_ENABLE();
   __HAL_RCC_TIM1_CLK_ENABLE();
   __HAL_RCC_DFSDM1_CLK_ENABLE();
   __HAL_RCC_SYSCFG_CLK_ENABLE();

   /* Initialize the system configurations */
   SystemClockConfig();
   SystemGPSConfig();
   SystemServoConfig();
   SystemDataConfig();
   SystemLEDConfig();
   SystemLoRaConfig();
   SystemDebugConfig();

   /* Start the system peripherals */
   HAL_GPIO_WritePin(GPIOC, PIN_LED1, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOB, PIN_LORA_M0, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOB, PIN_LORA_M1, GPIO_PIN_RESET);
   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

   /* Start the microphone readings */
   for (i = 0; i < CHANNEL_COUNT; i++) 
      HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter[i], buffer[i], SAMPLE_SIZE);

   while (TRUE) {
      HAL_GPIO_WritePin(GPIOC, PIN_LED2, GPIO_PIN_RESET);

      if (flags[0] && flags[1] && flags[2] && flags[3]) {
         
         /* Generate the compressed data to transmit over LoRa  */
         for (i = 0; i < CHANNEL_COUNT; i++) 
            for (j = 0; j < MIC_SIZE; j++) 
               all[2 * i][j] = buffer[i][2 * j],
               all[2 * i + 1][j] = buffer[i][2 * j + 1];

         for (i = 0; i < MIC_COUNT; i++) 
            for (j = 0; j < LORA_SIZE; j++) 
               compressed[i][j] = all[i][j * ((int) (MIC_SIZE / LORA_SIZE))];
         
         /* Get the GPS information */

         /* Pack the all data into proper structures */

         /* Transmit the packed data to ground station */

         /* Blink the LED to indicate LoRa packet transmission */
         HAL_GPIO_WritePin(GPIOC, PIN_LED2, GPIO_PIN_SET);

         /* Reset the flags for next conversion */
         memset(flags, 0, sizeof(flags));
      }
   }
}

void SysTick_Handler(void) 
{
   HAL_IncTick();
}
 
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm)
{
   int i;

   for (i = 0; i < CHANNEL_COUNT; i++) {
      if (hdfsdm->Instance == DFSDM1_Filter0 + i)
         flags[i] = TRUE;
   }
}
