cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_materials ${SOURCES})
set_target_properties(learnopengl_materials PROPERTIES
    OUTPUT_NAME "materials")

target_include_directories(learnopengl_materials PUBLIC ./include/)

target_link_libraries(learnopengl_materials glfw)
target_link_libraries(learnopengl_materials gloomy)
target_link_libraries(learnopengl_materials stb)
target_link_libraries(learnopengl_materials glm)

set_target_properties(learnopengl_materials PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")