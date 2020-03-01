cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_shaders ${SOURCES})
set_target_properties(learnopengl_shaders PROPERTIES
    OUTPUT_NAME "shaders")

target_include_directories(learnopengl_shaders PUBLIC ./include/)

target_link_libraries(learnopengl_shaders glfw)
target_link_libraries(learnopengl_shaders gloomy)

set_target_properties(learnopengl_shaders PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")