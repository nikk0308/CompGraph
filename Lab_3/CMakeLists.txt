cmake_minimum_required(VERSION 3.28)

project(Lab_3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
)

link_directories(
        ${CMAKE_SOURCE_DIR}/bin
)

add_executable(Lab_3
        main.cpp
        src/glad.c
        Skybox.h
        Skybox.cpp
        Camera.h
        Camera.cpp
        Loader.h
        Loader.cpp
        Planet.h
        Variables.h
        Variables.cpp
        Planet.cpp
)

target_link_libraries(Lab_3
        glfw3
        SOIL2
        opengl32
)

add_custom_command(TARGET Lab_3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/bin/glfw3.dll
        $<TARGET_FILE_DIR:Lab_3>
)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Lab_3>/resources
        DEPENDS ${CMAKE_SOURCE_DIR}/resources
)

add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:Lab_3>/shaders
        DEPENDS ${CMAKE_SOURCE_DIR}/shaders
)

add_dependencies(Lab_3 copy_resources copy_shaders)
