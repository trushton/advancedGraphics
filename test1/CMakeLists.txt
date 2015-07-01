cmake_minimum_required(VERSION 3.2)
project(firstTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -L/usr/lib/nvidia-304 -lMagick++")

set(SOURCE_FILES main.cpp shader.cpp )
add_executable(firstTest ${SOURCE_FILES}  camera.h Texture.h)

INCLUDE(FindPkgConfig)
include(FindGLEW)

pkg_check_modules(GL REQUIRED gl)
find_package(GLUT REQUIRED)
find_package(ImageMagick)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/Desktop/advancedGraphics/test1")

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

include(FindBullet)
include_directories(${BULLET_INCLUDE_DIRS})

target_link_libraries(firstTest ${OPENGL_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} GL glut GLEW assimp )
