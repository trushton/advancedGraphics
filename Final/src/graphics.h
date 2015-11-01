#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <vector>
#include <map>
#include <string>

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
//#include "cube.h"
#include "skybox.h"
#include "camera.h"
//#include "water.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Final.h"
#include "MousePicker.h"
//#include "water_frame_buffers.h"


class Engine;
class Camera;
class Texture;
class Program;
class Cube;
class Water;
class Simulation;
class Final;
class WaterFrameBuffers;

class Graphics {
public:
    Graphics(Engine *eng);
    ~Graphics();

    void init();
    void tick(float dt);
    void render();
    void stop();

    void updateView();
    void updateCamera();
    void windowResized();

    void getWindowSize(int &w, int &h) const;
    Program* getShaderProgram(const std::string& name) const;

    SDL_Window* getRenderWindow() const {return window;}
    void setClearColor(const glm::vec3& color);

    glm::mat4 view, projection;

    Camera * camera;
    Final *final;
    int windowWidth, windowHeight;


private:
    void initGL();

    Engine *engine;
    std::map<std::string, Program*> programs;

    SDL_Window *window;
    SDL_GLContext gl_context;

    Cube * cube;
    Skybox * skybox;
    Water * water;
    //Simulation * simulation;
        WaterFrameBuffers * frameBuffers;


};

#endif // GRAPHICS_HPP
