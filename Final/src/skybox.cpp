#include "skybox.h"
#include "Engine.h"
#include "graphics.h"



Skybox::Skybox()
{
    m_fileNames[0] = "../bin/skybox/hw_lagoon/lagoon_ft.tga";
    m_fileNames[1] = "../bin/skybox/hw_lagoon/lagoon_bk.tga";
    m_fileNames[3] = "../bin/skybox/hw_lagoon/lagoon_up.tga";
    m_fileNames[2] = "../bin/skybox/hw_lagoon/lagoon_dn.tga";
    m_fileNames[4] = "../bin/skybox/hw_lagoon/lagoon_rt.tga";
    m_fileNames[5] = "../bin/skybox/hw_lagoon/lagoon_lf.tga";

    init();
}

GLuint loadCubemap(string m_fileNames []);

void Skybox::init()
{
    // load and compile the shaders
    loadShaders();

    //  link all shaders together
    initShaderProgram();

    // get the variables from the shaders
    initShaderLocations();

    // create the VAO
    glUseProgram(program);

    // create the geometry
    GLfloat skyboxVertices[] = {
            // Positions
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };

    GLuint skyboxVBO;
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    skyboxTexture = loadCubemap(m_fileNames);
}


GLuint loadCubemap(string m_fileNames [])
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (GLuint i = 0; i < 6; i++)
    {
        fipImage image;
        image.load(m_fileNames[i].c_str());
        image.convertTo32Bits();

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_BGRA,
                     GL_UNSIGNED_BYTE, image.accessPixels());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

void Skybox::loadShaders()
{
    shaders[0] = Program::loadShader("../shaders/skybox.vs", GL_VERTEX_SHADER);
    shaders[1] = Program::loadShader("../shaders/skybox.fs", GL_FRAGMENT_SHADER);
}


void Skybox::initShaderProgram()
{
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);

    glLinkProgram(program);

    GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if (!shader_status)
    {
        std::cerr << "Unable to create shader program!" << std::endl;

        char buffer[512];
        glGetProgramInfoLog(program, 512, NULL, buffer); // inserts the error into the buffer
        std::cerr << buffer << std::endl;

        exit(1);
    }
}

void Skybox::initShaderLocations()
{
    glUseProgram(program);
    locations["view"] = glGetUniformLocation(program, "view");
    locations["projection"] = glGetUniformLocation(program, "projection");
}

void Skybox::tick(float dt)
{

}


void Skybox::render()
{
    // Change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);

    // enable shader program
    glUseProgram(program);

    // get the view from the camera and projection matrix
    view = glm::mat4(glm::mat3(Engine::getEngine()->graphics->camera->getView()));	// Remove any translation component of the view matrix
    projection = glm::perspective(45.0f, float(1600) / float(900), 0.01f, 100.0f);

    // pass the view and projection matrices to the shader
    set("view", view);
    set("projection", projection);

    // bind the VAO and texture to render the skybox
    glBindVertexArray(m_VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Set depth function back to default
    glDepthFunc(GL_LESS);
}


void Skybox::bind()
{

}

void Skybox::unbind()
{

}
