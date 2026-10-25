set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Arm'ın kendi toolchain'ini /opt altına kurduysan: export ARM_TOOLCHAIN_PATH=/opt/arm-gnu/bin
if(DEFINED ENV{ARM_TOOLCHAIN_PATH})
    set(_tc "$ENV{ARM_TOOLCHAIN_PATH}/")
endif()
set(_prefix "${_tc}arm-none-eabi-")

set(CMAKE_C_COMPILER   ${_prefix}gcc)
set(CMAKE_ASM_COMPILER ${_prefix}gcc)
set(CMAKE_OBJCOPY      ${_prefix}objcopy CACHE INTERNAL "")
set(CMAKE_SIZE         ${_prefix}size    CACHE INTERNAL "")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
# Linker script olmadan test programı bağlanamaz; sadece derleme dene
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_FLAGS_DEBUG_INIT          "-Og -g3")
set(CMAKE_C_FLAGS_RELEASE_INIT        "-Os -g0 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-Os -g3 -DNDEBUG")