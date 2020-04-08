#include "clock.h"
#include "stm32f4xx_hal.h"

//copied vcu clock config
static void varying_clock_config(RCC_OscInitTypeDef *RCC_OscInitStruct);

void clock_config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  // Configure the main internal regulator output voltage
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Initializes the CPU, AHB and APB busses clocks
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE;

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE;
  RCC_ClkInitStruct.AHBCLKDivider = AHB_CLK_DIVIDER;
  RCC_ClkInitStruct.APB1CLKDivider = APB1_CLK_DIVIDER;
  RCC_ClkInitStruct.APB2CLKDivider = APB2_CLK_DIVIDER;
 
  RCC_OscInitStruct.PLL.PLLState = PLL_STATE;

  varying_clock_config(&RCC_OscInitStruct);

  CHECK_HAL_OK(HAL_RCC_OscConfig(&RCC_OscInitStruct))
  CHECK_HAL_OK(HAL_RCC_OscConfig(&RCC_OscInitStruct))
  CHECK_HAL_OK(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY))

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

#ifdef HSI_CLOCK

static void varying_clock_config(RCC_OscInitTypeDef *RCC_OscInitStruct) {
  RCC_OscInitStruct->HSIState = RCC_HSI_ON;
  RCC_OscInitStruct->HSICalibrationValue = 16;
}

#endif

#ifdef HSE_PLL_CLOCK

static void varying_clock_config(RCC_OscInitTypeDef *RCC_OscInitStruct) {
  RCC_OscInitStruct->HSEState = RCC_HSE_ON;

  RCC_OscInitStruct->PLL.PLLState = RCC_PLL_ON;

  RCC_OscInitStruct->PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct->PLL.PLLM = 16;
  RCC_OscInitStruct->PLL.PLLN = 256; //240 , with div 4, pllm 16, 90 mhz
  RCC_OscInitStruct->PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct->PLL.PLLQ = 8;
  RCC_OscInitStruct->PLL.PLLR = 2;

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDIO
                                |RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
    PeriphClkInitStruct.SdioClockSelection = RCC_SDIOCLKSOURCE_CLK48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
      _Error_Handler(__FILE__, __LINE__);
    }
}

#endif

