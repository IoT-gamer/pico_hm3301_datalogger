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
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")


include(FetchContent)
FetchContent_Declare(
    sd_card_pico
    GIT_REPOSITORY https://github.com/carlk3/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico.git
    GIT_TAG main
)
# Find out where FetchContent will put the files
FetchContent_GetProperties(sd_card_pico)
# If the files aren't already downloaded, download them
if(NOT sd_card_pico_POPULATED)
    FetchContent_Populate(sd_card_pico)
endif()

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_hm3301_datalogger C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

FetchContent_Declare(
    hm3301_pico
    GIT_REPOSITORY https://github.com/IoT-gamer/pico-hm3301.git
    GIT_TAG        main # Recommended: Replace with specific commit hash or tag
)

FetchContent_MakeAvailable(hm3301_pico)


# Add sd_card_pico library source directory to the build
add_subdirectory(${sd_card_pico_SOURCE_DIR}/src ${sd_card_pico_BINARY_DIR}/src)

# Add executable. Default name is the project name, version 0.1

add_executable(pico_hm3301_datalogger 
        main.cpp
        ble_server.c
        hw_config.c
        sd_logger.c
)

# Process .gatt file into a C header
pico_btstack_make_gatt_header(pico_hm3301_datalogger PRIVATE "${CMAKE_CURRENT_LIST_DIR}/datalogger.gatt")

# target_compile_definitions(pico_hm3301_datalogger PRIVATE
#     RUNNING_AS_CLIENT=0 # Set to 1 to run as client, 0 for server
# )

pico_set_program_name(pico_hm3301_datalogger "pico_hm3301_datalogger")
pico_set_program_version(pico_hm3301_datalogger "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pico_hm3301_datalogger 0)
pico_enable_stdio_usb(pico_hm3301_datalogger 1)

# Add the standard library to the build
target_link_libraries(pico_hm3301_datalogger
        PRIVATE
        pico_stdlib
        pico_cyw43_arch_none
        pico_btstack_ble
        pico_btstack_cyw43
        
        hardware_pio
        hardware_dma
        hardware_spi
        no-OS-FatFS-SD-SDIO-SPI-RPi-Pico

        hardware_i2c
        hm3301_pico

        pico_aon_timer
)

# Add the standard include files to the build
target_include_directories(pico_hm3301_datalogger PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(pico_hm3301_datalogger)

