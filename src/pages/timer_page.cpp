#include "timer_page.h"

Timer_Page::Timer_Page(Widget* timer, Widget** lap_times, int num_lap_times, Page_Name page_name) {
    this->num_lap_times = num_lap_times;
    this->timer = timer;
    this->lap_times = new Lap_Time_Label[num_lap_times];
    for(int i = 0; i < num_lap_times; i++) {
        this->lap_times[i].widget = lap_times[i];
        this->lap_times[i].lap_time = 0;
    }
    this->lap_time = 0;
    this->start_time = 0;
    this->state = STOPPED;
    this->current_lap_num = 0;
    this->page_name = page_name;
}

void Timer_Page::update(bool force_update) {

    switch(this->state) {
        case STOPPED:
            if(force_update) {
                this->current_lap_num = 0;
                this->timer->set_ms(this->lap_time);
                for(int lap = 0; lap < this->num_lap_times; lap++) {
                    this->set_lap_time(lap, this->lap_times[lap].lap_time);
                }
            }
            break;

        case RUNNING:
            this->timer->set_ms(HAL_GetTick() - this->start_time);
            break;

        case RECORDING:
            this->set_lap_time(this->current_lap_num, this->lap_time);
            this->current_lap_num = (this->current_lap_num + 1) % (this->num_lap_times);
            this->state = STOPPED;
            break;
    }
}

void Timer_Page::set_lap_time(uint8_t lap_num, uint32_t lap_time) {
    if(lap_num < this->num_lap_times) {
        this->lap_times[lap_num].widget->set_ms(lap_time);
        this->lap_times[lap_num].lap_time = lap_time;
    }
}

void Timer_Page::set_visibility(gBool visible) {
    this->timer->set_visibility(visible);
    for(int i = 0; i < this->num_lap_times; i++) {
        this->lap_times[i].widget->set_visibility(visible);
    }
}

void Timer_Page::modify_page(B_State far_right_button, B_State far_left_button,
                        B_State wheel_1_button, B_State wheel_2_button, B_State wheel_3_button,
                        B_State wheel_4_button, B_State wheel_5_button, B_State wheel_6_button) {
    UNUSED(far_left_button);
    UNUSED(wheel_1_button);
    UNUSED(wheel_2_button);
    UNUSED(wheel_3_button);
    UNUSED(wheel_4_button);
    UNUSED(wheel_5_button);
    UNUSED(wheel_6_button);

    B_State lap_button = far_right_button;
    switch(this->state) {
        case STOPPED:
            if(lap_button == SHORT_PRESS || lap_button == LONG_PRESS) {
                this->state = RUNNING;
                this->start_time = HAL_GetTick();
            }
            break;

        case RUNNING:
            if(lap_button == SHORT_PRESS || lap_button == LONG_PRESS) {
                this->state = RECORDING;
                this->lap_time = HAL_GetTick() - this->start_time;
                this->state = RECORDING;
            }

            break;
        case RECORDING:
            break;
    }
}

