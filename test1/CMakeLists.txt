cmake_minimum_required(VERSION 3.2)
project(firstTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -L/usr/lib/nvidia-304 -I/usr/include/ImageMagick/ ")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/Desktop/advancedGraphics/test1")


set(SOURCE_FILES main.cpp shader.cpp )
add_executable(firstTest ${SOURCE_FILES} camera.h shader.h Mesh.cpp Mesh.h Vertex.h Model.cpp Model.h gbuffer.cpp gbuffer.h)

INCLUDE(FindPkgConfig)
include(FindGLEW)

pkg_check_modules(GL REQUIRED gl)
find_package(GLUT REQUIRED)
find_package(ImageMagick COMPONENTS Magick++)



find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

include(FindBullet)
include_directories(${BULLET_INCLUDE_DIRS})

target_link_libraries(firstTest ${OPENGL_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${ImageMagick_MAGICK++_INCLUDE_DIRS} Magick++ freeimageplus  GL glut GLEW assimp )
