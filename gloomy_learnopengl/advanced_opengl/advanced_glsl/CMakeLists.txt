cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_advanced_glsl ${SOURCES})
set_target_properties(learnopengl_advanced_glsl PROPERTIES
    OUTPUT_NAME "advanced_glsl")

target_include_directories(learnopengl_advanced_glsl PUBLIC ./include/)

target_link_libraries(learnopengl_advanced_glsl glfw)
target_link_libraries(learnopengl_advanced_glsl gloomy)
target_link_libraries(learnopengl_advanced_glsl stb)
target_link_libraries(learnopengl_advanced_glsl glm)

set_target_properties(learnopengl_advanced_glsl PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")