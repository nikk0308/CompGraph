cmake_minimum_required(VERSION 3.28)
project(Lab_1)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${CMAKE_SOURCE_DIR}/libs/glfw/include
)
link_directories(
        ${CMAKE_SOURCE_DIR}/libs/glfw/lib-mingw-w64
)

add_executable(Lab_1 main.cpp)
target_link_libraries(Lab_1 opengl32 glfw3)