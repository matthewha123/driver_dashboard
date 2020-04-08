#ifndef __STM_H
#define __STM_H

#include <stdint.h>
#define TESTING // Oled dot matrix code uses itoa, which isn't supported by my gcc compiler for some reason
				// This macro is used to use sprintf instead of itoa
#define UNUSED(expr) do { (void)(expr); } while (0)
#define GPIO_TypeDef uint32_t

#ifdef TESTING
char* testing_itoa(int num, char *str);
#endif // TESTING

typedef enum {
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

void HAL_Init(void);

uint32_t HAL_GetTick(void);

#endif // __STM_H