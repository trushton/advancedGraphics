#define GLM_FORCE_RADIANS
#include <iostream>
#include <assert.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

using namespace std;

//useful macros
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;

//transformation matrices
glm::mat4 model; //each object gets its own model matrix
glm::mat4 view;
glm::mat4 projection;
glm::mat4 mvp; //projection * view * model

int width = 640;
int height = 480;
GLint windowWidth, windowHeight;
GLuint ShaderProgram;

static void CreateVertexBuffer();
static void CreateIndexBuffer();
//static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
static void CompileShaders();


//Glut Callbacks
void update();
void render();
void reshape(int n_w, int n_h);

//resource management
void cleanUp();

void render()
{
    //clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    //pre-multiply the matrix
    mvp = projection * view * model;

    //enable the program
    glUseProgram(ShaderProgram);

    //upload the matrix to the shader
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, glm::value_ptr(model));//&World[0][0]);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

void update(){
    static float Scale = 0.0f;
    Scale += 0.001f;


    //lame way to do transformations
    //World[0][0] = cosf(Scale); World[0][1] = -sinf(Scale); World[0][2] = 0.0f; World[0][3] = sinf(Scale);
    //World[1][0] = sinf(Scale); World[1][1] = cosf(Scale); World[1][2] = 0.0f; World[1][3] = 0.0f;
    //World[2][0] = 0.0f; World[2][1] = 0.0f; World[2][2] = 1.0f; World[2][3] = 0.0f;
    //World[3][0] = 0.0f; World[3][1] = 0.0f; World[3][2] = 0.0f; World[3][3] = 1.0f;

    //much better way!
    //World = (glm::translate(glm::mat4(1.0f), glm::vec3(4.0f * sin(Scale), 0.0f, 4.0* cos(Scale))));
    model = (glm::rotate(model, sin(Scale*0.2f), glm::vec3(0.0, 1.0, 0.0)));
    glutPostRedisplay();
}


static void InitializeGlutCallbacks()
{
    glutDisplayFunc(render);
    glutIdleFunc(update);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutReshapeFunc(reshape);// Called if the window is resized
    glutCreateWindow("Tutorial 13");

    InitializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    printf("GL version: %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic
    //  for this project having them static will be fine
    view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(width)/float(height), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane,

    CreateVertexBuffer();
    CreateIndexBuffer();

    CompileShaders();

    glutMainLoop();

    cleanUp();

    return 0;
}

void cleanUp()
{
    // Clean up, Clean up
    glDeleteProgram(ShaderProgram);
    glDeleteBuffers(1, &VBO);
}

static void CreateVertexBuffer()
{
    glm::vec3 Vertices[4];
    Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    Vertices[1] = glm::vec3(0.0f, -1.0f, 1.0f);
    Vertices[2] = glm::vec3(1.0f, -1.0f, 0.0f);
    Vertices[3] = glm::vec3(0.0f, 1.0f, 0.0f);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
    unsigned int Indices[] = { 0, 3, 1,
                               1, 3, 2,
                               2, 3, 0,
                               0, 2, 1 };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}



static void CompileShaders()
{
    ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }


    //AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    //AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    shader vertexShader(GL_VERTEX_SHADER);
    shader fragmentShader(GL_FRAGMENT_SHADER);
    string vFile = "vertex.glsl";
    string fFile = "fragment.glsl";

    vertexShader.initialize(vFile);
    fragmentShader.initialize(fFile);

    glAttachShader(ShaderProgram, vertexShader.getShader());
    glAttachShader(ShaderProgram, fragmentShader.getShader());

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);

    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);
}

void reshape(GLsizei w, GLsizei h)
{
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, windowWidth, windowHeight);
}