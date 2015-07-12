#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <vector>
#include <string>
#include <map>

//class SDL_Keycode;

#define PRESSED true;
#define RELEASED false;

class Engine;

class Input
{
public:
    Input(Engine *eng);
    ~Input();
    void init();
    void tick(float dt);
    void stop();

private:

    void handleKeyboardEvent(SDL_Event &e);
    void resetKeyPresses();
    void handleMovementKeys();
    bool isPressed(int keyCode);
    bool isReleased(int keyCode);

    Engine *engine;
    float sensitivity;

    // std::map<int, bool> keyPresses;

    bool keyState[512];
    bool mouseEnabled;

};

#endif // INPUT_H