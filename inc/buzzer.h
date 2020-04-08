#ifndef __BUZZER__H__
#define __BUZZER__H__
#include "common.h"
#include "gpio.h"
#include "can.h"

#ifdef __cplusplus
extern "C" {
#endif
void arm_buzzer(void);
void buzz(void);
void reset_buzzer_state(void);
void handle_buzzer(void);

#ifdef __cplusplus
}
#endif
#endif // __BUZZER__H__

