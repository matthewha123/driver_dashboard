/**
  ******************************************************************************
  * @file    buzzer.c
  * @author  MCD Application Team
  * @brief   Source File for Fixing and Working with the Buzzer.
  *
  *   Note doxygen should be installed on your linux terminal. Instructions here:
  *   http://www.doxygen.nl/download.html
  *   You may edit the "newDoxy" file as you please.
  *         - Currently, the INPUT paramter is set to "buzzer.c" You can leave it blank to access
  *           all of the files in the folder. 
  *         - More info is found at: 
  *                 - http://www.doxygen.nl/manual/starting.html#step2 & 
  *                 - http://www.doxygen.nl/manual/doxygen_usage.html 
  * 
  *   This file provides two functions and one global variable to be called from 
  *   user application (Please Note that this IS A TEST FILE, with COPIED 
  *   DOCUMENTATION from system_stm32f4xx.c and therefore most of the notes do NOT
  *   make sense):
  *      - arm_buzzer(): This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - reset_buzzer_state(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f4xx_system
  * @{
  */  
  
/** @addtogroup STM32F4xx_System_Private_Includes
  * @{
  */
#include "buzzer.h"

bool buzzer_armed = false;
bool is_buzzing = false;
bool already_buzzed = false;
uint32_t buzz_start_time;
uint8_t buzz_piece_index = 0;
bool buzz_piece_state = false;

const uint16_t buzz_time = 2500;

/**
 * This ARMS THE BUZZER!
 * ARMS THE BUZZER!
*/

void arm_buzzer(void) {

    if(!already_buzzed) {
        buzzer_armed = true;
    }
    else {
        buzzer_armed = false;
    }
}

/**
 * This only resets the state of the Buzzer
 * It will change the "already_buzzed" parameter to False
*/

void reset_buzzer_state(void) {
    already_buzzed = false;
} 

/** 
 * @brief all buzz needs to start buzzing is buzzer_armed == true
 *      when entering zero_torque for the first time, buzzer_armed is set to true to start buzzing
 *      already_buzzed is set to true to indicate that only one buzz should occur for an entry into zero_torque
 * @Note other vcu states will cause reset_buzzer_state to be called, setting already_buzzed to false so 
 * @Note -that a later rentry into zero_torque will cause buzzer_armed to be set correctly
 *       -and buzz to buzz
*/
void buzz(void) {

    if(buzzer_armed) {
        buzz_start_time = HAL_GetTick();
        buzzer_armed = false;
        is_buzzing = true;
        already_buzzed = true;
    }

    if(is_buzzing) {
        if(HAL_GetTick() - buzz_start_time < buzz_time) {
            if (HAL_GetTick() - buzz_start_time > 2000)  Pin_Write(BUZZER_PORT, BUZZER_PIN, true);
            else if (HAL_GetTick() - buzz_start_time > 1500) Pin_Write(BUZZER_PORT, BUZZER_PIN, false);
            else if (HAL_GetTick() - buzz_start_time > 1000) Pin_Write(BUZZER_PORT, BUZZER_PIN, true);
            else if (HAL_GetTick() - buzz_start_time > 500) Pin_Write(BUZZER_PORT, BUZZER_PIN, false);
            else Pin_Write(BUZZER_PORT, BUZZER_PIN, true);
        } else {
            Pin_Write(BUZZER_PORT, BUZZER_PIN, false);
            is_buzzing = false;
        }        
    }
    else {
        Pin_Write(BUZZER_PORT, BUZZER_PIN, false);
    }
}

/**
 * This type of Buzzing
 * Handdles Buzzing
 * This is just a test message 
 * For DoXyGeN
*/
void handle_buzzer(void) {
    if(CANlib_critical_VCUState_Input.state == CANlib_critical_VCUState_state_ZERO_TORQUE) {
        arm_buzzer();
    } 
    // else {
    //     reset_buzzer_state();
    // }

    buzz();
}

