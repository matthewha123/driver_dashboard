#ifndef DWT_STM32_DELAY_H
#define DWT_STM32_DELAY_H
#include <unistd.h>
#include <stdint.h>

/**
 * @brief Initializes DWT_Cycle_Count for DWT_Delay_us function
 * @return Error DWT counter
 * 1: DWT counter Error
 * 0: DWT counter works
 */
uint32_t DWT_Delay_Init(void);
 
/**
 * @brief This function provides a delay (in microseconds)
 * @param microseconds: delay in microseconds
 */

// __STATIC_INLINE void DWT_Delay_us(uint32_t microseconds)
// {
// 	usleep(microseconds);
// }
#endif