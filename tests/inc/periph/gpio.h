#ifndef __GPIO__H_
#define __GPIO__H_
#include <stdint.h>
#include <stdbool.h>
#include "pins.h"
#include "stm32f4xx_hal.h"

#define DEBUG_LED_PIN GPIO_PIN_7
#define DEBUG_LED_PORT GPIOB

void GPIO_Init(void);
void Pin_Write(int port, Pin_T pin, bool pin_state);
bool Pin_Read(int port, Pin_T pin);

#endif