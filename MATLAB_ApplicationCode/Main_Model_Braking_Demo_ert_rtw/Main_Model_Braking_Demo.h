//
// File: Main_Model_Braking_Demo.h
//
// Code generated for Simulink model 'Main_Model_Braking_Demo'.
//
// Model version                  : 1.10
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Wed Sep  9 09:16:47 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_Main_Model_Braking_Demo_h_
#define RTW_HEADER_Main_Model_Braking_Demo_h_
#include <stdbool.h>
#include <stdint.h>
#include "complex_types.h"
#include "Main_Model_Braking_Demo_types.h"
#include "Wheel_Control_Model.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetErrorStatusPointer
#define rtmGetErrorStatusPointer(rtm)  ((const char **)(&((rtm)->errorStatus)))
#endif

// Class declaration for model Main_Model_Braking_Demo
class Main_Model_Braking_Demo final
{
  // public data and function members
 public:
  // External inputs (root inport signals with default storage)
  struct ExtU_Main_Model_Braking_Demo_T {
    uint16_t reference_speed;          // '<Root>/reference_speed'
    uint16_t requested_brake;          // '<Root>/requested_brake'
    uint16_t front_left_grip_loss;     // '<Root>/front_left_grip_loss'
    uint16_t front_right_grip_loss;    // '<Root>/front_right_grip_loss'
    uint16_t front_left_wheel_speed;   // '<Root>/front_left_wheel_speed'
    uint16_t front_right_wheel_speed;  // '<Root>/front_right_wheel_speed'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_Main_Model_Braking_Demo_T {
    uint16_t front_left_wheel_slip;    // '<Root>/front_left_wheel_slip'
    uint16_t front_right_wheel_slip;   // '<Root>/front_right_wheel_slip'
    uint16_t front_left_motor_command; // '<Root>/front_left_motor_command'
    uint16_t front_right_motor_command;// '<Root>/front_right_motor_command'
    bool front_left_slip_active;       // '<Root>/front_left_slip_active'
    bool front_right_slip_active;      // '<Root>/front_right_slip_active'
  };

  // Real-time Model Data Structure
  struct RT_MODEL_Main_Model_Braking_D_T {
    const char *errorStatus;
  };

  // Copy Constructor
  Main_Model_Braking_Demo(Main_Model_Braking_Demo const&) = delete;

  // Assignment Operator
  Main_Model_Braking_Demo& operator= (Main_Model_Braking_Demo const&) & = delete;

  // Move Constructor
  Main_Model_Braking_Demo(Main_Model_Braking_Demo &&) = delete;

  // Move Assignment Operator
  Main_Model_Braking_Demo& operator= (Main_Model_Braking_Demo &&) = delete;

  // Real-Time Model get method
  Main_Model_Braking_Demo::RT_MODEL_Main_Model_Braking_D_T * getRTM();

  // Root inports set method
  void setExternalInputs(const ExtU_Main_Model_Braking_Demo_T
    *pExtU_Main_Model_Braking_Demo_T)
  {
    Main_Model_Braking_Demo_U = *pExtU_Main_Model_Braking_Demo_T;
  }

  // Root outports get method
  const ExtY_Main_Model_Braking_Demo_T &getExternalOutputs() const
  {
    return Main_Model_Braking_Demo_Y;
  }

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Constructor
  Main_Model_Braking_Demo();

  // Destructor
  ~Main_Model_Braking_Demo();

  // private data and function members
 private:
  // External inputs
  ExtU_Main_Model_Braking_Demo_T Main_Model_Braking_Demo_U;

  // External outputs
  ExtY_Main_Model_Braking_Demo_T Main_Model_Braking_Demo_Y;

  // model instance variable for '<Root>/Wheel_Control_Model'
  Wheel_Control_Model Wheel_Control_ModelMDLOBJ1;

  // Real-Time Model
  RT_MODEL_Main_Model_Braking_D_T Main_Model_Braking_Demo_M;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Main_Model_Braking_Demo'

#endif                                 // RTW_HEADER_Main_Model_Braking_Demo_h_

//
// File trailer for generated code.
//
// [EOF]
//
