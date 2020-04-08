#ifndef __PAGE_WIDGETS_H_
#define __PAGE_WIDGETS_H_

#ifdef TUNING_POW_LIM
#ifdef TUNING_RG_TL_VL
#error "Both power limiting and regen/temp limiting/voltage limiting pages can't be on at once!"
#endif // TUNING_RG_TL_VL
#endif // TUNING_POW_LIM

#ifdef TUNING_POW_LIM
#ifdef TUNING_LC
#error "Both power limiting and launch control pages can't be on at once!"
#endif // TUNING_LC
#endif // TUNING_POW_LIM

#ifdef TUNING_LC
#ifdef TUNING_RG_TL_VL
#error "Both launch control and regen/temp limiting/voltage limiting pages can't be on at once!"
#endif // TUNING_RG_TL_VL
#endif // TUNING_LC

#ifdef TUNING_POW_LIM
#ifdef TUNING_LC
#error "Both power limiting and launch control pages can't be on at once!"
#endif  // TUNING_LC
#endif  // TUNING_POW_LIM

#ifdef TUNING_LC
#ifdef TUNING_RG_TL_VL
#error "Both launch control and regen/temp limiting/voltage limiting pages can't be on at once!"
#endif  // TUNING_RG_TL_VL
#endif  // TUNING_LC

// Roman General Mode Main Page Widgets
Widget *min_cell_temp_label_widget;
Widget *max_cell_temp_label_widget;
Widget *vcu_state_label_widget;
Widget *dash_mode_label_widget;
Widget *hv_bus_volt_label_widget;
Widget *min_cell_volt_label_widget;
Widget *max_cell_volt_label_widget;
Widget *active_fault_header_label_widget;
Widget *recent_faults_widget;
Widget *temp_lim_enable_label_widget;
Widget *volt_lim_enable_label_widget;
Widget *regen_enable_label_widget;
Widget *epl_enable_label_widget;
Widget *precool_label_widget;
Widget *trq_factor_label_widget;

GHandle temp_lim_enable_label;
GHandle volt_lim_enable_label;
GHandle regen_enable_label;
GHandle epl_enable_label;
GHandle precool_label;
GHandle trq_factor_label;
GHandle min_cell_volt_label, max_cell_temp_label;
GHandle vcu_state_label, hv_bus_volt_label;
GHandle active_fault_header_label, fault_1_label, fault_2_label, fault_3_label;
const uint8_t num_roman_general_main_widgets = 11;

// Roman General Mode Battery Page Widgets
GHandle pack_volt_label, battery_min_cell_volt_label, battery_max_cell_volt_label,
    battery_min_cell_temp_label, battery_max_cell_temp_label, battery_avg_cell_temp_label;
Widget *pack_volt_label_widget;
Widget *battery_min_cell_temp_label_widget;
Widget *battery_max_cell_temp_label_widget;
Widget *battery_min_cell_volt_label_widget;
Widget *battery_max_cell_volt_label_widget;
Widget *battery_avg_cell_temp_label_widget;
const uint8_t num_roman_general_battery_widgets = 6;

GHandle imd_fault_label, bms_fault_label, contactor_mismatch_fault_label, bspd_fault_label,
    throttle_implausibility_fault_label;
GHandle sensor_implausibility_fault_label, brake_throttle_conflict_fault_label, tsms_fault_label,
    shutdown_loop_latch_fault_label;
GHandle hvlv_interlock_fault_label, heartbeat_fault_label, leak_detect_fault_label;
Widget *imd_fault_label_widget;
Widget *bms_fault_label_widget;
Widget *contactor_mismatch_fault_label_widget;
Widget *bspd_fault_label_widget;
Widget *throttle_implausibility_fault_label_widget;
Widget *sensor_implausibility_fault_label_widget;
Widget *brake_throttle_conflict_fault_label_widget;
Widget *tsms_fault_label_widget;
Widget *shutdown_loop_latch_fault_label_widget;
Widget *hvlv_interlock_fault_label_widget;
Widget *heartbeat_fault_label_widget;
Widget *leak_detect_fault_label_widget;
const uint8_t num_roman_general_fault_widgets = 12;

GHandle mc_temps_a_label, mc_temps_b_label, mc_temps_c_label, mc_temps_gd_label, mc_temps_bl_label,
    mc_temps_br_label;
GHandle lside_cmd_label, lside_act_label, hside_cmd_label, hside_act_label;
GHandle vcu_sd_not_pres_label, vcu_sd_err_label, bms_sd_not_pres_label, bms_sd_err_label;
Widget *mc_temps_a_label_widget;
Widget *mc_temps_b_label_widget;
Widget *mc_temps_c_label_widget;
Widget *mc_temps_gd_label_widget;
// Widget* mc_temps_bl_label_widget; Widget* mc_temps_br_label_widget;
Widget *lside_cmd_label_widget;
Widget *lside_act_label_widget;
Widget *hside_cmd_label_widget;
Widget *hside_act_label_widget;
Widget *vcu_sd_not_pres_label_widget;
Widget *vcu_sd_err_label_widget;
Widget *bms_sd_not_pres_label_widget;
Widget *bms_sd_err_label_widget;
const uint8_t num_motor_page_widgets = 12;

#ifdef TUNING_POW_LIM
GHandle pow_lim_label;
GHandle electrical_P_label;
GHandle electrical_I_label;
GHandle using_vq_lim_label;
GHandle anti_windup_label;
GHandle rear_efficiency_label;
GHandle mpl_enable_label;

// GHandle front_efficiency_label;
Widget *pow_lim_label_widget;
Widget *electrical_P_label_widget;
Widget *electrical_I_label_widget;
Widget *using_vq_lim_label_widget;
Widget *anti_windup_label_widget;
Widget *rear_efficiency_label_widget;
Widget* mpl_enable_label_widget;

const uint8_t num_pow_lim_controls_page_widgets = 7;
#endif // TUNING_POW_LIM

#ifdef TUNING_RG_TL_VL
GHandle regen_bias_label;
GHandle volt_lim_min_gain_label;
GHandle temp_lim_min_gain_label;
GHandle volt_lim_cutoff_voltage_label;
GHandle temp_lim_cutoff_temp_label;

Widget *front_efficiency_label_widget;
Widget *regen_bias_label_widget;
Widget *volt_lim_min_gain_label_widget;
Widget *temp_lim_min_gain_label_widget;
Widget *volt_lim_cutoff_voltage_label_widget;
Widget *temp_lim_cutoff_temp_label_widget;

const uint8_t num_second_controls_page_widgets = 5;
#endif // TUNING_RG_TL_VL

#ifdef TUNING_LC
// Launch Control Page widgets
GHandle lc_title_label;
GHandle lc_fn_src_label;
GHandle lc_ws_th_label;
GHandle lc_account_for_kappa_label;
GHandle lc_lambda_label;
GHandle lc_enabled_label;
GHandle lc_min_kappa_label;


// Widget *lc_title_widget;
Widget *lc_fn_src_widget;
Widget *lc_ws_th_widget;
Widget *lc_account_for_kappa_widget;
Widget *lc_lambda_widget;
Widget *lc_enabled_widget;
Widget *lc_min_kappa_widget;
const uint8_t num_lc_widgets = 6;
#endif // TUNING_LC

// Timer page widgets
GHandle main_timer_label, lap1_timer_label, lap2_timer_label, lap3_timer_label, lap4_timer_label,
    lap5_timer_label, lap6_timer_label;
Widget *main_timer_widget;
Widget *lap1_timer_widget;
Widget *lap2_timer_widget;
Widget *lap3_timer_widget;
Widget *lap4_timer_widget;
Widget *lap5_timer_widget;
Widget *lap6_timer_widget;
const uint8_t num_timer_page_widgets = 7;

#endif // __PAGE_WIDGETS_H_
