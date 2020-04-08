#include "main.h"
#include "interrupts.h"

TLC5955 led_driver;

uint32_t last_blink           = 0;
uint32_t last_page_transition = 0;
bool blinker                  = true;

float percentage = 100.0;

Button far_right_button;
Button far_left_button;
Button right_button;
Button left_button;

Button wheel_1_button;
Button wheel_2_button;
Button wheel_3_button;
Button wheel_4_button;
Button wheel_5_button;
Button wheel_6_button;

int main(void) {

  initialize_hardware();

  gui_init();

  make_button(&left_button, LEFT_BUTTON_PORT, LEFT_BUTTON_PIN);
  make_button(&right_button, RIGHT_BUTTON_PORT, RIGHT_BUTTON_PIN);
  make_button(&far_left_button, FAR_LEFT_BUTTON_PORT, FAR_LEFT_BUTTON_PIN);
  make_button(&far_right_button, FAR_RIGHT_BUTTON_PORT, FAR_RIGHT_BUTTON_PIN);
  make_button(&wheel_1_button, GPIOA, WHEEL_1_Pin);
  make_button(&wheel_2_button, GPIOC, WHEEL_2_Pin);
  make_button(&wheel_3_button, GPIOA, WHEEL_3_Pin);
  make_button(&wheel_4_button, GPIOA, WHEEL_4_Pin);
  make_button(&wheel_5_button, GPIOB, WHEEL_5_Pin);
  make_button(&wheel_6_button, GPIOB, WHEEL_6_Pin);

  turn_dash_leds_off();

  Pin_Write(BUZZER_PORT, BUZZER_PIN, false);

  reset_buzzer_state();

  while (1) {

    debug_blink();

    handle_driver_reset();
    handle_ready_to_drive();
    handle_buzzer();
    handle_fault_leds();

    CANlib_update_can();
    can_transmit();
    // mock_tick_can_vals() ;
    handle_led_bars(&led_driver);
    gui_update();  // later replace with the actual paging buttons
  }

  return 0;
}

void handle_fault_leds() {
  Pin_Write(BMS_FLT_GPIO_Port, BMS_FLT_Pin, CANlib_critical_VCUDashInfo_Input.bms == 1);
  Pin_Write(IMD_FLT_GPIO_Port, IMD_FLT_Pin, CANlib_critical_VCUDashInfo_Input.imd == 1);
}

void initialize_hardware() {
  HAL_Init();
  clock_config();
  timers_init();
  interrupt_config();
  GPIO_Init();
  uart_init();
  spi_init(SPI1);
  can_init();
  if (DWT_Delay_Init()) {
    Error_Handler();
  }
  Init_LED_Driver(&led_driver);

  start_display(&ugfx_oled);
  oled_draw_racecar_bmp_greyscale(&ugfx_oled, 20, 0);

  int32_t splash_start_time = HAL_GetTick();

  while (HAL_GetTick() - splash_start_time < 2000) {
  }

  printf("Hardware Initalization Complete\r\n");
}

void debug_blink(void) {

  if (HAL_GetTick() - last_blink > 1000) {
    blinker    = !blinker;
    last_blink = HAL_GetTick();
  }
}

void handle_driver_reset(void) {
  B_State drv_rst_btn_val = poll(&right_button);
  if (drv_rst_btn_val == HELD) {
    dash_request.driver_reset_pressed = true;
  } else {
    dash_request.driver_reset_pressed = false;
  }
}

void handle_ready_to_drive(void) {
  B_State rtd_btn_val = poll(&left_button);

  if (rtd_btn_val == HELD) {
    dash_request.rtd_pressed = true;
  } else {
    dash_request.rtd_pressed = false;
  }

  if (CANlib_critical_VCUState_Input.state == CANlib_critical_VCUState_state_RTD) {
    Pin_Write(RTD_GPIO_Port, RTD_Pin, true);
    reset_buzzer_state();
  } else {
    Pin_Write(RTD_GPIO_Port, RTD_Pin, false);
  }
}

void turn_dash_leds_off(void) {
  Pin_Write(BMS_FLT_GPIO_Port, BMS_FLT_Pin, false);
  Pin_Write(IMD_FLT_GPIO_Port, IMD_FLT_Pin, false);
  Pin_Write(TS_ACTIVE_GPIO_Port, TS_ACTIVE_Pin, false);
  Pin_Write(RTD_GPIO_Port, RTD_Pin, false);
}

systemticks_t gfxSystemTicks(void) {
  return HAL_GetTick();
}

systemticks_t gfxMillisecondsToTicks(delaytime_t ms) {
  return ms;
}

void _Error_Handler(char *file, int line) {
  printf("Error in file: %s, at line: %d \r\n", file, line);
  while (1) {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line) {}
#endif
