#include "display_timer.h"

uint16_t get_minutes(uint32_t ms) {
    return ms / (60000);
}
void get_display_timer_vals(uint32_t ms, uint16_t * vals) {
    vals[0] = get_minutes(ms);
    vals[1] = (ms - (vals[0] * 60000)) / 1000;
    vals[2] = (ms - (vals[0] * 60000) - (vals[1] * 1000));
}
