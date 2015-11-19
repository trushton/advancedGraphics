#ifndef LIGHTS_H
#define LIGHTS_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

struct BaseLight
{
    glm::vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;

    BaseLight()
    {
        Color = glm::vec3(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};

struct DirectionalLight : public BaseLight
{
    glm::vec3 Direction;

    DirectionalLight()
    {
        Direction = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};

struct PointLight : public BaseLight
{
    glm::vec3 Position;

    struct
    {
        float Constant;
        float Linear;
        float Exp;
    } Attenuation;

    PointLight()
    {
        Position = glm::vec3(0.0f, 0.0f, 0.0f);
        Attenuation.Constant = 0.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
    }
};

struct SpotLight : public PointLight
{
    glm::vec3 Direction;
    float Cutoff;

    SpotLight()
    {
        Direction = glm::vec3(0.0f, 0.0f, 0.0f);
        Cutoff = 0.0f;
    }
};

#define COLOR_WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define COLOR_RED glm::vec3(1.0f, 0.0f, 0.0f)
#define COLOR_GREEN glm::vec3(0.0f, 1.0f, 0.0f)
#define COLOR_CYAN glm::vec3(0.0f, 1.0f, 1.0f)
#define COLOR_YELLOW glm::vec3(1.0f, 1.0f, 0.0f);
#define COLOR_MAGENTA glm::vec3(1.0f, 0.0f, 1.0f);
#define COLOR_BLUE glm::vec3(0.0f, 0.0f, 1.0f)

#endif //LIGHTS_H
