# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)


project(RTOS_LED_RGB C CXX ASM)


# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(RTOS_LED_RGB RTOS_LED_RGB.c inc_LEDRGB/LED_RGB_lib.c)

pico_set_program_name(RTOS_LED_RGB "RTOS_LED_RGB")
pico_set_program_version(RTOS_LED_RGB "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(RTOS_LED_RGB 0)
pico_enable_stdio_usb(RTOS_LED_RGB 1)

# Add the standard library to the build
target_link_libraries(RTOS_LED_RGB
        pico_stdlib
        FreeRTOS-Kernel 
        FreeRTOS-Kernel-Heap4
        hardware_pwm
        )

# Add the standard include files to the build
target_include_directories(RTOS_LED_RGB PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(RTOS_LED_RGB)

