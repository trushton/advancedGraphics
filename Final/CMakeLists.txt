cmake_minimum_required(VERSION 3.2)
project(Final)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -L/usr/lib/nvidia-304" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/Desktop/advancedGraphics/Final/bin")


set(SOURCE_FILES src/main.cpp)
add_executable(Final ${SOURCE_FILES} src/Final.cpp src/Final.h src/MousePicker.cpp src/MousePicker.h src/skybox.cpp src/skybox.h src/Engine.cpp src/Engine.h src/input.cpp
                src/input.h src/camera.cpp src/camera.h src/graphics.cpp src/graphics.h src/lights.h src/Technique.cpp src/Technique.h src/Program.cpp src/Program.h src/Shader.cpp src/Shader.h src/mesh.cpp
                src/mesh.h src/Model.cpp src/Model.h src/Vertex.h src/GBuffer.cpp src/GBuffer.h src/ds_geom_pass_tech.cpp src/ds_geom_pass_tech.h
                src/ds_dir_light_pass_tech.cpp src/ds_dir_light_pass_tech.h src/ds_point_light_pass_tech.cpp src/ds_point_light_pass_tech.h
                src/ds_spot_light_pass_tech.cpp src/ds_spot_light_pass_tech.h src/lights.h
                src/buffer.cpp src/buffer.h src/renderer.cpp src/renderer.h src/TerrainShader.cpp src/TerrainShader.h src/billboard_tech.cpp src/billboard_tech.h
                src/particle_system.cpp src/particle_system.h src/ps_update_tech.cpp src/ps_update_tech.h src/Texture.cpp src/Texture.h src/RandomTexture.cpp src/RandomTexture.h
                src/grass_tech.cpp src/grass_tech.h src/water_tech.cpp src/water_tech.h src/pbuffer.cpp src/pbuffer.h src/Plane.cpp src/Plane.h src/Flag.cpp src/Flag.h
                src/ParticleSystemUpdate.cpp src/ParticleSystemUpdate.h src/ParticleSystemRender.cpp src/ParticleSystemRender.h src/waterPlane.cpp src/waterPlane.h src/Terrain.cpp src/Terrain.h src/Projector.cpp src/Projector.h)

INCLUDE(FindPkgConfig)
include(FindGLEW)

pkg_check_modules(GL REQUIRED gl)
find_package(GLUT REQUIRED)
find_package(ImageMagick COMPONENTS Magick++)

pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("/usr/include/gdal")
include_directories(${ImageMagick_INCLUDE_DIRS})


find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

include(FindBullet)
include_directories(${BULLET_INCLUDE_DIRS})

target_link_libraries(Final ${OPENGL_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SDL2_LIBRARIES} ${ImageMagick_LIBRARIES}  gdal freeimageplus GL glut GLEW assimp)