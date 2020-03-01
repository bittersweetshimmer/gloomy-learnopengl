cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_camera ${SOURCES})
set_target_properties(learnopengl_camera PROPERTIES
    OUTPUT_NAME "camera")

target_include_directories(learnopengl_camera PUBLIC ./include/)

target_link_libraries(learnopengl_camera glfw)
target_link_libraries(learnopengl_camera gloomy)
target_link_libraries(learnopengl_camera stb)
target_link_libraries(learnopengl_camera glm)

set_target_properties(learnopengl_camera PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")