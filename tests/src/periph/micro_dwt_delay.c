#include "micro_dwt_delay.h"
#include <stdio.h>
/**
 * @brief Initializes DWT_Clock_Cycle_Count for DWT_Delay_us function
 * @return Error DWT counter
 * 1: clock cycle counter not started
 * 0: clock cycle counter works
 */
uint32_t DWT_Delay_Init(void) {
 /* Disable TRC */
	printf("DWT_Delay_Init called\r\n");
	return 1;
}

void DWT_Delay_us(uint32_t microseconds) {
	// printf("DWT DELAY called with %d microseconds\r\n", microseconds);
}

