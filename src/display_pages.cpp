#include "display_pages.h"
#include "Label_CAN_Fault_Widget.h"
#include "Label_CAN_Num_Widget.h"
#include "Label_CAN_VCU_State_Widget.h"
#include "Label_Control_Param_Widget.h"
#include "Label_Static_Widget.h"
#include "Label_Time_Widget.h"
#include "Mode.h"
#include "Recent_Faults_Widget.h"
#include "controls_page.h"
#include "controls_values.h"
#include "page.h"
#include "page_general.h"
#include "page_widgets.h"
#include "roman_general_page.h"
#include "timer_page.h"

#if defined TUNING_RG_TL_VL || defined TUNING_POW_LIM || defined TUNING_LC
#define NUM_PAGES 5
#else
#define NUM_PAGES 4
#endif

Page *rom_gen_main_page;
Page *rom_gen_battery_page;
Page *faults_page;
Page *motor_page;
Page *pow_lim_controls_page;
Page *second_controls_page;
Page *launch_control_page;
Page *timer_page;

Mode *roman_general_mode;
Mode_Name current_mode;

Page_Name current_roman_general_page;
Page_Name new_roman_general_page;

gFont fixed5x8;
gFont ui2;

GListener gui_listener;
GEvent *pe;

static uint32_t last_gui_update = 0;

char numBuffer[20];
uint16_t display_timer_vals[3];

const uint8_t MAX_CELL_VOLT_TAB_WIDTH = 62;
const uint8_t MIN_CELL_VOLT_TAB_WIDTH = 62;
const uint8_t MAX_CELL_TEMP_TAB_WIDTH = 62;
const uint8_t MIN_CELL_TEMP_TAB_WIDTH = 62;
const uint8_t VCU_STATE_TAB_WIDTH     = 75;
const uint8_t DASH_MODE_TAB_WIDTH     = 75;
const uint8_t ACTIVE_FAULTS_TAB_WIDTH = 75;

const uint8_t FAULT_TAB_WIDTH = 62;

const uint8_t GUI_EVENT_TIMEOUT    = 50;
const uint8_t GUI_UPDATE_PERIOD    = 50;
const uint8_t NUM_FAULTS_DISPLAYED = 5;

bool finished_gui_init = false;

static void init_roman_general_main_page(void) {

  create_label_with_attr(&min_cell_volt_label, "MIN CV", 85, 15, 0, 0, 50);
  create_label_with_attr(&max_cell_temp_label, "MAX CT", 85, 15, 0, 15, 50);
  create_label_with_attr(&hv_bus_volt_label, "HV_V", 85, 15, 0, 30, 40);
  create_label(&vcu_state_label, 85, 15, 0, 45);

  create_label_with_attr(&temp_lim_enable_label, "TL EN", 85, 15, 85, 0, 70);
  create_label_with_attr(&volt_lim_enable_label, "VL EN", 85, 15, 85, 15, 70);
  create_label_with_attr(&regen_enable_label, "RG EN", 85, 15, 85, 30, 70);
  create_label_with_attr(&precool_label, "PRECOOL", 85, 15, 85, 45, 70);

  create_label_with_attr(&epl_enable_label, "EPL EN", 120, 15, 170, 0, 50);
  create_label_with_attr(&trq_factor_label, "TQ FAC", 120, 15, 170, 15, 60);
  create_label(&fault_1_label, 120, 15, 170, 30);
  create_label(&fault_2_label, 120, 15, 170, 45);

  vcu_state_label_widget     = new Label_CAN_VCU_State_Widget(vcu_state_label, &CANlib_critical_VCUState_Input.state);
  max_cell_temp_label_widget = new Label_CAN_Num_Widget<int16_t>(max_cell_temp_label, &CANlib_critical_CellTemperatureRange_Input.max);
  min_cell_volt_label_widget = new Label_CAN_Num_Widget<uint16_t>(min_cell_volt_label, &CANlib_critical_CellVoltageRange_Input.min);
  hv_bus_volt_label_widget   = new Label_CAN_Num_Widget<int32_t>(hv_bus_volt_label, &CANlib_critical_CurrentSensor_Voltage1_Input.result);

  temp_lim_enable_label_widget = new Label_Control_Param_Widget<bool>(temp_lim_enable_label, "TL EN", temp_lim_enable_vals, sizeof(temp_lim_enable_vals) / sizeof(temp_lim_enable_vals[0]), &controls_settings2.temp_lim_enable, 70);
  volt_lim_enable_label_widget = new Label_Control_Param_Widget<bool>(volt_lim_enable_label, "VL EN", volt_lim_enable_vals, sizeof(volt_lim_enable_vals) / sizeof(volt_lim_enable_vals[0]), &controls_settings2.volt_lim_enable, 70);
  regen_enable_label_widget    = new Label_Control_Param_Widget<bool>(regen_enable_label, "RG EN", regen_enable_vals, sizeof(regen_enable_vals) / sizeof(regen_enable_vals[0]), &controls_settings2.regen_enable, 70);
  precool_label_widget         = new Label_Control_Param_Widget<bool>(precool_label, "PRECOOL", precool_vals, 2, &controls_settings2.precooling_enable, 70);

  epl_enable_label_widget = new Label_Control_Param_Widget<bool>(epl_enable_label, "EPL EN", epl_enable_vals, 2, &controls_settings2.epl_enable, 70);
  trq_factor_label_widget = new Label_Control_Param_Widget<uint8_t>(trq_factor_label, "TQ FAC", torque_factor_vals, sizeof(torque_factor_vals) / sizeof(torque_factor_vals[0]), &controls_settings2.torque_factor, 60);

  GHandle recent_faults_labels[2] = {fault_1_label, fault_2_label};

  recent_faults_widget = new Recent_Faults_Widget(recent_faults_labels, 2, &CANlib_critical_VCUDashInfo_Input);

  Widget *rom_gen_main_widgets[num_roman_general_main_widgets] = {vcu_state_label_widget, max_cell_temp_label_widget, min_cell_volt_label_widget, hv_bus_volt_label_widget, temp_lim_enable_label_widget, volt_lim_enable_label_widget, regen_enable_label_widget, precool_label_widget, epl_enable_label_widget, trq_factor_label_widget, recent_faults_widget};
  rom_gen_main_page                                            = new Controls_Page(rom_gen_main_widgets, num_roman_general_main_widgets, ROMAN_GENERAL_MAIN, 4, 10, num_roman_general_main_widgets);
}

// static void init_timer_page(void) {
//     create_label(&main_timer_label, 200, 25, 0,0);
//     gwinSetFont(main_timer_label, ui2);

//     create_label(&lap1_timer_label, 83, 15, 0, 30);
//     create_label(&lap2_timer_label, 83, 15, 0, 45);
//     create_label(&lap3_timer_label, 83, 15, 85, 30);
//     create_label(&lap4_timer_label, 83, 15, 85, 45);
//     create_label(&lap5_timer_label, 83, 15, 171, 30);
//     create_label(&lap6_timer_label, 83, 15, 171, 45);

//     main_timer_widget = new Label_Time_Widget(main_timer_label);
//     lap1_timer_widget = new Label_Time_Widget(lap1_timer_label);
//     lap2_timer_widget = new Label_Time_Widget(lap2_timer_label);
//     lap3_timer_widget = new Label_Time_Widget(lap3_timer_label);
//     lap4_timer_widget = new Label_Time_Widget(lap4_timer_label);
//     lap5_timer_widget = new Label_Time_Widget(lap5_timer_label);
//     lap6_timer_widget = new Label_Time_Widget(lap6_timer_label);

//     Widget* lap_times[num_timer_page_widgets - 1] = {lap1_timer_widget, lap2_timer_widget, lap3_timer_widget, lap4_timer_widget, lap5_timer_widget, lap6_timer_widget};
//     timer_page = new Timer_Page(main_timer_widget, lap_times, num_timer_page_widgets - 1, TIMER);

// }

#ifdef TUNING_POW_LIM
static void init_pow_lim_controls_page(void) {
  create_label_with_attr(&pow_lim_label, "POW LIM", 120, 15, 0, 0, 70);
  create_label_with_attr(&electrical_P_label, "ELEC P", 120, 15, 0, 15, 70);
  create_label_with_attr(&electrical_I_label, "ELEC I", 120, 15, 0, 30, 70);
  create_label_with_attr(&using_vq_lim_label, "VQ LIM", 120, 15, 0, 45, 70);
  create_label_with_attr(&anti_windup_label, "ANTIWIND", 120, 15, 122, 0, 70);
  create_label_with_attr(&rear_efficiency_label, "REAR EFF", 120, 15, 122, 15, 70);
  create_label_with_attr(&mpl_enable_label, "MPL EN", 120, 15, 122, 30, 70);

  pow_lim_label_widget         = new Label_Control_Param_Widget<uint16_t>(pow_lim_label, "POW LIM", power_lim_vals, sizeof(power_lim_vals) / sizeof(power_lim_vals[0]), &controls_settings1.power_lim, 70);
  electrical_P_label_widget    = new Label_Control_Param_Widget<uint8_t>(electrical_P_label, "ELEC P", electrical_P_vals, sizeof(electrical_P_vals) / sizeof(electrical_P_vals[0]), &controls_settings1.electrical_P, 70);
  electrical_I_label_widget    = new Label_Control_Param_Widget<uint8_t>(electrical_I_label, "ELEC I", electrical_I_vals, sizeof(electrical_I_vals) / sizeof(electrical_I_vals[0]), &controls_settings1.electrical_I, 70);
  using_vq_lim_label_widget    = new Label_Control_Param_Widget<bool>(using_vq_lim_label, "VQ LIM", using_vq_lim_vals, 2, &controls_settings2.using_vq_lim, 70);
  anti_windup_label_widget     = new Label_Control_Param_Widget<uint16_t>(anti_windup_label, "ANTIWIND", anti_windup_vals, sizeof(anti_windup_vals) / sizeof(anti_windup_vals[0]), &controls_settings1.anti_windup, 70);
  rear_efficiency_label_widget = new Label_Control_Param_Widget<uint8_t>(rear_efficiency_label, "REAR EFF", rear_efficiency_vals, sizeof(rear_efficiency_vals) / sizeof(rear_efficiency_vals[0]), &controls_settings1.rear_efficiency, 70);
  mpl_enable_label_widget      = new Label_Control_Param_Widget<bool>(mpl_enable_label, "MPL EN", mpl_enable_vals, 2, &controls_settings2.mpl_enable, 70);


  Widget *pow_lim_controls_page_widgets[num_pow_lim_controls_page_widgets] = {
      pow_lim_label_widget,
      electrical_P_label_widget,
      electrical_I_label_widget,
      // epl_enable_label_widget,
      // mpl_enable_label_widget,
      using_vq_lim_label_widget,
      anti_windup_label_widget,
      rear_efficiency_label_widget,
      mpl_enable_label_widget,
  };
  pow_lim_controls_page = new Controls_Page(pow_lim_controls_page_widgets, num_pow_lim_controls_page_widgets, POW_LIM_CONTROLS);
}
#endif // TUNING_POW_LIM

#ifdef TUNING_RG_TL_VL
static void init_second_controls_page(void) {
  // create_label_with_attr(&front_efficiency_label,        "FRONT EFF",  120, 15, 0, 0,  70);
  create_label_with_attr(&temp_lim_min_gain_label, "TL MIN G", 120, 15, 0, 0, 70);
  create_label_with_attr(&temp_lim_cutoff_temp_label, "TL CUT", 120, 15, 0, 15, 70);
  create_label_with_attr(&volt_lim_min_gain_label, "VL MIN G", 120, 15, 0, 30, 70);
  create_label_with_attr(&volt_lim_cutoff_voltage_label, "VL CUT", 120, 15, 0, 45, 70);
  create_label_with_attr(&regen_bias_label, "RG BIAS", 120, 15, 122, 0, 70);

  // front_efficiency_label_widget        = new Label_Control_Param_Widget<uint8_t>(front_efficiency_label,        "FRONT EFF",   front_efficiency_vals,
  // sizeof(front_efficiency_vals)/sizeof(front_efficiency_vals[0]),               &controls_settings1.front_efficiency,        70);
  temp_lim_min_gain_label_widget       = new Label_Control_Param_Widget<uint8_t>(temp_lim_min_gain_label, "TL MIN G", temp_lim_min_gain_vals, sizeof(temp_lim_min_gain_vals) / sizeof(temp_lim_min_gain_vals[0]), &controls_settings2.temp_lim_min_gain, 70);
  temp_lim_cutoff_temp_label_widget    = new Label_Control_Param_Widget<uint8_t>(temp_lim_cutoff_temp_label, "TL CUT", temp_lim_cutoff_temp_vals, sizeof(temp_lim_cutoff_temp_vals) / sizeof(temp_lim_cutoff_temp_vals[0]), &controls_settings2.temp_lim_cutoff_temp, 70);
  volt_lim_min_gain_label_widget       = new Label_Control_Param_Widget<uint8_t>(volt_lim_min_gain_label, "VL MIN G", volt_lim_min_gain_vals, sizeof(volt_lim_min_gain_vals) / sizeof(volt_lim_min_gain_vals[0]), &controls_settings2.volt_lim_min_gain, 70);
  volt_lim_cutoff_voltage_label_widget = new Label_Control_Param_Widget<uint8_t>(volt_lim_cutoff_voltage_label, "VL CUT", volt_lim_cutoff_voltage_vals, sizeof(volt_lim_cutoff_voltage_vals) / sizeof(volt_lim_cutoff_voltage_vals[0]), &controls_settings2.volt_lim_cutoff_voltage, 70);
  regen_bias_label_widget              = new Label_Control_Param_Widget<uint8_t>(regen_bias_label, "RG BIAS", regen_bias_vals, sizeof(regen_bias_vals) / sizeof(regen_bias_vals[0]), &controls_settings2.regen_bias, 70);

  Widget *second_control_page_widgets[num_second_controls_page_widgets] = {
      // front_efficiency_label_widget,
      // temp_lim_enable_label_widget,
      temp_lim_min_gain_label_widget,
      temp_lim_cutoff_temp_label_widget,
      // volt_lim_enable_label_widget,
      volt_lim_min_gain_label_widget,
      volt_lim_cutoff_voltage_label_widget,
      // regen_enable_label_widget,
      regen_bias_label_widget,
  };
  second_controls_page = new Controls_Page(second_control_page_widgets, num_second_controls_page_widgets, SECOND_CONTROLS);
}
#endif // TUNING_RG_TL_VL

#ifdef TUNING_LC
static void init_launch_control_page(void) {
  // create_label_with_attr(&lc_title_label, "LC SETTINGS", 200, 15, 0, 0, 0);
  create_label_with_attr(&lc_fn_src_label, "LDCLS FN?",              120, 15, 0,   15, 70);
  create_label_with_attr(&lc_ws_th_label, "WS TH",                   120, 15, 0,   30, 70);
  create_label_with_attr(&lc_account_for_kappa_label, "USE K",       120, 15, 0,   45, 70);
  create_label_with_attr(&lc_lambda_label, "LAMBDA",                 200, 15, 122, 15, 70);
  create_label_with_attr(&lc_enabled_label, "ENABLED",               200, 15, 122, 30, 70);
  create_label_with_attr(&lc_min_kappa_label, "MIN KAPPA",           200, 15, 122, 45, 70);

  // lc_title_widget = new Label_Static_Widget(lc_title_label, "LC SETTINGS");
  lc_fn_src_widget = new Label_Control_Param_Widget<bool>(lc_fn_src_label, "LDCLS FN?", lc_fn_src_vals, sizeof(lc_fn_src_vals) / sizeof(lc_fn_src_vals[0]), &launch_control_settings.load_cell_fn, 70);
  lc_ws_th_widget = new Label_Control_Param_Widget<uint8_t>(lc_ws_th_label, "WS TH", lc_ws_th_vals, sizeof(lc_ws_th_vals) / sizeof(lc_ws_th_vals[0]), &launch_control_settings.ws_th, 70);
  lc_account_for_kappa_widget = new Label_Control_Param_Widget<bool>(lc_account_for_kappa_label, "USE K", lc_account_for_kappa_vals, sizeof(lc_account_for_kappa_vals) / sizeof(lc_account_for_kappa_vals[0]), &launch_control_settings.account_for_kappa, 70);
  lc_lambda_widget = new Label_Control_Param_Widget<uint8_t>(lc_lambda_label, "LAMBDA", lc_lambda_vals, sizeof(lc_lambda_vals) / sizeof(lc_lambda_vals[0]), &launch_control_settings.lambda, 70);
  lc_enabled_widget  = new Label_Control_Param_Widget<bool>(lc_enabled_label, "ENABLED", lc_enabled_vals, sizeof(lc_enabled_vals) / sizeof(lc_enabled_vals[0]), &launch_control_settings.requested_next_driving_autom, 70);
  lc_min_kappa_widget = new Label_Control_Param_Widget<uint8_t>(lc_min_kappa_label, "MIN KAPPA", lc_min_kappa_vals, sizeof(lc_min_kappa_vals) / sizeof(lc_min_kappa_vals[0]), &launch_control_settings.min_kappa, 70);

  Widget *launch_control_page_widgets[num_lc_widgets] = {/*lc_title_widget,*/ lc_fn_src_widget, lc_ws_th_widget, lc_account_for_kappa_widget, lc_lambda_widget, lc_enabled_widget, lc_min_kappa_widget};

  launch_control_page = new Controls_Page(launch_control_page_widgets, num_lc_widgets, LAUNCH_CONTROL);
}
#endif // TUNING_LC

static void init_faults_page(void) {
  create_label_with_attr(&imd_fault_label, "IMD", 83, 15, 0, 0, FAULT_TAB_WIDTH);
  create_label_with_attr(&bms_fault_label, "BMS", 83, 15, 0, 15, FAULT_TAB_WIDTH);
  create_label_with_attr(&contactor_mismatch_fault_label, "CNTCR", 83, 15, 0, 30, FAULT_TAB_WIDTH);
  create_label_with_attr(&bspd_fault_label, "BSPD", 83, 15, 0, 45, FAULT_TAB_WIDTH);
  create_label_with_attr(&throttle_implausibility_fault_label, "THROT", 83, 15, 85, 0, FAULT_TAB_WIDTH);
  create_label_with_attr(&sensor_implausibility_fault_label, "SENSOR", 83, 15, 85, 15, FAULT_TAB_WIDTH);
  create_label_with_attr(&brake_throttle_conflict_fault_label, "BRAKE", 83, 15, 85, 30, FAULT_TAB_WIDTH);
  create_label_with_attr(&tsms_fault_label, "TSMS", 83, 15, 85, 45, FAULT_TAB_WIDTH);
  create_label_with_attr(&shutdown_loop_latch_fault_label, "SHUT_D", 83, 15, 170, 0, FAULT_TAB_WIDTH);
  create_label_with_attr(&hvlv_interlock_fault_label, "HVLV", 83, 15, 170, 15, FAULT_TAB_WIDTH);
  create_label_with_attr(&heartbeat_fault_label, "HB", 83, 15, 170, 30, FAULT_TAB_WIDTH);
  create_label_with_attr(&leak_detect_fault_label, "LEAK DETECT", 83, 15, 170, 45, FAULT_TAB_WIDTH);

  imd_fault_label_widget                     = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_imd_T>(imd_fault_label, &CANlib_critical_VCUDashInfo_Input.imd);
  bms_fault_label_widget                     = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_bms_T>(bms_fault_label, &CANlib_critical_VCUDashInfo_Input.bms);
  contactor_mismatch_fault_label_widget      = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_contactor_mismatch_T>(contactor_mismatch_fault_label, &CANlib_critical_VCUDashInfo_Input.contactor_mismatch);
  bspd_fault_label_widget                    = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_bspd_T>(bspd_fault_label, &CANlib_critical_VCUDashInfo_Input.bspd);
  throttle_implausibility_fault_label_widget = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_throttle_implausibility_T>(throttle_implausibility_fault_label, &CANlib_critical_VCUDashInfo_Input.throttle_implausibility);
  sensor_implausibility_fault_label_widget   = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_sensor_implausibility_T>(sensor_implausibility_fault_label, &CANlib_critical_VCUDashInfo_Input.sensor_implausibility);
  brake_throttle_conflict_fault_label_widget = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_brake_throttle_conflict_T>(brake_throttle_conflict_fault_label, &CANlib_critical_VCUDashInfo_Input.brake_throttle_conflict);
  tsms_fault_label_widget                    = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_tsms_T>(tsms_fault_label, &CANlib_critical_VCUDashInfo_Input.tsms);
  shutdown_loop_latch_fault_label_widget     = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_shutdown_loop_latch_T>(shutdown_loop_latch_fault_label, &CANlib_critical_VCUDashInfo_Input.shutdown_loop_latch);
  hvlv_interlock_fault_label_widget          = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_hvlv_interlock_T>(hvlv_interlock_fault_label, &CANlib_critical_VCUDashInfo_Input.hvlv_interlock);
  heartbeat_fault_label_widget               = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_heartbeat_T>(heartbeat_fault_label, &CANlib_critical_VCUDashInfo_Input.heartbeat);
  leak_detect_fault_label_widget             = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_leak_detect_T>(leak_detect_fault_label, &CANlib_critical_VCUDashInfo_Input.leak_detect);

  Widget *fault_page_widgets[num_roman_general_fault_widgets] = {imd_fault_label_widget, bms_fault_label_widget, contactor_mismatch_fault_label_widget, bspd_fault_label_widget, throttle_implausibility_fault_label_widget, sensor_implausibility_fault_label_widget, brake_throttle_conflict_fault_label_widget, tsms_fault_label_widget, shutdown_loop_latch_fault_label_widget, hvlv_interlock_fault_label_widget, heartbeat_fault_label_widget, leak_detect_fault_label_widget};
  faults_page                                                 = new Page_General(fault_page_widgets, num_roman_general_fault_widgets, FAULTS);
}

static void init_battery_page(void) {
  create_label_with_attr(&pack_volt_label, "PACK_V", 125, 15, 0, 0, FAULT_TAB_WIDTH);
  create_label_with_attr(&battery_max_cell_volt_label, "MAX CV", 125, 15, 0, 15, FAULT_TAB_WIDTH);
  create_label_with_attr(&battery_min_cell_volt_label, "MIN CV", 125, 15, 0, 30, FAULT_TAB_WIDTH);
  create_label_with_attr(&battery_max_cell_temp_label, "MAX CT", 125, 15, 127, 0, FAULT_TAB_WIDTH);
  create_label_with_attr(&battery_min_cell_temp_label, "MIN CT", 125, 15, 127, 15, FAULT_TAB_WIDTH);
  create_label_with_attr(&battery_avg_cell_temp_label, "AVG CT", 125, 15, 127, 30, FAULT_TAB_WIDTH);

  pack_volt_label_widget                                             = new Label_CAN_Num_Widget<uint32_t>(pack_volt_label, &CANlib_critical_CellVoltageRange_Input.sum);
  battery_max_cell_volt_label_widget                                 = new Label_CAN_Num_Widget<uint16_t>(battery_max_cell_volt_label, &CANlib_critical_CellVoltageRange_Input.max);
  battery_min_cell_volt_label_widget                                 = new Label_CAN_Num_Widget<uint16_t>(battery_min_cell_volt_label, &CANlib_critical_CellVoltageRange_Input.min);
  battery_max_cell_temp_label_widget                                 = new Label_CAN_Num_Widget<int16_t>(battery_max_cell_temp_label, &CANlib_critical_CellTemperatureRange_Input.max);
  battery_min_cell_temp_label_widget                                 = new Label_CAN_Num_Widget<int16_t>(battery_min_cell_temp_label, &CANlib_critical_CellTemperatureRange_Input.min);
  battery_avg_cell_temp_label_widget                                 = new Label_CAN_Num_Widget<int16_t>(battery_avg_cell_temp_label, &CANlib_critical_CellTemperatureRange_Input.avg);
  Widget *rom_gen_battery_widgets[num_roman_general_battery_widgets] = {pack_volt_label_widget, battery_max_cell_volt_label_widget, battery_min_cell_volt_label_widget, battery_max_cell_temp_label_widget, battery_min_cell_temp_label_widget, battery_avg_cell_temp_label_widget};

  rom_gen_battery_page = new Page_General(rom_gen_battery_widgets, num_roman_general_battery_widgets, BATTERY);
}

static void init_motor_page(void) {
  create_label_with_attr(&mc_temps_a_label, "MC T_A", 83, 15, 0, 0, FAULT_TAB_WIDTH);
  create_label_with_attr(&mc_temps_b_label, "MC T_B", 83, 15, 0, 15, FAULT_TAB_WIDTH);
  create_label_with_attr(&mc_temps_c_label, "MC T_C", 83, 15, 0, 30, FAULT_TAB_WIDTH);
  create_label_with_attr(&mc_temps_gd_label, "MC T_GD", 83, 15, 0, 45, FAULT_TAB_WIDTH);
  create_label_with_attr(&lside_cmd_label, "H CMD", 83, 15, 100, 0, FAULT_TAB_WIDTH);
  create_label_with_attr(&lside_act_label, "H ACT", 83, 15, 100, 15, FAULT_TAB_WIDTH);
  create_label_with_attr(&hside_cmd_label, "L CMD", 83, 15, 100, 30, FAULT_TAB_WIDTH);
  create_label_with_attr(&hside_act_label, "L ACT", 83, 15, 100, 45, FAULT_TAB_WIDTH);
  create_label_with_attr(&vcu_sd_not_pres_label, "VCU SDP", 83, 15, 170, 0, FAULT_TAB_WIDTH);
  create_label_with_attr(&vcu_sd_err_label, "VCU SDE", 83, 15, 170, 15, FAULT_TAB_WIDTH);
  create_label_with_attr(&bms_sd_not_pres_label, "BMS SDP", 83, 15, 170, 30, FAULT_TAB_WIDTH);
  create_label_with_attr(&bms_sd_err_label, "BMS SDE", 83, 15, 170, 45, FAULT_TAB_WIDTH);
  // create_label_with_attr(&mc_temps_bl_label, "MC T_BL", 83, 15, 100, 0, FAULT_TAB_WIDTH); // left bamocar
  // create_label_with_attr(&mc_temps_br_label, "MC T_BR", 83, 15, 100, 15, FAULT_TAB_WIDTH); // right bamocar
  mc_temps_a_label_widget      = new Label_CAN_Num_Widget<int16_t>(mc_temps_a_label, &CANlib_critical_RMSTemps_Input.module_a_temp);
  mc_temps_b_label_widget      = new Label_CAN_Num_Widget<int16_t>(mc_temps_b_label, &CANlib_critical_RMSTemps_Input.module_b_temp);
  mc_temps_c_label_widget      = new Label_CAN_Num_Widget<int16_t>(mc_temps_c_label, &CANlib_critical_RMSTemps_Input.module_c_temp);
  mc_temps_gd_label_widget     = new Label_CAN_Num_Widget<int16_t>(mc_temps_gd_label, &CANlib_critical_RMSTemps_Input.gate_driver_temp);
  lside_cmd_label_widget       = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_lside_cmd_T>(lside_cmd_label, &CANlib_critical_VCUDashInfo_Input.lside_cmd);
  lside_act_label_widget       = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_lside_act_T>(lside_act_label, &CANlib_critical_VCUDashInfo_Input.lside_act);
  hside_cmd_label_widget       = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_hside_cmd_T>(hside_cmd_label, &CANlib_critical_VCUDashInfo_Input.hside_cmd);
  hside_act_label_widget       = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_hside_act_T>(hside_act_label, &CANlib_critical_VCUDashInfo_Input.hside_act);
  vcu_sd_not_pres_label_widget = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_vcu_sd_not_present_T>(vcu_sd_not_pres_label, &CANlib_critical_VCUDashInfo_Input.vcu_sd_not_present);
  vcu_sd_err_label_widget      = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_vcu_sd_write_failure_T>(vcu_sd_err_label, &CANlib_critical_VCUDashInfo_Input.vcu_sd_write_failure);
  bms_sd_not_pres_label_widget = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_bms_sd_not_present_T>(bms_sd_not_pres_label, &CANlib_critical_VCUDashInfo_Input.bms_sd_not_present);
  bms_sd_err_label_widget      = new Label_CAN_Fault_Widget<CANlib_critical_VCUDashInfo_bms_sd_write_failure_T>(bms_sd_err_label, &CANlib_critical_VCUDashInfo_Input.bms_sd_write_failure);

  // mc_temps_bl_label_widget = new Label_CAN_Num_Widget<int16_t>(mc_temps_bl_label, &CANlib_critical_BamocarTemps_Input.left);
  // mc_temps_br_label_widget = new Label_CAN_Num_Widget<int16_t>(mc_temps_br_label, &CANlib_critical_BamocarTemps_Input.right);
  Widget *motor_page_widgets[num_motor_page_widgets] = {
      mc_temps_a_label_widget,
      mc_temps_b_label_widget,
      mc_temps_c_label_widget,
      mc_temps_gd_label_widget,
      // mc_temps_bl_label_widget,
      // mc_temps_br_label_widget,
      lside_cmd_label_widget,
      lside_act_label_widget,
      hside_cmd_label_widget,
      hside_act_label_widget,
      vcu_sd_not_pres_label_widget,
      vcu_sd_err_label_widget,
      bms_sd_not_pres_label_widget,
      bms_sd_err_label_widget,
  };
  motor_page = new Page_General(motor_page_widgets, num_motor_page_widgets, MOTOR);
}

static void init_roman_general_mode(void) {
  init_roman_general_main_page();
  init_faults_page();
  init_battery_page();
  // init_timer_page();
  init_motor_page();
#ifdef TUNING_POW_LIM
  init_pow_lim_controls_page();
#endif // TUNING_POW_LIM
#ifdef TUNING_RG_TL_VL
  init_second_controls_page();
#endif // TUNING_RG_TL_VL
#ifdef TUNING_LC
  init_launch_control_page();
#endif // TUNING_LC
  // order of pages in this array are the order they will be paged in
  Page *pages[NUM_PAGES] = {
    rom_gen_main_page,
    faults_page,
    rom_gen_battery_page,
    motor_page,
#ifdef TUNING_POW_LIM
    pow_lim_controls_page,
#endif // TUNING_POW_LIM
#ifdef TUNING_RG_TL_VL
    second_controls_page,
#endif // TUNING_RG_TL_VL
#ifdef TUNING_LC
    launch_control_page
#endif // TUNING_LC
  };
  roman_general_mode = new Mode(pages, NUM_PAGES, ROMAN_GENERAL);
}

static void update_mode_pages(Mode_Name current_mode) {
  switch (current_mode) {
  case ROMAN_GENERAL:
    roman_general_mode->update();
    break;

  case ROMAN_DEBUG:
    break;
  }
}

static void mode_transition(Mode_Name current_mode) {
  gdispClear(Black);

  switch (current_mode) {
  case ROMAN_GENERAL:
    roman_general_mode->set_current_page(rom_gen_main_page);
    break;

  case ROMAN_DEBUG:
    break;
  }
}

// called from TIM2 interrupt, found in timer.c
void interrupt_process_buttons(B_State far_right_button, B_State far_left_button, B_State wheel_1_button, B_State wheel_2_button, B_State wheel_3_button, B_State wheel_4_button, B_State wheel_5_button, B_State wheel_6_button) {
  UNUSED(wheel_1_button);
  UNUSED(wheel_2_button);
  UNUSED(wheel_3_button);
  UNUSED(wheel_4_button);
  UNUSED(wheel_5_button);
  UNUSED(wheel_6_button);

  switch (current_mode) {
  case ROMAN_GENERAL:
    roman_general_mode->process_buttons(far_right_button, far_left_button, wheel_1_button, wheel_2_button, wheel_3_button, wheel_4_button, wheel_5_button, wheel_6_button);
    break;

  case ROMAN_DEBUG:
    break;
  }
}

/*
    Called at the beginning of main
*/
void gui_init() {
  finished_gui_init = false;
  gfxInit();

  ui2 = gdispOpenFont("UI2");

  gwinSetDefaultFont(ui2);
  gwinSetDefaultStyle(&BlackWidgetStyle, FALSE);

  current_mode = ROMAN_GENERAL;
  init_roman_general_mode();
  mode_transition(current_mode);

  geventListenerInit(&gui_listener);
  gwinAttachListener(&gui_listener);

  finished_gui_init = true;
  last_gui_update   = HAL_GetTick();
}

/*
    Call periodically in main
*/
void gui_update() {

  roman_general_mode->page();
  if (HAL_GetTick() - last_gui_update > GUI_UPDATE_PERIOD) {
    update_mode_pages(current_mode);
    pe              = geventEventWait(&gui_listener, GUI_EVENT_TIMEOUT);
    last_gui_update = HAL_GetTick();
  }
}

bool has_finished_gui_init() {
  return finished_gui_init;
}
