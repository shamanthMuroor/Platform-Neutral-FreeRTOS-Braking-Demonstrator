//
// File: Slip_Calculation.h
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
#ifndef RTW_HEADER_Slip_Calculation_h_
#define RTW_HEADER_Slip_Calculation_h_
#include <stdbool.h>
#include <stdint.h>
#include "complex_types.h"
#include "Slip_Calculation_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Class declaration for model Slip_Calculation
class Slip_Calculation final
{
  // public data and function members
 public:
  // External inputs (root inport signals with default storage)
  struct ExtU_Slip_Calculation_T {
    uint16_t throttle;                 // '<Root>/throttle'
    uint16_t wheel_speed;              // '<Root>/wheel_speed'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_Slip_Calculation_T {
    uint32_t local_wheel_slip;         // '<Root>/local_wheel_slip'
  };

  // Real-time Model Data Structure
  struct RT_MODEL_Slip_Calculation_T {
    const char * volatile errorStatus;
  };

  // Copy Constructor
  Slip_Calculation(Slip_Calculation const&) = delete;

  // Assignment Operator
  Slip_Calculation& operator= (Slip_Calculation const&) & = delete;

  // Move Constructor
  Slip_Calculation(Slip_Calculation &&) = delete;

  // Move Assignment Operator
  Slip_Calculation& operator= (Slip_Calculation &&) = delete;

  // Real-Time Model get method
  Slip_Calculation::RT_MODEL_Slip_Calculation_T * getRTM();

  // Root inports set method
  void setExternalInputs(const ExtU_Slip_Calculation_T *pExtU_Slip_Calculation_T)
  {
    Slip_Calculation_U = *pExtU_Slip_Calculation_T;
  }

  // Root outports get method
  const ExtY_Slip_Calculation_T &getExternalOutputs() const
  {
    return Slip_Calculation_Y;
  }

  // model initialize function
  static void initialize();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Constructor
  Slip_Calculation();

  // Destructor
  ~Slip_Calculation();

  // private data and function members
 private:
  // External inputs
  ExtU_Slip_Calculation_T Slip_Calculation_U;

  // External outputs
  ExtY_Slip_Calculation_T Slip_Calculation_Y;

  // Real-Time Model
  RT_MODEL_Slip_Calculation_T Slip_Calculation_M;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Constant' : Unused code path elimination


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
//  '<Root>' : 'Slip_Calculation'

#endif                                 // RTW_HEADER_Slip_Calculation_h_

//
// File trailer for generated code.
//
// [EOF]
//
