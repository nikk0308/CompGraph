cmake_minimum_required(VERSION 3.28)
project(Lab_2)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${CMAKE_SOURCE_DIR}/libs/freeglut/include
)
link_directories(
        ${CMAKE_SOURCE_DIR}/libs/freeglut/lib/x64
)

add_executable(Lab_2 main.cpp)
target_link_libraries(Lab_2 opengl32 freeglut glu32)