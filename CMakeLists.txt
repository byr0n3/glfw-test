cmake_minimum_required(VERSION 3.27)
project(game_test)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_subdirectory(external)

include_directories(external/glfw/include/ external/glew/include/ .)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

set(ALL_LIBS ${OPENGL_LIBRARY} glfw GLEW)

add_executable(game_test main.cpp
        shaders.cpp include/shaders.h
)

target_link_libraries(game_test ${ALL_LIBS})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
