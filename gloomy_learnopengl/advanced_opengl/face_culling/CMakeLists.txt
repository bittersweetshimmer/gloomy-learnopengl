cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_face_culling ${SOURCES})
set_target_properties(learnopengl_face_culling PROPERTIES
    OUTPUT_NAME "face_culling")

target_include_directories(learnopengl_face_culling PUBLIC ./include/)

target_link_libraries(learnopengl_face_culling glfw)
target_link_libraries(learnopengl_face_culling gloomy)
target_link_libraries(learnopengl_face_culling stb)
target_link_libraries(learnopengl_face_culling glm)

set_target_properties(learnopengl_face_culling PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")