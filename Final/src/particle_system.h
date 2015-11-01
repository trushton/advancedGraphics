#ifndef PARTICLE_SYSTEM_H
#define	PARTICLE_SYSTEM_H

#include <GL/glew.h>

#include "ps_update_tech.h"
#include "billboard_tech.h"
#include "RandomTexture.h"
#include "Texture.h"

#include "ParticleSystemRender.h"
#include "ParticleSystemUpdate.h"
#include <glm/vec3.hpp>

struct Particle
{
    float type;
    glm::vec3 pos;
    glm::vec3 vel;
    float lifetime;
    glm::vec3 color;
};

class ParticleUpdateProgram;
class ParticleRenderProgram;
class Texture;

class ParticleSystem
{
public:
    ParticleSystem();

    ~ParticleSystem();

    void initWithPos(const glm::vec3 &pos);

    void renderWithDT(float dt);
    glm::vec3 initialPosition;

private:
    void updateParticles(float dt);

    void renderParticles();

    bool isFirst;
    unsigned int currVB, currTFB;
    GLuint particleBuffer[2], transformFeedback[2], vao;
    float time;

    ParticleUpdateProgram * update;
    ParticleRenderProgram * render;

    Texture *texture, *random_texture, *alphaTexture;
};

#endif	/* PARTICLE_SYSTEM_H */