cmake_minimum_required(VERSION 3.16)

set(SOURCES "src/main.cpp")

add_executable(learnopengl_light_casters ${SOURCES})
set_target_properties(learnopengl_light_casters PROPERTIES
    OUTPUT_NAME "light_casters")

target_include_directories(learnopengl_light_casters PUBLIC ./include/)

target_link_libraries(learnopengl_light_casters glfw)
target_link_libraries(learnopengl_light_casters gloomy)
target_link_libraries(learnopengl_light_casters stb)
target_link_libraries(learnopengl_light_casters glm)

set_target_properties(learnopengl_light_casters PROPERTIES
                        CXX_STANDARD 20)

file(COPY "assets" DESTINATION ".")