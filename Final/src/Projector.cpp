//
// Created by trushton on 11/1/15.
//

#include "Projector.h"

Projector::Projector()
{
    TexGen = glm::mat4(1.0 / 2.0, 0, 0, 1.0 / 2.0,
                       0, 1.0 / 2.0, 0, 1.0 / 2.0,
                       0, 0, 1.0 / 2.0, 1.0 / 2.0,
                       0, 0, 0, 1.0);

    projection = glm::ortho<float>(-1000, 1000, 1000, -1000, 0.9f, 10000.0f);


    up = glm::vec3(0, 1, 0);
    direction = glm::vec3(0, 0, -1);

    position = glm::vec3(110, 0, 110);
    view = glm::lookAt( position, //Eye Position
                        position + direction, //Focus point
                        up); //Positive Y is up
    alpha = .5;

}


void Projector::setup()
{
    Renderer.init();
    Renderer.addShader(GL_VERTEX_SHADER, "../shaders/projector.vs");
    Renderer.addShader(GL_FRAGMENT_SHADER,  "../shaders/projector.fs");



    cout << Renderer.compile() << endl;
    cout << Renderer.link() << endl;
    float verts[12] = { -1.0, -1.0,
                        1.0, -1.0,
                        1.0, 1.0,
                        -1.0, -1.0,
                        -1.0, 1.0,
                        1.0, 1.0
    };
    Buffer.generateBuffer(GL_ARRAY_BUFFER);
    Buffer.bindBuffer();
    Buffer.allocateBufferData(sizeof(verts), &verts[0], GL_STATIC_DRAW);
    cout << "FILENAME: " << filename << endl;

    tex = new Texture(filename.c_str(), GL_TEXTURE_2D);



}

void Projector::update(float dt)
{

}


void Projector::render(glm::mat4& view2, glm::mat4& projection2)
{
    view = glm::lookAt( position, //Eye Position
                        position + direction, //Focus point
                        up);

    tex->bind(GL_TEXTURE8);
    glActiveTexture(GL_TEXTURE8);

    Renderer.useProgram();
    Renderer.setUniformFloat("alpha",alpha);
    Buffer.bindBuffer();

    Renderer.enableVertexAttribPointer("Position");
    Renderer.setGLVertexAttribPointer("Position", 2, GL_FLOAT, GL_FALSE, sizeof(float), 0);

    // enable three textures
    Renderer.setUniformInteger("gPositionMap", 0);
    Renderer.setUniformInteger("gTextureMap", 1);
    Renderer.setUniformInteger("proj_tex", 8);

    float SCREEN_SIZE[2] = {(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
    Renderer.setUniformFloatArray2("gScreenSize", 1, SCREEN_SIZE);
    Renderer.setUniformMatrix4x4("projection", projection);
    Renderer.setUniformMatrix4x4("view", view);
    Renderer.setUniformMatrix4x4("tex", TexGen);

    Renderer.renderRaw(GL_TRIANGLES, 12);
    Renderer.disableVertexAttribPointer("Position");


}