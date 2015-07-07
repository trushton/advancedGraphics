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
#include "Model.h"

using namespace std;


/******************** Global Variables and Constants ********************/

//useful macros
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

//Window Variables
int window;
int width = 640, height = 480; //window size
Model object;
//Model skybox;
Camera camera;

//uniform locations
GLint texture_loc_mvpmat;

//attribute locations
GLint texture_loc_position;
GLint loc_texture_coord;

GLint loc_color;
GLint color_loc_position;
GLuint color_program;
GLint color_loc_mvpmat; // Location of the modelviewprojection matrix in the shader


//transformation matrices
glm::mat4 model; //each object gets its own model matrix
glm::mat4 view;
glm::mat4 projection;
glm::mat4 mvp; //projection * view * model

//shader variables
string vertexShaderName = "texture_vertex_shader.glsl";
string fragmentShaderName = "texture_fragment_shader.glsl";
GLuint texture_program;


//camera variables
bool freeCamera = true;
float yOffset = 0.0f;
float zoom = 1.0f;

//keyboard variables
bool keys[256];

GLuint VB;



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
bool createColorShader();

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

    object.loadModel("room.obj");
    //skybox.loadModel("skybox.obj");


    // initialize the keys array to false
    for(int i = 0; i < 256; i++)
    {
        keys[i] = false;
    }

    // create the texture shader
    if(!createTextureShader())
    {
        return false;
    }

    // create the color shader
    if(!createColorShader())
    {
        return false;
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

bool createTextureShader()
{
    // set up the texture vertex shader
    shader texture_vertex_shader(GL_VERTEX_SHADER);
    if(!texture_vertex_shader.initialize(vertexShaderName))
    {
        return false;
    }

    // set up the texture fragment shader
    shader texture_fragment_shader(GL_FRAGMENT_SHADER);
    if(!texture_fragment_shader.initialize(fragmentShaderName))
    {
        return false;
    }

    // link the texture shader program
    texture_program = glCreateProgram();
    glAttachShader(texture_program, texture_vertex_shader.getShader());
    glAttachShader(texture_program, texture_fragment_shader.getShader());
    glLinkProgram(texture_program);

    // check if everything linked ok
    GLint texture_shader_status;
    glGetProgramiv(texture_program, GL_LINK_STATUS, &texture_shader_status);
    if(!texture_shader_status)
    {
        cerr << "[F] THE TEXTURE SHADER PROGRAM FAILED TO LINK" << endl;
        return false;
    }

    // set up the vertex position attribute
    texture_loc_position = glGetAttribLocation(texture_program, const_cast<const char*>("v_position"));
    if(texture_loc_position == -1)
    {
        cerr << "[F] POSITION NOT FOUND" << endl;
        return false;
    }

    // set up the vertex uv coordinate attribute
    loc_texture_coord = glGetAttribLocation(texture_program, const_cast<const char*>("v_texture"));
    if(loc_texture_coord == -1)
    {
        cerr << "[F] V_COLOR NOT FOUND" << endl;
        return false;
    }

    // set up the MVP matrix attribute
    texture_loc_mvpmat = glGetUniformLocation(texture_program, const_cast<const char*>("mvpMatrix"));
    if(texture_loc_mvpmat == -1)
    {
        cerr << "[F] MVPMATRIX NOT FOUND" << endl;
        return false;
    }

    // return
    return true;
}


bool createColorShader()
{
    // set up the color vertex shader
    shader color_vertex_shader(GL_VERTEX_SHADER);
    if(!color_vertex_shader.initialize("color_vertex_shader.glsl"))
    {
        return false;
    }

    // set up the color fragment shader
    shader color_fragment_shader(GL_FRAGMENT_SHADER);
    if(!color_fragment_shader.initialize("color_fragment_shader.glsl"))
    {
        return false;
    }

    // link the color shader program
    color_program = glCreateProgram();
    glAttachShader(color_program, color_vertex_shader.getShader());
    glAttachShader(color_program, color_fragment_shader.getShader());
    glLinkProgram(color_program);

    // check if everything linked ok
    GLint color_shader_status;
    glGetProgramiv(color_program, GL_LINK_STATUS, &color_shader_status);
    if(!color_shader_status)
    {
        cerr << "[F] THE COLOR SHADER PROGRAM FAILED TO LINK" << endl;
        return false;
    }

    // set up the vertex position attribute
    color_loc_position = glGetAttribLocation(color_program, const_cast<const char*>("v_position"));
    if(color_loc_position == -1)
    {
        cerr << "[F] POSITION NOT FOUND" << endl;
        return false;
    }

    // set up the vertex color attribute
    loc_color = glGetAttribLocation(color_program, const_cast<const char*>("v_color"));
    if(loc_color == -1)
    {
        cerr << "[F] V_COLOR NOT FOUND" << endl;
        return false;
    }

    // set up the MVP matrix attribute
    color_loc_mvpmat = glGetUniformLocation(color_program, const_cast<const char*>("mvpMatrix"));
    if(color_loc_mvpmat == -1)
    {
        cerr << "[F] MVPMATRIX NOT FOUND" << endl;
        return false;
    }

    // return
    return true;
}

void render()
{
    //clear screen
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //set the shader program
    glUseProgram(texture_program);

    object.mvp = projection * view * object.model;
    glUniformMatrix4fv(texture_loc_mvpmat, 1, GL_FALSE, glm::value_ptr(object.mvp));
    object.renderModel(texture_loc_position, loc_texture_coord);

    mvp = projection * view * model;
    glUniformMatrix4fv(texture_loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    //skybox.renderModel(texture_loc_position, loc_texture_coord);


    glutSwapBuffers();
}

void update(){
    static float Scale = 0.0f;
    Scale += 0.001f;

    //perform transformations of object
    object.model = (glm::translate(glm::mat4(1.0f), glm::vec3(10,10,0)));
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
    glDeleteProgram(texture_program);
}

