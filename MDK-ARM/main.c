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
#include "dwt_delay.h"
#include "stdbool.h"
#include "max7219.h"
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
/* USER CODE BEGIN PV */
	uint8_t eye_speed = 20; //Eye animation speed
	static __IO uint32_t TimingDelay;
	uint8_t mp1[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 01, 0xFE, 0xE8, 0xEF};
	uint8_t mp2[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 02, 0xFE, 0xE7, 0xEF};
	uint8_t mp3[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 03, 0xFE, 0xE6, 0xEF};
	uint8_t mp4[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 04, 0xFE, 0xE5, 0xEF};
	uint8_t mp5[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 05, 0xFE, 0xE4, 0xEF};
	uint8_t mp6[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 06, 0xFE, 0xE3, 0xEF};
	uint8_t mp7[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 07, 0xFE, 0xE2, 0xEF};
	uint8_t mp8[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x08, 0xFE, 0xE1, 0xEF};
	uint8_t mp9[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x09, 0xFE, 0xE0, 0xEF};
	uint8_t mp10[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x0A, 0xFE, 0xDF, 0xEF};
	uint8_t mp11[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x0B, 0xFE, 0xDE, 0xEF};
	uint8_t mp12[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x0C, 0xFE, 0xDD, 0xEF};
	uint8_t mp13[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x0D, 0xFE, 0xDC, 0xEF};
	uint8_t mp14[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x0E, 0xFE, 0xDB, 0xEF};
	uint8_t mp15[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x0F, 0xFE, 0xDA, 0xEF};
	uint8_t mp16[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x10, 0xFE, 0xD9, 0xEF};
	uint8_t mp17[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x11, 0xFE, 0xD8, 0xEF};
	uint8_t mp18[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x12, 0xFE, 0xD7, 0xEF};
	uint8_t mp19[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x13, 0xFE, 0xD6, 0xEF};
	uint8_t mp20[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x14, 0xFE, 0xD5, 0xEF};
	uint8_t mp21[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x15, 0xFE, 0xD4, 0xEF};
	uint8_t mp22[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x16, 0xFE, 0xD3, 0xEF};
	uint8_t mp23[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x17, 0xFE, 0xD2, 0xEF};
	uint8_t mp24[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x18, 0xFE, 0xD1, 0xEF};
	uint8_t mp25[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x19, 0xFE, 0xD0, 0xEF};
	uint8_t mp26[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x1A, 0xFE, 0xCF, 0xEF};
	uint8_t mp27[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x1B, 0xFE, 0xCE, 0xEF};
	uint8_t mp28[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x1C, 0xFE, 0xCD, 0xEF};
	uint8_t mp29[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x1D, 0xFE, 0xCC, 0xEF};
	uint8_t mp30[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x1E, 0xFE, 0xCB, 0xEF};
	uint8_t mp31[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x1F, 0xFE, 0xCA, 0xEF};
	uint8_t mp32[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x20, 0xFE, 0xC9, 0xEF};
	uint8_t mp33[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x21, 0xFE, 0xC8, 0xEF};
	uint8_t mp34[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x22, 0xFE, 0xC7, 0xEF};
	uint8_t mp35[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x23, 0xFE, 0xC6, 0xEF};
	uint8_t mp36[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x24, 0xFE, 0xC5, 0xEF};
	uint8_t mp37[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x25, 0xFE, 0xC4, 0xEF};
	uint8_t mp38[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x26, 0xFE, 0xC3, 0xEF};
	uint8_t mp39[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x27, 0xFE, 0xC2, 0xEF};
	uint8_t mp40[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x28, 0xFE, 0xC1, 0xEF};
	uint8_t mp41[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x29, 0xFE, 0xC0, 0xEF};
	uint8_t mp42[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x2A, 0xFE, 0xBF, 0xEF};
	uint8_t mp43[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x2B, 0xFE, 0xBE, 0xEF};
	uint8_t mp44[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x2C, 0xFE, 0xBD, 0xEF};
	uint8_t mp45[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x2D, 0xFE, 0xBC, 0xEF};
	uint8_t mp46[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x2E, 0xFE, 0xBB, 0xEF};
	uint8_t mp47[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x2F, 0xFE, 0xBA, 0xEF};
	uint8_t mp48[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x30, 0xFE, 0xB9, 0xEF};
	uint8_t mp49[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x31, 0xFE, 0xB8, 0xEF};
	uint8_t mp50[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x32, 0xFE, 0xB7, 0xEF};
	uint8_t mp51[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x33, 0xFE, 0xB6, 0xEF};
	uint8_t mp52[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x34, 0xFE, 0xB5, 0xEF};
	uint8_t mp53[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x35, 0xFE, 0xB4, 0xEF};
	uint8_t mp54[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x36, 0xFE, 0xB3, 0xEF};
	uint8_t mp55[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x37, 0xFE, 0xB2, 0xEF};
	uint8_t mp56[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x38, 0xFE, 0xB1, 0xEF};
	uint8_t mp57[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x39, 0xFE, 0xB0, 0xEF};
	uint8_t mp58[10] = {0x7E, 0xFF, 06, 0x12, 00, 00, 0x3A, 0xFE, 0xAF, 0xEF};
	//Input buffer from Atmega328p. Raw data
	uint8_t in_buffer[9]={0}; // Raw data from 3 sensors
	//Sonar data chars
	char s1_capture[3]; //Sonar 1 char array
	char s2_capture[3];	//Sonar 2 char array
	char s3_capture[3];	//Sonar 3 char array
	//
	uint8_t measure_mess[]="1"; //Message for Atmega328p
	bool human = false; //If human is near Jora bool will be true
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

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void play_mp3(int numb)
{
	//Send MP3 number to DFPlayer using USART1
	switch(numb)
	{
		case 1: HAL_UART_Transmit(&huart1,mp1,10,1000);break;
		case 2:	HAL_UART_Transmit(&huart1,mp2,10,1000);break;
		case 3: HAL_UART_Transmit(&huart1,mp3,10,1000);break;
		case 4:	HAL_UART_Transmit(&huart1,mp4,10,1000);break;
		case 5: HAL_UART_Transmit(&huart1,mp5,10,1000);break;
		case 6:	HAL_UART_Transmit(&huart1,mp6,10,1000);break;
		case 7: HAL_UART_Transmit(&huart1,mp7,10,1000);break;
		case 8:	HAL_UART_Transmit(&huart1,mp8,10,1000);break;
		case 9: HAL_UART_Transmit(&huart1,mp9,10,1000);break;
		case 10:	HAL_UART_Transmit(&huart1,mp10,10,1000);break;
		case 11:	HAL_UART_Transmit(&huart1,mp11,10,1000);break;
		case 12:	HAL_UART_Transmit(&huart1,mp12,10,1000);break;
		case 13: 	HAL_UART_Transmit(&huart1,mp13,10,1000);break;
		case 14:	HAL_UART_Transmit(&huart1,mp14,10,1000);break;
		case 15: 	HAL_UART_Transmit(&huart1,mp15,10,1000);break;
		case 16:	HAL_UART_Transmit(&huart1,mp16,10,1000);break;
		case 17: 	HAL_UART_Transmit(&huart1,mp17,10,1000);break;
		case 18:	HAL_UART_Transmit(&huart1,mp18,10,1000);break;
		case 19: HAL_UART_Transmit(&huart1,mp19,10,1000);break;
		case 20:	HAL_UART_Transmit(&huart1,mp20,10,1000);break;
		case 21: HAL_UART_Transmit(&huart1,mp21,10,1000);break;
		case 22:	HAL_UART_Transmit(&huart1,mp22,10,1000);break;
		case 23: HAL_UART_Transmit(&huart1,mp23,10,1000);break;
		case 24:	HAL_UART_Transmit(&huart1,mp24,10,1000);break;
		case 25: HAL_UART_Transmit(&huart1,mp25,10,1000);break;
		case 26:	HAL_UART_Transmit(&huart1,mp26,10,1000);break;
		case 27: HAL_UART_Transmit(&huart1,mp27,10,1000);break;
		case 28:	HAL_UART_Transmit(&huart1,mp28,10,1000);break;
		case 29:	HAL_UART_Transmit(&huart1,mp29,10,1000);break;
		case 30:	HAL_UART_Transmit(&huart1,mp30,10,1000);break;
		case 31: 	HAL_UART_Transmit(&huart1,mp31,10,1000);break;
		case 32:	HAL_UART_Transmit(&huart1,mp32,10,1000);break;
		case 33: 	HAL_UART_Transmit(&huart1,mp33,10,1000);break;
		case 34:	HAL_UART_Transmit(&huart1,mp34,10,1000);break;
		case 35: 	HAL_UART_Transmit(&huart1,mp35,10,1000);break;
		case 36:	HAL_UART_Transmit(&huart1,mp36,10,1000);break;
		case 37:	HAL_UART_Transmit(&huart1,mp37,10,1000);break;
		case 38: HAL_UART_Transmit(&huart1,mp38,10,1000);break;
		case 39:	HAL_UART_Transmit(&huart1,mp39,10,1000);break;
		case 40: HAL_UART_Transmit(&huart1,mp40,10,1000);break;
		case 41:	HAL_UART_Transmit(&huart1,mp41,10,1000);break;
		case 42: HAL_UART_Transmit(&huart1,mp42,10,1000);break;
		case 43:	HAL_UART_Transmit(&huart1,mp43,10,1000);break;
		case 44: HAL_UART_Transmit(&huart1,mp44,10,1000);break;
		case 45:	HAL_UART_Transmit(&huart1,mp45,10,1000);break;
		case 46:	HAL_UART_Transmit(&huart1,mp46,10,1000);break;
		case 47:	HAL_UART_Transmit(&huart1,mp47,10,1000);break;
		case 48: 	HAL_UART_Transmit(&huart1,mp48,10,1000);break;
		case 49:	HAL_UART_Transmit(&huart1,mp49,10,1000);break;
		case 50: 	HAL_UART_Transmit(&huart1,mp50,10,1000);break;
		case 51:	HAL_UART_Transmit(&huart1,mp51,10,1000);break;
		case 52: 	HAL_UART_Transmit(&huart1,mp52,10,1000);break;
		case 53:	HAL_UART_Transmit(&huart1,mp53,10,1000);break;
		case 54:	HAL_UART_Transmit(&huart1,mp54,10,1000);break;
		case 55:	HAL_UART_Transmit(&huart1,mp55,10,1000);break;
		case 56: 	HAL_UART_Transmit(&huart1,mp56,10,1000);break;
		case 57:	HAL_UART_Transmit(&huart1,mp57,10,1000);break;
		case 58: 	HAL_UART_Transmit(&huart1,mp58,10,1000);break;
	}
}
void StepperMove(char channel, int pulsecount, int dir, int del, bool disable)
{
		//htim1.Instance = TIM1;
		HAL_GPIO_WritePin(GPIOE, EN_Pin, GPIO_PIN_SET); // Enable stepper driver
		//Forward: Dir = 1; Backward: Dir = 0;
		HAL_GPIO_WritePin(GPIOE, DIR_Pin, GPIO_PIN_RESET);
		if(dir == 1)
		{
			HAL_GPIO_WritePin(GPIOE, DIR_Pin, GPIO_PIN_SET); //HIGH level mean forward vector
		}
		if(dir == 0)
		{
			HAL_GPIO_WritePin(GPIOE, DIR_Pin, GPIO_PIN_RESET); //LOW level mean backward vector
		}
		HAL_TIM_PWM_Start(&htim1, channel); //Start PWM timer
		DWT_Delay(200*pulsecount);
		//HAL_Delay(1*pulsecount);
		//"pulsecount" is a number of stepper steps. Example, 5 count = 5 stepper steps
		//"1000" is a 1000 microseconds, which depend from PWM frequence. 1000 microseconds = 2kHz pwm frequence.
		HAL_TIM_PWM_Stop(&htim1, channel); // Disable PWM timer
		if(!disable)
		{
			HAL_GPIO_WritePin(GPIOE, EN_Pin, GPIO_PIN_RESET); //Disable stepper driver
		}
		if(disable)
		{
			HAL_GPIO_WritePin(GPIOE, EN_Pin, GPIO_PIN_SET); //Disable stepper driver
		}	
		HAL_Delay(del);
}

void sonnar()
{
		//If RX receive. Interrupt. USART2
		if(huart2.RxXferCount == 0)
		{
			HAL_UART_Receive_IT(&huart2,in_buffer, 9);
		}
		//From RAW data to sensors arrays
		s1_capture[0] = in_buffer[0];
		s1_capture[1] = in_buffer[1];
		s1_capture[2] = in_buffer[2];
		s2_capture[0] = in_buffer[3];
		s2_capture[1] = in_buffer[4];
		s2_capture[2] = in_buffer[5];
		s3_capture[0] = in_buffer[6];
		s3_capture[1] = in_buffer[7];
		s3_capture[2] = in_buffer[8];
		//Making register cell 0, becase RAW data send empty data.
		if(s1_capture[0] == 0x20)
		{
			s1_capture[0] = 0;
		}
		if(s2_capture[0] == 0x20)
		{
			s2_capture[0] = 0;
		}
		if(s3_capture[0] == 0x20)
		{
			s3_capture[0] = 0;
		}
		//
		if(s1_capture[1] == 0x20)
		{
			s1_capture[1] = 0x00;
		}
		if(s2_capture[1] == 0x20)
		{
			s2_capture[1] = 0x00;
		}
		if(s3_capture[1] == 0x20)
		{
			s3_capture[1] = 0x00;
		}
		if(s1_capture[1] <= 0x35)
		{
			s1_capture[1] = 0x00;
		}
		if(s2_capture[1] <= 0x35)
		{
			s2_capture[1] = 0x00;
		}
		if(s3_capture[1] <= 0x35)
		{
			s3_capture[1] = 0x00;
		}
		//Send command to Atmge328p compose data and transmit it to STM32 main board
		HAL_UART_Transmit(&huart2, measure_mess, 1, 500);
		//Checking if human is there
		if((s1_capture[0]<=0x00 && s1_capture[1]<=0x00 && s1_capture[2]<=0x39) || (s2_capture[0]<=0x00 && s2_capture[1]<=0x00 && s2_capture[2]<=0x39) || (s3_capture[0]<=0x00 && s3_capture[1]<=0x00 && s3_capture[2]<=0x39))
		{
			human = true; //If human near robot
		}
		else
		{
			human = false; //If human far away from robot
		}
		HAL_Delay(500);
}
void eye_standby(void)
{
		//Blinking eye animation
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x3C);//	00111100
		Send_7219(0x03,0x7E);//	01111110
		Send_7219(0x04,0x7E);//	01111110
		Send_7219(0x05,0x7E);//	01111110
		Send_7219(0x06,0x7E);//	01111110
		Send_7219(0x07,0x3C);//	00111100
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x00);//	00000000
		Send_7219(0x03,0x7E);//	01111110
		Send_7219(0x04,0x7E);//	01111110
		Send_7219(0x05,0x7E);//	01111110
		Send_7219(0x06,0x7E);//	01111110
		Send_7219(0x07,0x00);//	00000000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x00);//	00000000
		Send_7219(0x03,0x00);//	00000000
		Send_7219(0x04,0x7E);//	01111110
		Send_7219(0x05,0x7E);//	01111110
		Send_7219(0x06,0x00);//	00000000
		Send_7219(0x07,0x00);//	00000000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x00);//	00000000
		Send_7219(0x03,0x00);//	00000000
		Send_7219(0x04,0x00);//	00000000
		Send_7219(0x05,0x00);//	00000000
		Send_7219(0x06,0x00);//	00000000
		Send_7219(0x07,0x00);//	00000000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x00);//	00000000
		Send_7219(0x03,0x00);//	00000000
		Send_7219(0x04,0x7E);//	01111110
		Send_7219(0x05,0x7E);//	01111110
		Send_7219(0x06,0x00);//	00000000
		Send_7219(0x07,0x00);//	00000000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x00);//	00000000
		Send_7219(0x03,0x7E);//	01111110
		Send_7219(0x04,0x7E);//	01111110
		Send_7219(0x05,0x7E);//	01111110
		Send_7219(0x06,0x7E);//	01111110
		Send_7219(0x07,0x00);//	00000000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x3C);//	00111100
		Send_7219(0x03,0x7E);//	01111110
		Send_7219(0x04,0x7E);//	01111110
		Send_7219(0x05,0x7E);//	01111110
		Send_7219(0x06,0x7E);//	01111110
		Send_7219(0x07,0x3C);//	00111100
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(2000);
}
void eye_move_left(void)
{
		//Moving eye to left
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x78);//	01111000
		Send_7219(0x03,0x7C);//	11111100
		Send_7219(0x04,0x7C);//	11111100
		Send_7219(0x05,0x7C);//	11111100
		Send_7219(0x06,0x7C);//	11111100
		Send_7219(0x07,0x78);//	01111000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0xF0);//	11110000
		Send_7219(0x03,0xF8);//	11111000
		Send_7219(0x04,0xF8);//	11111000
		Send_7219(0x05,0xF8);//	11111000
		Send_7219(0x06,0xF8);//	11111000
		Send_7219(0x07,0xF0);//	11111000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0xE0);//	11100000
		Send_7219(0x03,0xF0);//	11110000
		Send_7219(0x04,0xF0);//	11110000
		Send_7219(0x05,0xF0);//	11110000
		Send_7219(0x06,0xF0);//	11110000
		Send_7219(0x07,0xE0);//	11100000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0xC0);//	11000000
		Send_7219(0x03,0xE0);//	11100000
		Send_7219(0x04,0xE0);//	11100000
		Send_7219(0x05,0xE0);//	11100000
		Send_7219(0x06,0xE0);//	11100000
		Send_7219(0x07,0xC0);//	11000000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(1000);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0xE0);//	11100000
		Send_7219(0x03,0xF0);//	11110000
		Send_7219(0x04,0xF0);//	11110000
		Send_7219(0x05,0xF0);//	11110000
		Send_7219(0x06,0xF0);//	11110000
		Send_7219(0x07,0xE0);//	11100000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0xF0);//	11110000
		Send_7219(0x03,0xF8);//	11111000
		Send_7219(0x04,0xF8);//	11111000
		Send_7219(0x05,0xF8);//	11111000
		Send_7219(0x06,0xF8);//	11111000
		Send_7219(0x07,0xF0);//	11111000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x78);//	01111000
		Send_7219(0x03,0x7C);//	11111100
		Send_7219(0x04,0x7C);//	11111100
		Send_7219(0x05,0x7C);//	11111100
		Send_7219(0x06,0x7C);//	11111100
		Send_7219(0x07,0x78);//	01111000
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x3C);//	00111100
		Send_7219(0x03,0x7E);//	01111110
		Send_7219(0x04,0x7E);//	01111110
		Send_7219(0x05,0x7E);//	01111110
		Send_7219(0x06,0x7E);//	01111110
		Send_7219(0x07,0x3C);//	00111100
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
}
void eye_move_right(void)
{
		//Moving eye to right
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x1F);//	00011111
		Send_7219(0x03,0x3F);// 00111111
		Send_7219(0x04,0x3F);//	00111111
		Send_7219(0x05,0x3F);//	00111111
		Send_7219(0x06,0x3F);//	00111111
		Send_7219(0x07,0x1F);//	00011111
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x0F);//	00001111
		Send_7219(0x03,0x1F);// 00011111
		Send_7219(0x04,0x1F);//	00011111
		Send_7219(0x05,0x1F);//	00011111
		Send_7219(0x06,0x1F);//	00011111
		Send_7219(0x07,0x0F);//	00001111
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x07);//	00000111
		Send_7219(0x03,0x0F);// 00001111
		Send_7219(0x04,0x0F);//	00001111
		Send_7219(0x05,0x0F);//	00001111
		Send_7219(0x06,0x0F);//	00001111
		Send_7219(0x07,0x07);//	00000111
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x03);//	00000011
		Send_7219(0x03,0x07);// 00000111
		Send_7219(0x04,0x07);//	00000111
		Send_7219(0x05,0x07);//	00000111
		Send_7219(0x06,0x07);//	00000111
		Send_7219(0x07,0x03);//	00000011
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(1000);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x07);//	00000111
		Send_7219(0x03,0x0F);// 00001111
		Send_7219(0x04,0x0F);//	00001111
		Send_7219(0x05,0x0F);//	00001111
		Send_7219(0x06,0x0F);//	00001111
		Send_7219(0x07,0x07);//	00000111
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x0F);//	00001111
		Send_7219(0x03,0x1F);// 00011111
		Send_7219(0x04,0x1F);//	00011111
		Send_7219(0x05,0x1F);//	00011111
		Send_7219(0x06,0x1F);//	00011111
		Send_7219(0x07,0x0F);//	00001111
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x1F);//	00011111
		Send_7219(0x03,0x3F);// 00111111
		Send_7219(0x04,0x3F);//	00111111
		Send_7219(0x05,0x3F);//	00111111
		Send_7219(0x06,0x3F);//	00111111
		Send_7219(0x07,0x1F);//	00011111
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
		Send_7219(0x01,0x00);//	00000000
		Send_7219(0x02,0x3C);//	00111100
		Send_7219(0x03,0x7E);//	01111110
		Send_7219(0x04,0x7E);//	01111110
		Send_7219(0x05,0x7E);//	01111110
		Send_7219(0x06,0x7E);//	01111110
		Send_7219(0x07,0x3C);//	00111100
		Send_7219(0x08,0x00);//	00000000
		HAL_Delay(eye_speed);
}
void bite_eye()
{
	//While bite
	Send_7219(0x01,0x00);//	00000000
	Send_7219(0x02,0x00);//	00000000
	Send_7219(0x03,0x00);//	00000000
	Send_7219(0x04,0x00);//	00000000
	Send_7219(0x05,0x81);//	10000001
	Send_7219(0x06,0x42);//	01000010
	Send_7219(0x07,0x3C);//	00111100
	Send_7219(0x08,0x18);//	00011000
		HAL_Delay(eye_speed);
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
	HAL_Delay(200);
	TIM1->CCR1=168; //Stepper PWM duty cycle set. Equal 50%
	TIM8->CCR1=600; //Led PWM duty cycle set. Equal 50%
	Init_7219(); //Init eye function
	Clear_7219(); //Clear eye matrix
	HAL_Delay(2000);
	Clear_7219();

	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); //Start PWM HTIM8
	HAL_UART_Receive_IT(&huart2,in_buffer, 9); //Start USART2 RX funcion
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
  huart1.Init.BaudRate = 9600;
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
  huart2.Init.BaudRate = 9600;
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);

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

  /*Configure GPIO pin : PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
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
  for(;;)
  {
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
  for(;;)
  {
		HAL_Delay(500);
		if(!human)
		{
			//HAL_UART_Transmit(&huart1,mp15,10,1000);
			if(standby_phrases == 12)
			{
				standby_phrases = 1;
			}
			switch(standby_phrases)
			{
				case 1: play_mp3(4);break;
				case 2:	play_mp3(5);break;
				case 3:	play_mp3(8);break;
				case 4:	play_mp3(9);break;
				case 5:	play_mp3(10);break;
				case 6:	play_mp3(13);break;
				case 7:	play_mp3(15);break;
				case 8:	play_mp3(20);break;
				case 9:	play_mp3(23);break;
				case 10:	play_mp3(24);break;
				case 11:	play_mp3(26);break;
				case 12:	play_mp3(37);break;
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
		if(human)
		{
			play_mp3(52);
			StepperMove(TIM_CHANNEL_1, 3000, 1, 200, true); //30 steps, forward
			HAL_Delay(3000);
			if((s1_capture[0]<=0x00 && s1_capture[1]<=0x00 && s1_capture[2]<=0x39) || (s2_capture[0]<=0x00 && s2_capture[1]<=0x00 && s2_capture[2]<=0x39) || (s3_capture[0]<=0x00 && s3_capture[1]<=0x00 && s3_capture[2]<=0x39))
			{
				StepperMove(TIM_CHANNEL_1, 0, 0, 0, false); //30 steps, forward
				play_mp3(54);
				bite_eye();
				HAL_Delay(2000);
			}
			else
			{
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
  for(;;)
  {
		if(!human)
		{
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
  for(;;)
  {
		if(human)
		{
			for(int cycle = 0; cycle<=500; cycle++)
			{
				TIM8->CCR1 = cycle;
				HAL_Delay(2);
			}
			for(int cycle = 500; cycle>=0; cycle--)
			{
				TIM8->CCR1 = cycle;
				HAL_Delay(2);
			}
		}
    osDelay(1);
  }
  /* USER CODE END LedTask */
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
