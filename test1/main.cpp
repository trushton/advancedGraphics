/******************** Header Files ********************/
#include <iostream>
#include <chrono>

#include <GL/glew.h> //must be include before the main gl libs
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "shader.h"
#include "camera.cpp"
#include "Model.h"
#include "ds_geom_pass_tech.h"
#include "gbuffer.h"

using namespace std;


/******************** Global Variables and Constants ********************/

//useful macros
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

//Window Variables
int window;
int width = 1280, height = 1024; //window size
Model object;
Camera camera;

GLuint shaderProgram;

//deferred shading
DSGeomPassTech m_DSGeomPassTech;
GBuffer gbuffer;


//transformation matrices
glm::mat4 model; //each object gets its own model matrix
glm::mat4 view;
glm::mat4 projection;
glm::mat4 mvp; //projection * view * model



//camera variables
bool freeCamera = true;

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
bool createTextureShader();

//resource management
bool initializeProgram();
void cleanUp();

//deferred shading
void DSGeometryPass();
void DSLightPass();

/************************ Main Program *************************/
int main(int argc, char** argv)
{
    //initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(width, height);


    //create the window
    window = glutCreateWindow("Tutorial 16");

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

    // create the texture shader
//    if(!createTextureShader())
//    {
//        return false;
//    }

    if (!gbuffer.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
        return false;
    }

    if (!m_DSGeomPassTech.Init()) {
        printf("Error initializing DSGeomPassTech\n");
        return false;
    }

    m_DSGeomPassTech.Enable();
    m_DSGeomPassTech.SetTextureUnit(0);

    object.loadModel("hheli.obj");


    // initialize the keys array to false
    for(int i = 0; i < 256; i++)
    {
        keys[i] = false;
    }

    //set initial perspective projection
    projection = glm::perspective( 90.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(width)/float(height), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   300.0f); //Distance to the far plane,

    //set initial background black
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //all done
    return true;
}



void DSGeometryPass()
{
    m_DSGeomPassTech.Enable();
    gbuffer.BindForWriting();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    object.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)));

    mvp = projection * view * object.model;
    m_DSGeomPassTech.SetMVP(mvp);
    m_DSGeomPassTech.SetModelMatrix(object.model);
    m_DSGeomPassTech.SetTextureUnit(0);

    object.renderModel(m_DSGeomPassTech.loc_vertexPosition, m_DSGeomPassTech.loc_vertexTexture);
}

void DSLightPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gbuffer.BindForReading();

    GLint HalfWidth = (GLint)(WINDOW_WIDTH / 2.0f);
    GLint HalfHeight = (GLint)(WINDOW_HEIGHT / 2.0f);

    gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, HalfHeight, HalfWidth, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void render()
{
//    //clear screen
//    glClearColor(0.0, 0.0, 0.0, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    //set the shader program
//    glUseProgram(shaderProgram);
//
    //object.mvp = projection * view * object.model;
//    glUniformMatrix4fv(loc_mvpMatrix, 1, GL_FALSE, glm::value_ptr(object.mvp));
//    object.renderModel(loc_vertexPosition, loc_vertexTexture);
//
    //mvp = projection * view * model;
//    glUniformMatrix4fv(loc_mvpMatrix, 1, GL_FALSE, glm::value_ptr(mvp));
//    //skybox.renderModel(texture_loc_position, loc_texture_coord);

    DSGeometryPass();
    DSLightPass();

    glutSwapBuffers();
}

void update(){
    static float Scale = 0.0f;
    Scale += 0.001f;

    //perform transformations of object
    object.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)));
    object.model = (glm::rotate(object.model, sin(Scale*0.2f), glm::vec3(0.0, 1.0, 0.0)));
    object.model = glm::scale(object.model, glm::vec3(5.0f,5.0f,5.0f));

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
    if(keys['o'])
        camera.RotateZ(-2.0);

    // roll left
    if(keys['u'])
        camera.RotateZ(2.0);

    // look left
    if(keys['j'])
        camera.RotateY(2.0);

    // look right
    if(keys['l'])
        camera.RotateY(-2.0);

    // look right
    if(keys['1']){
        cout << "pos x: " << camera.Position.x << " pos y: " << camera.Position.y << " pos z: " << camera.Position.z << endl;
        cout << "view x: " << camera.ViewDir.x << " view y: " << camera.ViewDir.y << " view z: " << camera.ViewDir.z << endl;

    }

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
    glDeleteProgram(shaderProgram);
}

//bool createTextureShader()
//{
//    // set up the texture vertex shader
//    shader texture_vertex_shader(GL_VERTEX_SHADER);
//    if(!texture_vertex_shader.initialize(vertexShaderName))
//    {
//        return false;
//    }
//
//    // set up the texture fragment shader
//    shader texture_fragment_shader(GL_FRAGMENT_SHADER);
//    if(!texture_fragment_shader.initialize(fragmentShaderName))
//    {
//        return false;
//    }
//
//    // link the texture shader program
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, texture_vertex_shader.getShader());
//    glAttachShader(shaderProgram, texture_fragment_shader.getShader());
//    glLinkProgram(shaderProgram);
//
//    // check if everything linked ok
//    GLint texture_shader_status;
//
//    // check if everything linked ok
//    char fragmentBuffer[512];
//
//    glGetProgramInfoLog(shaderProgram, 512, NULL, fragmentBuffer);
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &texture_shader_status);
//    if(!texture_shader_status)
//    {
//        cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << endl;
//        return false;
//    }
//
//    // set up the vertex position attribute
//    loc_vertexPosition = glGetAttribLocation(shaderProgram, const_cast<const char*>("v_position"));
//    if(loc_vertexPosition == 0xFFFFFFFF)
//    {
//        cerr << "[F] POSITION NOT FOUND" << endl;
//        cout << fragmentBuffer;
//
//        return false;
//    }
//
//    // set up the vertex uv coordinate attribute
//    loc_vertexTexture = glGetAttribLocation(shaderProgram, const_cast<const char*>("v_texture"));
//    if(loc_vertexTexture == 0xFFFFFFFF)
//    {
//        cerr << "[F] V_COLOR NOT FOUND" << endl;
//        cout << fragmentBuffer;
//
//        return false;
//    }
//
//    loc_vertexNormal = glGetAttribLocation(shaderProgram, const_cast<const char*>("v_normal"));
//    if(loc_vertexNormal == 0xFFFFFFFF)
//    {
//        std::cerr << "[F] V_NORMAL NOT FOUND" << std::endl;
//        return false;
//    }
//
//    // set up the MVP matrix attribute
//    loc_mvpMatrix = glGetUniformLocation(shaderProgram, const_cast<const char*>("mvpMatrix"));
//    if(loc_mvpMatrix == 0xFFFFFFFF)
//    {
//        cerr << "[F] MVPMATRIX NOT FOUND" << endl;
//        cout << fragmentBuffer;
//
//        return false;
//    }
//
//    loc_modelMatrix = glGetUniformLocation(shaderProgram, const_cast<const char*>("modelMatrix"));
//    if(loc_modelMatrix == 0xFFFFFFFF)
//    {
//        std::cerr << "[F] MODELMATRIX NOT FOUND" << std::endl;
//        return false;
//
//    }
//
//    // return
//    return true;
//}