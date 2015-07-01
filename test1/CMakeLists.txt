cmake_minimum_required(VERSION 3.2)
project(firstTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -L/usr/lib/nvidia-346.46 -I/usr/include/ImageMagick/ `Magick++-config --cppflags --cxxflags --ldflags --libs`")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/Desktop/advancedGraphics/test1")


set(SOURCE_FILES main.cpp shader.cpp Texture.cpp)
add_executable(firstTest ${SOURCE_FILES} Texture.h camera.h shader.h)

INCLUDE(FindPkgConfig)
include(FindGLEW)

pkg_check_modules(GL REQUIRED gl)
find_package(GLUT REQUIRED)
find_package(ImageMagick)



find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

include(FindBullet)
include_directories(${BULLET_INCLUDE_DIRS})

target_link_libraries(firstTest ${OPENGL_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${ImageMagick_LIBRARIES} freeimageplus  GL glut GLEW assimp )
