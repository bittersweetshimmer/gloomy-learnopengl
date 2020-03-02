cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_geometry_shader ${SOURCES})
set_target_properties(learnopengl_geometry_shader PROPERTIES
    OUTPUT_NAME "geometry_shader")

target_include_directories(learnopengl_geometry_shader PUBLIC ./include/)

target_link_libraries(learnopengl_geometry_shader glfw)
target_link_libraries(learnopengl_geometry_shader gloomy)
target_link_libraries(learnopengl_geometry_shader stb)
target_link_libraries(learnopengl_geometry_shader glm)

set_target_properties(learnopengl_geometry_shader PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")