cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_hello_triangle ${SOURCES})
set_target_properties(learnopengl_hello_triangle PROPERTIES
    OUTPUT_NAME "hello_triangle")

target_include_directories(learnopengl_hello_triangle PUBLIC ./include/)

target_link_libraries(learnopengl_hello_triangle glfw)
target_link_libraries(learnopengl_hello_triangle gloomy)

set_target_properties(learnopengl_hello_triangle PROPERTIES
                        CXX_STANDARD 20)