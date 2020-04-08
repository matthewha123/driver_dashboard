#ifndef __COMMON_H_
#define __COMMON_H_

#include <stm32f4xx_hal.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
void _Error_Handler(char *, int);
#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#define CHECK_HAL_OK(func) { \
  if (func != HAL_OK) { \
    _Error_Handler(__FILE__, __LINE__); \
  } \
}

#ifdef __cplusplus
}
#endif

#endif // __COMMON_H_

