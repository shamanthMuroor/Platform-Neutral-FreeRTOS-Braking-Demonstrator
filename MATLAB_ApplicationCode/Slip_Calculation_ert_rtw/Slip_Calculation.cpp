//
// File: Slip_Calculation.cpp
//
// Code generated for Simulink model 'Slip_Calculation'.
//
// Model version                  : 1.2
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Mon Sep  7 11:20:13 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "Slip_Calculation.h"
#include <stdint.h>
#include "Slip_Calculation_private.h"

int32_t div_s32_floor(int32_t numerator, int32_t denominator)
{
  int32_t quotient;
  if (denominator == 0) {
    quotient = numerator >= 0 ? INT32_MAX : INT32_MIN;

    // Divide by zero handler
  } else {
    uint32_t absDenominator;
    uint32_t absNumerator;
    uint32_t tempAbsQuotient;
    bool quotientNeedsNegation;
    absNumerator = numerator < 0 ? ~static_cast<uint32_t>(numerator) + 1U :
      static_cast<uint32_t>(numerator);
    absDenominator = denominator < 0 ? ~static_cast<uint32_t>(denominator) + 1U :
      static_cast<uint32_t>(denominator);
    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0U) {
        tempAbsQuotient++;
      }
    }

    quotient = quotientNeedsNegation ? -static_cast<int32_t>(tempAbsQuotient) :
      static_cast<int32_t>(tempAbsQuotient);
  }

  return quotient;
}

// Model step function
void Slip_Calculation::step()
{
  int32_t u0;
  uint16_t tmp;

  // Switch: '<Root>/Switch ' incorporates:
  //   Constant: '<Root>/Constant'
  //   Inport: '<Root>/reference_speed'

  if (Slip_Calculation_U.reference_speed > 0) {
    tmp = Slip_Calculation_U.reference_speed;
  } else {
    tmp = 1U;
  }

  // Product: '<Root>/slip_ratio' incorporates:
  //   Gain: '<Root>/Gain '
  //   Inport: '<Root>/reference_speed'
  //   Inport: '<Root>/wheel_speed'
  //   Sum: '<Root>/speed_error'
  //   Switch: '<Root>/Switch '

  u0 = div_s32_floor((Slip_Calculation_U.reference_speed -
                      Slip_Calculation_U.wheel_speed) * 1000,
                     static_cast<int32_t>(tmp));

  // Saturate: '<Root>/Saturation '
  if (u0 > 1000) {
    // Outport: '<Root>/local_wheel_slip'
    Slip_Calculation_Y.local_wheel_slip = 1000U;
  } else if (u0 < 0) {
    // Outport: '<Root>/local_wheel_slip'
    Slip_Calculation_Y.local_wheel_slip = 0U;
  } else {
    // Outport: '<Root>/local_wheel_slip'
    Slip_Calculation_Y.local_wheel_slip = static_cast<uint16_t>(u0);
  }

  // End of Saturate: '<Root>/Saturation '
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
