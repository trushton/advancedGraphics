//
// Created by trushton on 10/31/15.
//

#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H


#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "graphics.h"
class Terrain;
class MousePicker {
public:

      int RECURSION_COUNT = 200;
      float RAY_RANGE = 600;
    glm::vec3 CurrentRay;
    glm::vec3* CurrentTerrainPoint;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    Camera* camera;
    int width, height;
    Terrain* terrain;

    MousePicker(glm::mat4 projection, glm::mat4 view, int windowWidth, int windowHeight, Terrain* train){
        projectionMatrix = projection;
        viewMatrix = view;
        width = windowWidth;
        height = windowHeight;
        terrain = train;
        CurrentTerrainPoint = new glm::vec3(0,0,0);
    }

    glm::vec3 getCurrentRay();

    glm::vec3* getTerrainPoint();

    void update();

    glm::vec3 getPointOnRay(glm::vec3 ray, float distance);

    glm::vec3 binarySearch(int count, float start, float finish, glm::vec3 ray);

    bool intersectionInRange(float start, float finish, glm::vec3 ray);

    bool isUnderground(glm::vec3 testPoint);

    Terrain* getTerrain();

    glm::vec3 calculateMouseRay();

    glm::vec3 toWorldCoords(glm::vec4 eyeCoords);

    glm::vec4 toEyeCoords(glm::vec4 clipCoords);

    glm::vec2 getNormalizedDeviceCoords(int x, int y);

};


#endif //MOUSEPICKER_H
