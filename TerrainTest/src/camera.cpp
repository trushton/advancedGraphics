// Reference: http://www.codecolony.de/opengl.htm#Camera2

#include "camera.h"
#include <glm/gtx/rotate_vector.hpp>


Camera::Camera(Engine *eng)
        : engine(eng)
{
    // initialize view
    Position = glm::vec3(-10.0f, -0.6f, -3.7f);
    ViewDir = glm::vec3(0.931359f, 0.0308144f, 0.423266f);
    RightVector = glm::vec3(1.0f, 0.0, 0.0f);
    UpVector = glm::vec3(0.0f, 1.0f, 0.0f);


    RotatedX = RotatedY = RotatedZ = 0.0f;
    movementSpeed = 5.0f;
}

void Camera::Move(glm::vec3 Direction)
{
    Position = (Position + Direction) * movementSpeed;
}

void Camera::RotateX(GLfloat Angle)
{
    RotatedX += Angle;

    // Rotate viewdir around the right vector:
    ViewDir = glm::normalize(ViewDir * cos(Angle * PIdiv180) + UpVector * sin(Angle * PIdiv180));

    // now compute the new UpVector (by cross product)
    UpVector = glm::cross(ViewDir, RightVector) * -1.0f;
}

void Camera::RotateY(GLfloat Angle)
{
    RotatedY += Angle;

    ViewDir = glm::normalize(ViewDir * cos(Angle * PIdiv180) - RightVector * sin(Angle * PIdiv180));

    RightVector = glm::cross(ViewDir, UpVector);
}

void Camera::RotateZ(GLfloat Angle)
{
    RotatedZ += Angle;

    RightVector = glm::normalize(RightVector * cos(Angle * PIdiv180) + UpVector * sin(Angle * PIdiv180));

    //now compute the new UpVector (by cross product)
    UpVector = glm::cross(ViewDir, RightVector) * -1.0f;
}

void Camera::MoveForward(GLfloat Distance)
{
    Position = Position + (ViewDir * -(Distance * movementSpeed));
}

void Camera::StrafeRight(GLfloat Distance)
{
    Position = Position + (RightVector * -(Distance * movementSpeed));
    getView();
}

void Camera::MoveUpward(GLfloat Distance)
{
    Position = Position + (UpVector * (Distance * movementSpeed));
}

void Camera::printOut() {
    cout << Position.x << " | " << Position.y << " | " << Position.z << endl;
    cout << ViewDir.x << " | " << ViewDir.y << " | " << ViewDir.z << endl;
}

glm::mat4 Camera::getView()
{
    update();

    glm::vec3 ViewPoint = Position + ViewDir;
    glm::mat4 view = glm::lookAt(Position, ViewPoint, UpVector);
    /*cout << Position.x << " | " << Position.y << " | " << Position.z << endl;
    cout << ViewDir.x << " | " << ViewDir.y << " | " << ViewDir.z << endl;
    cout << ViewPoint.x << " | " << ViewPoint.y << " | " << ViewPoint.z << endl;*/


    return view;
}

glm::vec3 Camera::getPos()
{
    return Position;
}


void Camera::rotate(GLfloat x, GLfloat y)
{
    RotatedX -= x * 0.01f;
    RotatedY += y * 0.01f;
    RightVector = glm::cross(ViewDir, UpVector);
}

void Camera::update()
{

    if(RotatedX != 0.0f)
        ViewDir = glm::rotate(ViewDir, RotatedX, UpVector);

    if(RotatedY != 0.0f)
        ViewDir = glm::rotate(ViewDir, RotatedY, glm::cross(UpVector, ViewDir));

    RotatedX = RotatedY = 0.0f;
}