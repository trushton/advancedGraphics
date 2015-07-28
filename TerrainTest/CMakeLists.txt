cmake_minimum_required(VERSION 3.2)
project(TerrainTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -L/usr/lib/nvidia-304")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/Desktop/advancedGraphics/TerrainTest/bin")


set(SOURCE_FILES src/main.cpp)
add_executable(TerrainTest ${SOURCE_FILES} src/skybox.cpp src/skybox.h src/Engine.cpp src/Engine.h src/input.cpp
                src/input.h src/camera.cpp src/camera.h src/graphics.cpp src/graphics.h src/Simulation.cpp src/Simulation.h
                src/lights.h src/Technique.cpp src/Technique.h src/Program.cpp src/Program.h src/Shader.cpp src/Shader.h src/mesh.cpp
                src/mesh.h src/Model.cpp src/Model.h src/Vertex.h src/GBuffer.cpp src/GBuffer.h src/ds_geom_pass_tech.cpp src/ds_geom_pass_tech.h
                src/ds_dir_light_pass_tech.cpp src/ds_dir_light_pass_tech.h src/ds_point_light_pass_tech.cpp src/ds_point_light_pass_tech.h
                src/ds_spot_light_pass_tech.cpp src/ds_spot_light_pass_tech.h src/lights.h src/Terrain.cpp src/Terrain.h src/gishandler.cpp src/gishandler.h
                src/buffer.cpp src/buffer.h src/renderer.cpp src/renderer.h src/TerrainShader.cpp src/TerrainShader.h src/shape.cpp src/shape.h src/projector.cpp src/projector.h src/grass_tech.cpp src/grass_tech.h)

INCLUDE(FindPkgConfig)
include(FindGLEW)

pkg_check_modules(GL REQUIRED gl)
find_package(GLUT REQUIRED)
find_package(ImageMagick COMPONENTS Magick++)

pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("/usr/include/gdal")


find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

include(FindBullet)
include_directories(${BULLET_INCLUDE_DIRS})

target_link_libraries(TerrainTest ${OPENGL_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SDL2_LIBRARIES} gdal freeimageplus GL glut GLEW assimp)