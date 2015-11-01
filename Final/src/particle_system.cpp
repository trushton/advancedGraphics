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

ParticleSystem::ParticleSystem()
{
    currVB = 0;
    currTFB = 1;

    isFirst = true;
    time = 0;

    update = new ParticleUpdateProgram();
    render = new ParticleRenderProgram();


}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::initWithPos(const glm::vec3 &pos)
{
    Particle particles[MAX_PARTICLES];
    initialPosition = pos;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    particles[0].type = PARTICLE_TYPE_LAUNCHER;
    particles[0].pos = pos;
    particles[0].vel = glm::vec3(0.0f, 0.0001f, 0.0f);
    particles[0].lifetime = 0.0f;
    particles[0].color = glm::vec3(0.0,0.5,1.0);


    glGenTransformFeedbacks(2, transformFeedback);
    glGenBuffers(2, particleBuffer);

    for (int i = 0; i < 2; i++)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback[i]);
        glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffer[i]);
    }

    texture = new Texture("../bin/fireworks_red.jpg", GL_TEXTURE_2D);
    alphaTexture = new Texture("../bin/fireworks_red.jpg", GL_TEXTURE_2D);

    random_texture = new RandomTexture(MAX_PARTICLES, GL_TEXTURE_1D);

    update->enable();
    update->set("random_texture", RANDOM_TEXTURE_UNIT_INDEX);
    update->set("launcher_lifetime", 250.0f);
    update->set("shell_lifetime", 5000.0f);
    update->set("secondary_shell_lifetime", 2500.0f);

    render->enable();
    render->set("color_map", COLOR_TEXTURE_UNIT_INDEX);
    //render->set("alpha_map", 2);


    render->set("billboard_size", 2.0f);

    glBindVertexArray(0);

}

void ParticleSystem::renderWithDT(float dt)
{
    time += (dt);

    updateParticles(dt);
    renderParticles();

    currVB = currTFB;
    currTFB = (currTFB + 1) & 0x1;
}

void ParticleSystem::updateParticles(float dt)
{
    glBindVertexArray(vao);
    update->enable();
    update->set("time", time);
    update->set("dt", dt);

    glm::vec3 color(((rand()%11) / 10.0f), ((rand()%11) / 10.0f), ((rand()%11) / 10.0f));

    update->set("particleColor", color);

    random_texture->bind(RANDOM_TEXTURE_UNIT);
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[currVB]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback[currTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) 0); // type
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) 4); // position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) 16); // velocity
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) 28); // lifetime
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) 32); // Color

    glBeginTransformFeedback(GL_POINTS);

    if (isFirst)
    {
        glDrawArrays(GL_POINTS, 0, 1);
        isFirst = false;
    }
    else
    {
        glDrawTransformFeedback(GL_POINTS, transformFeedback[currVB]);
    }

    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);


}

void ParticleSystem::renderParticles()
{
    glBindVertexArray(vao);
    auto vp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view;

    render->enable();
    render->set("camera_pos", Engine::getEngine()->graphics->camera->getPos());
    render->set("vp", vp);


    if (texture)
    {
        texture->bind(COLOR_TEXTURE_UNIT);
    }

//    if(alphaTexture)
//    {
//        alphaTexture->bind(GL_TEXTURE2);
//    }

    glDisable(GL_RASTERIZER_DISCARD);
    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[currTFB]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(4);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) 4);  // position
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) 32); // Color

    glDrawTransformFeedback(GL_POINTS, transformFeedback[currTFB]);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(4);

    glBindVertexArray(0);
}