cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_basic_lighting ${SOURCES})
set_target_properties(learnopengl_basic_lighting PROPERTIES
    OUTPUT_NAME "basic_lighting")

target_include_directories(learnopengl_basic_lighting PUBLIC ./include/)

target_link_libraries(learnopengl_basic_lighting glfw)
target_link_libraries(learnopengl_basic_lighting gloomy)
target_link_libraries(learnopengl_basic_lighting stb)
target_link_libraries(learnopengl_basic_lighting glm)

set_target_properties(learnopengl_basic_lighting PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")