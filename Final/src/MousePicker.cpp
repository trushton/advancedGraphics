//
// Created by trushton on 10/31/15.
//

#include "MousePicker.h"

glm::vec3 MousePicker::getCurrentRay(){
    return CurrentRay;
}
glm::vec3* MousePicker::getTerrainPoint() {
    return CurrentTerrainPoint;
}



void MousePicker::update(){
    viewMatrix = Engine::getEngine()->graphics->camera->getView();
    CurrentRay = calculateMouseRay();
//    if(intersectionInRange(0,RAY_RANGE, CurrentRay)){
//        glm::vec3 result = binarySearch(0,0,RAY_RANGE, CurrentRay);
//        glm::vec3* temp = &result;
//        CurrentTerrainPoint = temp;
//    }
}

glm::vec3 MousePicker::calculateMouseRay(){
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    glm::vec2 normCoords = getNormalizedDeviceCoords(mouseX, mouseY);
    glm::vec4 clipCoords = glm::vec4(normCoords, -1.0f, 1.0f);
    glm::vec4 eyeCoords = toEyeCoords(clipCoords);
    glm::vec3 worldRay = toWorldCoords(eyeCoords);
    return worldRay;
}

glm::vec3 MousePicker::toWorldCoords(glm::vec4 eyeCoords){
    glm::mat4 invertedView = glm::inverse(viewMatrix);
    glm::vec4 rayWorld = invertedView * eyeCoords;
    glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
    mouseRay = glm::normalize(mouseRay);

    return mouseRay;
}

glm::vec4 MousePicker::toEyeCoords(glm::vec4 clipCoords){
    glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
    glm::vec4 eyeCoords = invertedProjection * clipCoords;
    return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec2 MousePicker::getNormalizedDeviceCoords(int x, int y){
    float outX = ((2.0f*x)/width)-1;
    float outY = 1.0f - (2.0f*y) / height;
    return glm::vec2(outX, outY);
}

/////////TERRAIN INTERSECTION//////////////////////////////////////
glm::vec3 MousePicker::getPointOnRay(glm::vec3 ray, float distance) {
    glm::vec3 camPos = camera->getPos();
    glm::vec3 start = glm::vec3(camPos.x, camPos.y, camPos.z);
    glm::vec3 scaledRay = glm::vec3(ray * distance);
    return glm::vec3(start + scaledRay);
}

glm::vec3 MousePicker::binarySearch(int count, float start, float finish, glm::vec3 ray) {
    float half = start + ((finish - start) / 2.0f);
    if(count >= RECURSION_COUNT){
        glm::vec3 endPoint = getPointOnRay(ray, half);
        Terrain* train = getTerrain();
        if(train != NULL){
            return endPoint;
        }
        else{
            return glm::vec3(0);
        }
    }
    if(intersectionInRange(start, half, ray)){
        return binarySearch(count+1, start, half, ray);
    } else {
        return binarySearch(count+1, half, finish, ray);
    }
}

bool MousePicker::intersectionInRange(float start, float finish, glm::vec3 ray) {
    glm::vec3 startPoint = getPointOnRay(ray, start);
    glm::vec3 endPoint = getPointOnRay(ray, finish);

    return !isUnderground(startPoint) && isUnderground(endPoint);
}

bool MousePicker::isUnderground(glm::vec3 testPoint) {
    Terrain* train = getTerrain();
    float height = 0;
    if(terrain != NULL){
        for(uint i = 0; i < train->Vertices.size(); i++){
            if(abs(train->Vertices[i].position.x - testPoint.x) < 5 && abs(train->Vertices[i].position.z - testPoint.z) < 5){
                height = train->Vertices[i].position.y;
                return testPoint.y < height;
            }

        }
    }
    return false;
}

Terrain* MousePicker::getTerrain(){
    return terrain;
}

