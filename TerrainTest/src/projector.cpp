#include "projector.h"

projector::projector()
{
    Texgen = glm::mat4(1.0 / 2.0, 0, 0, 1.0 / 2.0,
                       0, 1.0 / 2.0, 0, 1.0 / 2.0,
                       0, 0, 1.0 / 2.0, 1.0 / 2.0,
                       0, 0, 0, 1.0);
    //cout << "TEXGEN:!!!:!:!"<< Texgen[3][3] << endl;
    projection = glm::ortho<float>(-1000, 1000, 1000, -1000, 0.1f, 10000.0f); //glm::perspective<float>(
    //    35,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
    //   4.0f / 3.0f, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
    //  0.1f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
    //  100000.0f       // Far clipping plane. Keep as little as possible.
    //);

    up = glm::vec3(0, 0, -1);
    direction = glm::vec3(0, -1, 0);
    //direction = glm::rotate(direction, 180.0f, glm::cross(direction, up));
    position = glm::vec3(110, 0, 110);
    view = glm::lookAt( position, //Eye Position
                        position + direction, //Focus point
                        up); //Positive Y is up

}


void projector::setup()
{
    Renderer.init();
    Renderer.addShader(GL_VERTEX_SHADER, "../shaders/projector.vert");
    if (projtype == IMAGE)
    {
        Renderer.addShader(GL_FRAGMENT_SHADER,  "../shaders/projector.frag");
    }
    else
    {
        Renderer.addShader(GL_FRAGMENT_SHADER, "../shaders/projector2.frag");
    }

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
    double x, y;
    string projection;
    if (projtype == IMAGE)
    {
        generateImageTexture(filename, tex, projection, x, y, width, height, xres, yres);
    }
    else
    {
        generateTexture(filename, tex, bandnum,projection, x, y, width, height, xres, yres);
    }
    origin.x = x;
    origin.y = y;

    // Lets construct the projection
    char* test = &projection[0];
    sr.importFromWkt(&test);
    SetDimensions(width * xres, height * yres);
}

void projector::update(float dt)
{

}


void projector::render(glm::mat4& view2, glm::mat4& projection2)
{
    view = glm::lookAt( position, //Eye Position
                        position + direction, //Focus point
                        up);

    //cout << "TEXGEN:!!!:!:!"<< Texgen[0][0] << endl;


    glActiveTexture(GL_TEXTURE0 + 5);
    glBindTexture(GL_TEXTURE_2D, tex);

    Renderer.useProgram();
    Buffer.bindBuffer();
    if (projtype == IMAGE)
    {
        Renderer.enableVertexAttribPointer("Position");
        Renderer.setGLVertexAttribPointer("Position", 2, GL_FLOAT, GL_FALSE, sizeof(float), 0);

        // enable three textures
        Renderer.setUniformInteger("gPositionMap", 0);
        Renderer.setUniformInteger("gTextureMap", 4);
        Renderer.setUniformInteger("proj_tex", 5);
        //Renderer.setUniformInteger("gNormalMap",2);
        float SCREEN_SIZE[2] = {(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
        Renderer.setUniformFloatArray2("gScreenSize", 1, SCREEN_SIZE);
        Renderer.setUniformMatrix4x4("projection", projection);
        Renderer.setUniformMatrix4x4("view", view);
        Renderer.setUniformMatrix4x4("tex", Texgen);

        Renderer.renderRaw(GL_TRIANGLES, 12);
        Renderer.disableVertexAttribPointer("Position");
    }
    else
    {
        //cout << "HERE" << endl;
        Renderer.enableVertexAttribPointer("Position");
        Renderer.setGLVertexAttribPointer("Position", 2, GL_FLOAT, GL_FALSE, sizeof(float), 0);

        // enable three textures
        Renderer.setUniformInteger("gPositionMap", 0);
        Renderer.setUniformInteger("gTextureMap", 4);
        Renderer.setUniformInteger("proj_tex", 5);
        //Renderer.setUniformInteger("gNormalMap",2);
        float SCREEN_SIZE[2] = {(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
        Renderer.setUniformFloatArray2("gScreenSize", 1, SCREEN_SIZE);
        Renderer.setUniformMatrix4x4("projection", projection);
        Renderer.setUniformMatrix4x4("view", view);
        Renderer.setUniformMatrix4x4("tex", Texgen);

        Renderer.renderRaw(GL_TRIANGLES, 12);
        Renderer.disableVertexAttribPointer("Position");
    }
}