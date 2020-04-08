#ifndef __DISPLAY_TIMER_H__
#define __DISPLAY_TIMER_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t get_minutes(uint32_t ms);
uint16_t get_seconds(uint32_t ms);
uint32_t get_ms(uint32_t ms);

void get_display_timer_vals(uint32_t ms, uint16_t * vals);

#ifdef __cplusplus
}
#endif

#endif // __DISPLAY_TIMER_H__

