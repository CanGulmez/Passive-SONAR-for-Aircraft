/**
 * Passive SONAR for Aircraft
 * 
 * Maintainer: Ahmet Can GULMEZ
 * Version: 1.0.0
 * License: MIT License
 */

#include "main.h"

/**
 * System Oscillator Configuration
 * 
 * Oscillator:          HSI
 */
void SystemOscConfig(void) 
{ 
   HAL_StatusTypeDef status;

   iosc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
   iosc.HSIState = RCC_HSI_ON;
   iosc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

   status = HAL_RCC_OscConfig(&iosc);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);
}

/**
 * System Clock Configuration
 * 
 * SystemCoreClock:     16HMz
 * AHB Clock:           16MHz
 * APB1 Clock:          16MHz
 * APB2 Clock:          16MHz
 */
void SystemClockConfig(void)
{
   HAL_StatusTypeDef status;

   iclk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | 
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
   iclk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
   iclk.AHBCLKDivider = RCC_SYSCLK_DIV1;    
   iclk.APB1CLKDivider = RCC_HCLK_DIV1;     
   iclk.APB2CLKDivider = RCC_HCLK_DIV1;    

   status = HAL_RCC_ClockConfig(&iclk, FLASH_LATENCY_0);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);
}
 
/**
 * System Data 1/2 Configuration
 * 
 * Peripherals:         I2S1, GPIOA, DMA2
 * Pins:                PA4(WS), PA5(CK), P7(SD)
 */
void SystemData12Config(void)
{
   HAL_StatusTypeDef status;

   igpio.Pin = PIN_DATA12_WS | PIN_DATA12_CK;
   igpio.Mode = GPIO_MODE_AF_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   igpio.Alternate = GPIO_AF5_SPI1;
   HAL_GPIO_Init(GPIOA, &igpio);

   igpio.Pin = PIN_DATA12_SD;
   HAL_GPIO_Init(GPIOB, &igpio);

   hi2s1.Instance = SPI1;
   hi2s1.Init.Mode = I2S_MODE_MASTER_RX;
   hi2s1.Init.Standard = I2S_STANDARD_PHILIPS;
   hi2s1.Init.DataFormat = I2S_DATAFORMAT_16B;
   hi2s1.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
   hi2s1.Init.AudioFreq = I2S_AUDIOFREQ_8K;
   hi2s1.Init.CPOL = I2S_CPOL_LOW;
   hi2s1.Init.ClockSource = I2S_CLOCK_PLL;
   hi2s1.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
   status = HAL_I2S_Init(&hi2s1);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);

   hdma2_i2s1.Instance = DATA12_STREAM;
   hdma2_i2s1.Init.Channel = DATA12_CHANNEL;
   hdma2_i2s1.Init.Direction = DMA_PERIPH_TO_MEMORY;
   hdma2_i2s1.Init.MemInc = DMA_MINC_ENABLE;
   hdma2_i2s1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
   hdma2_i2s1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
   hdma2_i2s1.Init.Mode = DMA_CIRCULAR;
   hdma2_i2s1.Init.Priority = DMA_PRIORITY_HIGH;
   hdma2_i2s1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

   __HAL_LINKDMA(&hi2s1, hdmarx, hdma2_i2s1);

   status = HAL_DMA_Init(&hdma2_i2s1);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);

   HAL_NVIC_SetPriority(DATA12_IRQn, 1, 0);
   HAL_NVIC_EnableIRQ(DATA12_IRQn);
}

/**
 * System Data 3/4 Configuration
 * 
 * Peripherals:         I2S2, GPIOB, GPIOC, DMA1
 * Pins:                PB12(WS), PB13(CK), PB15(SD)
 */
void SystemData34Config(void)
{
   HAL_StatusTypeDef status;

   igpio.Pin = PIN_DATA34_WS | PIN_DATA34_CK | PIN_DATA34_SD;
   igpio.Mode = GPIO_MODE_AF_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   igpio.Alternate = GPIO_AF5_SPI2;
   HAL_GPIO_Init(GPIOB, &igpio);

   hi2s2.Instance = SPI2;
   hi2s2.Init.Mode = I2S_MODE_MASTER_RX;
   hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
   hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;
   hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
   hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_8K;
   hi2s2.Init.CPOL = I2S_CPOL_LOW;
   hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
   hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
   status = HAL_I2S_Init(&hi2s2);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);

   hdma1_i2s2.Instance = DATA34_STREAM;
   hdma1_i2s2.Init.Channel = DATA34_CHANNEL;
   hdma1_i2s2.Init.Direction = DMA_PERIPH_TO_MEMORY;
   hdma1_i2s2.Init.MemInc = DMA_MINC_ENABLE;
   hdma1_i2s2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
   hdma1_i2s2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
   hdma1_i2s2.Init.Mode = DMA_CIRCULAR;
   hdma1_i2s2.Init.Priority = DMA_PRIORITY_HIGH;
   hdma1_i2s2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

   __HAL_LINKDMA(&hi2s2, hdmarx, hdma1_i2s2);

   status = HAL_DMA_Init(&hdma1_i2s2);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);

   HAL_NVIC_SetPriority(DATA34_IRQn, 2, 0);
   HAL_NVIC_EnableIRQ(DATA34_IRQn);
}

/**
 * System Data 5/6 Configuration
 * 
 * Peripherals:         I2S3, GPIOA, GPIOB, GPIOC, DMA1
 * Pins:                PA15(WS), PC10(CK), PC12(SD)
 */
void SystemData56Config(void)
{
   HAL_StatusTypeDef status;

   igpio.Pin = PIN_DATA56_WS;
   igpio.Mode = GPIO_MODE_AF_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   igpio.Alternate = GPIO_AF6_SPI3;
   HAL_GPIO_Init(GPIOA, &igpio);

   igpio.Pin = PIN_DATA56_CK | PIN_DATA56_SD;
   HAL_GPIO_Init(GPIOC, &igpio);

   hi2s3.Instance = SPI3;
   hi2s3.Init.Mode = I2S_MODE_MASTER_RX;
   hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
   hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
   hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
   hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_8K;
   hi2s3.Init.CPOL = I2S_CPOL_LOW;
   hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
   hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
   status = HAL_I2S_Init(&hi2s3);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);

   hdma1_i2s3.Instance = DATA56_STREAM;
   hdma1_i2s3.Init.Channel = DATA56_CHANNEL;
   hdma1_i2s3.Init.Direction = DMA_PERIPH_TO_MEMORY;
   hdma1_i2s3.Init.MemInc = DMA_MINC_ENABLE;
   hdma1_i2s3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
   hdma1_i2s3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
   hdma1_i2s3.Init.Mode = DMA_CIRCULAR;
   hdma1_i2s3.Init.Priority = DMA_PRIORITY_HIGH;
   hdma1_i2s3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

   __HAL_LINKDMA(&hi2s3, hdmarx, hdma1_i2s3);

   status = HAL_DMA_Init(&hdma1_i2s3);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);

   HAL_NVIC_SetPriority(DATA56_IRQn, 3, 0);
   HAL_NVIC_EnableIRQ(DATA56_IRQn);
}
 
/**
 * System Debug Configuration
 * 
 * Peripherals:         USART3, GPIOB
 * Pins:                PB10 (TX), PB11(RX)
 * Baudrate:            115200
 */
void SystemDebugConfig(void)
{
   HAL_StatusTypeDef status;

   igpio.Pin = GPIO_PIN_10;
   igpio.Mode = GPIO_MODE_AF_PP;
   igpio.Pull = GPIO_NOPULL;
   igpio.Speed = GPIO_SPEED_FREQ_LOW;
   igpio.Alternate = GPIO_AF7_USART3;
   HAL_GPIO_Init(GPIOB, &igpio);

   huart3.Instance = USART3;
   huart3.Init.BaudRate = 115200;
   huart3.Init.WordLength = UART_WORDLENGTH_8B;
   huart3.Init.StopBits = UART_STOPBITS_1;
   huart3.Init.Parity = UART_PARITY_NONE;
   huart3.Init.Mode = UART_MODE_TX;
   huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart3.Init.OverSampling = UART_OVERSAMPLING_16;

   status = HAL_UART_Init(&huart3);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);
}

void main(void) 
{
   int i, j, index = 0;
   HAL_StatusTypeDef status;
   char buffer[BUFFER_SIZE];

   HAL_Init();

   /* Enable the required peripheral clocks */
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_DMA1_CLK_ENABLE();
   __HAL_RCC_DMA2_CLK_ENABLE();
   __HAL_RCC_SPI1_CLK_ENABLE();
   __HAL_RCC_SPI2_CLK_ENABLE();
   __HAL_RCC_SPI3_CLK_ENABLE();
   __HAL_RCC_USART3_CLK_ENABLE();

   /* Configure the system peripherals */
   SystemOscConfig();
   SystemClockConfig();

   /* Make I2Sx clock settings */
   hperip.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_I2S_APB2;
   hperip.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
   hperip.I2sApb2ClockSelection = RCC_I2SAPB2CLKSOURCE_PLLI2S;
   hperip.PLLI2S.PLLI2SM = 8;                    /* HSI=16MHz → 2MHz */
   hperip.PLLI2S.PLLI2SN = 192;                  /* 2*192=384MHz     */
   hperip.PLLI2S.PLLI2SR = 5;                    /* 384/5=76.8MHz    */
   status = HAL_RCCEx_PeriphCLKConfig(&hperip);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);

   /* Configure the system peripherals */
   SystemData12Config();
   SystemData34Config();
   SystemData56Config();
   SystemDebugConfig();
 
   /* Start the microhone readings in DMA circular mode */
   status = HAL_I2S_Receive_DMA(&hi2s1, data[0], DATA_SIZE);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status); 

   status = HAL_I2S_Receive_DMA(&hi2s2, data[1], DATA_SIZE);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);

   status = HAL_I2S_Receive_DMA(&hi2s3, data[2], DATA_SIZE);
   if (status != HAL_OK)
      SYS_ERROR(&huart3, status);

   while (TRUE) {

      /* Processes the captured data after all DMAs completed */
      if (flags[0] && flags[1] && flags[2]) {

         /* Split the each channel into left and right buffers */
         for (i = 0; i < CHANNEL_COUNT; i++) 
            for (j = 0; j < MIC_SIZE; j++)
               extracted[2 * i][j] = data[i][2 * j],
               extracted[2 * i + 1][j] = data[i][2 * j + 1];
         
         /* Compress the splitted data to reduce sample payloads */
         for (i = 0; i < MIC_COUNT; i++)
            for (j = 0; j < COM_SIZE; j++)
               compressed[index] = extracted[i][(int) (j*MIC_SIZE/COM_SIZE)],
               index ++;
         index = 0;

         /* Transmit the compressed raw data over serial line */
         HAL_UART_Transmit(&huart3, compressed, MIC_COUNT*COM_SIZE*2, HAL_MAX_DELAY);
        
         /* Reset the 'flags' variable for next compilation */
         memset(flags, FALSE, sizeof(flags));
      }
   }
} 

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
   /* Set the flags to transmit the over serial line. */
   if (hi2s->Instance == SPI1)   flags[0] = TRUE;
   if (hi2s->Instance == SPI2)   flags[1] = TRUE;
   if (hi2s->Instance == SPI3)   flags[2] = TRUE;
}

void SysTick_Handler(void) 
{
   HAL_IncTick();
}

void DMA2_Stream0_IRQHandler(void) 
{
   HAL_DMA_IRQHandler(&hdma2_i2s1); 
}

void DMA1_Stream3_IRQHandler(void) 
{
   HAL_DMA_IRQHandler(&hdma1_i2s2); 
}

void DMA1_Stream0_IRQHandler(void) 
{
   HAL_DMA_IRQHandler(&hdma1_i2s3); 
}
