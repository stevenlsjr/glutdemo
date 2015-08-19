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

    src/macros/applyall.h
    src/macros/constants.h
    src/macros/dbg.h
    src/macros/linmath.h)