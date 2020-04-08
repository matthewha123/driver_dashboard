#ifndef _CONTROLS_VALUES_H_
#define _CONTROLS_VALUES_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef TUNING_POW_LIM
uint16_t power_lim_vals[]   = {5, 10, 20, 50, 100, 150, 200, 250, 300, 350, 400, 500, 600, 700, 800};
uint8_t electrical_P_vals[] = {0, 1, 5, 10, 25, 50, 100, 125};
uint8_t electrical_I_vals[] = {0, 1, 5, 10, 25, 50, 100, 125};
bool using_vq_lim_vals[]    = {false, true};
uint16_t anti_windup_vals[] = {500, 1000, 2000};
// uint8_t front_efficiency_vals[] = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50};
uint8_t rear_efficiency_vals[] = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45};
bool mpl_enable_vals[] = {false, true};
#endif // TUNING_POW_LIM

#ifdef TUNING_RG_TL_VL
uint8_t regen_bias_vals[]              = {40, 43, 45, 47, 50, 53, 55, 57, 60};
uint8_t volt_lim_min_gain_vals[]       = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
uint8_t temp_lim_min_gain_vals[]       = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
uint8_t volt_lim_cutoff_voltage_vals[] = {25, 26, 27, 28, 29, 30, 31, 32,
                                          33, 34, 35, 36, 37, 38, 39, 40};
uint8_t temp_lim_cutoff_temp_vals[]    = {10, 15, 20, 25, 39, 35, 40, 45, 50, 55, 60};
#endif // TUNING_RG_TL_VL

bool epl_enable_vals[]       = {false, true};
bool precool_vals[]          = {false, true};
bool temp_lim_enable_vals[]  = {false, true};
bool volt_lim_enable_vals[]  = {false, true};
bool regen_enable_vals[]     = {false, true};
uint8_t torque_factor_vals[] = {100, 75, 50, 25};

#ifdef TUNING_LC
// Launch Control values
bool lc_fn_src_vals[] = {false, true};
uint8_t lc_ws_th_vals[]       = {5, 40, 100};
bool lc_account_for_kappa_vals[]       = {false, true};
uint8_t lc_lambda_vals[] = {50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70};
bool lc_enabled_vals[]      = {false, true};
uint8_t lc_min_kappa_vals[]       = {1, 5, 10, 50, 100};
#endif // TUNING_LC

#endif // _CONTROLS_VALUES_H_
