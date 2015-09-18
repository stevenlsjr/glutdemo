cmake_minimum_required(VERSION 2.8)

project(glutdemo)

set(GLUTDEMO_SRC
    src/image.c
    src/image.h
    src/main.c
    src/mesh.c
    src/mesh.h
    src/renderer.c
    src/renderer.h
    src/shaderutils.c
    src/shaderutils.h
    src/transform.c
    src/transform.h
    src/scene.c
    src/scene.h

    src/macros/applyall.h
    src/macros/constants.h
    src/macros/dbg.h
    src/macros/linmath.h)

find_package(OpenGL)
find_package(Glut)
find_package(Glew)
find_package(DevIL)

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -std=gnu11")

set(DEPENDENCIES
    ${OPENGL_LIBRARIES}
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${ILUT_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLUT_LIBRARY})

include_directories(
    ${IL_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    )

add_executable(glutdemo_bin
    ${GLUTDEMO_SRC})

target_link_libraries(
    glutdemo_bin
    ${DEPENDENCIES})

add_custom_command(
    TARGET glutdemo_bin PRE_BUILD
    COMMAND
    ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources &&
    ${CMAKE_COMMAND} -E cmake_echo_color --green
    "--    copying resources/ to ${CMAKE_BINARY_DIR}"
    COMMENT "")


