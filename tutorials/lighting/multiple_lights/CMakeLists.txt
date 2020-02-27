cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_multiple_lights ${SOURCES})
set_target_properties(learnopengl_multiple_lights PROPERTIES
    OUTPUT_NAME "multiple_lights")

target_include_directories(learnopengl_multiple_lights PUBLIC ./include/)

target_link_libraries(learnopengl_multiple_lights glfw)
target_link_libraries(learnopengl_multiple_lights gloomy)
target_link_libraries(learnopengl_multiple_lights stb)
target_link_libraries(learnopengl_multiple_lights glm)

set_target_properties(learnopengl_multiple_lights PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")