#!/bin/sh

echo "============== PREBUILD ============================="

ARM_GPP="C:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin\arm-none-eabi-g++.exe"

PROJ="${1:-..}"
ROOT="$PROJ/.."
APPLI_SRC="$PROJ/Application_Code"
ASPECT_SRC="$PROJ/Platform_Aspects"
OUT="$PROJ/Debug/Weaved_Output"


echo "[PRE-BUILD] Compiling ThrottleControl.cpp (C++) -> ThrottleControl.o"
echo ARM_GPP=$ARM_GPP

echo "============================="

"C:\aspectc++\ag++.exe" -v9\
  -p "$PROJ" \
  -a "$ASPECT_SRC/platform_specification.ah" \
  --c_compiler $ARM_GPP \
  --keep_woven \
  --data_joinpoints \
  --builtin_operators \
  --Xcompiler \
  -c "$APPLI_SRC/Src/ThrottleControl.cpp" -o "$OUT/ThrottleControl.o" \
  -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb \
  -std=gnu++14 -O0 -g3 -ffunction-sections -fdata-sections \
  -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
  -I "$APPLI_SRC/Inc" \
  -I "$PROJ/Core/Inc/Platform"

if [ $? -ne 0 ]; then
  echo "[PRE-BUILD] FAILED"
  exit 1
fi

echo "[PRE-BUILD] OK -> Generated ThrottleControl.o"

exit 0