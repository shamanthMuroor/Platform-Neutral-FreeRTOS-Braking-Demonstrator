%% build_slip_calculation.m
% Increment 1 of the braking-demonstrator ASW: the Slip_Calculation SWC.
% Platform-neutral, integer-only application logic (no hardware, no floating point).
% Builds:
%   1) Slip_Calculation.slx        -> the deployable SWC (Embedded Coder -> C++)
%   2) Slip_Calculation_Harness.slx-> a test harness (sources + scope) to verify it
%
% Run this from your project's MATLAB folder:  >> build_slip_calculation
%
% Signal contract (matches concept docs and your PDF):
%   Inports : throttle (uint16 0..1000), wheel_speed (uint16 0..1000)
%   Outport : local_wheel_slip (uint16 0..1000, slip carried x1000)
%   Formula : slip = (throttle - wheel_speed) * SLIP_UNIT / throttle
%             guarded: throttle < MIN_SPEED -> slip = 0 (no divide-by-zero)
%             clamped: [0, 1000]

clc;

%% ---------------------------------------------------------------------
%  Tunable parameters -> exported as #define in the generated C++
%  (create them in the base workspace; the model resolves them by name)
% ---------------------------------------------------------------------
SLIP_UNIT = Simulink.Parameter(uint16(1000));   % fixed-point scale (x1000)
SLIP_UNIT.CoderInfo.StorageClass = 'Custom';
SLIP_UNIT.CoderInfo.CustomStorageClass = 'Define';

MIN_SPEED = Simulink.Parameter(uint16(50));      % deadband: below this, slip = 0
MIN_SPEED.CoderInfo.StorageClass = 'Custom';
MIN_SPEED.CoderInfo.CustomStorageClass = 'Define';

assignin('base','SLIP_UNIT',SLIP_UNIT);
assignin('base','MIN_SPEED',MIN_SPEED);

%% ---------------------------------------------------------------------
%  1) Build the SWC model
% ---------------------------------------------------------------------
mdl = 'Slip_Calculation';
if bdIsLoaded(mdl); close_system(mdl,0); end
new_system(mdl);
open_system(mdl);

add = @(lib,name,pos) add_block(lib,[mdl '/' name],'Position',pos);

% --- Root inports ---
add('simulink/Sources/In1','throttle',[40 60 80 80]);
set_param([mdl '/throttle'],'OutDataTypeStr','uint16','PortDimensions','1');

add('simulink/Sources/In1','wheel_speed',[40 160 80 180]);
set_param([mdl '/wheel_speed'],'OutDataTypeStr','uint16','PortDimensions','1');

% --- speed_error = throttle - wheel_speed  (SIGNED int32: can go negative) ---
add('simulink/Math Operations/Subtract','speed_error',[160 100 200 140]);
set_param([mdl '/speed_error'],'OutDataTypeStr','int32');

% --- to_milli = speed_error * SLIP_UNIT   (int32; max 1000*1000 fits) ---
add('simulink/Math Operations/Gain','to_milli',[260 105 300 135]);
set_param([mdl '/to_milli'],'Gain','SLIP_UNIT', ...
    'OutDataTypeStr','int32','RndMeth','Zero','SaturateOnIntegerOverflow','off');

% --- Divisor protection: divisor = max(throttle, MIN_SPEED) -> never 0 ---
add('simulink/Sources/Constant','min_speed_const',[160 220 220 240]);
set_param([mdl '/min_speed_const'],'Value','MIN_SPEED','OutDataTypeStr','uint16');

add('simulink/Math Operations/MinMax','divisor_protect',[260 190 300 240]);
set_param([mdl '/divisor_protect'],'Function','max','Inputs','2', ...
    'OutDataTypeStr','int32');

% --- slip_ratio = to_milli / divisor_protect  (truncate toward zero = C) ---
add('simulink/Math Operations/Divide','slip_ratio',[360 120 400 170]);
set_param([mdl '/slip_ratio'],'Inputs','*/','OutDataTypeStr','int32', ...
    'RndMeth','Zero','SaturateOnIntegerOverflow','off');

% --- Guard: throttle >= MIN_SPEED ? slip_ratio : 0 ---
add('simulink/Logic and Bit Operations/Relational Operator','speed_ok',[360 240 400 270]);
set_param([mdl '/speed_ok'],'Operator','>=');

add('simulink/Sources/Constant','zero_slip',[360 320 400 340]);
set_param([mdl '/zero_slip'],'Value','0','OutDataTypeStr','int32');

add('simulink/Signal Routing/Switch','guard_switch',[460 150 500 300]);
set_param([mdl '/guard_switch'],'Criteria','u2 ~= 0','OutDataTypeStr','int32');

% --- Clamp [0,1000], output uint16 ---
add('simulink/Discontinuities/Saturation','slip_clamp',[560 205 610 245]);
set_param([mdl '/slip_clamp'],'LowerLimit','0','UpperLimit','1000', ...
    'OutDataTypeStr','uint16');

add('simulink/Sinks/Out1','local_wheel_slip',[660 215 700 235]);
set_param([mdl '/local_wheel_slip'],'OutDataTypeStr','uint16');

% --- Wiring ---
add_line(mdl,'throttle/1','speed_error/1','autorouting','on');
add_line(mdl,'wheel_speed/1','speed_error/2','autorouting','on');
add_line(mdl,'speed_error/1','to_milli/1','autorouting','on');
add_line(mdl,'to_milli/1','slip_ratio/1','autorouting','on');

add_line(mdl,'throttle/1','divisor_protect/1','autorouting','on');
add_line(mdl,'min_speed_const/1','divisor_protect/2','autorouting','on');
add_line(mdl,'divisor_protect/1','slip_ratio/2','autorouting','on');

add_line(mdl,'throttle/1','speed_ok/1','autorouting','on');
add_line(mdl,'min_speed_const/1','speed_ok/2','autorouting','on');

add_line(mdl,'slip_ratio/1','guard_switch/1','autorouting','on');
add_line(mdl,'speed_ok/1','guard_switch/2','autorouting','on');
add_line(mdl,'zero_slip/1','guard_switch/3','autorouting','on');

add_line(mdl,'guard_switch/1','slip_clamp/1','autorouting','on');
add_line(mdl,'slip_clamp/1','local_wheel_slip/1','autorouting','on');

%% ---------------------------------------------------------------------
%  Model configuration: fixed-step discrete + Embedded Coder + C++ + integer-only
% ---------------------------------------------------------------------
cs = getActiveConfigSet(mdl);

% Solver: discrete, 10 ms (100 Hz control period)
set_param(cs,'SolverType','Fixed-step');
set_param(cs,'Solver','FixedStepDiscrete');
set_param(cs,'FixedStep','0.01');

% Embedded Coder, C++, integer-only
set_param(cs,'SystemTargetFile','ert.tlc');
set_param(cs,'TargetLang','C++');
set_param(cs,'PurelyIntegerCode','on');       % enforce NO floating point
set_param(cs,'SupportNonFinite','off');
set_param(cs,'CodeInterfacePackaging','Nonreusable function');
set_param(cs,'GenerateReport','on');

% Target hardware = ARM Cortex-M (correct integer widths for H7 and C031)
try
    set_param(cs,'ProdHWDeviceType','ARM Compatible->ARM Cortex');
catch
    warning('Set the target hardware manually: Config > Hardware Implementation > ARM Cortex-M.');
end

% Function name of the generated step -> stable, weavable by AspectC++
set_param(cs,'CustomSymbolStrFcn','SlipCalculation_$N');

save_system(mdl);
fprintf('Built %s.slx\n', mdl);

%% ---------------------------------------------------------------------
%  2) Build a standalone test harness (sources + scope). NOT part of the SWC.
%     throttle: ramps 0..1000; wheel_speed: a fixed slower value -> slip > 0.
% ---------------------------------------------------------------------
harness = 'Slip_Calculation_Harness';
if bdIsLoaded(harness); close_system(harness,0); end
new_system(harness);
open_system(harness);

addh = @(lib,name,pos) add_block(lib,[harness '/' name],'Position',pos);

% Reference the SWC (so the harness cannot pollute the generated code)
addh('simulink/Ports & Subsystems/Model','SWC_Under_Test',[300 80 460 180]);
set_param([harness '/SWC_Under_Test'],'ModelName',mdl);

% throttle source: ramp 0..1000 over the sim, saturated + cast to uint16
addh('simulink/Sources/Ramp','throttle_ramp',[40 60 70 90]);
set_param([harness '/throttle_ramp'],'slope','200','start','0');   % 0..1000 over 5 s
addh('simulink/Discontinuities/Saturation','throttle_sat',[110 60 150 90]);
set_param([harness '/throttle_sat'],'LowerLimit','0','UpperLimit','1000', ...
    'OutDataTypeStr','uint16');

% wheel_speed source: constant slower than throttle so slip is visible
addh('simulink/Sources/Constant','wheel_speed_src',[110 140 170 170]);
set_param([harness '/wheel_speed_src'],'Value','300','OutDataTypeStr','uint16');

% Scope on the slip output
addh('simulink/Sinks/Scope','slip_scope',[560 110 600 150]);

add_line(harness,'throttle_ramp/1','throttle_sat/1','autorouting','on');
add_line(harness,'throttle_sat/1','SWC_Under_Test/1','autorouting','on');
add_line(harness,'wheel_speed_src/1','SWC_Under_Test/2','autorouting','on');
add_line(harness,'SWC_Under_Test/1','slip_scope/1','autorouting','on');

set_param(harness,'SolverType','Fixed-step','Solver','FixedStepDiscrete', ...
    'FixedStep','0.01','StopTime','5');

save_system(harness);
fprintf('Built %s.slx\n', harness);

fprintf('\nNext:\n');
fprintf('  1) Simulate the harness:   sim(''%s'')  and open the scope.\n', harness);
fprintf('     Expected: while throttle < 300 -> slip = 0; at throttle=600 -> slip=500; rising toward ~700.\n');
fprintf('  2) Generate C++:           open %s, press Ctrl+B (or slbuild(''%s'')).\n', mdl, mdl);
fprintf('  3) Check the report: one integer step function, NO float, NO malloc.\n');
