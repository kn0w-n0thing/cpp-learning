cmake_minimum_required(VERSION 3.21)
project(fruit-singleton)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HELLO_WORLD_SOURCES
main.cpp
)

add_executable(hello ${HELLO_WORLD_SOURCES})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    execute_process(
            COMMAND brew --prefix fruit
            OUTPUT_VARIABLE FRUIT_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    include_directories(${FRUIT_PREFIX}/include)

    target_link_libraries(hello ${FRUIT_PREFIX}/lib/libfruit.dylib)
else ()
    target_link_libraries(hello fruit)
endif ()
