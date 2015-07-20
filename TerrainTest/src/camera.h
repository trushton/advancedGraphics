// Reference: http://www.codecolony.de/opengl.htm#Camera2
#ifndef CAMERA_H
#define CAMERA_H

#define PI 3.1415926f
#define PIdiv180 (PI/180.0f)

#include <GL/glew.h>
#include <GL/gl.h>
#include "math.h"
#include <iostream>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine.h"

using namespace std;

class Camera
{
public:

    Camera(Engine *eng);

    void Move(glm::vec3 Direction);

    void RotateX(GLfloat Angle);
    void RotateY(GLfloat Angle);
    void RotateZ(GLfloat Angle);

    void MoveForward(GLfloat Distance);
    void MoveUpward(GLfloat Distance);
    void StrafeRight(GLfloat Distance);

    void rotate(GLfloat, GLfloat);

    void update();
    void printOut();

    glm::mat4 getView();
    glm::vec3 getPos();

//        glm::vec3 getPos() const { return pos; }
    //glm::vec3 getOrientation() const { return orientation; }
    GLfloat totalX;
    GLfloat totalY;

private:
    glm::vec3 ViewDir;
    glm::vec3 RightVector;
    glm::vec3 UpVector;
    glm::vec3 Position;

    //glm::vec3 pos, orientation ,up;
    //GLfloat rotateX, rotateY, speed, sensitivity, boost;

    GLfloat RotatedX, RotatedY, RotatedZ;

    GLfloat movementSpeed;

    Engine *engine;
};


#endif