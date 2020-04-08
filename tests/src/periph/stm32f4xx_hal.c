#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <time.h>

#define UNUSED(expr) do { (void)(expr); } while (0)


struct timespec tp;

uint32_t HAL_GetTick() {
  // Return Unix time
  clock_gettime(CLOCK_MONOTONIC, &tp);
  uint32_t ms = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
  return ms;
}