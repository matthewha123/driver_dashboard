#include "button.h"

void make_button(Button* b, GPIO_TypeDef * port, uint32_t pin) {

    b->port = port;
    b->pin = pin;
    b->last_val = true;
    b->edge = false;
    b->press_count = 0;
}

B_State poll(Button* b) {

    bool value = Pin_Read(b->port, b->pin);

    if(!value && !b->edge) {
        b->edge = true;
        b->last_time = HAL_GetTick();
    }
    if (value && b->edge) {
        b->press_count = b->press_count + 1;
        b->edge = false;

        if(HAL_GetTick() - b->last_time < MIN_HOLD_TIME) {
            return SHORT_PRESS;
        } 
        else {
            return  LONG_PRESS;
        }
    }
    if(b->edge) {
        return HELD;
    } else {
        return UNPRESSED;
    }

}

