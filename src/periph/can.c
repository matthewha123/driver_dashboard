#include "can.h"
static uint32_t last_mock_can_tick = 0;

CANlib_critical_DashRequest_T dash_request;
CANlib_critical_Controls_Settings1_T controls_settings1 = {
  .power_lim = 5,
  .electrical_P = 0,
  .electrical_I = 0,
  .anti_windup = 100,
  .rear_efficiency = 100,
  .front_efficiency = 100,
};

CANlib_critical_Controls_Settings2_T controls_settings2 = {
  .epl_enable = false,
  .mpl_enable = false,
  .precooling_enable = false,
  .using_vq_lim = false,
  .temp_lim_enable = false,
  .volt_lim_enable = false,
  .regen_enable = false,
  .regen_bias = 57,
  .volt_lim_min_gain = 100,
  .temp_lim_min_gain = 100,
  .volt_lim_cutoff_voltage = 30,
  .temp_lim_cutoff_temp = 60,
  .torque_factor = 100
};

CANlib_critical_LaunchControlSettings_T launch_control_settings = {};

CANlib_critical_VCUState_T CANlib_critical_VCUState_Input = {
  .state = 0,
};
CANlib_critical_CurrentSensor_Voltage1_T CANlib_critical_CurrentSensor_Voltage1_Input;
CANlib_critical_VCUDashInfo_T CANlib_critical_VCUDashInfo_Input = {
  .bms = 0,
  .contactor_mismatch = 0,
  .bspd = 0,
  .throttle_implausibility = 0,
  .sensor_implausibility = 0,
  .brake_throttle_conflict = 0,
  .tsms = 0,
  .shutdown_loop_latch = 0,
  .hvlv_interlock = 0,
  .heartbeat = 0,
  .leak_detect = 0
};
CANlib_critical_CellVoltageRange_T CANlib_critical_CellVoltageRange_Input;
CANlib_critical_CellTemperatureRange_T CANlib_critical_CellTemperatureRange_Input;
CANlib_critical_Incrementing_Number_T inc_num = {.num = 0};

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;
CAN_HandleTypeDef hcan3;


CANlib_critical_VCUState_state_T vcu_state_old;
CANlib_critical_VCUDashInfo_imd_T imd_fault_old;
CANlib_critical_VCUDashInfo_bms_T bms_fault_old = 0;
CANlib_critical_VCUDashInfo_contactor_mismatch_T contactor_mismatch_fault_old = 0;
CANlib_critical_VCUDashInfo_bspd_T bspd_fault_old = 0;
CANlib_critical_VCUDashInfo_throttle_implausibility_T throttle_implausibility_fault_old = 0;
CANlib_critical_VCUDashInfo_sensor_implausibility_T sensor_implausibility_fault_old;
CANlib_critical_VCUDashInfo_brake_throttle_conflict_T brake_throttle_conflict_fault_old = 0;
CANlib_critical_VCUDashInfo_tsms_T tsms_fault_old = 0;
CANlib_critical_VCUDashInfo_shutdown_loop_latch_T shutdown_loop_latch_fault_old = 0;
CANlib_critical_VCUDashInfo_hvlv_interlock_T hvlv_interlock_fault_old = 0;
CANlib_critical_VCUDashInfo_heartbeat_T heartbeat_fault_old = 0;
CANlib_critical_VCUDashInfo_leak_detect_T leak_detect_fault_old = 0;

static void config_handle_parameters(CAN_HandleTypeDef *hcan) {
  // Universal configuration parameters
  hcan->Init.Prescaler = 4;
  hcan->Init.Mode = CAN_MODE_NORMAL;
  hcan->Init.SyncJumpWidth = CAN_SJW_3TQ;
  hcan->Init.TimeSeg1 = CAN_BS1_16TQ;
  hcan->Init.TimeSeg2 = CAN_BS2_7TQ;
  hcan->Init.TimeTriggeredMode = DISABLE;
  hcan->Init.AutoBusOff = DISABLE;
  hcan->Init.AutoWakeUp = DISABLE;
  hcan->Init.AutoRetransmission = DISABLE;
  hcan->Init.ReceiveFifoLocked = DISABLE;
  hcan->Init.TransmitFifoPriority = DISABLE;
}

HAL_StatusTypeDef can_init() {
  CAN_FilterTypeDef filter = {0};
  filter.FilterIdHigh = 0x1fff;
  filter.FilterIdLow = 0xffff;
  filter.FilterMaskIdHigh = 0;
  filter.FilterMaskIdLow = 0;
  filter.FilterFIFOAssignment = CAN_RX_FIFO0;
  filter.FilterBank = 0;
  filter.FilterMode = CAN_FILTERMODE_IDMASK; // Standard CAN filtering
  filter.FilterScale = CAN_FILTERSCALE_16BIT;
  filter.FilterActivation = ENABLE;
  filter.SlaveStartFilterBank = 0;

  hcan1.Instance = CAN1;
  config_handle_parameters(&hcan1);
  HAL_StatusTypeDef init_status = HAL_CAN_Init(&hcan1);
  HAL_CAN_ConfigFilter(&hcan1, &filter);
  HAL_CAN_Start(&hcan1);

  return init_status;
}

void can_transmit() {
  CANlib_Send_critical_DashRequest(&dash_request);
  CANlib_Send_critical_Controls_Settings1(&controls_settings1);
  CANlib_Send_critical_Controls_Settings2(&controls_settings2);
  CANlib_Send_critical_LaunchControlSettings(&launch_control_settings);
}

void mock_tick_can_vals() {

  if(HAL_GetTick() - last_mock_can_tick > 50) {
    CANlib_critical_CellTemperatureRange_Input.max+=100;
    CANlib_critical_CellTemperatureRange_Input.min+=100;
    CANlib_critical_CellVoltageRange_Input.max+=100;
    CANlib_critical_CellVoltageRange_Input.min+=100;
    CANlib_critical_CurrentSensor_Voltage1_Input.result++;
    CANlib_critical_CellTemperatureRange_Input.avg++;
    CANlib_critical_CellVoltageRange_Input.sum+=1000;
    CANlib_critical_VCUState_Input.state = CANlib_critical_VCUState_state_DRIVER_RESET_LOW;
    //CANlib_critical_RMSTemps_Input.module_a_temp+=1000;
    //CANlib_critical_RMSTemps_Input.gate_driver_temp+=1000;
    CANlib_critical_CurrentSensor_Voltage1_Input.result+=10000;
    CANlib_critical_CurrentSensor_Power_Input.result += 400;
    last_mock_can_tick = HAL_GetTick();
  }

  if(HAL_GetTick() > 5000) {
      CANlib_critical_VCUDashInfo_Input.imd = 1;
      CANlib_critical_VCUDashInfo_Input.bms = 0;
      CANlib_critical_VCUDashInfo_Input.bspd = 1;
      CANlib_critical_VCUDashInfo_Input.contactor_mismatch = 2;
      CANlib_critical_VCUDashInfo_Input.heartbeat = 1;
      CANlib_critical_VCUDashInfo_Input.sensor_implausibility = 1;
      CANlib_critical_VCUDashInfo_Input.throttle_implausibility = 2;
  }
  if(HAL_GetTick() > 9000) {
      CANlib_critical_VCUDashInfo_Input.imd = 0;
      CANlib_critical_VCUDashInfo_Input.bspd = 2;
      CANlib_critical_VCUDashInfo_Input.bms = 1;
      CANlib_critical_VCUDashInfo_Input.sensor_implausibility = 1;
      CANlib_critical_VCUDashInfo_Input.throttle_implausibility = 1;
      CANlib_critical_VCUDashInfo_Input.heartbeat = 1;
  }
  if(HAL_GetTick() > 14000) {
      CANlib_critical_VCUDashInfo_Input.bms = 0;
      CANlib_critical_VCUDashInfo_Input.sensor_implausibility = 0;
      CANlib_critical_VCUDashInfo_Input.throttle_implausibility = 0;
      CANlib_critical_VCUDashInfo_Input.heartbeat = 1;
  }


}
