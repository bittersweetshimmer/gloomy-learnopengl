cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_cubemaps ${SOURCES})
set_target_properties(learnopengl_cubemaps PROPERTIES
    OUTPUT_NAME "cubemaps")

target_include_directories(learnopengl_cubemaps PUBLIC ./include/)

target_link_libraries(learnopengl_cubemaps glfw)
target_link_libraries(learnopengl_cubemaps gloomy)
target_link_libraries(learnopengl_cubemaps stb)
target_link_libraries(learnopengl_cubemaps glm)

set_target_properties(learnopengl_cubemaps PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")