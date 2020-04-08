#ifndef __LABEL_CAN_FAULT_WIDGET_H_
#define __LABEL_CAN_FAULT_WIDGET_H_

#include "widget.h"

template <class T>
class Label_CAN_Fault_Widget: public Widget {
public:
    Label_CAN_Fault_Widget(GHandle ugfx_widget, T* current_can_val);
    void update(bool force_update);
    void set_visibility(gBool visible);
protected:
    T old_can_val;
    T* current_can_val;
    void set_error(uint8_t error_state);
};

#include "Label_CAN_Fault_Widget_impl.h"

#endif // __LABEL_CAN_FAULT_WIDGET_H_

