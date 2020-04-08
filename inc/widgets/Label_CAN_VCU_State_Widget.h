#ifndef __LABEL_CAN_VCU_STATE_WIDGET_H_
#define __LABEL_CAN_VCU_STATE_WIDGET_H_

#include "widget.h"

class Label_CAN_VCU_State_Widget: public Widget {
public:
    Label_CAN_VCU_State_Widget(GHandle ugfx_widget, CANlib_critical_VCUState_state_T* current_can_val);
    void update(bool force_update);
    void set_visibility(gBool visible);
protected:
    CANlib_critical_VCUState_state_T old_can_val;
    CANlib_critical_VCUState_state_T* current_can_val;
    void set_state(CANlib_critical_VCUState_state_T state);
};

#include "Label_CAN_VCU_State_Widget_impl.h"

#endif // __LABEL_CAN_FAULT_WIDGET_H_

