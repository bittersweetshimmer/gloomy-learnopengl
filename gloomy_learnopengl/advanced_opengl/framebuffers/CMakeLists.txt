cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_framebuffers ${SOURCES})
set_target_properties(learnopengl_framebuffers PROPERTIES
    OUTPUT_NAME "framebuffers")

target_include_directories(learnopengl_framebuffers PUBLIC ./include/)

target_link_libraries(learnopengl_framebuffers glfw)
target_link_libraries(learnopengl_framebuffers gloomy)
target_link_libraries(learnopengl_framebuffers stb)
target_link_libraries(learnopengl_framebuffers glm)

set_target_properties(learnopengl_framebuffers PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")