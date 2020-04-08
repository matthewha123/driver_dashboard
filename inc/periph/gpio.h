#ifndef __GPIO__H_
#define __GPIO__H_

#include "common.h"
#include "pins.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define DEBUG_LED_PIN GPIO_PIN_7
#define DEBUG_LED_PORT GPIOB

void GPIO_Init(void);
void OLED_Pin_Init(void);
void OLED_Data_Pins_Init(void);
void Pin_Write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
bool Pin_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif
#endif

