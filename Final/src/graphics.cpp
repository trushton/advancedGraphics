#include "Engine.h"
#include "graphics.h"

#define GLM_FORCE_RADIANS

#include <glm/gtx/rotate_vector.hpp>

#include <fstream>


Graphics::Graphics(Engine *eng)
        : engine(eng)
{
    camera = new Camera(engine);

}

Graphics::~Graphics()
{

}

void Graphics::init()
{

    int windowWidth = 1600, windowHeight = 900;
    //int windowWidth = 1600, windowHeight = 900;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    window = SDL_CreateWindow("Deferred Shading With Terrain2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        std::cerr << "Unable to create SDL Window" << std::endl;
        exit(1);
    }

    gl_context = SDL_GL_CreateContext(window);

    std::cout << glGetString(GL_VERSION) << std::endl;

    SDL_GL_SetSwapInterval(1);

    initGL();
}

void Graphics::initGL()
{
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum status = glewInit();
    if (status != GLEW_OK)
    {
        std::cerr << "Unable to initialize glew" << std::endl;
        exit(1);
    }
#endif


    glClearColor(0.0, 0.0, 0.0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    updateView();
    windowResized();

    //cube = new Cube();
    final = new Final();

    skybox = new Skybox();




}

void Graphics::tick(float dt)
{
    //cube->tick(dt);
    final->tick(dt);
    render();
}

void Graphics::render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateView();
    //glClear(GL_DEPTH_BUFFER_BIT);
    final->render();

    SDL_GL_SwapWindow(window);
}

void Graphics::stop()
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}

void Graphics::updateView()
{
    view = camera->getView();
}

void Graphics::updateCamera()
{

}

void Graphics::windowResized()
{
    int width, height;
    getWindowSize(width, height);
    glViewport(0, 0, width, height);
    projection = glm::perspective(45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                  float(width) /
                                  float(height), //Aspect Ratio, so Circles stay Circular
                                  0.9f, //Distance to the near plane, normally a small value like this
                                  1000000.0f); //Distance to the far plane,
}

void Graphics::getWindowSize(int &w, int &h) const
{
    SDL_GetWindowSize(window, &w, &h);
}

Program *Graphics::getShaderProgram(const std::string &name) const
{
    return programs.at(name);
}

void Graphics::setClearColor(const glm::vec3 &color)
{
    glClearColor(color[0], color[1], color[2], 1.0f);
}
