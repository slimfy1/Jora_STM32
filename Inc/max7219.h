#ifndef MAX7219_H_
#define MAX7219_H_

#include "stm32f4xx_hal.h"
#include "main.h"

void Send_7219 (uint8_t rg, uint8_t dt);
void Clear_7219 (void);
void Number_7219 (volatile long n);
void Init_7219 (void);
void eye_standby(void);
void eye_move_left(void);
void eye_move_right(void);
void bite_eye(void);

#endif /* MAX7219_H_ */
