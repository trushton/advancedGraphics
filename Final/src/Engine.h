#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <string>
#include <chrono>

class Input;
class Graphics;

class Engine
{
public:
    Engine();
    ~Engine();

    void init();

    int run();
    void stop(int exit_code = 0);

    static Engine *getEngine();

    float tick();

    Input *input;
    Graphics *graphics;
    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
    bool clicked, rayColor, project;

private:
    static Engine *currentEngine;
};

#endif // ENGINE_H