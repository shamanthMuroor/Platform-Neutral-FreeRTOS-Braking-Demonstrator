#!/bin/sh

echo "============== PREBUILD ============================="

ARM_GPP="C:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin\arm-none-eabi-g++.exe"

PROJ="${1:-..}"
ROOT="$PROJ/.."
SRC="$PROJ/ApplicationCode"
ASPECT_SRC="$PROJ/PlatformAspects"
OUT="$PROJ/Debug/WeavedOutput"


echo "[PRE-BUILD] Compiling SpeedEstimator.cpp (C++) -> SpeedEstimator.o"
echo ARM_GPP=$ARM_GPP

echo "============================="

"C:\aspectc++\ag++.exe" -v 9\
  -p "$PROJ" \
  -a "$ASPECT_SRC/PlatformSpecification.ah" \
  --c_compiler $ARM_GPP \
  --keep_woven \
  --data_joinpoints \
  --builtin_operators \
  --Xcompiler \
  -c "$SRC/SpeedEstimator.cpp" -o "$OUT/SpeedEstimator.o" \
  -mcpu=cortex-m0plus -mthumb \
  -std=gnu++14 -O0 -g3 -ffunction-sections -fdata-sections \
  -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
  -I "$PROJ/ApplicationCode/Inc"

if [ $? -ne 0 ]; then
  echo "[PRE-BUILD] FAILED"
  exit 1
fi

echo "[PRE-BUILD] OK -> Generated SpeedEstimator.o"

exit 0