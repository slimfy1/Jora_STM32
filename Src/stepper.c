#include "stepper.h"
#include "dwt_delay.h"

extern TIM_HandleTypeDef htim1;

void StepperMove(char channel, int pulsecount, int dir, int del, bool disable) {
  HAL_GPIO_WritePin(GPIOE, EN_Pin, GPIO_PIN_SET); // Enable stepper driver
  //Forward: Dir = 1; Backward: Dir = 0;
  HAL_GPIO_WritePin(GPIOE, DIR_Pin, GPIO_PIN_RESET);
  if (dir == 1) {
    HAL_GPIO_WritePin(GPIOE, DIR_Pin, GPIO_PIN_SET); //HIGH level mean forward vector
  }
  if (dir == 0) {
    HAL_GPIO_WritePin(GPIOE, DIR_Pin, GPIO_PIN_RESET); //LOW level mean backward vector
  }
  HAL_TIM_PWM_Start(&htim1, channel); //Start PWM timer
  DWT_Delay(200 * pulsecount);
  //HAL_Delay(1*pulsecount);
  //"pulsecount" is a number of stepper steps. Example, 5 count = 5 stepper steps
  //"1000" is a 1000 microseconds, which depend from PWM frequence. 1000 microseconds = 2kHz pwm frequence.
  HAL_TIM_PWM_Stop(&htim1, channel); // Disable PWM timer
  if (!disable) {
    HAL_GPIO_WritePin(GPIOE, EN_Pin, GPIO_PIN_RESET); //Disable stepper driver
  }
  if (disable) {
    HAL_GPIO_WritePin(GPIOE, EN_Pin, GPIO_PIN_SET); //Disable stepper driver
  }
  HAL_Delay(del);
}
