#ifndef __CLOCK_H_
#define __CLOCK_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void clock_config(void);


#define HSE_PLL_CLOCK

#ifdef HSI_CLOCK

#define	RCC_OSCILLATORTYPE RCC_OSCILLATORTYPE_HSI
#define RCC_SYSCLKSOURCE   RCC_SYSCLKSOURCE_HSI
#define AHB_CLK_DIVIDER    RCC_SYSCLK_DIV1
#define APB1_CLK_DIVIDER   RCC_HCLK_DIV1
#define APB2_CLK_DIVIDER   RCC_HCLK_DIV1
#define PLL_STATE          RCC_PLL_NONE
#define FLASH_LATENCY      FLASH_LATENCY_0

#endif //HSI_CLOCK

#ifdef HSE_PLL_CLOCK

#define	RCC_OSCILLATORTYPE RCC_OSCILLATORTYPE_HSE
#define RCC_SYSCLKSOURCE   RCC_SYSCLKSOURCE_PLLCLK
#define AHB_CLK_DIVIDER    RCC_SYSCLK_DIV1
#define APB1_CLK_DIVIDER   RCC_HCLK_DIV2
#define APB2_CLK_DIVIDER   RCC_HCLK_DIV1
#define PLL_STATE          RCC_PLL_ON
#define FLASH_LATENCY      FLASH_LATENCY_3

#endif//HSE_PLL_CLOCK

#ifdef __cplusplus
}
#endif
#endif //__CLOCK_H_
