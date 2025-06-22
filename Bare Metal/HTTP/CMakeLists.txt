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

project(Comunicacao_http C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Comunicacao_http Comunicacao_http.c inc_WIFI/WIFI_lib.c cJSON/cJSON.c)

pico_set_program_name(Comunicacao_http "Comunicacao_http")
pico_set_program_version(Comunicacao_http "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Comunicacao_http 0)
pico_enable_stdio_usb(Comunicacao_http 1)

# Add the standard library to the build
target_link_libraries(Comunicacao_http
        pico_stdlib
        pico_lwip_http
        pico_cyw43_arch_lwip_threadsafe_background  
        )

# Add the standard include files to the build
target_include_directories(Comunicacao_http PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(Comunicacao_http)

