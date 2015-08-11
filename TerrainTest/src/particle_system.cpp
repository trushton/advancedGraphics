//
// Created by trushton on 7/28/15.
//

#include "particle_system.h"
#include "RandomTexture.h"
#include "Engine.h"
#include "graphics.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

#define COLOR_TEXTURE_UNIT GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX 0
#define RANDOM_TEXTURE_UNIT GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX 3

struct Particle
{
    float Type;
    glm::vec3 Pos;
    glm::vec3 Vel;
    float LifetimeMillis;
};

ParticleSystem::ParticleSystem()
{

    currVB = 0;
    currTFB = 1;

    isFirst = true;
    timeT = 0;
    texture = NULL;

    ZERO_MEM(m_transformFeedback);
    ZERO_MEM(m_particleBuffer);

}

ParticleSystem::~ParticleSystem()
{
    SAFE_DELETE(texture);
    if (m_transformFeedback[0] != 0) {
        glDeleteTransformFeedbacks(2, m_transformFeedback);
    }
    if (m_particleBuffer[0] != 0) {
        glDeleteBuffers(2, m_particleBuffer);
    }
}

void ParticleSystem::InitParticleSystem(const glm::vec3 Pos)
{
    Particle Particles[MAX_PARTICLES];
    ZERO_MEM(Particles);

    Particles[0].Type = PARTICLE_TYPE_LAUNCHER;
    Particles[0].Pos = Pos;
    Particles[0].Vel = glm::vec3(0.0f, 0.0001f, 0.0f);
    Particles[0].LifetimeMillis = 0.0f;

    glGenTransformFeedbacks(2, m_transformFeedback);
    glGenBuffers(2, m_particleBuffer);

    for (unsigned int i = 0; i < 2 ; i++) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), Particles, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i]);
    }

    random_texture.InitRandomTexture(MAX_PARTICLES);
    random_texture.Bind(RANDOM_TEXTURE_UNIT);

    m_updateTechnique.init();
    m_updateTechnique.enable();
    m_updateTechnique.set("gRandomTexture", RANDOM_TEXTURE_UNIT_INDEX);
    m_updateTechnique.set("gLauncherLife", 250.f);
    m_updateTechnique.set("gShellLife", 2000.f);
    m_updateTechnique.set("gSecondaryLife", 2500.f);

    m_billboardTechnique.init();
    m_billboardTechnique.enable();
    m_billboardTechnique.set("gColorMap", COLOR_TEXTURE_UNIT_INDEX);
    m_billboardTechnique.set("gBillboardSize", 0.5f);

    texture = new Texture(GL_TEXTURE_2D, "../bin/fireworks_red.jpg");
    texture->Load();


}

void ParticleSystem::Render(int DeltaTimeMillis)
{
    timeT += DeltaTimeMillis;
    cout << "total time: " << timeT << " |  DeltaTime: " << DeltaTimeMillis << endl;
    updateParticles(DeltaTimeMillis);
    renderParticles();

    currVB = currTFB;
    currTFB = (currTFB + 1) & 0x1;

}

void ParticleSystem::updateParticles(int DeltaTimeMillis)
{
    m_updateTechnique.enable();
    m_updateTechnique.set("gTime", timeT);
    m_updateTechnique.set("gDeltaTime", DeltaTimeMillis);

    random_texture.Bind(RANDOM_TEXTURE_UNIT);

    glEnable(GL_RASTERIZER_DISCARD);
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[currVB]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[currTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0,1,GL_FLOAT,GL_FALSE,sizeof(Particle), 0); // type
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Particle),(const GLvoid*) 4); // position
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Particle),(const GLvoid*) 16); // velocity
    glVertexAttribPointer(3,1,GL_FLOAT,GL_FALSE,sizeof(Particle),(const GLvoid*) 28); // lifetime

    glBeginTransformFeedback(GL_POINTS);

    if (isFirst) {
        glDrawArrays(GL_POINTS, 0, 1);
        isFirst = false;

    }
    else {
        glDrawTransformFeedback(GL_POINTS, m_transformFeedback[currVB]);
        cout << m_transformFeedback[currVB] << endl;
    }

    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void ParticleSystem::renderParticles()
{
    glm::mat4 vp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->camera->getView();

    m_billboardTechnique.enable();
    m_billboardTechnique.set("gCameraPos", Engine::getEngine()->graphics->camera->getPos());
    m_billboardTechnique.set("gVP", vp);

    texture->Bind(COLOR_TEXTURE_UNIT);


    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[currTFB]);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4); // position

    glDrawTransformFeedback(GL_POINTS, m_transformFeedback[currTFB]);

    glDisableVertexAttribArray(0);
}