cmake_minimum_required(VERSION 3.15)
project(os_lab1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# по умолчанию Debug (-g), чтобы gdb показывал исходники; Release через -DCMAKE_BUILD_TYPE=Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(Threads REQUIRED)

add_executable(main
    src/main.c
    src/app.c
    src/lock.c
    src/utils.c
    src/sharedMemoryModule.c
)
target_include_directories(main PRIVATE src)
target_link_libraries(main PRIVATE Threads::Threads)
