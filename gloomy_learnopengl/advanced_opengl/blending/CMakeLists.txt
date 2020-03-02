cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_blending ${SOURCES})
set_target_properties(learnopengl_blending PROPERTIES
    OUTPUT_NAME "blending")

target_include_directories(learnopengl_blending PUBLIC ./include/)

target_link_libraries(learnopengl_blending glfw)
target_link_libraries(learnopengl_blending gloomy)
target_link_libraries(learnopengl_blending stb)
target_link_libraries(learnopengl_blending glm)

set_target_properties(learnopengl_blending PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")