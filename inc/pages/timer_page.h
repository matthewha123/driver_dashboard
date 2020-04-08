#ifndef __TIMER_PAGE_H___
#define __TIMER_PAGE_H___
#include "page.h"
#include "display_timer.h"

typedef enum {
    STOPPED = 0,
    RUNNING,
    RECORDING,
} Timer_Page_State;

typedef struct {
    Widget* widget;
    uint32_t lap_time;
} Lap_Time_Label;

class Timer_Page : public Page {
public:
    Timer_Page(Widget* timer, Widget** lap_times, int num_lap_times, Page_Name page_name);
    void update(bool force_update);
    void set_visibility(gBool visible);
    void modify_page(B_State far_right_button, B_State far_left_button,
                        B_State wheel_1_button, B_State wheel_2_button, B_State wheel_3_button,
                        B_State wheel_4_button, B_State wheel_5_button, B_State wheel_6_button);
private:
    void set_lap_time(uint8_t lap_num, uint32_t lap_time);
    int num_lap_times;
    Widget* timer;
    Lap_Time_Label* lap_times;
    uint32_t lap_time;
    uint32_t start_time;
    Timer_Page_State state;
    uint8_t current_lap_num;
};

#endif // __TIMER_PAGE_H_

