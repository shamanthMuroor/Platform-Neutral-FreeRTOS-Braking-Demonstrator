//
// File: Main_Model_Braking_Demo.cpp
//
// Code generated for Simulink model 'Main_Model_Braking_Demo'.
//
// Model version                  : 1.13
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Thu Sep 17 09:48:31 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "Main_Model_Braking_Demo.h"
#include "Wheel_Control_Model.h"

// Model step function
void Main_Model_Braking_Demo::step()
{
  // ModelReference: '<Root>/Wheel_Control_Model' incorporates:
  //   Inport: '<Root>/front_left_grip_loss'
  //   Inport: '<Root>/front_left_wheel_speed'
  //   Inport: '<Root>/front_right_grip_loss'
  //   Inport: '<Root>/front_right_wheel_speed'
  //   Inport: '<Root>/reference_speed'
  //   Inport: '<Root>/requested_brake'
  //   Outport: '<Root>/front_left_motor_command'
  //   Outport: '<Root>/front_left_slip_active'
  //   Outport: '<Root>/front_left_wheel_slip'
  //   Outport: '<Root>/front_right_motor_command'
  //   Outport: '<Root>/front_right_slip_active'
  //   Outport: '<Root>/front_right_wheel_slip'

  Wheel_Control_ModelMDLOBJ1.step(&Main_Model_Braking_Demo_U.reference_speed,
    &Main_Model_Braking_Demo_U.requested_brake,
    &Main_Model_Braking_Demo_U.front_left_grip_loss,
    &Main_Model_Braking_Demo_U.front_right_grip_loss,
    &Main_Model_Braking_Demo_U.front_left_wheel_speed,
    &Main_Model_Braking_Demo_U.front_right_wheel_speed,
    &Main_Model_Braking_Demo_Y.front_left_wheel_slip,
    &Main_Model_Braking_Demo_Y.front_right_wheel_slip,
    &Main_Model_Braking_Demo_Y.front_left_motor_command,
    &Main_Model_Braking_Demo_Y.front_right_motor_command,
    &Main_Model_Braking_Demo_Y.front_left_slip_active,
    &Main_Model_Braking_Demo_Y.front_right_slip_active);
}

// Model initialize function
void Main_Model_Braking_Demo::initialize()
{
  // Model Initialize function for ModelReference Block: '<Root>/Wheel_Control_Model' 

  // Set error status pointer for ModelReference Block: '<Root>/Wheel_Control_Model' 
  Wheel_Control_ModelMDLOBJ1.setErrorStatusPointer(rtmGetErrorStatusPointer
    ((&Main_Model_Braking_Demo_M)));
  Wheel_Control_ModelMDLOBJ1.initialize();
}

// Model terminate function
void Main_Model_Braking_Demo::terminate()
{
  // (no terminate code required)
}

// Root inports set method
void Main_Model_Braking_Demo::setExternalInputs(const Main_Model_Braking_Demo::
  ExtU_Main_Model_Braking_Demo_T *pExtU_Main_Model_Braking_Demo_T)
{
  Main_Model_Braking_Demo_U = *pExtU_Main_Model_Braking_Demo_T;
}

// Root outports get method
const Main_Model_Braking_Demo::ExtY_Main_Model_Braking_Demo_T
  &Main_Model_Braking_Demo::getExternalOutputs() const
{
  return Main_Model_Braking_Demo_Y;
}

// Constructor
Main_Model_Braking_Demo::Main_Model_Braking_Demo() :
  Main_Model_Braking_Demo_U(),
  Main_Model_Braking_Demo_Y(),
  Main_Model_Braking_Demo_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
Main_Model_Braking_Demo::~Main_Model_Braking_Demo() = default;

// Real-Time Model get method
Main_Model_Braking_Demo::RT_MODEL_Main_Model_Braking_D_T
  * Main_Model_Braking_Demo::getRTM()
{
  return (&Main_Model_Braking_Demo_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
