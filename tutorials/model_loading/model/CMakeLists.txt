cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_model ${SOURCES})
set_target_properties(learnopengl_model PROPERTIES
    OUTPUT_NAME "model")

target_include_directories(learnopengl_model PUBLIC ./include/)

target_link_libraries(learnopengl_model glfw)
target_link_libraries(learnopengl_model gloomy)
target_link_libraries(learnopengl_model stb)
target_link_libraries(learnopengl_model glm)

set_target_properties(learnopengl_model PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")