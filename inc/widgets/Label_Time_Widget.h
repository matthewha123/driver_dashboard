#ifndef __LABEL_TIME_WIDGET_H_
#define __LABEL_TIME_WIDGET_H_
#include "widget.h"
#include "display_timer.h"
class Label_Time_Widget : public Widget {
public:
    Label_Time_Widget(GHandle ugfx_widget);
    void set_ms(uint32_t ms);
    void set_visibility(gBool visible);
    void update(bool force_update);
private:
    char time_string_buffer[25];
    uint16_t display_timer_values[3];
    uint32_t ms;
};
#endif // __LABEL_TIME_WIDGET_H_

