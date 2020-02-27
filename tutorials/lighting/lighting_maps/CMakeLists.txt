cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_lighting_maps ${SOURCES})
set_target_properties(learnopengl_lighting_maps PROPERTIES
    OUTPUT_NAME "lighting_maps")

target_include_directories(learnopengl_lighting_maps PUBLIC ./include/)

target_link_libraries(learnopengl_lighting_maps glfw)
target_link_libraries(learnopengl_lighting_maps gloomy)
target_link_libraries(learnopengl_lighting_maps stb)
target_link_libraries(learnopengl_lighting_maps glm)

set_target_properties(learnopengl_lighting_maps PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")