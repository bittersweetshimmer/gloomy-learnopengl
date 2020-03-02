cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_advanced_data ${SOURCES})
set_target_properties(learnopengl_advanced_data PROPERTIES
    OUTPUT_NAME "advanced_data")

target_include_directories(learnopengl_advanced_data PUBLIC ./include/)

target_link_libraries(learnopengl_advanced_data glfw)
target_link_libraries(learnopengl_advanced_data gloomy)
target_link_libraries(learnopengl_advanced_data stb)
target_link_libraries(learnopengl_advanced_data glm)

set_target_properties(learnopengl_advanced_data PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")