cmake_minimum_required(VERSION 3.10)
project(Ecc C)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_C_STANDARD 99)

add_definitions( -std=gnu99)
add_executable(Elio-compiler
        main.c
        vm.c
        IS.c
        lexical.c
        grammar.c)