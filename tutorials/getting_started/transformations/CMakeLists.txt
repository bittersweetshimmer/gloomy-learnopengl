cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_transformations ${SOURCES})
set_target_properties(learnopengl_transformations PROPERTIES
    OUTPUT_NAME "transformations")

target_include_directories(learnopengl_transformations PUBLIC ./include/)

target_link_libraries(learnopengl_transformations glfw)
target_link_libraries(learnopengl_transformations gloomy)
target_link_libraries(learnopengl_transformations stb)
target_link_libraries(learnopengl_transformations glm)

set_target_properties(learnopengl_transformations PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")