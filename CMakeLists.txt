cmake_minimum_required(VERSION 3.16)

project(gloomy_learnopengl)

if(WIN32)
    set(GLFW_PATH "C:/Path/glfw-3.3.2.bin.WIN64")
    set(GLFW_INCLUDE_DIRS ${GLFW_PATH}/include/)
    set(GLFW_LIB_PATH ${GLFW_PATH}/lib-vc2019/)
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
endif()

add_subdirectory(vendor/stb)
add_subdirectory(vendor/glm)
add_subdirectory(tutorials/getting_started)