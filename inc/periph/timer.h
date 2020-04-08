#ifndef __TIMER___H_
#define __TIMER___H_

#include "common.h"
#include "gpio.h"
#include "display_pages.h"

#ifdef __cplusplus
extern "C" {
#endif

void timers_init(void);
void timer_2_interrupt(void);

#ifdef __cplusplus
}
#endif

#endif // __TIMER___H_

