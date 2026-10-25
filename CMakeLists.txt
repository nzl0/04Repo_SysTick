cmake_minimum_required(VERSION 3.22)
project(systick LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32F407VGTx_FLASH.ld)
set(MCU_FLAGS -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16)

add_executable(${PROJECT_NAME}
    src/main.c
    startup/startup_stm32f407xx.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")


target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    STM32F407xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -Wall -Wextra -Wshadow
    -ffunction-sections -fdata-sections
)

target_link_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -T${LINKER_SCRIPT}
    --specs=nano.specs --specs=nosys.specs
    -nostartfiles
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
)

find_program(OPENOCD openocd)
if(OPENOCD)
    add_custom_target(flash
        COMMAND ${OPENOCD} -f ${CMAKE_SOURCE_DIR}/openocd.cfg
                -c "program $<TARGET_FILE:${PROJECT_NAME}> verify reset exit"
        DEPENDS ${PROJECT_NAME}
        USES_TERMINAL
    )
endif()