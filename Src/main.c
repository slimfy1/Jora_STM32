/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "dwt_delay.h"
#include "stdbool.h"
#include "max7219.h"
#include "dfplayer.h"
#include "stepper.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

osThreadId_t ScanHandle;
osThreadId_t StandbyHandle;
osThreadId_t EyeHandle;
osThreadId_t LedHandle;
osThreadId_t MPEHandle;
/* USER CODE BEGIN PV */
//Input buffer from Atmega328p. Raw data
uint8_t in_buffer[9]={0}; // Raw data from 3 sensors
//Sonar data chars
char s1_capture[3]; //Sonar 1 char array
char s2_capture[3];	//Sonar 2 char array
char s3_capture[3];	//Sonar 3 char array
//
uint8_t measure_mess[1]="1"; //Message for Atmega328p
bool human = false; //If human is near Jora bool will be true
bool mainpwr_en = false;
int estate;
int standby_phrases = 1; //Counter for standby phrases
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM8_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART2_UART_Init(void);
void SonnarTask(void *argument);
void StandbyTask(void *argument);
void EyeTask(void *argument);
void LedTask(void *argument);
void En_Task(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void sonnar() {
  //If RX receive. Interrupt. USART2
  if (huart2.RxXferCount == 0) {
    HAL_UART_Receive_IT(&huart2, in_buffer, 9);
  }
  //From RAW data to sensors arrays
  for(int i = 0; i<=2;i++) {
  s1_capture[i]=in_buffer[i];
  }

  for(int i = 0; i<=2;i++) {
  s2_capture[i]=in_buffer[i+3];
  }
  
  for(int i = 0; i<=2;i++) {
  s3_capture[i]=in_buffer[i+6];
  }

  //Making register cell 0, becase RAW data send empty data.
  if (s1_capture[0] == 0x20) {
    s1_capture[0] = 0;
  }
  if (s2_capture[0] == 0x20) {
    s2_capture[0] = 0;
  }
  if (s3_capture[0] == 0x20) {
    s3_capture[0] = 0;
  }
  //
  if (s1_capture[1] == 0x20) {
    s1_capture[1] = 0x00;
  }
  if (s2_capture[1] == 0x20) {
    s2_capture[1] = 0x00;
  }
  if (s3_capture[1] == 0x20) {
    s3_capture[1] = 0x00;
  }
  if (s1_capture[1] <= 0x35) {
    s1_capture[1] = 0x00;
  }
  if (s2_capture[1] <= 0x35) {
    s2_capture[1] = 0x00;
  }
  if (s3_capture[1] <= 0x35) {
    s3_capture[1] = 0x00;
  }
  //Send command to Atmge328p compose data and transmit it to STM32 main board
  memset(s1_capture, 0, 3); //Erase Sonar 1 buffer
  memset(s2_capture, 0, 3); //Erase Sonar 2 buffer
  memset(s3_capture, 0, 3); //Erase Sonar 3 buffer
  HAL_UART_Transmit(&huart2, measure_mess, 1, 500);
  //Checking if human is there
  if ((s1_capture[0] <= 0x00 && s1_capture[1] <= 0x00 && s1_capture[2] <= 0x39) || (s2_capture[0] <= 0x00 && s2_capture[1] <= 0x00 && s2_capture[2] <= 0x39) || (s3_capture[0] <= 0x00 && s3_capture[1] <= 0x00 && s3_capture[2] <= 0x39)) {
    human = true; //If human near robot
  } else {
    human = false; //If human far away from robot
  }
  HAL_Delay(500);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  DWT_Init(); //Micro seconds funcion
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_USART1_UART_Init();
  MX_SPI3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  TIM1->CCR1 = 168; //Stepper PWM duty cycle set. Equal 50%
  TIM8->CCR1 = 600; //Led PWM duty cycle set. Equal 50%
  Init_7219();      //Init eye function
  Clear_7219();     //Clear eye matrix
  HAL_Delay(1000);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);   //Start PWM HTIM8
  HAL_UART_Receive_IT(&huart2, in_buffer, 9); //Start USART2 RX funcion
  /* USER CODE END 2 */

  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Scan */
  const osThreadAttr_t Scan_attributes = {
    .name = "Scan",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  ScanHandle = osThreadNew(SonnarTask, NULL, &Scan_attributes);

  /* definition and creation of Standby */
  const osThreadAttr_t Standby_attributes = {
    .name = "Standby",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  StandbyHandle = osThreadNew(StandbyTask, NULL, &Standby_attributes);

  /* definition and creation of Eye */
  const osThreadAttr_t Eye_attributes = {
    .name = "Eye",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  EyeHandle = osThreadNew(EyeTask, NULL, &Eye_attributes);

  /* definition and creation of Led */
  const osThreadAttr_t Led_attributes = {
    .name = "Led",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  LedHandle = osThreadNew(LedTask, NULL, &Led_attributes);

  /* definition and creation of MPE */
  const osThreadAttr_t MPE_attributes = {
    .name = "MPE",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  MPEHandle = osThreadNew(En_Task, NULL, &MPE_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 99;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 335;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 9;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 1679;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */
  HAL_TIM_MspPostInit(&htim8);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STP_PWR_EN_GPIO_Port, STP_PWR_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, EN_5V_Pin|GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : DIR_Pin */
  GPIO_InitStruct.Pin = DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EN_Pin */
  GPIO_InitStruct.Pin = EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MP_EN_Pin */
  GPIO_InitStruct.Pin = MP_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MP_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : STP_PWR_EN_Pin */
  GPIO_InitStruct.Pin = STP_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STP_PWR_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : EN_5V_Pin PD0 */
  GPIO_InitStruct.Pin = EN_5V_Pin|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : S1_IN_Pin S2_IN_Pin S3_IN_Pin */
  GPIO_InitStruct.Pin = S1_IN_Pin|S2_IN_Pin|S3_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_SonnarTask */
/**
  * @brief  Function implementing the Scan thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_SonnarTask */
void SonnarTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;) {
    sonnar();
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StandbyTask */
/**
* @brief Function implementing the Standby thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StandbyTask */
void StandbyTask(void *argument)
{
  /* USER CODE BEGIN StandbyTask */
  /* Infinite loop */
  for (;;) {
    HAL_Delay(500);
    if (!human && mainpwr_en) {
      //HAL_UART_Transmit(&huart1,mp15,10,1000);
      if (standby_phrases == 12) {
        standby_phrases = 1;
      }
      switch (standby_phrases) {
      case 1:
        mp3_play(4);
        break;
      case 2:
        mp3_play(5);
        break;
      case 3:
        mp3_play(8);
        break;
      case 4:
        mp3_play(9);
        break;
      case 5:
        mp3_play(10);
        break;
      case 6:
        mp3_play(13);
        break;
      case 7:
        mp3_play(15);
        break;
      case 8:
        mp3_play(20);
        break;
      case 9:
        mp3_play(23);
        break;
      case 10:
        mp3_play(24);
        break;
      case 11:
        mp3_play(26);
        break;
      case 12:
        mp3_play(37);
        break;
      }

      StepperMove(TIM_CHANNEL_1, 2000, 1, 200, true); //30 steps, forward
      TIM8->CCR1 = 1200;
      StepperMove(TIM_CHANNEL_1, 500, 0, 200, true); //30 steps, forward
      TIM8->CCR1 = 300;
      StepperMove(TIM_CHANNEL_1, 500, 1, 200, true); //30 steps, forward
      TIM8->CCR1 = 700;
      StepperMove(TIM_CHANNEL_1, 1000, 1, 200, true); //30 steps, forward
      TIM8->CCR1 = 900;
      StepperMove(TIM_CHANNEL_1, 1500, 0, 200, true); //30 steps, forward
      TIM8->CCR1 = 200;
      StepperMove(TIM_CHANNEL_1, 1500, 0, 200, false); //30 steps, forward
      TIM8->CCR1 = 600;
      standby_phrases++;
      HAL_Delay(3000);
    }
    if (human && mainpwr_en) {
      mp3_play(52);
      StepperMove(TIM_CHANNEL_1, 3000, 1, 200, true); //30 steps, forward
      HAL_Delay(3000);
      if ((s1_capture[0] <= 0x00 && s1_capture[1] <= 0x00 && s1_capture[2] <= 0x39) || (s2_capture[0] <= 0x00 && s2_capture[1] <= 0x00 && s2_capture[2] <= 0x39) || (s3_capture[0] <= 0x00 && s3_capture[1] <= 0x00 && s3_capture[2] <= 0x39)) {
        StepperMove(TIM_CHANNEL_1, 0, 0, 0, false); //30 steps, forward
        mp3_play(54);
        bite_eye();
        HAL_Delay(2000);
      } else {
        StepperMove(TIM_CHANNEL_1, 3000, 0, 200, false); //30 steps, forward
      }
    }
    osDelay(1);
  }
  /* USER CODE END StandbyTask */
}

/* USER CODE BEGIN Header_EyeTask */
/**
* @brief Function implementing the Eye thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_EyeTask */
void EyeTask(void *argument)
{
  /* USER CODE BEGIN EyeTask */
  /* Infinite loop */
  for (;;) {
    if (!human && mainpwr_en) {
      eye_standby();
      eye_standby();
      eye_move_left();
      eye_move_right();
      osDelay(1);
    }
  }
  /* USER CODE END EyeTask */
}

/* USER CODE BEGIN Header_LedTask */
/**
* @brief Function implementing the Led thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LedTask */
void LedTask(void *argument)
{
  /* USER CODE BEGIN LedTask */
  /* Infinite loop */
  for (;;) {
    if (human && mainpwr_en) {
      for (int cycle = 0; cycle <= 500; cycle++) {
        TIM8->CCR1 = cycle;
        HAL_Delay(2);
      }
      for (int cycle = 500; cycle >= 0; cycle--) {
        TIM8->CCR1 = cycle;
        HAL_Delay(2);
      }
    }
    osDelay(1);
  }
  /* USER CODE END LedTask */
}

/* USER CODE BEGIN Header_En_Task */
/**
* @brief Function implementing the MPE thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_En_Task */
void En_Task(void *argument)
{
  /* USER CODE BEGIN En_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
    estate = HAL_GPIO_ReadPin(GPIOD, EN_5V_Pin);
    if(!HAL_GPIO_ReadPin(GPIOB, MP_EN_Pin))
    {
      mainpwr_en = !mainpwr_en;
      HAL_GPIO_TogglePin(GPIOD, EN_5V_Pin);
      HAL_GPIO_TogglePin(GPIOB, STP_PWR_EN_Pin);
      HAL_Delay(100);
      while(!HAL_GPIO_ReadPin(GPIOB, MP_EN_Pin))
      {
        HAL_Delay(10);
      }
    }
  }
  /* USER CODE END En_Task */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
