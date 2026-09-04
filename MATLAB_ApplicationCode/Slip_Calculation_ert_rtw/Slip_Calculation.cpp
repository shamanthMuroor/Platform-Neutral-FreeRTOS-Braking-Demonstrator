//
// File: Slip_Calculation.cpp
//
// Code generated for Simulink model 'Slip_Calculation'.
//
// Model version                  : 1.0
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Fri Sep  4 16:12:01 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "Slip_Calculation.h"
#include <stdint.h>

// Model step function
void Slip_Calculation::step()
{
  // Outport: '<Root>/local_wheel_slip' incorporates:
  //   Inport: '<Root>/throttle'
  //   Inport: '<Root>/wheel_speed'
  //   Sum: '<Root>/speed_error'

  Slip_Calculation_Y.local_wheel_slip = static_cast<uint32_t>
    (Slip_Calculation_U.throttle - Slip_Calculation_U.wheel_speed);
}

// Model initialize function
void Slip_Calculation::initialize()
{
  // (no initialization code required)
}

// Model terminate function
void Slip_Calculation::terminate()
{
  // (no terminate code required)
}

// Constructor
Slip_Calculation::Slip_Calculation() :
  Slip_Calculation_U(),
  Slip_Calculation_Y(),
  Slip_Calculation_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
Slip_Calculation::~Slip_Calculation() = default;

// Real-Time Model get method
Slip_Calculation::RT_MODEL_Slip_Calculation_T * Slip_Calculation::getRTM()
{
  return (&Slip_Calculation_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
