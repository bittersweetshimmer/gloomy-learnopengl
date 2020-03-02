cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_instancing ${SOURCES})
set_target_properties(learnopengl_instancing PROPERTIES
    OUTPUT_NAME "instancing")

target_include_directories(learnopengl_instancing PUBLIC ./include/)

target_link_libraries(learnopengl_instancing glfw)
target_link_libraries(learnopengl_instancing gloomy)
target_link_libraries(learnopengl_instancing stb)
target_link_libraries(learnopengl_instancing glm)

set_target_properties(learnopengl_instancing PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")