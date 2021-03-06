/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DIR_Pin GPIO_PIN_10
#define DIR_GPIO_Port GPIOE
#define EN_Pin GPIO_PIN_11
#define EN_GPIO_Port GPIOE
#define MP_EN_Pin GPIO_PIN_12
#define MP_EN_GPIO_Port GPIOB
#define STP_PWR_EN_Pin GPIO_PIN_13
#define STP_PWR_EN_GPIO_Port GPIOB
#define EN_5V_Pin GPIO_PIN_15
#define EN_5V_GPIO_Port GPIOD
#define CS_SELECT_Pin GPIO_PIN_0
#define CS_SELECT_GPIO_Port GPIOD
#define S1_IN_Pin GPIO_PIN_2
#define S1_IN_GPIO_Port GPIOD
#define S2_IN_Pin GPIO_PIN_3
#define S2_IN_GPIO_Port GPIOD
#define S3_IN_Pin GPIO_PIN_4
#define S3_IN_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
