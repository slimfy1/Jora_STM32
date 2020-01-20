#ifndef STEPPER_H_
#define STEPPER_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "stdbool.h"

void StepperMove(char channel, int pulsecount, int dir, int del, bool disable);

#endif /* STEPPER_H_ */
