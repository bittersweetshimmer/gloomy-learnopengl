cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_depth_testing ${SOURCES})
set_target_properties(learnopengl_depth_testing PROPERTIES
    OUTPUT_NAME "depth_testing")

target_include_directories(learnopengl_depth_testing PUBLIC ./include/)

target_link_libraries(learnopengl_depth_testing glfw)
target_link_libraries(learnopengl_depth_testing gloomy)
target_link_libraries(learnopengl_depth_testing stb)
target_link_libraries(learnopengl_depth_testing glm)

set_target_properties(learnopengl_depth_testing PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")