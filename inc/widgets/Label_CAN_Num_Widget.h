
#ifndef __LABEL_CAN_NUM_WIDGET_H_
#define __LABEL_CAN_NUM_WIDGET_H_

#include "widget.h"

template <class T>
class Label_CAN_Num_Widget: public Widget {
public:
    Label_CAN_Num_Widget(GHandle ugfx_widget, T* current_can_val);
    void update(bool force_update);
    void set_visibility(gBool visible);
protected:
    T old_can_val;
    T* current_can_val;
    char num_buffer[20];
};

#include "Label_CAN_Num_Widget_impl.h"

#endif // __LABEL_CAN_NUM_WIDGET_H_

