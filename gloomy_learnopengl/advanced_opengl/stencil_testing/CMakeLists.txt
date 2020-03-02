cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_stencil_testing ${SOURCES})
set_target_properties(learnopengl_stencil_testing PROPERTIES
    OUTPUT_NAME "stencil_testing")

target_include_directories(learnopengl_stencil_testing PUBLIC ./include/)

target_link_libraries(learnopengl_stencil_testing glfw)
target_link_libraries(learnopengl_stencil_testing gloomy)
target_link_libraries(learnopengl_stencil_testing stb)
target_link_libraries(learnopengl_stencil_testing glm)

set_target_properties(learnopengl_stencil_testing PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")