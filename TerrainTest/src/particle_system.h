#ifndef PARTICLE_SYSTEM_H
#define	PARTICLE_SYSTEM_H

#include <GL/glew.h>

#include "ps_update_tech.h"
#include "billboard_tech.h"
#include "RandomTexture.h"
#include "Texture.h"



struct Particle
{
    float Type;
    glm::vec3 Pos;
    glm::vec3 Vel;
    float LifetimeMillis;
};

class ParticleSystem {

    public:
        ParticleSystem();
        ~ParticleSystem();
        void InitParticleSystem(const glm::vec3 Pos);
        void Render(int DeltaTimeMillis);

    private:
        void updateParticles(int DeltaTimeMillis);
        void renderParticles();

        bool isFirst;

        unsigned int currVB;
        unsigned int currTFB;

        GLuint m_particleBuffer[2];
        GLuint m_transformFeedback[2];

        billboard_tech m_billboardTechnique;
        PSUpdate m_updateTechnique;

        RandomTexture random_texture;
        Texture* texture;
        int timeT;
};

#endif	/* PARTICLE_SYSTEM_H */