/******************** Header Files ********************/
#include <iostream>
#include <assert.h>
#include <chrono>

#include <GL/glew.h> //must be include before the main gl libs
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "shader.h"
#include "camera.cpp"

using namespace std;


/******************** Global Variables and Constants ********************/

//useful macros
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

//Window Variables
int window;
int width = 640, height = 480; //window size
Camera camera;

//uniform locations
GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;

//transformation matrices
glm::mat4 model; //each object gets its own model matrix
glm::mat4 view;
glm::mat4 projection;
glm::mat4 mvp; //projection * view * model

//shader variables
string vertexShaderName = "vertex.glsl";
string fragmentShaderName = "fragment.glsl";
GLuint ShaderProgram;

//camera variables
bool freeCamera = true;
float yOffset = 0.0f;
float zoom = 1.0f;

//keyboard variables
bool keys[256];

/************************* Function Declarations ***********************/
void InitializeGlutCallbacks();

//Glut Callbacks
void render();
void update();
void reshape(GLsizei w, GLsizei h);
void keyboardListener(unsigned char key, int x_pos, int y_pos);
void keyboardUpListener(unsigned char key, int x_pos, int y_pos);
void checkKeyboard();

//Shader Functions
static void CreateVertexBuffer();
static void CreateIndexBuffer();
static void CompileShaders();

//resource management
bool initializeProgram();
void cleanUp();


/************************ Main Program *************************/
int main(int argc, char** argv)
{
    //initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(width, height);

    //initialize program and make sure everything is set up correctly
    if(!initializeProgram()){
        cerr << "Error: could not initialize program";
    }

    //if all is good, begin simulation
    glutMainLoop();

    //free up used memory
    cleanUp();

    return 0;
}

/******************** Function Implementations ********************/
bool initializeProgram()
{
    //create the window
    window = glutCreateWindow("Tutorial 13");

    // Must be done after glut is initialized!
    // Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        cerr << "[F] GLEW NOT INITIALIZED: ";
        cerr << glewGetErrorString(status) << endl;
        return false;
    }

    //print out openGL version and nvidia drivers begin used
    printf("GL version: %s\n", glGetString(GL_VERSION));

    //Set all of the GLUT callbacks that will be used
    InitializeGlutCallbacks();

    // initialize the keys array to false
    for(int i = 0; i < 256; i++)
    {
        keys[i] = false;
    }

    //set initial background black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //set initial perspective projection
    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(width)/float(height), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   300.0f); //Distance to the far plane,

    CreateVertexBuffer();
    CreateIndexBuffer();

    CompileShaders();
    return true;
}

void render()
{
    //clear screen
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //pre-multiply the matrix
    mvp = projection * view * model;

    //enable the program
    glUseProgram(ShaderProgram);

    //upload the matrix to the shader
    glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, glm::value_ptr(mvp));//&World[0][0]);


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

    //perform transformations of object
    model = (glm::rotate(model, sin(Scale*0.2f), glm::vec3(0.0, 1.0, 0.0)));

    //render camera position
    if(freeCamera){
        checkKeyboard();
    }

    glm::vec3 ViewPoint = camera.Position + camera.ViewDir;
    view = glm::lookAt( glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z),
                        glm::vec3(ViewPoint.x, ViewPoint.y, ViewPoint.z),
                        glm::vec3(camera.UpVector.x, camera.UpVector.y, camera.UpVector.z));

    //update the state of the scene
    glutPostRedisplay(); //call the display callback
}

void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    projection = glm::infinitePerspective(45.0f, float(w)/float(h), 0.01f);
}

void keyboardListener(unsigned char key, int x_pos, int y_pos)
{
    // check if key is escape
    if(key == 27)
    {
        glutDestroyWindow(window);
        return;
    }

    // check if key is the minus or =/+ button
    if(key == '-' || key == '=')
    {
        if(key == '-')
        {
            camera.movementSpeed -= 0.2f;
            if(camera.movementSpeed <= 0.0f)
                camera.movementSpeed = 0.2f;
        }
        else
        {
            camera.movementSpeed += 0.2f;
        }
    }

        // assume key pressed is for movement
    else
    {
        keys[key] = true;
        freeCamera = true;
    }

    glutPostRedisplay();

}


void keyboardUpListener(unsigned char key, int x_pos, int y_pos)
{
    keys[key] = false;
    glutPostRedisplay();
}

void checkKeyboard()
{
    // Strafe left
    if(keys['a'])
        camera.StrafeRight(-2.1);

    // Strafe right
    if(keys['d'])
        camera.StrafeRight(2.1);

    // Move forward
    if(keys['w'])
        camera.MoveForward( -2.1 ) ;

    // Move backward
    if(keys['s'])
        camera.MoveForward( 2.1 ) ;

    // move up
    if(keys['q'])
        camera.MoveUpward(3.3);

    // move down
    if(keys['e'])
        camera.MoveUpward(-3.3);

    // look up
    if(keys['i'])
        camera.RotateX(1.0);

    // look down
    if(keys['k'])
        camera.RotateX(-1.0);

    // roll right
    if(keys['l'])
        camera.RotateZ(-2.0);

    // roll left
    if(keys['j'])
        camera.RotateZ(2.0);

    // look left
    if(keys['u'])
        camera.RotateY(2.0);

    // look right
    if(keys['o'])
        camera.RotateY(-2.0);
}


void InitializeGlutCallbacks()
{
    glutDisplayFunc(render); //Called when its time to display
    glutIdleFunc(update); //Called if there is nothing else to do
    glutReshapeFunc(reshape); //Called if the window is resized
    glutKeyboardFunc(keyboardListener); //Called if there is keyboard input
    glutKeyboardUpFunc(keyboardUpListener);
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

    vertexShader.initialize(vertexShaderName);
    fragmentShader.initialize(fragmentShaderName);

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

