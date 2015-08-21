#include "input.h"
#include "graphics.h"
#include "Engine.h"
#include <chrono>
#include <iostream>


Engine *Engine::currentEngine = nullptr;

Engine::Engine()
{
    Engine::currentEngine = this;
    init();
}

Engine::~Engine()
{
    stop();
}

void Engine::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Unable to initilize SDL" << std::endl;
        exit(1);
    }

    input = new Input(this);
    graphics = new Graphics(this);

    graphics->init();
    input->init();
}

int Engine::run()
{
    float dt;
    t1 = std::chrono::high_resolution_clock::now();

    while (true)
    {
        dt = tick();
        input->tick(dt);
        graphics->tick(dt);

    }

    return 0;
}

void Engine::stop(int exit_code)
{
    input->stop();
    graphics->stop();

    delete input;
    delete graphics;

    SDL_Quit();

    exit(exit_code);
}


Engine *Engine::getEngine()
{
    return currentEngine;
}


float Engine::tick()
{
    float dt;

    t2 = std::chrono::high_resolution_clock::now();
    dt = std::chrono::duration_cast<std::chrono::duration<float> >(t2 - t1).count();
    t1 = t2;

    return dt;
}