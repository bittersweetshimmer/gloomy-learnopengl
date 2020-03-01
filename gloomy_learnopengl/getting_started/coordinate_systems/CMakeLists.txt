cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_coordinate_systems ${SOURCES})
set_target_properties(learnopengl_coordinate_systems PROPERTIES
    OUTPUT_NAME "coordinate_systems")

target_include_directories(learnopengl_coordinate_systems PUBLIC ./include/)

target_link_libraries(learnopengl_coordinate_systems glfw)
target_link_libraries(learnopengl_coordinate_systems gloomy)
target_link_libraries(learnopengl_coordinate_systems stb)
target_link_libraries(learnopengl_coordinate_systems glm)

set_target_properties(learnopengl_coordinate_systems PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")