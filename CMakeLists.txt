cmake_minimum_required(VERSION 3.15)
project(os_lab1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(Threads REQUIRED)

add_executable(main
    main.c
)
target_include_directories(main PRIVATE src)
target_link_libraries(main PRIVATE Threads::Threads)
