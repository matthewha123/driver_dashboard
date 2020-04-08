#ifndef __RECENT_FAULTS_WIDGET_H_
#define __RECENT_FAULTS_WIDGET_H_
#include "widget.h"
#include "can.h"

typedef enum {
    BMS = 0,
    IMD,
    CONTACTOR_MISMATCH,
    BSPD,
    THROTTLE_IMPLAUSIBILITY,
    SENSOR_IMPLAUSIBILITY,
    BRAKE_THROTTLE_CONFLICT,
    TSMS,
    SHUTDOWN_LOOP_LATCH,
    HVLV_INTERLOCK,
    HEARTBEAT,
    LEAK_DETECT,
    NO_FAULT,
} Fault_Name;

typedef struct {
    Fault_Name name;
    GHandle ugfx_widget;
} Fault_Label;

class Recent_Faults_Widget : public Widget {
public:
    Recent_Faults_Widget(GHandle* fault_labels, uint8_t num_fault_labels, CANlib_critical_VCUDashInfo_T* faults);
    void update(bool force_update);
    void set_visibility(gBool visible);
private:
    void clear_fault(uint8_t fault_idx);
    void update_fault_label(Fault_Name name, uint8_t idx, bool force_update);
    void update_fault_label_inc(Fault_Name name, uint8_t error_value, bool force_update);
    void clear_old_fault_labels(bool force_update);
    CANlib_critical_VCUDashInfo_T* faults;
    Fault_Label* fault_labels;
    uint8_t num_fault_labels;
    uint8_t current_fault_idx;
};
#endif // __RECENT_FAULTS_WIDGET_H_

