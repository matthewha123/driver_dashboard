#include "timer.h"

TIM_HandleTypeDef htim2;

// HACK: page when the number of controls parameters has been paged through
// then page from controls
static void timer_2_init(void) {
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 9999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 200;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  CHECK_HAL_OK(HAL_TIM_Base_Init(&htim2))

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  CHECK_HAL_OK(HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig))

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  CHECK_HAL_OK(HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)) 
}

void timers_init(void) {
  timer_2_init();

  HAL_TIM_Base_Start_IT(&htim2);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
      B_State far_left_btn_val = poll(&far_left_button);
      B_State far_right_btn_val = poll(&far_right_button);

      B_State wheel_1_btn_val = UNPRESSED; // repalce with poll(&wheel_#_button);
      B_State wheel_2_btn_val = UNPRESSED;
      B_State wheel_3_btn_val = UNPRESSED;
      B_State wheel_4_btn_val = UNPRESSED;
      B_State wheel_5_btn_val = UNPRESSED;
      B_State wheel_6_btn_val = UNPRESSED;

      if(has_finished_gui_init()) {
        interrupt_process_buttons(far_right_btn_val, far_left_btn_val, 
          wheel_1_btn_val, wheel_2_btn_val, wheel_3_btn_val,
          wheel_4_btn_val, wheel_5_btn_val, wheel_6_btn_val);
      }
    }
}

