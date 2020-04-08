#include "Label_Time_Widget.h"
#include <stdio.h>

Label_Time_Widget::Label_Time_Widget(GHandle ugfx_widget) {
    this->ugfx_widget = ugfx_widget;
    this->set_ms(0);
}

void Label_Time_Widget::update(bool force_update) {
    if(force_update) {
        this->set_ms(this->ms);
    }
}

void Label_Time_Widget::set_ms(uint32_t ms) {
    this->ms = ms;
    get_display_timer_vals(ms, this->display_timer_values);
    sprintf(this->time_string_buffer, "%d:%d:%d",this->display_timer_values[0], display_timer_values[1], display_timer_values[2]);
    gwinSetText(this->ugfx_widget, this->time_string_buffer, TRUE);
}

void Label_Time_Widget::set_visibility(gBool visible) {
    gwinSetVisible(this->ugfx_widget, visible);
}

