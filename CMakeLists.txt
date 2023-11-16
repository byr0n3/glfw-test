cmake_minimum_required(VERSION 3.27)
project(game_test)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_subdirectory(external)

include_directories(
        .
        external/glfw/include/
        external/glew/include/
        external/glm/
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

set(ALL_LIBS ${OPENGL_LIBRARY} glfw GLEW)

add_executable(
        game_test src/main.cpp
        src/debug.h
        src/game.cpp include/game.h
        src/game_object.cpp include/game_object.h
        src/level.cpp include/level.h
        src/shader.cpp include/shader.h
        src/texture2d.cpp include/texture2d.h
        src/sprite_renderer.cpp include/sprite_renderer.h
        src/resource_manager.cpp include/resource_manager.h

        external/stb_image.h
)

target_link_libraries(game_test ${ALL_LIBS})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
