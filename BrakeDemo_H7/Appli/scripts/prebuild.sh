#!/bin/sh

echo "============== PREBUILD ============================="

ARM_GPP="C:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin\arm-none-eabi-g++.exe"

PROJ="${1:-..}"
ROOT=$(cd "$PROJ/../.." && pwd)
APPLICATION_SRC="$ROOT/MATLAB_ApplicationCode"
ASPECT_SRC="$PROJ/Platform_Aspects"
OUT="$PROJ/Debug/Weaved_Output"

echo "[PRE-BUILD] Project: $PROJ"
echo "[PRE-BUILD] ROOT: $ROOT"
echo "[PRE-BUILD] Source folder: $APPLICATION_SRC"
echo "[PRE-BUILD] Aspect folder: $ASPECT_SRC"
echo "[PRE-BUILD] Output folder: $OUT"
echo ARM_GPP=$ARM_GPP

if [ ! -d "$APPLICATION_SRC" ]; then
	echo "[PRE-BUILD] ERROR: Source folder does not exist on disk:"
	echo " $APPLICATION_SRC"
	exit 1
fi

mkdir -p "$OUT"

APPCODE_FOLDERS="$APPLICATION_SRC/Main_Model_Braking_Demo_ert_rtw"
if [ -d "$APPLICATION_SRC/slprj/ert" ]; then
	APPCODE_FOLDERS="$APPCODE_FOLDERS $(find "$APPLICATION_SRC/slprj/ert" -type d | sort)"
else
	echo "[PRE-BUILD] Note: $APPLICATION_SRC/slprj/ert not found - no referenced models."
fi
echo "APPCODE_FOLDERS: $APPCODE_FOLDERS"

PROJECT_INCLUDE_FLAGS="-I $PROJ/Core/Inc/Platform"
for include_folder in $APPCODE_FOLDERS
	do
		PROJECT_INCLUDE_FLAGS="$PROJECT_INCLUDE_FLAGS -I $include_folder"
	done
echo "PROJECT_INCLUDE_FLAGS: $PROJECT_INCLUDE_FLAGS" 


compiled_file_count=0

for appcode_folder in $APPCODE_FOLDERS
do
	echo ""
	echo "appcode_folder: $appcode_folder"
	for source_file in "$appcode_folder"/*.cpp
	do
		echo "source_file: $source_file"
		if [ ! -f "$source_file" ]; then
			continue
		fi
		
		source_filename=$(basename "$source_file")
		source_name="${source_filename%.cpp}"
		object_file="$OUT/${source_name}.o"
		
		echo ""
		echo "[PRE-BUILD] Weaving and compiling: $source_filename"
		
		rm -f "$object_file"
		
		"C:\aspectc++\ag++.exe" \
		  -p "$ROOT" \
		  -a "$ASPECT_SRC/platform_specification.ah" \
		  --c_compiler $ARM_GPP \
		  --keep_woven \
		  --data_joinpoints \
		  --builtin_operators \
		  --Xcompiler \
		  -c "$source_file" -o "$object_file" \
		  -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb \
		  -std=gnu++14 -O0 -g3 -ffunction-sections -fdata-sections \
		  -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
		  $PROJECT_INCLUDE_FLAGS

		if [ $? -ne 0 ]; then
			echo "[PRE-BUILD] FAILED"
		 	exit 1
		fi

		if [ ! -s "$object_file" ]; then
			echo "[PRE-BUILD] ERROR: Object file missing or empty: $object_file"
			exit 1
		fi

		echo "[PRE-BUILD] OK: $source_filename -> ${source_name}.o"

		compiled_file_count=$((compiled_file_count + 1))
	done
done

		echo "[PRE-BUILD] activation file: $PROJ/Core/Src/Platform/application_task_activation.c"
		"C:\aspectc++\ag++.exe" \
		  -p "$ROOT" \
		  -a "$ASPECT_SRC/platform_specification.ah" \
		  --c_compiler $ARM_GPP \
		  --keep_woven \
		  --data_joinpoints \
		  --builtin_operators \
		  --Xcompiler \
		  -c "$PROJ/Core/Src/Platform/application_task_activation.c" -o "$OUT/application_task_activation.o" \
		  -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -fno-threadsafe-statics \
		  -std=gnu++14 -O0 -g3 -ffunction-sections -fdata-sections \
		  -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
		  $PROJECT_INCLUDE_FLAGS
		  
		  if [ $? -ne 0 ]; then
			echo "[PRE-BUILD] FAILED"
		 	exit 1
		  fi

if [ "$compiled_file_count" -eq 0 ]; then
	echo "[PRE-BUILD] ERROR: No .cpp files found in $APPLICATION_SRC"
	exit 1
fi

echo ""
echo "============== PREBUILD COMPLETE ===================="
echo "[PRE-BUILD] Compiled files: $compiled_file_count"
echo "[PRE-BUILD] Objects in: $OUT"
echo "====================================================="

exit 0