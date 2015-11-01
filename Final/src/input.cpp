#include "Engine.h"
#include "input.h"
#include "graphics.h"
#include "camera.h"


Input::Input(Engine *eng)
        : engine(eng)
{
    sensitivity = 1.0f;
    mouseEnabled = true;

}

Input::~Input()
{
}

void Input::init() {
    SDL_SetRelativeMouseMode(SDL_FALSE);
    resetKeyPresses();
}


void Input::tick(float dt)
{
    SDL_Event e;
    float x, y;


    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        engine->clicked = false;
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
        {
            handleKeyboardEvent(e);
        }

        else if (e.type == SDL_MOUSEMOTION)
        {
            if (mouseEnabled)
            {
                x = (float) e.motion.xrel * sensitivity;
                y = (float) e.motion.yrel * sensitivity;
                engine->graphics->camera->rotate(x, y);
            }
        }
        else if (e.type == SDL_WINDOWEVENT)
        {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                engine->graphics->windowResized();
            }
        }
        else if(e.type == SDL_MOUSEBUTTONDOWN){
            engine->clicked = true;
        }


    }

    handleMovementKeys();
}

void Input::stop()
{

}


void Input::handleKeyboardEvent(SDL_Event &e)
{
    int code;
    if (e.key.keysym.sym < 128)
    {
        code = e.key.keysym.sym;
    }
    else
    {
        code = 128 + (e.key.keysym.sym - 1073741880);
    }

    if (e.type == SDL_KEYDOWN)
    {
        keyState[code] = PRESSED;
    }
    else
    {
        keyState[code] = RELEASED;
    }
}

bool Input::isPressed(int keyCode)
{
    int code;

    if (keyCode < 128)
    {
        code = keyCode;
    }
    else
    {
        code = 128 + (keyCode - 1073741880);
    }

    return keyState[code] == PRESSED;
}

bool Input::isReleased(int keyCode)
{
    int code;
    if (keyCode < 128)
    {
        code = keyCode;
    }
    else
    {
        code = 128 + (keyCode - 1073741880);
    }

    return keyState[code] == RELEASED;
}

void Input::resetKeyPresses()
{
    for(int i = 0; i < 512; i++)
    {
        keyState[i] = false;
    }
}

void Input::handleMovementKeys()
{
    if (isPressed(SDLK_w))
    {
        engine->graphics->camera->MoveForward(-10.5f);
    }

    if (isPressed(SDLK_s))
    {
        engine->graphics->camera->MoveForward(10.5f);
    }

    if (isPressed(SDLK_d))
    {
        engine->graphics->camera->StrafeRight(-10.5f);
    }

    if (isPressed(SDLK_a))
    {
        engine->graphics->camera->StrafeRight(10.5f);
    }

    if (isPressed(SDLK_i))
    {
        engine->graphics->camera->RotateY(-0.05);
    }
    if (isPressed(SDLK_k))
    {
        engine->graphics->camera->RotateY(0.05);
    }
    if (isPressed(SDLK_j))
    {
        engine->graphics->camera->RotateX(0.05);
    }
    if (isPressed(SDLK_l))
    {
        engine->graphics->camera->RotateX(-0.05);
    }
    if (isPressed(SDLK_u))
    {
        engine->graphics->camera->RotateZ(0.05);
    }
    if (isPressed(SDLK_o))
    {
        engine->graphics->camera->RotateZ(-0.05);
    }
    if(isPressed(SDLK_1)){
        engine->graphics->camera->printOut();
    }
    


    if (isPressed(SDLK_r))
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        //mouseEnabled = !mouseEnabled;

    }
    if(isPressed(SDLK_TAB)){
        static float angle = 0.0f;
        static float total = 0.0f;
        angle += 0.0001;
        engine->graphics->final->flag.model = glm::rotate(engine->graphics->final->flag.model, angle, glm::vec3(1,0,0));
        total += angle;
        cout << total << endl;
    }


    //User requests quit
    if (isPressed(SDLK_ESCAPE))
    {
        engine->stop();
    }

    /*
    if(keyPresses[SDLK_LEFT])
        if(engine->graphics->water->lightAngle > 0){
            engine->graphics->water->lightAngle -= engine->getOptions().light_speed;

            // set the color of the sky based distance form 1.57
            float dist = 1.57 - engine->graphics->water->lightAngle;
            if(dist < 0){ dist = -dist; }

            float c = (1.57-dist)/1.57;
            float r = c * 0;
            float g = c * .75;mat4
            float b = c;
            glm::vec3 color = glm::vec3(r, g, b);
            engine->graphics->setClearColor(color);

            engine->graphics->water->setAmbient(c * 0.5);
        }

    if(keyPresses[SDLK_RIGHT])
        if(engine->graphics->water->lightAngle < 3.14){
            engine->graphics->water->lightAngle += engine->getOptions().light_speed;

            // set the color of the sky based distance form 1.57
            float dist = 1.57 - engine->graphics->water->lightAngle;
            if(dist < 0){ dist = -dist; }
            float c = (1.57-dist)/1.57;
            float r = c * 0;
            float g = c * .75;
            float b = c;
            glm::vec3 color = glm::vec3(r, g, b);
            engine->graphics->setClearColor(color);

            engine->graphics->water->setAmbient(c * 0.5);
        }
    */
}