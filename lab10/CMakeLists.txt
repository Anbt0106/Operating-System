cmake_minimum_required(VERSION 3.27)
project(Input_code C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Input_code
        clock.c
        clock.h
        config.h
        fifo.c
        fifo.h
        lru.c
        lru.h
        main.c
        ramEmu.c
        ramEmu.h
        virtualMem.c
        virtualMem.h)
