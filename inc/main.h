#ifndef __MAIN_H_
#define __MAIN_H_

#include "common.h"
#include "gfx.h"
#include "gpio.h"
#include "uart.h"
#include "pins.h"
#include "TLC5955.h"
#include "micro_dwt_delay.h"
#include "can.h"
#include "clock.h"
#include "Dot_Matrix_NHD_US25664.h"
#include "button.h"
#include "display_pages.h"
#include "canlib_dash.h"
#include "timer.h"
#include "buzzer.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void debug_blink(void);
void start_dot_matrix(void);
void cycle_single_led(void);
void initialize_hardware(void);
void mock_paging(void);
void handle_fault_leds(void);
void handle_ready_to_drive(void);
void handle_driver_reset(void);
void turn_dash_leds_off(void);

#ifdef __cplusplus
}
#endif
#endif

