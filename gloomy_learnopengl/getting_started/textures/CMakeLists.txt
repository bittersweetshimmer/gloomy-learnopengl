cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_textures ${SOURCES})
set_target_properties(learnopengl_textures PROPERTIES
    OUTPUT_NAME "textures")

target_include_directories(learnopengl_textures PUBLIC ./include/)

target_link_libraries(learnopengl_textures glfw)
target_link_libraries(learnopengl_textures gloomy)
target_link_libraries(learnopengl_textures stb)

set_target_properties(learnopengl_textures PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")