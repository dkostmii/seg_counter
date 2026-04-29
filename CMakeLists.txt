cmake_minimum_required(VERSION 4.2)

# Specify cross-compilation systems
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
# This prevents CMake from trying to link a standard executable during testing
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Project Name
project(seg_counter C)

# Manually strip Apple-specific flags if they persist
set(CMAKE_C_LINK_FLAGS "")
set(CMAKE_CXX_LINK_FLAGS "")
set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")

include_directories(SYSTEM /opt/local/avr/include)

# Set MCU and Frequency (Arduino Uno)
set(MCU atmega328p)
set(F_CPU 16000000UL)

# --- Find AVR Tools ---
# Ensure these are in your path (installed via Homebrew)
find_program(AVR_CC avr-gcc)
find_program(AVR_CXX avr-g++)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_SIZE avr-size)

if(NOT AVR_CC)
    message(FATAL_ERROR "avr-gcc not found. Install via: brew install avr-gcc")
endif()

# Set Compiler to AVR
set(CMAKE_C_COMPILER ${AVR_CC})
set(CMAKE_CXX_COMPILER ${AVR_CXX})

# --- Compile Flags ---
set(FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os")
set(CMAKE_C_FLAGS "${FLAGS}")
set(CMAKE_CXX_FLAGS "${FLAGS}")

# --- Target ---
add_executable(${PROJECT_NAME} src/main.c)

# --- Post-build: Create HEX file ---
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom ${PROJECT_NAME} ${PROJECT_NAME}.hex
        COMMAND ${AVR_SIZE} ${PROJECT_NAME}
        COMMENT "Generating HEX file and calculating size"
)

# --- Optional: Add upload target (needs avrdude) ---
find_program(AVRDUDE avrdude)
add_custom_target(upload
    COMMAND ${AVRDUDE} -c arduino -p ${MCU} -P /dev/tty.usbserial-14330 -b 115200 -U flash:w:${PROJECT_NAME}.hex
    DEPENDS ${PROJECT_NAME}.hex
)
