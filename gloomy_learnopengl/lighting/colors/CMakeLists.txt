cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_colors ${SOURCES})
set_target_properties(learnopengl_colors PROPERTIES
    OUTPUT_NAME "colors")

target_include_directories(learnopengl_colors PUBLIC ./include/)

target_link_libraries(learnopengl_colors glfw)
target_link_libraries(learnopengl_colors gloomy)
target_link_libraries(learnopengl_colors stb)
target_link_libraries(learnopengl_colors glm)

set_target_properties(learnopengl_colors PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")