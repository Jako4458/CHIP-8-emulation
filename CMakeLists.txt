cmake_minimum_required(VERSION 3.16)

project(MyProject LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(chip-8-emulation
    src/main.c
    src/ctx.c
    src/constants.h
)

# target_compile_options(chip-8-emulation PRIVATE
	# -Wall
    # -Wextra
    # -Wpedantic
    # )

target_include_directories(chip-8-emulation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
