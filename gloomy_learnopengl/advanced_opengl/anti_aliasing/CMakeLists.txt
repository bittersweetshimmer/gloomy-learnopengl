cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_anti_aliasing ${SOURCES})
set_target_properties(learnopengl_anti_aliasing PROPERTIES
    OUTPUT_NAME "anti_aliasing")

target_include_directories(learnopengl_anti_aliasing PUBLIC ./include/)

target_link_libraries(learnopengl_anti_aliasing glfw)
target_link_libraries(learnopengl_anti_aliasing gloomy)
target_link_libraries(learnopengl_anti_aliasing stb)
target_link_libraries(learnopengl_anti_aliasing glm)

set_target_properties(learnopengl_anti_aliasing PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")