#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "common.h"
#include "gpio.h"
#include "pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIN_HOLD_TIME 700

typedef enum {

    SHORT_PRESS,
    LONG_PRESS,
    HELD,
    UNPRESSED

} B_State;

typedef struct {

    uint32_t pin;
    GPIO_TypeDef * port;
    bool last_val;
    uint32_t last_time;
    uint32_t press_count;
    bool edge;

} Button;

void make_button(Button* b, GPIO_TypeDef * port, uint32_t pin);

B_State poll(Button* b);

extern Button far_left_button;
extern Button far_right_button;
extern Button wheel_1_button;
extern Button wheel_2_button;
extern Button wheel_3_button;
extern Button wheel_4_button;
extern Button wheel_5_button;
extern Button wheel_6_button;

#ifdef __cplusplus
}
#endif
#endif //_BUTTON_H_

