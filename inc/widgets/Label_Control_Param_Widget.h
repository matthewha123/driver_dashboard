#ifndef __LABEL_CONTROL_PARAM_WIDGET_H_
#define __LABEL_CONTROL_PARAM_WIDGET_H_

#include "widget.h"

typedef enum {
    HOLDING_VALUE,
    SHORT_VALUE_INC_HOLD,
    LONG_VALUE_INC_HOLD,
    SHORT_VALUE_DEC_HOLD,
    LONG_VALUE_DEC_HOLD,
} Control_Param_State;

template <class T>
class Label_Control_Param_Widget: public Widget {
public:
    Label_Control_Param_Widget(GHandle ugfx_widget, const char* control_param_name, T* values, uint16_t num_values, T* current_val, uint8_t label_tab);
    void update(bool force_update);
    void set_visibility(gBool visible);
    void set_selected(gBool is_selected);
    void modify_param(B_State inc_button);
protected:
    T old_val;
    T* current_val;
    char num_buffer[20];
    char control_param_name[25];
    Control_Param_State state;
    T* values;
    uint16_t num_values;
    uint8_t label_tab;
    uint16_t curr_val_idx;

    void set_control_param_name(const char* control_param_name);
};

#include "Label_Control_Param_Widget_impl.h"

#endif // __LABEL_CAN_NUM_WIDGET_H_

